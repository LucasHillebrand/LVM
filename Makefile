CC = gcc
GENHEAD = header/src/stack.c header/src/math.c header/src/string.c header/src/file.c
VM = header/src/vmbase.c
COMPILER = header/src/asambler.c
VERSION = 1.0.0

build: lasm vm

run: run-lasm run-vm

run-vm: vm
	bin/vm -f bin/main.vos -d

run-lasm: lasm
	bin/lasm main.lasm -o bin/main.vos

lasm:
	$(CC) $(GENHEAD) $(COMPILER) compiler.c -o bin/lasm

vm:
	$(CC) $(GENHEAD) $(VM) mashine.c -o bin/vm

clean: build
	rm bin/lasm
	rm bin/vm
	rm bin/main.vos

compress:
	tar -cf archives/LVM-v$(VERSION).tar *
	gzip -f archives/LVM-v$(VERSION).tar

release: build run-lasm
	bash -c "cd bin && tar -cf ../archives/LVM-v$(VERSION)-release.tar *"
	gzip -f archives/LVM-v$(VERSION)-release.tar

exec:
	bin/lasm main.lasm bin/main.vos
	bin/vm -f bin/main.vos -d|less

install: build
	cp bin/vm /usr/local/bin/l-vm
	cp bin/lasm /usr/local/bin/lasm

uninstall:
	rm /usr/local/bin/l-vm
	rm /usr/local/bin/lasm