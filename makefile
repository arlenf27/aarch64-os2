build:
	mkdir -p build
	aarch64-linux-gnu-gcc -ffreestanding -fno-builtin -c src/boot.S -o build/boot.o
	aarch64-linux-gnu-gcc -ffreestanding -fno-builtin -c src/kernel.c -o build/kernel.o
	aarch64-linux-gnu-ld -nostdlib -T linker/linker.ld build/boot.o build/kernel.o -o build/kernel.elf

qemu_run:
	qemu-system-aarch64 -M virt -cpu cortex-a57 -m 1024 -nographic -serial mon:stdio -kernel build/kernel.elf

qemu_dtb_dump:
	qemu-system-aarch64 -machine virt,dumpdtb=virt.dtb -cpu max

qemu_dtb_decompile:
	dtc -I dtb -O dts -o virt.dts virt.dtb

clean:
	rm -f build/*.o build/*.elf *.dtb *.dts
	rm -rf build
