# Set the name of your application:
APPLICATION = tfmicro_test
 
PROJECT_BASE = $(CURDIR)/../../
BOARD ?= b-l072z-lrwan1
#BOARD ?= esp32-ttgo-t-beam
#BOARD ?= native
RIOTBASE ?= $(PROJECT_BASE)/RIOT
EXTERNAL_MODULE_DIRS += $(CURDIR)
USEMODULE += tflite_model
DEVELHELP = 1
CREATE_STACKTEST = 0

include $(RIOTBASE)/Makefile.include
