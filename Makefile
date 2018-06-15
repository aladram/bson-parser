CC = g++

CPPFLAGS = -MMD

CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -pedantic

VPATH = src

FILES = \
        bson.cc \
        bson-parser.cc \

OBJS = $(FILES:%.cc=%.o)

DEPS = $(OBJS:%.o=%.d)

BIN = bson-parser

all: $(BIN)

$(BIN): $(OBJS)

release: CPPFLAGS += -DNDEBUG -Ofast -march=native
release: all

debug: CXXFLAGS += -g3 -O0
debug: LDFLAGS += -g3
debug: all

-include $(DEPS)

clean:
	$(RM) $(BIN) $(OBJS) $(DEPS)

.PHONY: all release debug clean
