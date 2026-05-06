build:
	aarch64-linux-gnu-gcc -ffreestanding -fno-builtin -c boot.S -o boot.o
	aarch64-linux-gnu-gcc -ffreestanding -fno-builtin -c kernel.c -o kernel.o
	aarch64-linux-gnu-ld -nostdlib -T linker.ld boot.o kernel.o -o kernel.elf

qemu_run:
	qemu-system-aarch64 -M virt -cpu cortex-a57 -m 1024 -nographic -serial mon:stdio -kernel kernel.elf

qemu_dtb_dump:
	qemu-system-aarch64 -machine virt,dumpdtb=virt.dtb -cpu max
