# RIOT Stuff
APPLICATION = NSED_Runtime
PROJECTBASE = $(CURDIR)
BOARD ?= b-l072z-lrwan1


# APPLICATION SPECIFIC CONFIGURATION ENTRIES
## EXECUTE INTERVAL
EXECUTION_INTERVAL_S ?= 20
## LORA
ACTIVATION_MODE ?= otaa # only otaa is supported
LORA_REGION ?= EU868
APPEUI ?= 0000000000000000
DEVEUI ?= 70B3D57ED005EA59
APPKEY ?= 385794DDE70CE2EAB5B5B12A4807822C
# change stack size
MAIN_STACKSIZE ?= 10480


# Different build configurations. Optimized builds run without any debug symbols and o3 optimizations
APPLICATION_BUILD_OPTIMIZED ?= 0

# Application run tests instead of the normal "main" function
APPLICATION_RUN_TEST ?= 0


# for native builds we want to disable lora
DEFAULT_QUERY ?= {.amount=1,.queries={{{CONST},0},{.data._int=2,INSTRUCTION_INT},{{VAR},0},{.data._int=0,INSTRUCTION_INT},{{MUL},0}}}

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

## Apply configuration
CFLAGS += -DEXECUTION_INTERVAL_S=$(EXECUTION_INTERVAL_S)
CFLAGS += -DTHREAD_STACKSIZE_MAIN=$(MAIN_STACKSIZE)


## RIOT CONFIGURATION
## We assume riot is placed next to this repo
RIOTBASE ?= $(CURDIR)/../../RIOT

# Set the compile_commands.json file to a known location
COMPILE_COMMANDS_PATH = $(CURDIR)/bin/compile_commands.json


# PACKAGES
#USEMODULE += saul_default
USEMODULE += fmt

## Nanopb stuff
USEPKG += nanopb
PROTOBUF_PATH = $(CURDIR)/proto

## Timer stuff
USEMODULE += ztimer
USEMODULE += ztimer_sec
USEMODULE += ztimer_msec

# features
FEATURES_OPTIONAL += periph_uart

ifeq (1,$(APPLICATION_BUILD_OPTIMIZED))
CFLAGS += -O3
DEVELHELP ?= 0
CREATE_STACKTEST ?= 0
else
# Debug stuff. We also want debug stuff to run when testing

CFLAGS += -g3 -O0 # unoptimized and with debug symbols
CFLAGS += -Wall -Werror -Wextra -Wformat -Wformat-nonliteral -Werror=format-security -Werror=array-bounds -Wpedantic # extra warnings
DEVELHELP ?= 1 # development helpers
CREATE_STACKTEST ?= 1 # check stacksizes
endif

ifeq (1, $(APPLICATION_RUN_TEST))
# Test stuff
CFLAGS += -DAPPLICATION_RUN_TEST
USEMODULE += embunit
endif

## handle native builds and builds without lora
ifeq ($(strip $(BOARD)),native)
DISABLE_LORA ?= 1
else
DISABLE_LORA ?= 0
endif

ifeq (1,$(DISABLE_LORA))
CFLAGS += -DDISABLE_LORA
CFLAGS += -DDEFAULT_QUERY=$(DEFAULT_QUERY)
endif

# Different board configurations. Native and hardware
## if board is not native we can load the LoRa Stack
ifeq ($(DISABLE_LORA),0)

# Include the Semtech-loramac package
#USEMODULE += netdev_default
DRIVER ?= sx1276
USEMODULE += $(DRIVER)
USEPKG += semtech-loramac
USEMODULE += auto_init_loramac
USEMODULE += semtech_loramac_rx
endif


# LoRaWAN Stuff
ifneq (,$(filter semtech-loramac,$(USEPKG)))

CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
CFLAGS += -DUSE_OTAA
endif



include $(RIOTBASE)/Makefile.include

## add IOT_LAB Support
IOTLAB_NODES ?= 1
IOTLAB_DURATION ?= 5 # minutes
IOTLAB_SITE ?= grenoble
IOTLAB_TYPE ?= nucleo-wl55jc:stm32wl
IOTLAB_PROFILE ?= stm32Profile
include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab