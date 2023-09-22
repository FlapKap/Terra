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
from importlib import resources as impressources
CREATE_SQL = impressources.open_text("experiment_runner", "experiment.db.sql").readlines()
from . import configuration_parser

parser = argparse.ArgumentParser(description="Experiment test runner")
parser.add_argument("--attach", action="store_true", help="attach to the experiment")
parser.add_argument("--dont-make", action="store_true", help="dont make binaries")
parser.add_argument("--dont-upload", action="store_true", help="dont upload binaries")
parser.add_argument("--dont-run", action="store_true", help="don't run the experiment")
parser.add_argument("-c", "--config", type=Path, default=Path.cwd() / "experiment.json", help="Path to configuration file. Default: experiment.json")
parser.add_argument(
    "experiment_folder",
    type=Path,
    help="folder containing to contain binaries and database files",
)
args = parser.parse_args()



EXPERIMENT_FOLDER: Path = args.experiment_folder
EXPERIMENT_CONFIG_PATH = args.config
CONFIG = configuration_parser.configuration_from_json(
    EXPERIMENT_CONFIG_PATH.read_text()
)

assert all([node.site for node in CONFIG.nodes])  # make sure all nodes have a site
sites: set[str] = {node.site for node in CONFIG.nodes}  # type: ignore[misc] # we know after this that all nodes have a site
USER = CONFIG.user

# RIOT info
SRC_PATH = Path.cwd() / "src"

## some nice functions


# make firmwares
# make and upload binaries
if not args.dont_make:
    print("make firmware")
    for node in CONFIG.nodes:
        # first make binary
        print(f"make binary for device: {node.deveui}")
        env = os.environ.copy()
        env["BOARD"] = node.riot_board
        env["DEVEUI"] = node.deveui
        env["APPEUI"] = node.appeui
        env["APPKEY"] = node.appkey
        env["SENSOR_NAMES"] = " ".join([sensor.name for sensor in node.sensors])
        env["SENSOR_TYPES"] = " ".join([sensor.type for sensor in node.sensors])

        # make
        p = subprocess.run(["make", "all"], cwd=SRC_PATH, env=env)

        ## find flash file
        p = subprocess.run(
            ["make", "info-build-json"], cwd=SRC_PATH, env=env, capture_output=True
        )
        build_info = json.loads(p.stdout)
        flash_file = Path(build_info["FLASHFILE"])
        copy(flash_file, EXPERIMENT_FOLDER / f"{node.deveui}{flash_file.suffix}")

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
    print("Registering experiment... ", end="")
    # create experiment
    ## create arguments for nodes
    node_strings = []
    for node in CONFIG.nodes:
        node_strings.extend(
            [
                "-l",
                f"1,archi={node.archi}+site={node.site},,{node.profile}",
            ]
        )
    p = subprocess.run(
        [
            "iotlab",
            "experiment",
            "submit",
            "-d",
            CONFIG.duration,
            *node_strings,
        ],
        capture_output=True,
        check=True,
    )
    output = p.stdout.decode("utf-8")
    exp_id = json.loads(output).get("id")
    if exp_id != None:
        print(f"Sucess! Got id {exp_id}")
    else:
        print(output)

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
# Check if file already exists
if DB_PATH.exists():
    answer = input(
        f"Database already exists at {str(DB_PATH)}. Do you want us to delete it? [y/n] "
    )
    if answer == "y":
        DB_PATH.unlink()
    else:
        exit()
# Create DB for results

create_sql_script = open("./experiment.db.sql").read()
db_con = duckdb.connect(f"{str(DB_PATH)}")

db_con.execute(create_sql_script)

print("Populating sites table")
db_con.executemany("INSERT INTO Site (name) VALUES (?)", [sites])

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
            filter(lambda n: n.archi == node["archi"] and n.node_id is None, CONFIG.nodes)
        ).network_address = node["network_address"]
    except StopIteration:
        print("Node not found while populating internal node table")
        exit()


db_con.begin()
for node in CONFIG.nodes:
    db_con.execute(
        "INSERT INTO Node (node_deveui,node_appeui,node_appkey,board_id,radio_chipset,node_site,profile,riot_board) VALUES (?,?,?,?,?,?,?,?)",
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
        len(flash_files)  == len(CONFIG.nodes)
    ), f"Number of flash file ({len(flash_files)}) does not match number of nodes in config ({len(CONFIG.nodes)}) or number of created nodes in iot-lab ({len(nodes)})"

    for flash_file, node in zip(flash_files, CONFIG.nodes):
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
        print(
            f"uploading binary file {str(flash_file)} to node_string {node_string}...",
            end="",
            flush=True,
        )
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
            capture_output=True,
            check=True,
        )
        ## check to see if we succeeded
        try:
            if json.loads(p.stdout.decode("utf-8"))["0"][0] == node.network_address:
                print("success")
        except:
            print("failed")
            exit()


