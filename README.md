<img alt="License" src="https://img.shields.io/badge/license-MIT-brightgreen.svg"/>

# NebulaStream End Device Runtime
An implementation of: [NebulaStream End Device Runtime]( https://github.com/FlapKap/nebulastream-ed-runtime) in C

## How to Build
The project is written as a [RIOT OS](https://github.com/RIOT-OS/RIOT/blob/master/examples/lorawan/main.c) application and uses the RIOT build system. Building requires a copy of the RIOT codebase either locally (linux only), in a docker container or on a virtual machine.

To build the binary you must set the following variables in the `Makefile`
- `RIOTBASE` The path to you RIOT code base
- `BOARD` The specific board that riot should build for. [List of boards supported by RIOT](https://doc.riot-os.org/group__boards.html). By default this is set to `native` which will build the executable for linux.
- `DEVEUI`, `APPEUI`and `APPKEY`. Settings for your specific LoRaWAN connection

To build the application, flash to a device, and open a terminal to the flashed device, run:

`$ make flash term`

If you wish to flash using an alternative programming tool than the default for your board, this can be done by setting the `PROGRAMMER` macro. For instance, to flash to the Nucleo-WL55JC using the cpy2remed (copy to removable media) script, use:

`$ make BOARD=nucleo-wl55jc PROGRAMMER=cpy2remed flash`

For more information see documentation on [The riot build system](https://doc.riot-os.org/flashing.html#flashing-general).

## LoRaWAN
The application can connect to The Things Network (TTN) over LoRaWAN. The following steps are required to set this up:

- Login to TTN and open console.
- Create a new application.
- Register an end device.
- Note the DEVEUI, JOINEUI (APPEUI) and APPKEY.
- Enter those values in the corresponding makefile macros

## Contributors
Laurits Bonde & Markus Johansen for our Bachelor Thesis @ ITU
