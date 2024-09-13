default: build

cmake:
	cmake -S . -B Build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1

build:
	@if [ ! -d Build ]; then \
		$(MAKE) cmake; \
	fi
	make -C Build

run:
	./build/TrueClearanceFieldExample

.PHONY: build