# serial aggregation
serial_count = 0

nodes_by_id = {node.node_id: node for node in CONFIG.nodes}
nodes_by_oml_name = {node.oml_name: node for node in CONFIG.nodes}


async def serial_aggregation_coroutine(site_url):
    p = None  # initialize p to None so we in finally can check and terminate if it has been set
    try:
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

            time_unix_s = float(record[0])
            node = nodes_by_id[record[1]]
            msg = record[2]
            db_con.execute(
                "INSERT INTO Trace (node_id, timestamp, message) VALUES (?,?,?)",
                (node.deveui, datetime.fromtimestamp(time_unix_s), msg),
            )
            serial_count += 1
    finally:
        print("Stopping serial aggregation collection")
        if p:
            p.terminate()


## power consumption metrics
power_consumption_count = 0


async def power_consumption_coroutine(site_url, oml_files):
    p = None  # initialize p to None so we in finally can check and terminate if it has been set
    try:
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
            timestamp = int(
                int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"])
            )
            db_con.execute(
                "INSERT INTO Power_Consumption (node_id, timestamp, voltage, current, power) VALUES (?, ?, ?, ?, ?)",
                (
                    nodes_by_oml_name[record["node_name"]].deveui,
                    datetime.fromtimestamp(timestamp / 1e6),
                    float(record["voltage"]),
                    float(record["current"]),
                    float(record["power"]),
                ),
            )
            power_consumption_count += 1
    finally:
        print("Stopping power consumption collection")
        if p:
            p.terminate()


radio_count = 0


async def radio_coroutine(site_url, oml_files):
    p = None  # initialize p to None so we in finally can check and terminate if it has been set
    try:
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
            timestamp = int(
                int(record["timestamp_s"]) * 1e6 + int(record["timestamp_us"])
            )
            db_con.execute(
                "INSERT INTO Radio (node_id, timestamp, channel, rssi) VALUES (?, ?, ?, ?)",
                (
                    nodes_by_oml_name[record["node_name"]].deveui,
                    datetime.fromtimestamp(timestamp / 1e6),
                    int(record["channel"]),
                    int(record["rssi"]),
                ),
            )
            radio_count += 1
    finally:
        print("Stopping radio collection")
        if p:
            p.terminate()



