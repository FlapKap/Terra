# RIOT APPLICATION INFORMATION
APPLICATION = Terra
PROJECT_BASE = $(CURDIR)/../../
BOARD ?= b-l072z-lrwan1
RIOTBASE ?= $(PROJECT_BASE)/RIOT

EXTERNAL_MODULE_DIRS += $(PROJECT_BASE)/modules $(CURDIR)/
EXTERNAL_BOARD_DIRS += $(PROJECT_BASE)/boards

MAIN_STACKSIZE ?= 5120 # change stack size

# Terra APPLICATION CONFIGURATION

EXECUTION_EPOCH_S ?= 20 # EXECUTE INTERVAL
RUNTIME_STACK_MEMORY ?= 10 # RUNTIME STACK SIZE. Amount of Number's in the stack
FORCED_LISTEN_EVERY_N_LOOP ?= 1000 # FORCED LISTEN: send a heartbeat every N loop to guarantee a listen for incoming packets


## LORA
#DISABLE_LORA ?= 0
ACTIVATION_MODE ?= otaa # only otaa is supported
LORAMAC_DATA_RATE ?= 3
LORA_REGION ?= EU868
APPEUI ?= 0000000000000000
DEVEUI ?= 70B3D57ED005EA59
APPKEY ?= 385794DDE70CE2EAB5B5B12A4807822C

LORAMAC_DEFAULT_RX1_DELAY ?= "(5000U)" # TTN default
LORAMAC_DEFAULT_RX2_DR ?= LORAMAC_DR_3 # TTN default
LORAMAC_DEFAULT_SYSTEM_MAX_RX_ERROR ?= 150

LOG_LEVEL ?= LOG_INFO

# Different build configurations. Optimized builds run without any debug symbols and with o3 optimizations
APPLICATION_BUILD_OPTIMIZED = 0

# Application run tests instead of the normal "main" function
APPLICATION_RUN_TEST ?= 0


# Default query to run, if any.

#DEFAULT_QUERY_AS_PB_BASE64 ?= ChQKEhoQCgQIBBgBEAIYAyIECODUAw==




# TFLITE MICRO
## if a tflite micro model is wished to be used, add it in the tflite_model module and set USEMODULE += tflite_model
USEMODULE += tflite_model

# MODULES
USEMODULE += saul_default
USEMODULE += fmt
USEMODULE += periph_hwrng # make sure that enough entropy is provided for the PRNG
## Timer stuff
USEMODULE += ztimer ztimer_msec
USEMODULE += periph_rtc
#USEMODULE += ztimer_ondemand ztimer_ondemand_rtt ztimer_msec
#USEMODULE += ztimer_sec

#USEMODULE += idle_thread
#USEMODULE += pm_layered
# DISABLE_MODULE += pm_layered
# DISABLE_MODULE += auto_init

# PACKAGES
## Nanopb stuff
USEPKG += nanopb
#PROTOBUF_PATH = $(PROJECT_BASE)/proto

# BOARD SPECIFIC CONFIGURATION ENTRIES
##SENSORS (ONLY APPLIED FOR NON-NATIVE BUILDS)
## to handle sensor boards with multiple sensors, we need both name and type, as they are defined in RIOT
## The easiest way to find this is to search for the sensor at https://doc.riot-os.org/. The first part of the header files is the module name
## for the sensor driver, and if you enter the <sensor_name>_saul.c file in the source code, you can see which types are defined

## we define below per board settings like sensors and modules available just because different boards have different sensors available
ifeq ($(strip $(BOARD)),b-l072z-lrwan1)
USEMODULE += hts221
SENSOR_NAMES ?= hts221 hts221 
SENSOR_TYPES ?= SAUL_SENSE_TEMP SAUL_SENSE_HUM
FEATURES_REQUIRED += periph_eeprom 
USEMODULE += periph_eeprom eepreg
endif

ifeq ($(strip $(BOARD)),esp32-ttgo-t-beam)

SENSOR_NAMES ?= LED PRG
SENSOR_TYPES ?= SAUL_ACT_SWITCH SAUL_SENSE_BTN

USEMODULE += ztimer_no_periph_rtt
SLOT_AUX_LEN := 0x8000
PORT ?= "/dev/ttyACM0"
endif

ifeq ($(strip $(BOARD)),samr34-xpro)
SENSOR_NAMES ?= "Button(SW0)" "Button(SW0)"
SENSOR_TYPES ?= SAUL_SENSE_BTN SAUL_SENSE_BTN
FEATURES_REQUIRED += periph_flashpage_aux
USEMODULE += mtd_flashpage mtd_write_page periph_flashpage_aux 
USEMODULE += ztimer_no_periph_rtt
SLOT_AUX_LEN := 0x8000
endif
ifneq ($(words SENSOR_NAMES),$(words SENSOR_TYPES))
$(error "Number of SENSOR_NAMES and SENSOR_TYPES must be the same")
endif


