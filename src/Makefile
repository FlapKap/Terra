# RIOT Stuff
APPLICATION = NSED_Runtime
PROJECTBASE = $(CURDIR)
BOARD ?= b-l072z-lrwan1

## We assume riot is placed next to this repo
RIOTBASE ?= $(CURDIR)/../../RIOT

# Different build configurations. Optimized builds run without any debug symbols and o3 optimizations
APPLICATION_BUILD_OPTIMIZED ?= 0

# Application run tests instead of the normal "main" function
APPLICATION_RUN_TEST ?= 1

# for native builds we want to disable lora
DISABLE_LORA ?= 0

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Packages
USEMODULE += saul_default
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


# change stack size
CFLAGS += -DTHREAD_STACKSIZE_MAIN='20480'

ifeq (1, $(APPLICATION_BUILD_OPTIMIZED))
CFLAGS += -O3
DEVELHELP ?= 0
CREATE_STACKTEST ?= 0
else
# Debug stuff. We also want debug stuff to run when testing
## bonus info: the way the ifneq works is that it filters application build for development or test and returns one or the other if present.
## this is then checked against an empty string which evaluates to false. If its not development or test then the filter is an empty string and evaluates to true
CFLAGS += -g3 -O0 # unoptimized and with debug symbols
CFLAGS += -Wall -Werror -Wextra -Wformat -Wformat-nonliteral -Werror=format-security -Werror=array-bounds -Wpedantic # extra warnings
DEVELHELP ?= 1 # development helpers
CREATE_STACKTEST ?= 1 # check stacksizes
endif

ifdef APPLICATION_RUN_TEST
# Test stuff
CFLAGS += -DAPPLICATION_RUN_TEST
USEMODULE += embunit
endif

# Different board configurations. Native and hardware
## if board is not native we can load the LoRa Stack
ifneq ($(BOARD), native)

# Include the Semtech-loramac package
USEMODULE += netdev_default
DRIVER ?= sx1276
USEMODULE += $(DRIVER)
USEPKG += semtech-loramac
USEMODULE += auto_init_loramac
USEMODULE += semtech_loramac_rx
endif


# LoRaWAN Stuff
ifeq (,$(filter semtech-loramac,$(USEPKG)))
# Currently only supports otaa
ACTIVATION_MODE ?= otaa

APPEUI ?= 0000000000000000
DEVEUI ?= 0000000000000000
APPKEY ?= 00000000000000000000000000000000
CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"

LORA_REGION ?= EU868

CFLAGS += -DSEND_PERIOD_S=$(SEND_PERIOD_S)
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