async def mqtt_coroutine():
    print("starting mqtt collection")

    def from_str_to_datetime(s):
        ## annoyingly fromisoformat does not support arbitrary iso 8601 formatted strings
        ## so we have to manually strip some information and convert Z into +00:00
        ## this is fixed in python 3.11 but we use 3.10

        return datetime.fromisoformat(s[:26] + "+00:00")

    async with aiomqtt.Client(
        hostname=CONFIG.mqtt.address,
        port=CONFIG.mqtt.port,
        username=CONFIG.mqtt.username,
        password=CONFIG.mqtt.password,
        clean_session=False,
        client_id=str(EXPERIMENT),
    ) as client:
        async with client.messages() as messages:
            await client.subscribe(CONFIG.mqtt.topic, qos=2)
            print("subscribed to topic", CONFIG.mqtt.topic)

            async for msg in messages:
                print(msg.topic, msg.payload.decode("utf-8"))
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
                msg_topic = msg.topic.value
                ## skip if simulated
                if parsed_msg.get("simulated"):
                    continue
                # formats taken from https://www.thethingsindustries.com/docs/the-things-stack/concepts/data-formats/
                ## insert the message into the database. we wrap with a transaction
                db_con.begin()
                match msg_topic.split("/")[4:]:
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
                        app_received_at = from_str_to_datetime(
                            parsed_msg["received_at"]
                        )
                        network_received_at = from_str_to_datetime(
                            parsed_msg["join_accept"]["received_at"]
                        )

                        db_con.execute(
                            "INSERT INTO Message (related_node, network_received_at) VALUES (?,?) RETURNING message_id",
                            (dev_eui, network_received_at),
                        )
                        message_id = db_con.fetchone()[0]
                        db_con.execute(
                            "INSERT INTO Join_Message (join_message_id, app_received_at) VALUES (?,?)",
                            (message_id, app_received_at),
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
                        app_recieved_at = from_str_to_datetime(
                            parsed_msg["received_at"]
                        )
                        gateway_recieved_at = from_str_to_datetime(
                            parsed_msg["uplink_message"]["rx_metadata"][0]["time"]
                        )
                        network_recieved_at = from_str_to_datetime(
                            parsed_msg["uplink_message"]["rx_metadata"][0][
                                "received_at"
                            ]
                        )
                        frame_counter = (
                            parsed_msg["uplink_message"]["f_cnt"]
                            if parsed_msg["uplink_message"].get("f_cnt")
                            else 0
                        )
                        frame_port = parsed_msg["uplink_message"]["f_port"]
                        frame_payload = parsed_msg["uplink_message"]["frm_payload"]
                        rx_metadata = parsed_msg["uplink_message"]["rx_metadata"][0]
                        gateway_deveui = rx_metadata["gateway_ids"]["gateway_id"]
                        rssi = rx_metadata["rssi"]
                        snr = rx_metadata["snr"]

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
                                error_code,
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

                        ## example is wrong? at least api seems to not provide dev_eui, join_eui, dev_addr and received_at.
                        ## we will work around it so far, but this is far from ideal
                        ## TTN is notified and a fix is coming in next release

                        ## get needed info from mqtt payload
                        dev_eui = (
                            parsed_msg["end_device_ids"]["device_id"]
                            .split("-")[1]
                            .upper()
                        )
                        network_received_at = datetime.now()
                        downlink_key = f"downlink_{down_type}"
                        frame_port: int = parsed_msg[downlink_key]["f_port"]
                        frame_counter: int = (
                            -1
                        )  ## TODO: fix. right now f_cnt is missing from the api response
                        # frame_counter: int = parsed_msg[downlink_key]["f_cnt"]
                        frame_payload: str = parsed_msg[downlink_key]["frm_payload"]
                        confirmed: bool = parsed_msg[downlink_key]["confirmed"]
                        priority: str = "NORMAL"  # TODO: as soon as priority is provided by API fix this
                        # priority: str = parsed_msg[downlink_key]["priority"]
                        correlation_ids: list[str] = parsed_msg["correlation_ids"]

                        # add to db
                        ## add message
                        db_con.execute(
                            "INSERT INTO Message (related_node, network_received_at) VALUES (?,?) RETURNING message_id",
                            (dev_eui, network_received_at),
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
            print("this shouldnt print")


async def print_progress():
    while True:
        # print("\n" * 3, end="")
        ## fetch stats from db
        nodes_count = db_con.execute("SELECT COUNT(*) FROM Node").fetchone()
        serial_count = db_con.execute("SELECT COUNT(*) FROM Trace").fetchone()
        power_consumption_count = db_con.execute(
            "SELECT COUNT(*) FROM Power_Consumption"
        ).fetchone()
        radio_count = db_con.execute("SELECT COUNT(*) FROM Radio").fetchone()
        sites = db_con.execute("SELECT * from Site").fetchall()
        mqtt_messages_count = db_con.execute("SELECT COUNT(*) FROM Message").fetchone()
        gateways_count = db_con.execute("SELECT COUNT(*) FROM Gateway").fetchone()
        # print stats
        output_strings = [
            f"Nodes: {nodes_count}",
            f"Sites: {str(sites)}",
            f"Traces: {serial_count}",
            f"Power Consumption: {power_consumption_count}",
            f"Radio: {radio_count}",
            f"Gateway: {gateways_count}",
            f"Messages: {mqtt_messages_count}",
        ]
        print(*output_strings)
        await asyncio.sleep(1)
    #    print("\033[F\033[K" * len(output_strings), end="")


async def commit():
    while True:
        await asyncio.sleep(5)
        db_con.execute("CHECKPOINT")  # for some reason this is needed


# run all data collection tasks and await their completion
async def main():
    print("starting data collection")
    # build list of tasks
    tasks = []
    nodes_by_site = {
        site: list(filter(lambda n: n.site == site, CONFIG.nodes)) for site in sites
    }
    for nodelist in nodes_by_site.values():
        site_url = nodelist[0].site_url
        tasks.append(serial_aggregation_coroutine(site_url))
        tasks.append(
            power_consumption_coroutine(
                site_url, [n.oml_name for n in nodelist]
            )
        )
        tasks.append(radio_coroutine(site_url, [n.oml_name for n in nodelist]))
        tasks.append(mqtt_coroutine())
    tasks.append(print_progress())
    # tasks.append(commit())
    group = asyncio.gather(
        *tasks,
    )
    ## reset all boards
    await asyncio.sleep(5)
    subprocess.run(["iotlab", "node", "--reset"], check=True)
    try:
        await group
    except Exception as e:
        print(e)
        for task in group:
            task.cancel()


asyncio.run(main())
