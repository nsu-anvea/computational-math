CC = gcc
LDFLAGS = -lm

SRC_DIR = src
BLD_DIR = build

TARGET = calcint


define compile
$(CC) $< -o $(BLD_DIR)/$(1) $(LDFLAGS)
endef


all: $(TARGET) runge

$(TARGET): $(SRC_DIR)/main.c | $(BLD_DIR)
	$(call compile,$(TARGET))

runge: $(SRC_DIR)/runge.c | $(BLD_DIR)
	$(call compile,$@)


$(BLD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BLD_DIR)


.PHONY: all build clean