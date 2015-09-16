CC=gcc
CFLAGS=-I$(INC_DIR) -lm -lrt

LIBS=-lm -lrt

INC_DIR=./includes
BIN_DIR=./bin
SRC_DIR=./src

OBJS=$(BIN_DIR)/gos_interface_driver.o
DEPS = $(INC_DIR)/gos_interface_driver.h

$(BIN_DIR)/%.o: $(INC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: hlsKicker monitor reset injectErrors

hlsKicker: $(OBJS)
	$(CC) $(SRC_DIR)/$@.c -o $(BIN_DIR)/$@ $^ $(CFLAGS)

monitor: $(OBJS)
	$(CC) $(SRC_DIR)/$@.c -o $(BIN_DIR)/$@ $^ $(CFLAGS)

reset: $(OBJS)
	$(CC) $(SRC_DIR)/$@.c -o $(BIN_DIR)/$@ $^ $(CFLAGS)

injectErrors: $(OBJS)
	$(CC) $(SRC_DIR)/$@.c -o $(BIN_DIR)/$@ $^ $(CFLAGS)
