qemu_build:
	mkdir -p build
	aarch64-none-elf-gcc --version
	aarch64-none-elf-gcc -ffreestanding -fno-builtin -fno-stack-protector -fno-pie -fno-pic -Isrc -Isrc/dtb -c src/boot.S src/kernel.c src/utils.c src/dtb/dtb.c
	mv *.o ./build/
	aarch64-none-elf-ld -nostdlib -T linker/qemu_virt/linker.ld build/boot.o build/kernel.o build/utils.o build/dtb.o -o build/kernel.elf

qemu_run:
	qemu-system-aarch64 -M virt -cpu cortex-a57 -m 1024 -nographic -serial mon:stdio -kernel build/kernel.elf

qemu_dtb_dump:
	qemu-system-aarch64 -M virt,dumpdtb=virt.dtb -cpu cortex-a57 -m 1024

qemu_dtb_decompile:
	dtc -I dtb -O dts -o virt.dts virt.dtb

clean:
	rm -f build/*.o build/*.elf *.dtb *.dts
	rm -rf build
