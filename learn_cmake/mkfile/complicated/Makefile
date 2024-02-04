.PHONY: all clean
MKDIR = mkdir
RM = rm
RMFLAGS = -fr
CC = gcc

DIR_OBJS = objs
DIR_EXES = exes
DIR_DEPS = deps
DIRS = $(DIR_OBJS) $(DIR_EXES) $(DIR_DEPS)

EXE = complicated
EXE := $(addprefix $(DIR_EXES)/, $(EXE))
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))
DEPS = $(SRCS:.c=.dep)
DEPS := $(addprefix $(DIR_DEPS)/, $(DEPS))

all: $(EXE)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(DIRS):
	$(MKDIR) $@

$(EXE): $(OBJS) | $(DIR_EXES)
	$(CC) -o $@ $(filter %.o, $^)

$(DIR_OBJS)/%.o: %.c | $(DIR_OBJS)
	$(CC) -o $@ -c $<

$(DIR_DEPS)/%.dep: %.c | $(DIR_DEPS)
	@echo "Making $@ ..."
	@set -e; \
	$(CC) -E -MM -MT '$(DIR_OBJS)/$*.o' $< > $@

clean:
	$(RM) $(RMFLAGS) $(DIRS)

