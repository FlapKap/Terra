#!/usr/bin/python3
from pathlib import Path
import subprocess
import json
import os
import regex
import sys
import sqlite3
import asyncio
from shutil import which, copy
import argparse

parser = argparse.ArgumentParser(description="Experiment test runner")
parser.add_argument("--attach", action="store_true", help="attach to the experiment")
parser.add_argument("--make", action="store_true", help="make binaries")
parser.add_argument("--dont-run", action="store_true", help="don't run the experiment")
parser.add_argument(
    "experiment_folder",
    type=Path,
    help="folder containing the binaries and metadata to run the experiment",
)
args = parser.parse_args()

# Experiment info
EXPERIMENT_CONFIG_DEFAULT = {
    "SITE": "saclay",
    "USER": "berthels",
    "DURATION": "20",
    "DEVICES": [
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005E88A",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA55",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA56",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA57",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA59",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
        },
    ],
}

EXPERIMENT_FOLDER: Path = args.experiment_folder
EXPERIMENT_CONFIG_PATH = EXPERIMENT_FOLDER / "experiment.json"
## if no experiment.json exists in EXPERIMENT_FOLDER, create default one
if not EXPERIMENT_FOLDER.exists():
    EXPERIMENT_FOLDER.mkdir(parents=True)
if not EXPERIMENT_CONFIG_PATH.exists():
    json.dump(EXPERIMENT_CONFIG_DEFAULT, open(EXPERIMENT_CONFIG_PATH, "w"), indent=4)

EXPERIMENT_CONFIG = json.load(open(EXPERIMENT_FOLDER / "experiment.json"))


# IoT-lab info
SITE = EXPERIMENT_CONFIG["SITE"]
USER = EXPERIMENT_CONFIG["USER"]
SSH_URL = f"{SITE}.iot-lab.info"
# RIOT info
SRC_PATH = Path.cwd() / "src"

# make firmwares
# make and upload binaries
if args.make:
    print("make firmware")
    for device in EXPERIMENT_CONFIG["DEVICES"]:
        # first make binary
        print(f"make binary for device: {device['DEVEUI']}")
        env = os.environ.copy()
        env["BOARD"] = device["RIOT_BOARD"]
        env["DEVEUI"] = device["DEVEUI"]
        env["APPEUI"] = device["APPEUI"]
        env["APPKEY"] = device["APPKEY"]

        # make
        p = subprocess.run(["make", "all"], cwd=SRC_PATH, env=env)

        ## find flash file
        p = subprocess.run(
            ["make", "info-build-json"], cwd=SRC_PATH, env=env, capture_output=True
        )
        build_info = json.loads(p.stdout)
        flash_file = Path(build_info["FLASHFILE"])
        copy(flash_file, EXPERIMENT_FOLDER / f"{device['DEVEUI']}{flash_file.suffix}")

# we run the experiment below, so if we dont want to do that: early exit
if args.dont_run:
    exit()

# Check prerequisites
for tool in ["iotlab", "parallel", "ssh"]:
    if not which(tool):
        print(f"{tool} not installed. Please install {tool}")
        exit()

## check if passwordless ssh is set up
if subprocess.run(["ssh","-oNumberOfPasswordPrompts=0", f"{USER}@{SSH_URL}", "hostname"]).returncode != 0:
    print(f"Passwordless SSH access to the ssh front-end is required.\ncant log in to {SSH_URL} without password. Check if ssh-agent is running and if not run 'eval $(ssh-agent);ssh-add' to start the agent and add your key")
    exit()

if not args.attach:
    print("Registering experiment")
    # create experiment
    ## create arguments for nodes
    node_strings = []
    for device in EXPERIMENT_CONFIG["DEVICES"]:
        node_strings.extend(
            ["-l", f"1,archi={device['IOT-LAB_BOARD']}+site={SITE},,{device['PROFILE']}"]
        )
    subprocess.run(
        [
            "iotlab",
            "experiment",
            "submit",
            "-d",
            EXPERIMENT_CONFIG["DURATION"],
            *node_strings,
        ],
        stdout=sys.stdout,
        check=True,
    )

print("Waiting for experiment to start")
# find experiment
subprocess.run(["iotlab", "experiment", "wait"])  # wait for experiment to start

