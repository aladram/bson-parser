CC = g++

CFLAGS = -Wall -Werror -Wextra -std=c++17 -pedantic

VPATH = src

FILES = \
        bson.cc \
        bson-parser.cc \

OBJS = $(FILES:.cc:.o)

BIN = bson-parser

all: $(BIN)

release: CPPFLAGS += -DNDEBUG
release: $(OBJS)
