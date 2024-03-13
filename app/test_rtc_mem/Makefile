# Set the name of your application:
APPLICATION = test_rtc_mem
 
PROJECT_BASE = $(CURDIR)/../../
BOARD ?= b-l072z-lrwan1
RIOTBASE ?= $(PROJECT_BASE)/RIOT
USEMODULE += periph_rtc periph_rtc_mem
DEFAULT_MODULE += test_utils_interactive_sync

DEVELHELP = 1 # development helpers
CREATE_STACKTEST = 1 # check stacksizes
VERBOSE_ASSERT = 1
CFLAGS += -fstack-protector-all
CFLAGS += -g3 -Og # unoptimized and with debug symbols
CFLAGS += -Wall -Werror -Wextra -Wformat -Wformat-nonliteral -Werror=format-security -Werror=array-bounds -Wpedantic # extra warnings

include $(RIOTBASE)/Makefile.include
