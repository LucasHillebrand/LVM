CC = gcc
GENHEAD = header/src/stack.c header/src/math.c header/src/string.c
VM = header/src/vmbase.c
COMPILER = 

build:
	$(CC) $(GENHEAD) $(COMPILER) compiler.c -o bin/lasm
	$(CC) $(GENHEAD) $(VM) mashine.c -o bin/vm 

run: build
	bin/lasm main.lasm bin/main.vos
	bin/vm bin/main.vos

clean: build
	rm bin/*