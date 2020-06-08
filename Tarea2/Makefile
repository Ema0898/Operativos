all: build run

clean:
	cd bin && rm -rf ./*.bin

build:
	nasm -fbin ./src/kernel.asm -o ./bin/kernel.com
	nasm -fbin ./src/bootloader.asm -o ./bin/bootloader.com	
	cat ./bin/bootloader.com ./bin/kernel.com > ./bin/bootable_tanks.com

run:
	qemu-system-i386 ./bin/bootable_tanks.com