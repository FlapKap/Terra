#!/usr/bin/python3
from dataclasses import dataclass
from pathlib import Path
import subprocess
import json
import os
from typing import List, Optional
import regex
import sys
import duckdb
import asyncio
from shutil import which, copy
import argparse
from datetime import datetime
import aiomqtt

parser = argparse.ArgumentParser(description="Experiment test runner")
parser.add_argument("--attach", action="store_true", help="attach to the experiment")
parser.add_argument("--dont-make", action="store_true", help="dont make binaries")
parser.add_argument("--dont-upload", action="store_true", help="dont upload binaries")
parser.add_argument("--dont-run", action="store_true", help="don't run the experiment")
parser.add_argument(
    "experiment_folder",
    type=Path,
    help="folder containing the binaries and metadata to run the experiment",
)
args = parser.parse_args()

# Experiment info
EXPERIMENT_CONFIG_DEFAULT = {
    "USER": "berthels",
    "DURATION": "20",
    "NODES": [
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005E88A",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
            "SITE": "saclay",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA55",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
            "SITE": "saclay",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA56",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
            "SITE": "saclay",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA57",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
            "SITE": "saclay",
        },
        {
            "PROFILE": "stm32Profile",
            "RIOT_BOARD": "b-l072z-lrwan1",
            "IOT-LAB_BOARD": "st-lrwan1:sx1276",
            "DEVEUI": "70B3D57ED005EA59",
            "APPKEY": "385794DDE70CE2EAB5B5B12A4807822C",
            "APPEUI": "0000000000000000",
            "SITE": "saclay",
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


@dataclass(init=False)
class Node:
    """
    Class for all relevant node info. To be populated as information becomes available
    """

    # IoT-lab info
    node_id: Optional[str] = None
    board_id: Optional[str] = None
    radio_chipset: Optional[str] = None
    site: Optional[str] = None
    profile: Optional[str] = None

    # RIOT info
    riot_board: Optional[str] = None
    deveui: Optional[str] = None
    appeui: Optional[str] = None
    appkey: Optional[str] = None

    @property
    def network_address(self):
        return f"{self.node_id}.{self.site}.iot-lab.info"

    @network_address.setter
    def network_address(self, value):
        self.node_id, self.site, *_ = value.split(".")

    @property
    def archi(self):
        return f"{self.board_id}:{self.radio_chipset}"

    @archi.setter
    def archi(self, value):
        self.board_id, self.radio_chipset = value.split(":")

    @property
    def oml_name(self):
        return f"{self.node_id.replace('-', '_')}.oml"

    @property
    def node_id_number(self):
        return self.node_id.split("-")[-1]


# IoT-lab info
## load nodes. at this point we dont know their ids yet
nodes: List[Node] = []
for node in EXPERIMENT_CONFIG["NODES"]:
    no = Node()
    no.profile = node["PROFILE"]
    no.riot_board = node["RIOT_BOARD"]
    no.archi = node["IOT-LAB_BOARD"]
    no.deveui = node["DEVEUI"]
    no.appeui = node["APPEUI"]
    no.appkey = node["APPKEY"]
    no.site = node["SITE"]
    nodes.append(no)

assert all([node.site for node in nodes])  # make sure all nodes have a site
sites: set[str] = {node.site for node in nodes}  # type: ignore[misc] # we know after this that all nodes have a site
site_to_site_urls = {site: f"{site}.iot-lab.info" for site in sites}
USER = EXPERIMENT_CONFIG["USER"]

# RIOT info
SRC_PATH = Path.cwd() / "src"

## some nice functions


# make firmwares
# make and upload binaries
if not args.dont_make:
    print("make firmware")
    for device in EXPERIMENT_CONFIG["NODES"]:
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
if (
    subprocess.run(
        [
            "ssh",
            "-oNumberOfPasswordPrompts=0",
            f"{USER}@saclay.iot-lab.info",
            "hostname",
        ]
    ).returncode
    != 0
):
    print(
        f"Passwordless SSH access to the ssh front-end is required.\ncant log in to {USER}@saclay.iot-lab.info without password. Check if ssh-agent is running and if not run 'eval $(ssh-agent);ssh-add' to start the agent and add your key"
    )
    exit()

if not args.attach:
    print("Registering experiment")
    # create experiment
    ## create arguments for nodes
    node_strings = []
    for device in EXPERIMENT_CONFIG["NODES"]:
        node_strings.extend(
            [
                "-l",
                f"1,archi={device['IOT-LAB_BOARD']}+site={device['SITE']},,{device['PROFILE']}",
            ]
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
DB_PATH = EXPERIMENT_FOLDER / f"{EXPERIMENT}.duckdb"
print(f"Create DuckDB for experiment data at {str(DB_PATH)}")
# Create DB for results
create_sql_script = open("./experiment.db.sql").read()
db_con = duckdb.connect(f"{str(DB_PATH)}")

db_con.execute(create_sql_script)

print("Populating sites table")
db_con.executemany("INSERT INTO sites (name) VALUES (?)", [sites])

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

# go through nodes returned and populate internal node list with proper addresses
for node in json.loads(p.stdout)["items"]:
    try:
        next(
            filter(lambda n: n.archi == node["archi"] and n.node_id is None, nodes)
        ).network_address = node["network_address"]
    except StopIteration:
        print("Node not found while populating internal node table")
        exit()


db_con.begin()
for node in nodes:
    db_con.execute(
        "INSERT INTO nodes (node_deveui,node_appeui,node_appkey,board_id,radio_chipset,site,profile,riot_board) VALUES (?,?,?,?,?,?,?,?)",
        (
            node.deveui,
            node.appeui,
            node.appkey,
            node.board_id,
            node.radio_chipset,
            node.site,
            node.profile,
            node.riot_board,
        ),
    )
db_con.commit()

if not args.dont_upload:
    # get .bin files from experiment folder
    flash_files = [flash_file for flash_file in EXPERIMENT_FOLDER.glob("*.elf")]

    ## sanity check that number of files correspond with number of nodes in config and number of created nodes in iot-lab
    assert (
        len(flash_files) == len(nodes) == len(EXPERIMENT_CONFIG["NODES"])
    ), f"Number of flash file ({len(flash_files)}) does not match number of nodes in config ({len(EXPERIMENT_CONFIG['NODES'])}) or number of created nodes in iot-lab ({len(nodes)})"

    for flash_file, node in zip(flash_files, nodes):
        # construct nodelist for single node
        node_string = f"{node.site},{node.board_id},{node.node_id_number}"

        # find correct flash file
        try:
            flash_file = next(
                filter(lambda f: f.stem.lower() == node.deveui.lower(), flash_files)
            )
        except StopIteration:
            print(f"Flash file for node {node.deveui} not found in experiment folder")
            exit()

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

# serial aggregation
serial_count = 0

nodes_by_id = {node.node_id: node for node in nodes}
nodes_by_oml_name = {node.oml_name: node for node in nodes}


async def serial_aggregation_coroutine(site_url):
    global serial_count
    print("Starting serial aggregation collection")
    p = await asyncio.create_subprocess_exec(
        "ssh",
        f"{USER}@{site_url}",
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
        node = nodes_by_id[record[1]]
        msg = record[2]
        db_con.execute(
            "INSERT INTO traces (node_id, timestamp, message) VALUES (?,?,?)",
            (node.node_id, time_us, msg),
        )
        serial_count += 1


## power consumption metrics
power_consumption_count = 0


async def power_consumption_coroutine(site_url, oml_files):
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
        f"{USER}@{site_url}",
        "--workdir",
        f"/senslab/users/berthels/.iot-lab/{EXPERIMENT}/consumption",
        "tail -F",
        ":::",
        *oml_files,
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
        record = matcher.match(line)
        if record is None:
            continue
        timestamp = int(int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"]))
        db_con.execute(
            "INSERT INTO power_consumptions (node_id, timestamp, voltage, current, power) VALUES (?, ?, ?, ?, ?)",
            (
                nodes_by_oml_name[record["node_name"]].deveui,
                datetime.fromtimestamp(timestamp / 1e6),
                float(record["voltage"]),
                float(record["current"]),
                float(record["power"]),
            ),
        )
        power_consumption_count += 1


radio_count = 0


async def radio_coroutine(site_url, oml_files):
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
        f"{USER}@{site_url}",
        "--workdir",
        f"/senslab/users/berthels/.iot-lab/{EXPERIMENT}/radio",
        "tail -F",
        ":::",
        *oml_files,
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
        timestamp = int(int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"]))
        db_con.execute(
            "INSERT INTO radios (node_id, timestamp, channel, rssi) VALUES (?, ?, ?, ?)",
            (
                nodes_by_oml_name[record["node_name"]].deveui,
                datetime.fromtimestamp(timestamp / 1e6),
                int(record["channel"]),
                int(record["rssi"]),
            ),
        )
        radio_count += 1


MQTT_CONFIG = EXPERIMENT_CONFIG["MQTT"]


async def mqtt_coroutine():
    async with aiomqtt.Client(
        hostname=MQTT_CONFIG["ADDRESS"],
        port=8883,
        username=MQTT_CONFIG["USERNAME"],
        password=MQTT_CONFIG["PASSWORD"],
        clean_session=False,
    ) as client:
        async with client.messages() as messages:
            await client.subscribe(MQTT_CONFIG["TOPIC"], qos=2)
            async for msg in messages:
                # region topics:
                # v3/{application id}@{tenant id}/devices/{device id}/join
                # v3/{application id}@{tenant id}/devices/{device id}/up
                # v3/{application id}@{tenant id}/devices/{device id}/down/queued
                # v3/{application id}@{tenant id}/devices/{device id}/down/sent
                # v3/{application id}@{tenant id}/devices/{device id}/down/ack
                # v3/{application id}@{tenant id}/devices/{device id}/down/nack
                # v3/{application id}@{tenant id}/devices/{device id}/down/failed
                # v3/{application id}@{tenant id}/devices/{device id}/service/data
                # v3/{application id}@{tenant id}/devices/{device id}/location/solved
                # endregion
                parsed_msg = json.loads(msg.payload.decode("utf-8"))
                msg_topic = msg.topic.decode("utf-8")

                # formats taken from https://www.thethingsindustries.com/docs/the-things-stack/concepts/data-formats/
                ## insert the message into the database. we wrap with a transaction
                db_con.begin()
                match msg_topic.split("")[4:]:
                    case ["join"]:
                        # region example
                        # {
                        #     "end_device_ids" : {
                        #         "device_id" : "dev1",                      // Device ID
                        #         "application_ids" : {
                        #         "application_id" : "app1"                  // Application ID
                        #         },
                        #         "dev_eui" : "0004A30B001C0530",            // DevEUI of the end device
                        #         "join_eui" : "800000000000000C",           // JoinEUI of the end device (also known as AppEUI in LoRaWAN versions below 1.1)
                        #         "dev_addr" : "00BCB929"                    // Device address known by the Network Server
                        #     },
                        #     "correlation_ids" : [ "as:up:01..." ],         // Correlation identifiers of the message
                        #     "received_at" : "2020-02-12T15:15..."          // ISO 8601 UTC timestamp at which the message has been received by the Application Server
                        #     "join_accept" : {
                        #         "session_key_id" : "AXBSH1Pk6Z0G166...",   // Join Server issued identifier for the session keys
                        #         "received_at" : "2020-02-17T07:49..."      // ISO 8601 UTC timestamp at which the uplink has been received by the Network Server
                        #     }
                        # }
                        # endregion

                        ## extract relevant info from mqtt payload
                        dev_eui = parsed_msg["end_device_ids"]["dev_eui"]
                        app_recieved_at = datetime.fromisoformat(
                            parsed_msg["received_at"]
                        )
                        network_recieved_at = datetime.fromisoformat(
                            parsed_msg["join_accept"]["received_at"]
                        )

                        db_con.execute(
                            "INSERT INTO Message (related_node, network_recieved_at) VALUES (?,?) RETURNING message_id",
                            (dev_eui, network_recieved_at),
                        )
                        message_id = db_con.fetchone()[0]
                        db_con.execute(
                            "INSERT INTO Join_Message (join_message_id, app_received_at) VALUES (?,?)",
                            (message_id, app_recieved_at),
                        )

                    case ["up"]:
                        # region example
                        # {
                        # "end_device_ids" : {
                        #     "device_id" : "dev1",                    // Device ID
                        #     "application_ids" : {
                        #     "application_id" : "app1"              // Application ID
                        #     },
                        #     "dev_eui" : "0004A30B001C0530",          // DevEUI of the end device
                        #     "join_eui" : "800000000000000C",         // JoinEUI of the end device (also known as AppEUI in LoRaWAN versions below 1.1)
                        #     "dev_addr" : "00BCB929"                  // Device address known by the Network Server
                        # },
                        # "correlation_ids" : [ "as:up:01...", ... ],// Correlation identifiers of the message
                        # "received_at" : "2020-02-12T15:15..."      // ISO 8601 UTC timestamp at which the message has been received by the Application Server
                        # "uplink_message" : {
                        #     "session_key_id": "AXA50...",            // Join Server issued identifier for the session keys used by this uplink
                        #     "f_cnt": 1,                              // Frame counter
                        #     "f_port": 1,                             // Frame port
                        #     "frm_payload": "gkHe",                   // Frame payload (Base64)
                        #     "decoded_payload" : {                    // Decoded payload object, decoded by the device payload formatter
                        #     "temperature": 1.0,
                        #     "luminosity": 0.64
                        #     },
                        #     "rx_metadata": [{                        // A list of metadata for each antenna of each gateway that received this message
                        #     "gateway_ids": {
                        #         "gateway_id": "gtw1",                // Gateway ID
                        #         "eui": "9C5C8E00001A05C4"            // Gateway EUI
                        #     },
                        #     "time": "2020-02-12T15:15:45.787Z",    // ISO 8601 UTC timestamp at which the uplink has been received by the gateway
                        #     "timestamp": 2463457000,               // Timestamp of the gateway concentrator when the message has been received
                        #     "rssi": -35,                           // Received signal strength indicator (dBm)
                        #     "channel_rssi": -35,                   // Received signal strength indicator of the channel (dBm)
                        #     "snr": 5.2,                            // Signal-to-noise ratio (dB)
                        #     "uplink_token": "ChIKEA...",           // Uplink token injected by gateway, Gateway Server or fNS
                        #     "channel_index": 2                     // Index of the gateway channel that received the message
                        #     "location": {                          // Gateway location metadata (only for gateways with location set to public)
                        #         "latitude": 37.97155556731436,       // Location latitude
                        #         "longitude": 23.72678801175413,      // Location longitude
                        #         "altitude": 2,                       // Location altitude
                        #         "source": "SOURCE_REGISTRY"          // Location source. SOURCE_REGISTRY is the location from the Identity Server.
                        #     }
                        #     }],
                        #     "settings": {                            // Settings for the transmission
                        #     "data_rate": {                         // Data rate settings
                        #         "lora": {                            // LoRa modulation settings
                        #         "bandwidth": 125000,               // Bandwidth (Hz)
                        #         "spreading_factor": 7              // Spreading factor
                        #         }
                        #     },
                        #     "coding_rate": "4/6",                  // LoRa coding rate
                        #     "frequency": "868300000",              // Frequency (Hz)
                        #     },
                        #     "received_at": "2020-02-12T15:15..."     // ISO 8601 UTC timestamp at which the uplink has been received by the Network Server
                        #     "consumed_airtime": "0.056576s",         // Time-on-air, calculated by the Network Server using payload size and transmission settings
                        #     "locations": {                           // End device location metadata
                        #     "user": {
                        #         "latitude": 37.97155556731436,       // Location latitude
                        #         "longitude": 23.72678801175413,      // Location longitude
                        #         "altitude": 10,                      // Location altitude
                        #         "source": "SOURCE_REGISTRY"          // Location source. SOURCE_REGISTRY is the location from the Identity Server.
                        #     }
                        #     },
                        #     "version_ids": {                          // End device version information
                        #         "brand_id": "the-things-products",    // Device brand
                        #         "model_id": "the-things-uno",         // Device model
                        #         "hardware_version": "1.0",            // Device hardware version
                        #         "firmware_version": "quickstart",     // Device firmware version
                        #         "band_id": "EU_863_870"               // Supported band ID
                        #     },
                        #     "network_ids": {                          // Network information
                        #     "net_id": "000013",                     // Network ID
                        #     "tenant_id": "tenant1",                 // Tenant ID
                        #     "cluster_id": "eu1"                     // Cluster ID
                        #     }
                        # },
                        # "simulated": true,                         // Signals if the message is coming from the Network Server or is simulated.
                        # }
                        # endregion
                        ## extract needed info from mqtt payload
                        dev_eui = parsed_msg["end_device_ids"]["dev_eui"]
                        app_recieved_at = datetime.fromisoformat(
                            parsed_msg["received_at"]
                        )
                        gateway_recieved_at = datetime.fromisoformat(
                            parsed_msg["uplink_message"]["time"]
                        )
                        network_recieved_at = datetime.fromisoformat(
                            parsed_msg["uplink_message"]["received_at"]
                        )
                        frame_counter = parsed_msg["uplink_message"]["f_cnt"]
                        frame_port = parsed_msg["uplink_message"]["f_port"]
                        frame_payload = parsed_msg["uplink_message"]["frm_payload"]
                        gateway_deveui = parsed_msg["uplink_message"]["gateway_ids"][
                            "eui"
                        ]
                        rssi = parsed_msg["uplink_message"]["rssi"]
                        snr = parsed_msg["uplink_message"]["snr"]
                        bandwidth = parsed_msg["uplink_message"]["data_rate"]["lora"][
                            "bandwidth"
                        ]
                        spreading_factor = f'SF{parsed_msg["uplink_message"]["data_rate"]["lora"]["spreading_factor"]}'
                        frequency = parsed_msg["uplink_message"]["frequency"]
                        coding_rate = parsed_msg["uplink_message"]["coding_rate"]
                        consumed_airtime_s = parsed_msg["uplink_message"][
                            "consumed_airtime"
                        ]

                        # add to db
                        db_con.begin()
                        ## create message
                        db_con.execute(
                            "INSERT INTO Message (related_node, network_recieved_at) VALUES (?,?) RETURNING message_id",
                            (dev_eui, network_recieved_at),
                        )
                        message_id = db_con.fetchone()[0]
                        ## create content_message
                        db_con.execute(
                            "INSERT INTO Content_Message VALUES (?,?,?,?) RETURNING content_message_id",
                            (message_id, frame_counter, frame_port, frame_payload),
                        )
                        content_message_id = db_con.fetchone()[0]
                        ## create uplink_message
                        db_con.execute(
                            "INSERT INTO Uplink_Message VALUES (?,?,?,?,?,?,?,?,?,?,?)",
                            (
                                content_message_id,
                                gateway_deveui,
                                gateway_recieved_at,
                                app_recieved_at,
                                rssi,
                                snr,
                                bandwidth,
                                frequency,
                                consumed_airtime_s,
                                spreading_factor,
                                coding_rate,
                            ),
                        )
                        db_con.commit()
                    case ["down", "failed"]:
                        # region example
                        # {
                        # "end_device_ids" : {
                        #     "device_id" : "dev1",                    // Device ID
                        #     "application_ids" : {
                        #     "application_id" : "app1"              // Application ID
                        #     }
                        # },
                        # "correlation_ids" : [ "as:downlink:..." ], // Correlation identifiers of the message
                        # "downlink_failed" : {
                        #     "downlink" : {                           // Downlink that which triggered the failure
                        #     "f_port" : 15,                         // Frame port
                        #     "frm_payload" : "YWFhYWFhY...",        // Frame payload (Base64)
                        #     "confirmed" : true,                    // If the downlink expects a confirmation from the device or not
                        #     "priority" : "NORMAL",                 // Priority of the message in the downlink queue
                        #     "correlation_ids" : [ "as:downli..." ] // Correlation identifiers of the message
                        #     },
                        #     "error" : {                              // Error that was encountered while sending the downlink
                        #     "namespace" : "pkg/networkserver",     // Component in which the error occurred
                        #     "name" : "application_downlink_to...", // Error ID
                        #     "message_format" : "application ...",  // Error message
                        #     "correlation_id" : "2e7f786912e94...", // Correlation identifiers of the error
                        #     "code" : 3                             // gRPC error code
                        #     }
                        # }
                        # }
                        # endregion

                        # Extract needed info from mqtt payload
                        error_namespace = parsed_msg["downlink_failed"]["error"][
                            "namespace"
                        ]
                        error_id = parsed_msg["downlink_failed"]["error"]["name"]
                        error_message = parsed_msg["downlink_failed"]["error"][
                            "message_format"
                        ]
                        error_code = parsed_msg["downlink_failed"]["error"]["code"]
                        ## find the related Downlink event message
                        error_correlation_ids = parsed_msg["correlation_ids"]
                        ##TODO: verify that this actually works. Maybe correlation_ids use many ids and only 1 of them is relevant
                        db_con.execute(
                            "SELECT downlink_event_message_id FROM Downlink_Event_Message WHERE list_contains(correlation_ids, ?)",
                            (error_correlation_ids[0]),
                        )
                        downlink_event_message_id = db_con.fetchone()[0]

                        ## create the downlink_event_error message
                        db_con.execute(
                            "INSERT INTO Downlink_Event_Error_Message VALUES (?,?,?,?,?)",
                            (
                                downlink_event_message_id,
                                error_namespace,
                                error_id,
                                error_message,
                                error_code
                            ),
                        )

                    case ["down", down_type]:
                        # region example
                        # {
                        # "end_device_ids" : {
                        #     "device_id" : "dev1",                    // Device ID
                        #     "application_ids" : {
                        #     "application_id" : "app1"              // Application ID
                        #     },
                        #     "dev_eui" : "0004A30B001C0530",          // DevEUI of the end device
                        #     "join_eui" : "800000000000000C",         // JoinEUI of the end device (also known as AppEUI in LoRaWAN versions below 1.1)
                        #     "dev_addr" : "00BCB929"                  // Device address known by the Network Server
                        # },
                        # "correlation_ids" : [ "as:downlink:..." ], // Correlation identifiers of the message
                        # "received_at" : "2020-02-17T10:32:24...",  // ISO 8601 UTC timestamp at which the message has been received by the Network Server
                        # "downlink_queued" : {                      // Name of the event (ack, nack, queued or sent)
                        #     "session_key_id" : "AXBSH1Pk6Z0G166...", // Join Server issued identifier for the session keys
                        #     "f_port" : 15,                           // Frame port
                        #     "f_cnt" : 1,                             // Frame counter
                        #     "frm_payload" : "vu8=",                  // Frame payload (Base64)
                        #     "confirmed" : true,                      // If the downlink expects a confirmation from the device or not
                        #     "priority" : "NORMAL",                   // Priority of the message in the downlink queue
                        #     "correlation_ids" : [ "as:downlink..." ] // Correlation identifiers of the message
                        # }
                        # }

                        # endregion

                        ## get needed info from mqtt payload
                        dev_eui = parsed_msg["end_device_ids"]["dev_eui"]
                        network_recieved_at = datetime.fromisoformat(
                            parsed_msg["received_at"]
                        )
                        downlink_key = f"downlink_{down_type}"
                        frame_port: int = parsed_msg[downlink_key]["f_port"]
                        frame_counter: int = parsed_msg[downlink_key]["f_cnt"]
                        frame_payload: str = parsed_msg[downlink_key]["frm_payload"]
                        confirmed: bool = parsed_msg[downlink_key]["confirmed"]
                        priority: str = parsed_msg[downlink_key]["priority"]
                        correlation_ids: list[str] = parsed_msg["correlation_ids"]

                        # add to db
                        ## add message
                        db_con.execute(
                            "INSERT INTO Message (related_node, network_recieved_at) VALUES (?,?) RETURNING message_id",
                            (dev_eui, network_recieved_at),
                        )
                        message_id = db_con.fetchone()[0]

                        ## add content message
                        db_con.execute(
                            "INSERT INTO Content_Message VALUES (?,?,?,?) RETURNING content_message_id",
                            (message_id, frame_counter, frame_port, frame_payload),
                        )
                        content_message_id = db_con.fetchone()[0]

                        ## add downlink event message
                        db_con.execute(
                            "INSERT INTO Downlink_Event_Message VALUES (?,?,?,?,?)",
                            (
                                content_message_id,
                                confirmed,
                                down_type,
                                priority,
                                correlation_ids,
                            ),
                        )
                    case ["service", "data"]:
                        pass
                    case ["location", "solved"]:
                        pass

                db_con.commit()


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


async def commit():
    while True:
        await asyncio.sleep(1)
        db_con.commit()


## reset all boards
# subprocess.run(["iotlab", "node", "--reset"], check=True)


# run all data collection tasks and await their completion
async def main():
    print("starting data collection")
    # build list of tasks
    tasks = []
    nodes_by_site = {
        site: list(filter(lambda n: n.site == site, nodes)) for site in sites
    }
    for site, ns in nodes_by_site.items():
        tasks.append(serial_aggregation_coroutine(site_to_site_urls[site]))
        tasks.append(
            power_consumption_coroutine(
                site_to_site_urls[site], [n.oml_name for n in ns]
            )
        )
        tasks.append(radio_coroutine(site_to_site_urls[site], [n.oml_name for n in ns]))
    tasks.append(print_progress())
    tasks.append(commit())
    await asyncio.gather(*tasks)


asyncio.run(main())