#build C array of sensors
comma:= ,
empty:=
space:= $(empty) $(empty) # this is so stupid... https://www.gnu.org/software/make/manual/html_node/Syntax-of-Functions.html
SENSORS_ARRAY ?=  {$(subst $(space),$(comma),$(patsubst %,\"%\",$(SENSOR_NAMES)))} # first we quote each word, then we replace space with comma, and lastly we wrap in {}
SENSORS_ARRAY_LENGTH := $(words $(SENSOR_NAMES))
SENSORS_TYPES_ARRAY ?= {$(subst $(space),$(comma),$(SENSOR_TYPES))}
# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

## Apply configuration
CFLAGS += -DLOG_LEVEL=$(LOG_LEVEL)

CFLAGS += -DEXECUTION_EPOCH_S=$(EXECUTION_EPOCH_S)
CFLAGS += -DFORCED_LISTEN_EVERY_N_LOOP=$(FORCED_LISTEN_EVERY_N_LOOP)
CFLAGS += -DTHREAD_STACKSIZE_MAIN=$(MAIN_STACKSIZE)
CFLAGS += -DRUNTIME_STACK_MEMORY=$(RUNTIME_STACK_MEMORY)
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_DR_$(LORAMAC_DATA_RATE)
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_RX1_DELAY=$(LORAMAC_DEFAULT_RX1_DELAY)
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_RX2_DR=$(LORAMAC_DEFAULT_RX2_DR)
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_SYSTEM_MAX_RX_ERROR=$(LORAMAC_DEFAULT_SYSTEM_MAX_RX_ERROR)
CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
CFLAGS += -DUSE_OTAA


CFLAGS += -DDEFAULT_QUERY_AS_PB_CHAR_ARRAY={$(shell echo -n {$(DEFAULT_QUERY_AS_PB_BASE64)} | base64 -di | xxd -p -c 10000 | sed 's/\(..\)/0x\1,/g')}
## RIOT CONFIGURATION
# Set the compile_commands.json file to a known location
COMPILE_COMMANDS_PATH = $(CURDIR)/bin/compile_commands.json


ifeq ($(APPLICATION_BUILD_OPTIMIZED),1)
  CFLAGS += -O3
  DEVELHELP = 0
  CREATE_STACKTEST = 0
else
# Debug stuff. We also want debug stuff to run when testing
  #USEMODULE += ssp # enable stack smashing protection. Mainly to get explicit panics if a stack overflow happens
  DEVELHELP = 1 # development helpers
  #CREATE_STACKTEST = 1 # check stacksizes
  VERBOSE_ASSERT = 1
  #CFLAGS += -fstack-protector-all
  #CFLAGS += -g3 -Og # unoptimized and with debug symbols
  CFLAGS += -Wall -Wextra  -Werror=array-bounds  # extra warnings
  #CFLAGS += -Werror -Wpedantic
  #CFLAGS += -Wformat -Wformat-nonliteral -Werror=format-security 
endif

ifeq (1,$(APPLICATION_RUN_TEST))
# Test stuff
CFLAGS += -DAPPLICATION_RUN_TEST
USEMODULE += embunit
endif

## handle native builds and builds without lora
ifeq ($(strip $(BOARD)),native)
  DISABLE_LORA = 1
else
## add configured sensors
  ## all drivers goes through all folders in the drivers subfolder and extracts the folder names in a single list. I.e. ALL_DRIVERS contain a list like ad7746 adcxx1c ads101x
  ALL_DRIVERS = $(notdir $(patsubst %/,%,$(dir $(wildcard $(RIOTBASE)/drivers/*/))))
  SENSOR_NAMES_UNIQUE = $(sort $(SENSOR_NAMES))
  # $(info SENSOR_NAMES_UNIQUE = $(SENSOR_NAMES_UNIQUE))
  # $(info ALL_DRIVERS = $(ALL_DRIVERS))
  # $(info filtered = $(filter $(ALL_DRIVERS),$(SENSOR_NAMES_UNIQUE)))
  USEMODULE += $(filter $(ALL_DRIVERS),$(SENSOR_NAMES_UNIQUE))
  CFLAGS += -DSENSORS_ARRAY=$(SENSORS_ARRAY) -DSENSORS_ARRAY_LENGTH=$(SENSORS_ARRAY_LENGTH) -DSENSORS_TYPES_ARRAY=$(SENSORS_TYPES_ARRAY)
  DISABLE_LORA ?= 0
endif

ifeq (1,$(DISABLE_LORA))
  CFLAGS += -DDISABLE_LORA
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

  #FEATURES_REQUIRED += periph_rtt # needed for loramac
endif

include $(RIOTBASE)/Makefile.include
## add IOT_LAB Support
IOTLAB_NODES ?= 1
IOTLAB_DURATION ?= 5 # minutes
IOTLAB_SITE ?= saclay
IOTLAB_TYPE ?= st-lrwan1:sx1276
IOTLAB_PROFILE ?= stm32Profile
include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab