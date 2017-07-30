#
# project settings
#
SRC_DIR = ./src
DST_DIR = ./build
BIN_NAME = fcomp
EXT = cpp

COLOR_NC='\e[0m' # No Color
COLOR_WHITE='\e[1;37m'
COLOR_BLACK='\e[0;30m'
COLOR_BLUE='\e[0;34m'
COLOR_LIGHT_BLUE='\e[1;34m'
COLOR_GREEN='\e[0;32m'
COLOR_LIGHT_GREEN='\e[1;32m'
COLOR_CYAN='\e[0;36m'
COLOR_LIGHT_CYAN='\e[1;36m'
COLOR_RED='\e[0;31m'
COLOR_LIGHT_RED='\e[1;31m'
COLOR_PURPLE='\e[0;35m'
COLOR_LIGHT_PURPLE='\e[1;35m'
COLOR_BROWN='\e[0;33m'
COLOR_YELLOW='\e[1;33m'
COLOR_GRAY='\e[0;30m'
COLOR_LIGHT_GRAY='\e[0;37m'

#
# used programs
#
CC=/usr/bin/arm-none-eabi-c++
LD = /usr/bin/arm-none-eabi-ld
SIZE = /usr/bin/arm-none-eabi-size
RM = rm -f
MKDIR = mkdir

#
# compiler flags
#
CXX_FLAGS += -c
CXX_FLAGS += -g3
CXX_FLAGS += -O3
CXX_FLAGS += -fno-exceptions
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -fno-builtin
CXX_FLAGS += -fmessage-length=0
CXX_FLAGS += -fno-common
CXX_FLAGS += -std=c++1z
CXX_FLAGS += -Wall
CXX_FLAGS += -Wextra
CXX_FLAGS += -mcpu=cortex-m0
CXX_FLAGS += -mthumb

#
# linker flags
#
LD_FLAGS  = -no-stdlibs
LD_FLAGS += -nodefaultlibs
LD_FLAGS += --gc-sections
LD_FLAGS += -Map="${DST_DIR}/${BIN_NAME}.map"
LD_FLAGS += -T "ld/debug.ld"
LD_FLAGS += -L "/usr/arm-none-eabi/lib/armv6-m"
LD_FLAGS += -l "stdc++"

#
# source and object files
#
SRC := $(shell find $(SRC_DIR) -name "*.${EXT}")
OBJ := $(SRC:$(SRC_DIR)/%.${EXT}=$(DST_DIR)/%.o)
DEP := $(OBJ:.o=.d)

#
# rules
#

.PHONY: all clean

all: size

clean:
	@echo -e ${COLOR_GREEN}"Clean all"${COLOR_LIGHT_GRAY}
	@$(RM) -rf ${DST_DIR}/*

size: $(BIN_NAME).axf
	@echo -e ${COLOR_GREEN}"binary size"${COLOR_LIGHT_GRAY}
	@$(SIZE) ${DST_DIR}/$(BIN_NAME).axf

$(BIN_NAME).axf: $(OBJ)
	@echo -e ${COLOR_GREEN}"Linking : "\
		${COLOR_WHITE}${DST_DIR}/${BIN_NAME}.axf\
		${COLOR_LIGHT_GRAY}
	@$(LD) $(LD_FLAGS) ${OBJ} -o ${DST_DIR}/${BIN_NAME}.axf

.SECONDEXPANSION:
$(DST_DIR)/%.o: $(SRC_DIR)/%.${EXT} | $$(@D)/
	@echo -e ${COLOR_GREEN}"Compiling : "${COLOR_WHITE} $<\
		${COLOR_LIGHT_GRAY}
	@$(CC) $(CXX_FLAGS) -o $@ -c $<

%/:
	$(MKDIR) -p $*

ifeq "$(MAKECMDGOALS)" ""
-include $(DEP)
endif
