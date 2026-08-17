# Aarch64 OS

Aarch64 bare-metal operating system with custom boot file, kernel, and linker files

# Testing/Execution
As of right now, this program is expected to be tested with QEMU using the "virt" machine (Cortex a57 core), a generic, vendor-neutral Aarch64 virtual platform that provides emulation for RAM and common devices.  

The makefile that comes with this program uses the aarch64-none-elf-gcc cross compiler (tested on Debian AMD64 host) to build the Aarch64 assembly and C code. It uses qemu-system-aarch64 to emulate/execute this program. 

