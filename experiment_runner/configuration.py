from dataclasses import dataclass
import json
from pathlib import Path
from typing import Optional, Any, List, TypeVar, Callable, Type, cast
import logging

T = TypeVar("T")


def from_str(x: Any) -> str:
    assert isinstance(x, str)
    return x


def from_none(x: Any) -> Any:
    assert x is None
    return x


def from_union(fs, x):
    for f in fs:
        try:
            return f(x)
        except:
            pass
    assert False


def from_int(x: Any) -> int:
    assert isinstance(x, int) and not isinstance(x, bool)
    return x


def from_list(f: Callable[[Any], T], x: Any) -> List[T]:
    assert isinstance(x, list)
    return [f(y) for y in x]


def to_class(c: Type[T], x: Any) -> dict:
    assert isinstance(x, c)
    return cast(Any, x).to_dict()


def is_type(t: Type[T], x: Any) -> T:
    assert isinstance(x, t)
    return x


@dataclass
class Mqtt:
    address: str
    port: int
    username: str
    password: str
    topic: str

    @staticmethod
    def from_dict(obj: Any) -> 'Mqtt':
        assert isinstance(obj, dict)
        address = from_str(obj.get("ADDRESS"))
        port = from_int(obj.get("PORT"))
        username = from_str(obj.get("USERNAME"))
        password = from_str(obj.get("PASSWORD"))
        topic = from_str(obj.get("TOPIC"))
        return Mqtt(address, port, username, password, topic)

    def to_dict(self) -> dict:
        return {
            "address": from_str(self.address),
            "port": from_int(self.port),
            "username": from_str(self.username),
            "password": from_str(self.password),
            "topic": from_str(self.topic)
        }


@dataclass
class Sensor:
    name: str
    type: str

    @staticmethod
    def from_dict(obj: Any) -> 'Sensor':
        assert isinstance(obj, dict)
        name = from_str(obj.get("NAME"))
        type = from_str(obj.get("TYPE"))
        return Sensor(name, type)

    def to_dict(self) -> dict:
        return {
            "NAME": self.name,
            "TYPE": self.type
        }


@dataclass
class Node:
    ttn_device_id: str
    riot_board: str
    iot_lab_board_id: str
    iot_lab_radio_chipset: str
    deveui: str
    appkey: str
    appeui: str
    site: str
    node_id: Optional[str] = None
    profile: Optional[str] = None
    sensors: Optional[List[Sensor]] = None
    firmware_path: Optional[Path] = None

    @staticmethod
    def from_dict(obj: Any) -> 'Node':
        assert isinstance(obj, dict)
        ttn_device_id = from_str(obj.get("TTN_DEVICE_ID"))
        profile = from_union([from_str, from_none], obj.get("PROFILE"))
        riot_board = from_str(obj.get("RIOT_BOARD"))
        iot_lab_board_id = from_str(obj.get("IOT-LAB_BOARD_ID"))
        iot_lab_radio_chipset = from_str(obj.get("IOT-LAB_RADIO_CHIPSET"))
        deveui = from_str(obj.get("DEVEUI"))
        appkey = from_str(obj.get("APPKEY"))
        appeui = from_str(obj.get("APPEUI"))
        site = from_str(obj.get("SITE"))
        sensors = from_list(Sensor.from_dict,obj.get("SENSORS"))
        node_id = from_union([from_str, from_none], obj.get("NODE_ID"))
        return Node(ttn_device_id,riot_board, iot_lab_board_id, iot_lab_radio_chipset, deveui, appkey, appeui, site, node_id, profile, sensors)

    def to_dict(self) -> dict:
        return {
        "TTN_DEVICE_ID": self.ttn_device_id,
        "PROFILE": from_union([from_str, from_none], self.profile) if self.profile is not None else None,
        "RIOT_BOARD": self.riot_board,
        "IOT-LAB_BOARD_ID": self.iot_lab_board_id,
        "IOT-LAB_RADIO_CHIPSET": self.iot_lab_radio_chipset,
        "DEVEUI": self.deveui,
        "APPKEY":self.appkey, 
        "APPEUI": self.appeui,
        "SITE": self.site,
        "SENSORS": from_list(lambda x: to_class(Sensor, x), self.sensors),
        "NODE_ID": from_union([from_str, from_none], self.node_id)
        }

    @property
    def network_address(self):
        return f"{self.node_id}.{self.site}.iot-lab.info"

    @network_address.setter
    def network_address(self, value):
        self.node_id, self.site, *_ = value.split(".")

    @property
    def site_url(self):
        return f"{self.site}.iot-lab.info"
    @property
    def archi(self):
        return f"{self.iot_lab_board_id}:{self.iot_lab_radio_chipset}"

    @archi.setter
    def archi(self, value):
        self.board_id, self.radio_chipset = value.split(":")

    @property
    def oml_name(self):
        if self.node_id is None:
            logging.debug(f"oml_name called when Node {self.deveui} has no node id")
            return None
        return f"{self.node_id.replace('-', '_')}.oml"

    @property
    def node_id_number(self):
        if self.node_id is None:
            logging.debug(f"node_id_number called when Node {self.deveui} has no node id")
            return None
        return self.node_id.split("-")[-1]

    @property
    def node_string_by_id(self):
        return f"{self.site},{self.iot_lab_board_id},{self.node_id_number}"

@dataclass
class Configuration:
    user: str
    duration: int
    mqtt: Mqtt
    nodes: List[Node]

    @staticmethod
    def from_dict(obj: Any) -> 'Configuration':
        assert isinstance(obj, dict)
        user = from_str(obj.get("USER"))
        duration = int(from_str(obj.get("DURATION")))
        mqtt = Mqtt.from_dict(obj.get("MQTT"))
        nodes = from_list(Node.from_dict, obj.get("NODES"))
        return Configuration(user, duration, mqtt, nodes)

    def to_dict(self) -> dict:
        result = {
            "USER": from_str(self.user),
            "DURATION": from_str(str(self.duration)),
            "MQTT": to_class(Mqtt, self.mqtt),
            "NODES": from_list(lambda x: to_class(Node, x), self.nodes)
        }
        return result

def configuration_from_dict(s: Any) -> Configuration:
    return Configuration.from_dict(s)

def configuration_to_dict(x: Configuration) -> Any:
    return to_class(Configuration, x)

def configuration_from_json(s: str) -> Configuration:
    return configuration_from_dict(json.loads(s))

def configuration_to_json(x: Configuration) -> str:
    return json.dumps(x, default=configuration_to_dict)