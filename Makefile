CC = g++
ASM = nasm -felf64
TARGET = run

OFLAG ?= -O0

CFLAGS_BASE = -Iinclude -Wall -pedantic -pedantic-errors -g3 $(shell sdl2-config --cflags) -msse4.2 -march=native -maes -mavx -mavx2 -mfma
CFLAGS_RELEASE = $(CFLAGS_BASE) -DNDEBUG $(OFLAG)
CFLAGS_DEBUG = $(CFLAGS_BASE) -O0 -fsanitize=address

LDFLAGS = $(shell sdl2-config --libs) -lcrypto -lssl
LDFLAGS_DEBUG = $(LDFLAGS) -fsanitize=address

DEPFLAGS = -MMD -MP

SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

GCOV_DATA_DIR = $(BIN_DIR)/gcov_data

SRCS_CPP := $(wildcard $(SRC_DIR)/*.cpp)
SRCS_ASM := $(wildcard $(SRC_DIR)/*.asm)

OBJS_CPP := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS_CPP))
OBJS_ASM := $(patsubst $(SRC_DIR)/%.asm,$(BIN_DIR)/%.o,$(SRCS_ASM))
OBJS := $(OBJS_CPP) $(OBJS_ASM)

DEPS := $(OBJS_CPP:.o=.d)

$(shell mkdir -p $(BIN_DIR))

all: release

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm
	$(ASM) -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: LDFLAGS = -fsanitize=address
debug: $(TARGET)

release: CFLAGS = $(CFLAGS_RELEASE)
release: $(TARGET)

pgo-gen:
	mkdir -p $(GCOV_DATA_DIR)
	$(MAKE) CFLAGS="$(CFLAGS_RELEASE) -fprofile-generate=$(GCOV_DATA_DIR)" LDFLAGS="$(LDFLAGS) -fprofile-generate=$(GCOV_DATA_DIR)" $(TARGET)

pgo-use:
	$(MAKE) CFLAGS="$(CFLAGS_RELEASE) -fprofile-use=$(GCOV_DATA_DIR) -fprofile-correction" LDFLAGS="$(LDFLAGS)" $(TARGET)

pgo: pgo-gen
	./$(TARGET) --default
	$(MAKE) pgo-use

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
	rm -rf $(GCOV_DATA_DIR)

-include $(DEPS)

.PHONY: all debug release clean pgo-gen pgo-use pgo