# gets active experiments
p = subprocess.run(
    ["iotlab", "experiment", "get", "-e"], capture_output=True, check=True
)
EXPERIMENT = json.loads(p.stdout)["Running"][
    0
]  # TODO: work with more than 1 experiment?
DB_PATH = EXPERIMENT_FOLDER / f"{EXPERIMENT}.db"
print(f"Create SqliteDB for experiment data at {str(DB_PATH)}")
# Create DB for results
create_sql_script = open("./experiment.db.sql").read()
db_con = sqlite3.connect(f"{str(DB_PATH)}")

db_cursor = db_con.executescript(create_sql_script)

# NODES is a list of dictionaries with following info
# {
#     "archi": "nucleo-wl55jc:stm32wl",
#     "camera": "0",
#     "mobile": "0",
#     "mobility_type": null,
#     "network_address": "nucleo-wl55jc-1.grenoble.iot-lab.info",
#     "power_consumption": "1",
#     "power_control": "1",
#     "production": "YES",
#     "radio_sniffing": "1",
#     "site": "grenoble",
#     "state": "Alive",
#     "uid": " ",
#     "x": "13.36",
#     "y": "31.97",
#     "z": "2.58"
# }
print("Fetching nodes in experiment...")
p = subprocess.run(
    ["iotlab", "experiment", "get", "-i", str(EXPERIMENT), "-n"],
    capture_output=True,
    check=True,
)
NODES = json.loads(p.stdout)["items"]
node_ids = dict()
# populate nodes table
nodes = [(node["network_address"].split(".")[0],) for node in NODES]
db_cursor.executemany("INSERT INTO nodes (name) VALUES (?)", nodes)

for nid, name in db_cursor.execute("SELECT id, name FROM nodes"):
    node_ids[name] = nid

# get .bin files from experiment folder
flash_files = [flash_file for flash_file in EXPERIMENT_FOLDER.glob("*.{bin,elf}")]
## sanity check that number of files correspond with number of nodes in config and number of created nodes in iot-lab
if len(flash_files) != len(NODES) != len(EXPERIMENT_CONFIG["DEVICES"]):
    print(
        f"Number of flash file ({len(flash_files)}) does not match number of nodes in config ({len(EXPERIMENT_CONFIG['DEVICES'])}) or number of created nodes in iot-lab ({len(NODES)})"
    )
    exit()

for flash_file, node_info in zip(flash_files, NODES):
    # construct nodelist for single node
    # might be easier with iotl"ab experiment get -d instead of -n
    node_id = regex.match(".*-(\d+)\.", node_info["network_address"]).group(1)

    node_string = f"{node_info['site']},{node_info['archi'].split(':')[0]},{node_id}"

    # upload
    print(f"uploading binary file {str(flash_file)} to node_string {node_string}")
    p = subprocess.run(
        [
            "iotlab",
            "node",
            "-i",
            str(EXPERIMENT),
            "-l",
            node_string,
            "-fl",
            str(flash_file.absolute()),
        ],
        stdout=sys.stdout,
        check=True,
    )

import asyncssh


## define async functions to handle lines coming from serial_aggregator, powerconsumption oml files and radio oml files
# async def handle_serial_aggregator(process):
#     async for record in process.stdout:
#         print(f"serial: {record}")
#         record = record.split(";", maxsplit=2)
#         if len(record) <= 2:  # probably something like <time>;Aggregator started
#             continue

#         time_us = int(float(record[0]) * 1e6)
#         node_id = node_ids[record[1]]
#         msg = record[2]
#         db_cursor.execute(
#             "INSERT INTO metrics (node_id, timestamp, metric_string) VALUES (?,?,?)",
#             node_id,
#             time_us,
#             msg,
#         )


# async def handle_power_consumption(process):
#     matcher = regex.compile(
#         r"^(?P<exp_runtime>\d+(\.\d*)?)\s+(?P<schema>\d+)\s+(?P<cnmc>\d+)\s+(?P<timestamp_s>\d+)\s+(?P<timestamp_us>\d+)\s+(?P<power>\d+(\.\d*)?)\s+(?P<voltage>\d+(\.\d*)?)\s+(?P<current>\d+(\.\d*)?)$"
#     )

#     async for line in process.stdout:
#         print(f"pc: {line}")
#         record = matcher.match(line)
#         if record is None:
#             continue
#         timestamp: int = int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"])
#         db_cursor.execute(
#             "INSERT INTO power_consumption (node_id, timestamp, voltage, current, power) VALUES (?, ?, ?, ?, ?)",
#             (
#                 node_ids[record["node_name"]],
#                 timestamp,
#                 float(record["voltage"]),
#                 float(record["current"]),
#                 float(record["power"]),
#             ),
#         )


