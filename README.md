# __timeshift
One more OS - Useless, Invalid, Unnecessary - Call it what you want.

Tested only in:
QEMU emulator version 3.1.0 (Debian 1:3.1+dfsg-2ubuntu3.7)

To BUILD:
Just change you working directory to __timeshift
Then, Type "./Run" in terminal

C Code Compiled Using:
gcc version 8.3.0 (Ubuntu 8.3.0-6ubuntu1)

Build Script in:
./run

Currently, Creates only Floopy Disk image during build.
Free Standing, Relocatable binary in ELF32 Format is created when build script runs.

Entry Point in kernel binary (ELF32) can be retrieved from offset 0x18.
Normally, Entry point is fixed at offset 0x10000.

For more information on ELF32, Please read this wikipedia article
https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
