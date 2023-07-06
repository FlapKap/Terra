BEGIN TRANSACTION;
DROP TABLE IF EXISTS sites;
DROP SEQUENCE IF EXISTS seq_sites_id;
CREATE SEQUENCE seq_sites_id START 1;
CREATE TABLE IF NOT EXISTS sites (
	site_id	INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('seq_sites_id'),
	name	VARCHAR NOT NULL,
);

DROP TABLE IF EXISTS nodes;
CREATE TABLE IF NOT EXISTS nodes (
	node_deveui	BIGINT PRIMARY KEY,
	name		VARCHAR NOT NULL,
	site		INTEGER NOT NULL REFERENCES sites(site_id),
);
-- radios table
DROP TABLE IF EXISTS radios;
DROP SEQUENCE IF EXISTS seq_radios_id;
CREATE SEQUENCE seq_radios_id START 1;
CREATE TABLE IF NOT EXISTS radios (
	radios_id	INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('seq_radios_id'),
	timestamp	TIMESTAMP NOT NULL,
	node_id		BIGINT NOT NULL REFERENCES nodes(node_deveui),
	channel		INTEGER NOT NULL,
	rssi		INTEGER NOT NULL
);
CREATE INDEX idx_radios_timestamp ON radios (timestamp);

-- traces table
DROP TABLE IF EXISTS traces;
DROP SEQUENCE IF EXISTS seq_traces_id;
CREATE SEQUENCE seq_traces_id START 1;
CREATE TABLE IF NOT EXISTS traces (
	traces_id		INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('seq_traces_id'),
	node_id			BIGINT NOT NULL REFERENCES nodes(node_deveui),
	timestamp		TIMESTAMP NOT NULL,
	message			VARCHAR
);
CREATE INDEX idx_traces_timestamp ON traces (timestamp);

-- power_consumptions table
DROP TABLE IF EXISTS power_consumptions;
DROP SEQUENCE IF EXISTS seq_power_consumptions_id;
CREATE SEQUENCE seq_power_consumptions_id START 1;
CREATE TABLE IF NOT EXISTS power_consumptions (
	power_consumptions_id	INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('seq_power_consumptions_id'),
	node_id					BIGINT NOT NULL REFERENCES nodes(node_deveui),
	timestamp				TIMESTAMP NOT NULL,
	voltage					REAL NOT NULL,
	current					REAL NOT NULL,
	power					REAL NOT NULL
);

CREATE INDEX idx_power_consumptions_timestamp ON power_consumptions (timestamp);

CREATE TYPE MessageDirection AS ENUM ('UPLINK', 'DOWNLINK');
CREATE TYPE SpreadingFactor AS ENUM ('SF7', 'SF8', 'SF9', 'SF10', 'SF11', 'SF12');
CREATE TYPE CodingRate AS ENUM ('4/5','4/6','5/7','4/8');

--gateways table
DROP TABLE IF EXISTS gateways;
DROP SEQUENCE IF EXISTS seq_gateways_id;
CREATE SEQUENCE seq_gateways_id START 1;
CREATE TABLE IF NOT EXISTS gateways (
	gateway_deveui		UBIGINT PRIMARY KEY NOT NULL DEFAULT nextval('seq_gateways_id')
);

-- messages table
DROP TABLE IF EXISTS messages;
DROP SEQUENCE IF EXISTS seq_messages_id;
CREATE SEQUENCE seq_messages_id START 1;
CREATE TABLE IF NOT EXISTS messages (
	messages_id			INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('seq_messages_id'),
	node_id				BIGINT NOT NULL REFERENCES nodes(node_deveui),
	timestamp			TIMESTAMP NOT NULL,
	direction			MessageDirection NOT NULL,
	rssi 				INTEGER NOT NULL,
	sni 				INTEGER NOT NULL,
	bandwidth			INTEGER NOT NULL,
	spreading_factor	SpreadingFactor NOT NULL,
	coding_rate			CodingRate NOT NULL,
	frequency			INTEGER NOT NULL,
	port 				TINYINT NOT NULL,
	payload				BLOB NOT NULL
);
CREATE INDEX idx_messages_timestamp ON messages (timestamp);

COMMIT;
