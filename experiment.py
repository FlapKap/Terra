#!/usr/bin/python3
from pathlib import Path
import subprocess
import json

DEVEUIS=["70B3D57ED005E88A", "70B3D57ED005EA55", "70B3D57ED005EA56", "70B3D57ED005EA57", "70B3D57ED005EA59"]
APPKEY="385794DDE70CE2EAB5B5B12A4807822C"
APPEUI="0000000000000000"

SRC_PATH = Path.cwd() / "src"

# find experiment
subprocess.run("iotlab experiment wait") #wait for experiment to start

#gets active experiments
p = subprocess.run("iotlab experiment get -e", capture_output=True)
EXPERIMENTS = json.loads(p.stdout)["Running"][0] #TODO: work with more than 1 experiment?


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
p = subprocess.run("iotlab experiment get -i 374209 -n", capture_output=True)
NODES = json.loads(p.stdout)


for deveui in DEVEUIS:
    #first make binary
