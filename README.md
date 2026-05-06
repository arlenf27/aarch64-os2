# Aarch64 OS

Aarch64 bare-metal operating system with custom boot file, kernel, and linker files

# Testing/Execution
As of right now, this program is expected to be tested with QEMU using the "virt" machine, a generic, vendor-neutral Aarch64 virtual platform that provides emulation for RAM and common devices. This program should be able to run on many Armv8 core, but I have chosen Cortex-A57 in my makefile as of right now. 

The makefile that comes with this program uses the aarch-64-linux-gnu-gcc cross compiler (tested on Debian AMD64 host) to build the Aarch64 assembly and C code. It uses qemu-system-aarch64 to emulate/execute this program. 

