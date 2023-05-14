CC = gcc
GENHEAD = header/src/stack.c header/src/math.c header/src/string.c
VM = header/src/vmbase.c
VERSION = 0.1.0

build: lasm vm

run: build
	bin/lasm main.lasm bin/main.vos
	bin/vm bin/main.vos

lasm:
	$(CC) $(GENHEAD) $(COMPILER) compiler.c -o bin/lasm

vm:
	$(CC) $(GENHEAD) $(VM) mashine.c -o bin/vm

clean: build
	rm bin/lasm
	rm bin/vm

compress-src: clean
	tar -cf LVM-v$(VERSION).tar *
	gzip LVM-v$(VERSION).tar