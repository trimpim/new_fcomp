#
# project settings
#
SRC_DIR = ./src
DST_DIR = ./build
BIN_NAME = fcomp

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
OBJCOPY = /usr/bin/arm-none-eabi-objcopy
SIZE = /usr/bin/arm-none-eabi-size
RM = rm -f

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
SRC += $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(wildcard $(DST_DIR)/*.o)

#
# rules
#
all: compile link bin size

compile: ${SRC}
	@echo -e ${COLOR_GREEN}"Compiling : "${COLOR_WHITE} $<${COLOR_LIGHT_GRAY}
	@$(CC) ${CXX_FLAGS} $< -o ${DST_DIR}/$(notdir $(basename $<)).o

link: compile
	@echo -e "Linking : " ${DST_DIR}/${BIN_NAME}.axf
	@$(LD) $(LD_FLAGS) ${OBJ} -o ${DST_DIR}/${BIN_NAME}.axf

size: link
	$(SIZE) ${DST_DIR}/${BIN_NAME}.axf

bin: link
	@echo -e "Converting : " ${DST_DIR}/${BIN_NAME}.axf  " -> " ${DST_DIR}/${BIN_NAME}.bin
	@$(OBJCOPY) -v -O binary ${DST_DIR}/${BIN_NAME}.axf ${DST_DIR}/${BIN_NAME}.bin

clean:
	$(RM) ${DST_DIR}/*.o
	$(RM) ${DST_DIR}/*.axf
	$(RM) ${DST_DIR}/*.bin
	$(RM) ${DST_DIR}/*.map
