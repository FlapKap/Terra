#!/usr/bin/python3
from pathlib import Path
import subprocess
import json
import os
import regex
import sys
import sqlite3
import asyncio

# Experiment info
DEVEUIS = [
    "70B3D57ED005E88A",
    "70B3D57ED005EA55",
    "70B3D57ED005EA56",
    "70B3D57ED005EA57",
    "70B3D57ED005EA59",
]
APPKEY = "385794DDE70CE2EAB5B5B12A4807822C"
APPEUI = "0000000000000000"
NUM_NODES = len(DEVEUIS)


# IoT-lab info
SITE = "saclay"
USER = "berthels"
PROFILE = "stm32Profile"
SSH_URL = f"{USER}@{SITE}.iot-lab.info"
# RIOT info
BOARD = "b-l072z-lrwan1"
SRC_PATH = Path.cwd() / "src"


# create experiment
subprocess.run(
    [
        "iotlab",
        "experiment",
        "submit",
        "-d",
        "20",
        "-l",
        f"{NUM_NODES},archi=st-lrwan1:sx1276+site={SITE},,{PROFILE}",
    ],
    stdout=sys.stdout,
)

# find experiment
subprocess.run(["iotlab", "experiment", "wait"])  # wait for experiment to start

# gets active experiments
p = subprocess.run(["iotlab", "experiment", "get", "-e"], capture_output=True)
EXPERIMENT = json.loads(p.stdout)["Running"][
    0
]  # TODO: work with more than 1 experiment?


# Create DB for results
create_sql_script = open("./experiment.db.sql")
db_con = sqlite3.connect(f"{str(EXPERIMENT)}.db")

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
p = subprocess.run(
    ["iotlab", "experiment", "get", "-i", str(EXPERIMENT), "-n"], capture_output=True
)
NODES = json.loads(p.stdout)["items"]
node_ids = dict()

# populate nodes table
nodes = [(node["network_address"].split(".")[0],) for node in NODES]
db_cursor.executemany("INSERT INTO nodes (name) VALUES (?)", nodes)

for nid, name in db_cursor.execute("SELECT id, name FROM nodes"):
    node_ids[name] = nid

# make and upload binaries
for deveui, node_info in zip(DEVEUIS, NODES):
    # first make binary
    env = os.environ.copy()
    env["BOARD"] = BOARD
    env["DEVEUI"] = deveui
    env["APPEUI"] = APPEUI
    env["APPKEY"] = APPKEY

    # make
    p = subprocess.run(["make", "all"], cwd=SRC_PATH, env=env, capture_output=True)

    ## find flash file
    p = subprocess.run(
        ["make", "info-build-json"], cwd=SRC_PATH, env=env, capture_output=True
    )
    build_info = json.loads(p.stdout)
    flash_file = Path(build_info["FLASHFILE"])

    # construct nodelist for single node
    # might be easier with iotl"ab experiment get -d instead of -n
    node_id = regex.match(".*-(\d+)\.", node_info["network_address"]).group(1)

    node_string = f"{node_info['site']},{node_info['archi'].split(':')[0]},{node_id}"

    # upload
    print(node_string)
    p = subprocess.run(
        [
            "iotlab",
            "node",
            "-i",
            str(EXPERIMENT),
            "-l",
            node_string,
            "-fl",
            str(flash_file),
        ],
        stdout=sys.stdout,
    )


# define data collection functions using asyncio
## serial aggregation
async def serial_aggregation_listener():
    p = await asyncio.create_subprocess_exec(
        "ssh",
        SSH_URL,
        "serial_aggregator",
        "-i",
        str(EXPERIMENT),
        stdout=asyncio.subprocess.PIPE,
    )
    while True:
        record = p.stdout.readline().split(";")
        if len(record) <= 2:  # probably something like <time>;Aggregator started
            continue

        time_us = int(float(record[0]) * 1e6)
        node_id = node_ids[record[1]]
        msg = record[2]
        db_cursor.execute(
            "INSERT INTO metrics (node_id, timestamp, metric_string) VALUES (?,?,?)",
            node_id,
            time_us,
            msg,
        )


## power consumption metrics
async def power_consumption_listener():
    ## use GNU Parallel to run multiple processes through a single ssh connection and collect the results in 1 stdout stream
    p = await asyncio.create_subprocess_exec(
        "parallel",
        "--jobs",
        "0",
        "--line-buffer",
        "--tag",
        "--workdir",
        "~/.iot-lab/last/consumption/",
        "-S",
        "berthels@saclay.iot-lab.info",
        "tail",
        "-f",
        ":::",
        *node_ids.keys(),
        stdout=asyncio.subprocess.PIPE,
    )

    matcher = regex.compile(r"*^(?P<first_string>\S+)\s+(?P<number1>(?P<float1>\d+(\.\d*)?|\.\d+))\s+(?P<number2>\d+)\s+(?P<number3>\d+)\s+(?P<number4>\d+)\s+(?P<number5>\d+)\s+(?P<number6>(?P<float2>\d+(\.\d*)?|\.\d+))\s+(?P<number7>(?P<float3>\d+(\.\d*)?|\.\d+))\s+(?P<number8>(?P<float4>\d+(\.\d*)?|\.\.\d+)))$")

    while True:
        record = p.stdout.readline().split()


## reset all boards
subprocess.run(["iotlab", "node", "--reset"])
