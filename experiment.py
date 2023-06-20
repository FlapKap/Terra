#!/usr/bin/python3
from pathlib import Path
import subprocess
import json
import os
import re
import sys

DEVEUIS=["70B3D57ED005E88A", "70B3D57ED005EA55", "70B3D57ED005EA56", "70B3D57ED005EA57", "70B3D57ED005EA59"]
APPKEY="385794DDE70CE2EAB5B5B12A4807822C"
APPEUI="0000000000000000"

BOARD="b-l072z-lrwan1"
NUM_NODES=len(DEVEUIS)

SRC_PATH = Path.cwd() / "src"

# create experiment
subprocess.run(["iotlab", "experiment", "submit", "-d", "20", "-l", f"{NUM_NODES},archi=st-lrwan1:sx1276+site=saclay,,stm32Profile"],stdout=sys.stdout)

# find experiment
subprocess.run(["iotlab", "experiment", "wait"]) #wait for experiment to start

#gets active experiments
p = subprocess.run(["iotlab", "experiment", "get", "-e"], capture_output=True)
EXPERIMENT = json.loads(p.stdout)["Running"][0] #TODO: work with more than 1 experiment?


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
p = subprocess.run(["iotlab", "experiment", "get", "-i", str(EXPERIMENT), "-n"], capture_output=True)
NODES = json.loads(p.stdout)["items"]

for deveui, node_info in zip(DEVEUIS,NODES):
    #first make binary
    env = os.environ.copy()
    env["BOARD"] = BOARD
    env["DEVEUI"] = deveui
    env["APPEUI"] = APPEUI
    env["APPKEY"] = APPKEY

    # make
    p = subprocess.run(["make", "all"],cwd=SRC_PATH,env=env, capture_output=True)
    
    ## find flash file
    p = subprocess.run(["make","info-build-json"], cwd=SRC_PATH, env=env, capture_output=True)
    build_info = json.loads(p.stdout)
    flash_file = Path(build_info["FLASHFILE"])
    
    # construct nodelist for single node
    # might be easier with iotl"ab experiment get -d instead of -n
    node_id = re.match('.*-(\d+)\.',node_info['network_address']).group(1)

    node_string = f"{node_info['site']},{node_info['archi'].split(':')[0]},{node_id}"

    # upload
    print(node_string)
    p = subprocess.run(["iotlab", "node", "-i", str(EXPERIMENT),  "-l", node_string, "-fl", str(flash_file)], stdout=sys.stdout)