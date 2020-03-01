ifdef VERBOSE
QUIET :=
else
QUIET := @
endif
SAY := @echo
RM := rm -rf

FEATURES := BASE
CMAKE_FLAGS := -G "MSYS Makefiles"

ifdef DEBUG
BUILD_TYPE := Debug
else
BUILD_TYPE := Release
endif
CMAKE_FLAGS := $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

ifdef MSDOS
CMAKE_FLAGS := $(CMAKE_FLAGS) -DFOR_MSDOS=ON
FEATURES := $(FEATURES)_MSDOS
endif

BUILD_DIR	:= ./build/$(FEATURES)/$(BUILD_TYPE)
SRC_DIR		:= ../../../src # relative to BUILD_DIR

all:
	$(QUIET)mkdir -p $(BUILD_DIR)
	$(QUIET)cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(SRC_DIR)
	$(QUIET)$(MAKE) -s -C $(BUILD_DIR)

clean:
	$(RM) $(BUILD_DIR)
