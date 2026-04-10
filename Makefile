.PHONY: all clean

all:
	@mkdir -p build
	@cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
	@cmake --build build

clean:
	@rm -rf build/
