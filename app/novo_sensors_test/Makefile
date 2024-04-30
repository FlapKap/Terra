# Set the name of your application:
APPLICATION = novo_sensors_test
 
PROJECT_BASE = $(CURDIR)/../../
BOARD ?= esp32-ttgo-t-beam
RIOTBASE ?= $(PROJECT_BASE)/RIOT
PORT="/dev/ttyACM0"
USEMODULE +=  fmt ztimer ztimer_msec
CFLAGS+=-DPARAM_SPS30_I2C=I2C_DEV\(0\)
CFLAGS+=-DI2C0_SPEED=I2C_SPEED_NORMAL
# sensors
USEMODULE += dht
USEMODULE += sps30 scd30
FEATURES_REQUIRED += periph_gpio periph_i2c

include $(RIOTBASE)/Makefile.include