# async def handle_radio(process):
#     matcher = regex.compile(
#         r"^(?P<exp_runtime>\d+(\.\d*)?)\s+(?P<schema>\d+)\s+(?P<cnmc>\d+)\s+(?P<timestamp_s>\d+)\s+(?P<timestamp_us>\d+)\s+(?P<channel>\d+)\s+(?P<rssi>\-?\d+)$"
#     )
#     for line in process.stdout:
#         print(f"radio: {line}")
#         record = matcher.match(line)
#         if record is None:
#             continue
#         timestamp: int = int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"])
#         db_cursor.execute(
#             "INSERT INTO radio (node_id, timestamp, channel, rssi) VALUES (?, ?, ?, ?)",
#             (
#                 node_ids[record["node_name"]],
#                 timestamp,
#                 int(record["channel"]),
#                 int(record["rssi"]),
#             ),
#         )


# passphrase = input("Enter passphrase: ")


# # create ssh session to remote
# async def data_collection():
#     async with asyncssh.connect(SSH_URL, username=USER, passphrase=passphrase) as ssh:
#         start processes
#         # serial_aggregator
#         serial_aggregator = await ssh.create_process("serial_aggregator")

#         # power consumption
#         pc_commands = [
#             f"tail -F .iot-lab/last/consumption/{node_name.replace('-','_')}.oml"
#             for node_name in node_ids.keys()
#         ]
#         pc_processes = [await ssh.create_process(cmd) for cmd in pc_commands]

#         # radio
#         radio_commands = [
#             f"tail -F .iot-lab/last/radio/{node_name.replace('-','_')}.oml"
#             for node_name in node_ids.keys()
#         ]
#         radio_processes = [await ssh.create_process(cmd) for cmd in radio_commands]

#         attach handlers
#         await asyncio.gather(
#             handle_serial_aggregator(serial_aggregator),
#             *[handle_power_consumption(pc) for pc in pc_processes],
#             *[handle_radio(radio) for radio in radio_processes],
#         )


# import asyncio, asyncssh
# async def test():
#     print("1")
#     async with asyncssh.connect("saclay.iot-lab.info", username="berthels", passphrase="X_U&kz`N]HXH$BD?") as ssh:
#         serial_aggregator = await ssh.create_process("serial_aggregator")
#         print("2")
#         async for record in serial_aggregator.stdout:
#             print("3")
#             print(record)
#         print("4")
# define data collection functions using asyncio
# serial aggregation
serial_count = 0


async def serial_aggregation_coroutine():
    global serial_count
    print("Starting serial aggregation collection")
    p = await asyncio.create_subprocess_exec(
        "ssh",
        f"{USER}@{SSH_URL}",
        "serial_aggregator",
        "-i",
        str(EXPERIMENT),
        stdout=asyncio.subprocess.PIPE,
    )
    while True:
        record = (await p.stdout.readline()).decode("utf-8")
        record = record.split(";")
        if len(record) <= 2:  # probably something like <time>;Aggregator started
            continue

        time_us = int(float(record[0]) * 1e6)
        node_id = node_ids[record[1]]
        msg = record[2]
        db_cursor.execute(
            "INSERT INTO metrics (node_id, timestamp, metric_string) VALUES (?,?,?)",
            (node_id, time_us, msg),
        )
        serial_count += 1


## power consumption metrics
power_consumption_count = 0


