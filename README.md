# ROSH Operating System

_ROSH_ is an operating system developed by Roei Aviad and Yoav Shaham from scratch.

The system includes:
- Boot loader
- GDT (Global Descriptor Table)
- IDT (Interrupt Descriptor Table)
- Heap memory implementation
- Paging system
- Keyboard handling
- Text mode output to screen
- Usermode and System calls
- Shell for user commands
- EXT-2 based file system (self implemented)
- Process manager and scheduling
- ASLR (Address space layout randomization)
- Networking system

## Requirements
- [QEmu](https://www.qemu.org/download/) emulator on a linux machine.
- Cross Compiler. See [crosscompiler.md](/crosscompiler.md) for more information.
- [Nasm](https://www.nasm.us/) assembler.

---

## Run
- In order to launch the OS, navigate to the project's main folder, and run `make`.
- Debug the OS using `make debug`.
- Offline run using `make offline`.

**:warning: Notice:**
> - The OS uses a tap interface in order to connect to the real network. Therefore, **it might shut the host machine's network access**.
> If the network does not go back up after _ROSH_ is down, it can be restored manually:
>> 1. run `make clean_network`.
>> 2. restart the host's networking interface.

> - The networking is unstable and may cause _ROSH_ to restart or fault unexpectedly.

## Shell Commands

- help - Print information about available commands.
- echo - Print message to the screen or text to file.
- color - Change the shell's colors.
- shutdown - shutdown the computer.
- bc - basic calculator.
- ls - list items in folder.
- pwd - print working directory.
- cd - change working directory.
- cat - print file content.
- rm - remove file or folder.
- touch - create file.
- mkdir - create folder.
- test - test several key features of the OS.
- exec - run binary file from base64 file. run `./assembler asm_code` and enter the output to a _ROSH_ file.
- net - perform network commands.

## Screenshots
![Alt text](/images/s1.png)
![Alt text](/images/s2.png)

## LICENSE

MIT License

Copyright (c) 2022 Roei Aviad and Yoav Shaham

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.