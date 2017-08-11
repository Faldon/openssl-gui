DEBUG := $(filter debug,$(MAKECMDGOALS))

all:
	mkdir bin
	gcc $(if $(DEBUG),-ggdb -DDEBUG) -o bin/openssl-gui \
		main.c config.c ui.c -lncurses

clean:
	rm -f bin/openssl-gui

debug: all

.PHONY: clean debug
