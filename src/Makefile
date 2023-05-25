# RIOT Stuff
APPLICATION = NSED_Runtime
PROJECTBASE = $(CURDIR)
BOARD ?= native

## We assume riot is placed next to this repo
RIOTBASE ?= $(CURDIR)/../../RIOT


FEATURES_OPTIONAL += periph_uart

ifneq ($(BOARD), native)
# LoRaWAN Stuff

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

# Include the Semtech-loramac package
USEMODULE += netdev_default
DRIVER ?= sx1276
USEMODULE += $(DRIVER)
USEPKG += semtech-loramac
USEMODULE += auto_init_loramac
USEMODULE += semtech_loramac_rx


USEMODULE += saul_default

USEMODULE += fmt


endif

# Nanopb stuff
USEPKG += nanopb
PROTOBUF_PATH = $(CURDIR)/proto

# Timer stuff
USEMODULE += ztimer
USEMODULE += ztimer_sec
USEMODULE += ztimer_msec

# Test stuff
USEMODULE += embunit

# change stack size and add some extra error checking
CFLAGS += -DTHREAD_STACKSIZE_MAIN='(20 * 1024)'
CFLAGS += -Wall -Wextra -Wformat -Wformat-nonliteral -Werror=format-security -Werror=array-bounds

# make sure build is unoptimized and with as many debug symbols as possible
CFLAGS +=  -g3 -O0

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

CREATE_STACKTEST = 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

## add IOT_LAB Support
IOTLAB_NODES ?= 1
IOTLAB_DURATION ?= 5 # minutes
IOTLAB_SITE ?= grenoble
IOTLAB_TYPE ?= nucleo-wl55jc:stm32wl
IOTLAB_PROFILE ?= stm32Profile
include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab