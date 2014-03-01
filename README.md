PS3 Autotests
=============

A repository of PS3 programs performing several tests on the PS3 platform. Inspired by the [PSP Autotests](https://github.com/hrydgard/pspautotests) repository, and shares some goals with it and proposes some other ones:

* Allow people to see how to use some obscure-newly-discovered APIs and features.
* Allow PS3 emulators to avoid some regressions while performing refactorings and to have a reference while implementing APIs.
* Provide a platform for benchmarking PS3 emulators and comparing their performance with a real PS3.

The main idea behind this is having several files per test unit:

* _file.expected_:  File with the expected TTY output, preferably from a real PS3.
* _file.elf_:  The program that will call printf / sys_tty_write SysCall in order to generate an output.
* _file.c_:  Include the source code or at least a brief description of the unit test (Optional).

## Building the tests
TODO

## Running tests on a real PS3
TODO

## Running tests on a PS3 emulator
TODO