async def power_consumption_coroutine():
    global power_consumption_count
    print("starting power consumption collection")
    ## wait 5 sec for the files to be created
    await asyncio.sleep(5)
    ## use GNU Parallel to run multiple processes through a single ssh connection and collect the results in 1 stdout stream
    p = await asyncio.create_subprocess_exec(
        "parallel",
        "--jobs",
        "0",
        "--line-buffer",
        "--tag",
        "--controlmaster",
        "-S",
        "berthels@saclay.iot-lab.info",
        "--workdir",
        f"/senslab/users/berthels/.iot-lab/{EXPERIMENT}/consumption",
        "tail -F",
        ":::",
        *map(lambda k: f"{k.replace('-','_')}.oml", node_ids.keys()),
        stdout=asyncio.subprocess.PIPE,
    )
    ## matches strings from the .oml files prepended with the name of the file.
    ## The regex is made to match lines of (newlines is whitespace):
    # "
    # <node name :str>
    # <run time in second since experiment start :float>
    # <oml schema? :int>
    # <some counter :int>
    # <seconds part of timestamp :int>
    # <microsecond part of timestamp :int>
    # <power measurement :float>
    # <voltage measurement :float>
    # <current measurement :float>
    matcher = regex.compile(
        r"^(?P<node_name>\S+)\s+(?P<exp_runtime>\d+(\.\d*)?)\s+(?P<schema>\d+)\s+(?P<cnmc>\d+)\s+(?P<timestamp_s>\d+)\s+(?P<timestamp_us>\d+)\s+(?P<power>\d+(\.\d*)?)\s+(?P<voltage>\d+(\.\d*)?)\s+(?P<current>\d+(\.\d*)?)$"
    )

    while True:
        line = (await p.stdout.readline()).decode("utf-8")
        # print(line)
        record = matcher.match(line)
        if record is None:
            continue
        timestamp: int = int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"])
        ## we need to convert the node name back from the .oml file name to the proper node name
        name = record["node_name"].replace("_", "-").split(".")[0]
        db_cursor.execute(
            "INSERT INTO power_consumption (node_id, timestamp, voltage, current, power) VALUES (?, ?, ?, ?, ?)",
            (
                node_ids[name],
                timestamp,
                float(record["voltage"]),
                float(record["current"]),
                float(record["power"]),
            ),
        )
        power_consumption_count += 1


radio_count = 0


async def radio_coroutine():
    global radio_count
    await asyncio.sleep(60)
    print("starting radio collection")
    ## use GNU Parallel to run multiple processes through a single ssh connection and collect the results in 1 stdout stream
    p = await asyncio.create_subprocess_exec(
        "parallel",
        "--jobs",
        "0",
        "--line-buffer",
        "--tag",
        "--controlmaster",
        "-S",
        "berthels@saclay.iot-lab.info",
        "--workdir",
        f"/senslab/users/berthels/.iot-lab/{EXPERIMENT}/radio",
        "tail -F",
        ":::",
        *map(lambda k: f"{k.replace('-','_')}.oml", node_ids.keys()),
        stdout=asyncio.subprocess.PIPE,
    )
    ## matches strings from the .oml files prepended with the name of the file.
    ## The regex is made to match lines of (newlines is whitespace):
    # "
    # <node name :str>
    # <run time in second since experiment start :float>
    # <oml schema? :int>
    # <some counter :int>
    # <seconds part of timestamp :int>
    # <microsecond part of timestamp :int>
    # <power measurement :float>
    # <voltage measurement :float>
    # <current measurement :float>
    matcher = regex.compile(
        r"^(?P<node_name>\S+)\s+(?P<exp_runtime>\d+(\.\d*)?)\s+(?P<schema>\d+)\s+(?P<cnmc>\d+)\s+(?P<timestamp_s>\d+)\s+(?P<timestamp_us>\d+)\s+(?P<channel>\d+)\s+(?P<rssi>\-?\d+)$"
    )

    while True:
        line = await p.stdout.readline()
        record = matcher.match(line.decode("utf-8"))
        if record is None:
            continue
        timestamp: int = int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"])
        name = (
            record["node_name"].replace("_", "-").split(".")[0]
        )  # we need to convert the node name back from the .oml file name to the proper node name
        db_cursor.execute(
            "INSERT INTO radio (node_id, timestamp, channel, rssi) VALUES (?, ?, ?, ?)",
            (node_ids[name], timestamp, int(record["channel"]), int(record["rssi"])),
        )
        radio_count += 1


async def print_progress():
    print("\n" * 3, end="")
    while True:
        print(
            f"serial lines: {serial_count}",
            f"power_consumption lines: {power_consumption_count}",
            f"radio lines: {radio_count}",
            sep="\n",
        )
        print("\033[F\033[K" * 3, end="")
        await asyncio.sleep(0.5)
        db_con.commit()


## reset all boards
# subprocess.run(["iotlab", "node", "--reset"], check=True)


# run all data collection tasks and await their completion
async def main():
    print("starting data collection")
    # await power_consumption_coroutine()
    await asyncio.gather(
        serial_aggregation_coroutine(),
        power_consumption_coroutine(),
        radio_coroutine(),
        print_progress(),
    )


asyncio.run(main())
