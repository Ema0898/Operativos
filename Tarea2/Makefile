all: build run

clean:
	cd bin && rm -rf ./*.bin

build:
	nasm -fbin ./src/kernel.asm -o ./bin/kernel.bin
	nasm -fbin ./src/bootloader.asm -o ./bin/bootloader.bin	
	cat ./bin/bootloader.bin ./bin/kernel.bin > ./bin/bootable_tanks.bin

run:
	qemu-system-i386 ./bin/bootable_tanks.bin