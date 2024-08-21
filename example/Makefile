default: build

cmake:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1

build:
	make -C build

run:
	./build/true-clearance-field-example

.PHONY: build

