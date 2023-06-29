BEGIN TRANSACTION;
DROP TABLE IF EXISTS "nodes";
CREATE TABLE IF NOT EXISTS "nodes" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "radio";
CREATE TABLE IF NOT EXISTS "radio" (
	"id"	INTEGER,
	"timestamp"	INTEGER NOT NULL,
	"node_id"	INTEGER NOT NULL,
	"channel"	INTEGER NOT NULL,
	"RSSI"	INTEGER NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("node_id") REFERENCES "nodes"("id")
);
DROP TABLE IF EXISTS "metrics";
CREATE TABLE IF NOT EXISTS "metrics" (
	"id"	INTEGER,
	"node_id"	INTEGER NOT NULL,
	"timestamp"	INTEGER NOT NULL,
	"metric_string"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("node_id") REFERENCES "nodes"("id")
);
DROP TABLE IF EXISTS "power_consumption";
CREATE TABLE IF NOT EXISTS "power_consumption" (
	"id"	INTEGER,
	"node_id"	INTEGER NOT NULL,
	"timestamp"	INTEGER NOT NULL,
	"voltage"	REAL NOT NULL,
	"current"	REAL NOT NULL,
	"power"	REAL NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("node_id") REFERENCES "nodes"("id")
);
DROP INDEX IF EXISTS "timestamp_idx";
CREATE INDEX IF NOT EXISTS "timestamp_idx" ON "radio" (
	"timestamp"	ASC
);
COMMIT;
