
CC = gcc
CFLAGS = -g -O0

RM = rm -rf
MKDIR = mkdir -p

SERVER_BIN := bin/server
SERVER_O = server.o
SERVER_C = server.c

default: all

all: $(SERVER_BIN)

$(SERVER_BIN): $(SERVER_O)
	[[ -d ./bin ]] || $(MKDIR) ./bin
	$(CC) $(CFLAGS) $^ -o $@

$(SERVER_O): $(SERVER_C)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) $(SERVER_O)
	$(RM) $(SERVER_BIN)

run:
	@$(SERVER_BIN)

.PHONY: default all clean run
