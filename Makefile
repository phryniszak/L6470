# https://stackoverflow.com/questions/172587/what-is-the-difference-between-g-and-gcc
CXX      := -gcc
# https://stackoverflow.com/questions/1662909/undefined-reference-to-pthread-create-in-linux
CXXFLAGS := -Wall	

LDFLAGS  := -lpthread -lpigpio -lm
LDFLAGS_STATIC :=

BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := app.out
INCLUDE  := -Iinc/
SRC      :=                      \
   $(wildcard src/*.c)         \

OBJECTS := $(SRC:%.c=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(APP_DIR)/$(TARGET) $(INCLUDE) $(OBJECTS) $(LDFLAGS_STATIC) $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2 # -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/* 

rebuild:
	$(MAKE) clean
	$(MAKE) debug
