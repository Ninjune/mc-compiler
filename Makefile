BUILD_DIR := build

.PHONY: all configure build clean

all: build

configure:
	cmake -B $(BUILD_DIR) -S . -DCMAKE_CXX_COMPILER=$(CXX)

build: configure
	cmake --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
