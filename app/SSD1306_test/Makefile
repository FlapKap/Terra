# Set the name of your application:
APPLICATION = SSD1306_test
 
PROJECT_BASE = $(CURDIR)/../../
BOARD ?= esp32-ttgo-t-beam
RIOTBASE ?= $(PROJECT_BASE)/RIOT
FEATURES_REQUIRED += periph_gpio periph_i2c
USEMODULE += esp_i2c_sw periph_i2c
CFLAGS += '-DTHREAD_STACKSIZE_MAIN= 48*1024'
USEPKG += u8g2
PORT="/dev/ttyACM0"
include $(RIOTBASE)/Makefile.include
