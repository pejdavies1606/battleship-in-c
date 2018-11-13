
ifdef VERBOSE
QUIET :=
else
QUIET := @
endif

SUMMARY := $(QUIET)echo

TGT := battleship-in-c.exe

ifdef DEBUG
REL_DIR := Debug
else
REL_DIR := Release
endif

CC := gcc
CFLAGS := -Wall -I. -std=c99
ifdef DEBUG
#CFLAGS += -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
CFLAGS += -O0 -g3 -c -fmessage-length=0
endif

LINKER := gcc
LFLAGS := -Wall -I. -lm

SRC_DIR := src
OBJ_DIR := $(REL_DIR)/obj
BIN_DIR := $(REL_DIR)/bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
INCS := $(wildcard $(SRC_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

RM := rm -rf

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

.PHONY: all
all: $(BIN_DIR)/$(TGT)

$(BIN_DIR)/$(TGT): $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(SUMMARY) 'Linking $@'
	$(QUIET)$(LINKER) $(OBJS) $(LFLAGS) -o $@

# more complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets
#    sed:    strip the target (everything before colon)
#    sed:    remove any continuation backslashes
#    fmt -1: list words one per line
#    sed:    strip leading spaces
#    sed:    add trailing colons

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(SUMMARY) "Compiling $<"
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@
	$(QUIET)$(CC) $(CFLAGS) -MM $< > $(@:%.o=%.d)
	$(QUIET)#cp -f $(@:%.o=%.d) $(@:%.o=%.d.tmp)
	$(QUIET)#sed -e 's|.*:|$@:|' -i $(@:%.o=%.d.tmp)
	$(QUIET)#sed -e 's/.*://' -e 's/\\\$$//' < $(@:%.o=%.d.tmp) | fmt -1 | sed -e 's/^ *//' -e 's/\$$/:/' #>> $(@:%.o=%.d)
	$(QUIET)#rm -f $(@:%.o=%.d.tmp)

$(BIN_DIR):
	$(QUIET)mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	$(QUIET)mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	$(QUIET)$(RM) $(OBJ_DIR)
	$(SUMMARY) "Cleanup complete"

.PHONY: remove
remove: clean
	$(QUIET)$(RM) $(BIN_DIR)
	$(SUMMARY) "Executable removed"
