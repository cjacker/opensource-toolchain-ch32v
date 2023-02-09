# Opensource toolchain for WCH CH32V RISC-V 32bit MCU

CH32V RISC-V 32bit MCU series is a family of General-Purpose RISC-V MCU from WCH.

If you want to learn more about it, please refer to http://www.wch-ic.com/products/categories/47.html?pid=5.

CH32V103/203/305/307 use a proprietary debugging interface named 'RVSWD' (similar to SWD) and requires a special (but not expensive) usb adapter named 'WCH-LINK' or 'WCH-LINKE' to program/debug. it was implemented in WCH forked OpenOCD as 'wlink' interface. 

At first, the WCH forked OpenOCD is close source and only provide binaries compiled for Windows and Linux by MounRiver Studio (an IDE based on eclipse for CH32V developent). Recently (2022-03), the private forked OpenOCD (ver 0.11.0-dev) is opensourced by the request of opensource developers (https://github.com/kprasadvnsi/riscv-openocd-wch).

When CH32V003 released, A new 1-wire proprietary interface named 'SDI' was introduced with CH32V003, it also need a 'WCH-LINKE' adapter, old 'WCH-LINK' adatper can not support this 1-wire interface. Up to now, only this OpenOCD fork (https://github.com/karlp/openocd-hacks/) can support it.


# Hardware prerequist

* A CH32V board, either ch32v003 or v103/v203/v305/v307, etc.
* A 'WCH-LINK' or 'WCH-LINKE' adapter
  - only 'WCH-LINKE' support programming CH32V003 with 1-wire SDI interface.
  - 'WCH-LINK' / 'WCH-LINKE' USB adapter usually support dual modes (SWD for ARM and RVSWD for CH32V) and can be toggled by jumpers or on-board buttons.

# Toolchain overview

* Compiler: gcc
* Debugger: openocd/gdb
* SDK: official EVT source package
* Programmer: ch55xtool for ISP mode / openocd for RVSWD(2 wire) and SDI(1 wire, used by CH32V003)

# RISC-V GNU Toolchain

CH32V is well supported by [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain).

The RISC-V GNU toolchain, which contains compilers and linkers like gcc and g++ as well as helper programs like objcopy and size is available from [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain). There are also some prebuilt release provided by nuclei or other teams, such as 'xpack', so you can either build it yourself or download a prebuilt release.

## Building from source

If you want to use a prebuilt release, just ignore this section.

Building a cross compile gnu toolchain was difficult long time ago, you need to understand and use configuration options very carefully. [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain) provided a simpler way to help us building a workable toolchain. It supports two build modes: a generic ELF/Newlib toolchain and a more sophisticated Linux-ELF/glibc toolchain. we only need the 'generic ELF/Newlib toolchain' for CH32V.

```
git clone https://github.com/riscv-collab/riscv-gnu-toolchain.git
cd riscv-gnu-toolchain
mkdir build
cd build
../configure --prefix=/opt/riscv-gnu-toolchain --disable-linux --with-abi=ilp32 --with-arch=rv32imac
make -j<nprocs>
make install
```

It will be installed to "/opt/riscv-gnu-toolchain" dir, and the [target triplet](https://wiki.osdev.org/Target_Triplet) of gcc should be 'riscv32-unknown-elf-'.

Since the prefix is not set to standard dir, you need add '/opt/riscv-gnu-toolchain/bin' to PATH env. for example, for bash, add it to ~/.bashrc:
```
export PATH=/opt/riscv-gnu-toolchain/bin:$PATH
```

## Use prebuilt toolchain

There are a lot of prebuilt riscv toolchains you can download and use directly if it support the arch 'rv32imac'. Here are two choices with well support.

*   MounRiver studio toolchain

MounRiver provide a prebuilt toolchain for linux, you can download it from [MounRiver Studio website](http://file.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.60.tar.xz). up to this tutorial written, the lastest version is "MRS_Toolchain_Linux_x64_V1.60.tar.xz", it include a prebuilt gnu toolchain and prebuilt openocd with RVSWD and SDI support.

After download finished, extract it to somewhere, and change the unprofessinal dir name and modify the PATH env, for example:

```
sudo mkdir -p /opt/mrs-riscv-toolchain
sudo tar xf MRS_Toolchain_Linux_x64_V1.60.tar.xz -C /opt/mrs-riscv-toolchain --strip-components=1 
# correct the dir name
sudo mv /opt/mrs-riscv-toolchain/"RISC-V Embedded GCC" /opt/mrs-riscv-toolchain/risv-none-embed-gcc
```

And add `/opt/mrs-riscv-toolchain/riscv-none-embed-gcc/bin` to PATH env according to your shell.

You also need add `/opt/mrs-riscv-toolchain/OpenOCD/bin` to PATH env if you want to use this prebuild OpenOCD.


**NOTE**, the target triplet of mrs riscv toolchain is **`riscv-none-embed`**.

*   Xpack riscv toolchain

[xpack-dev-tools](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack) provde a prebuilt toolchain for riscv. you can download it from [here](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases). 

**Note:** Due to the '-march' changes happened in gcc v12.0 and above, the latest prebuilt gcc version you can used with CH32V003 is 11.3.0. or use v10.2.0 from the deprecated [riscv-none-embed-gcc-xpack](https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack).

```
sudo mkdir -p /opt/xpack-riscv-toolchain
sudo tar xf xpack-riscv-none-elf-gcc-11.3.0-1-linux-x64.tar.gz -C /opt/xpack-riscv-toolchain --strip-components=1
```

and add `/opt/xpack-riscv-toolchain/bin` to PATH env according to your shell.

**NOTE**, the target triplet of xpack riscv toolchain is **`riscv-none-elf`**.

# SDK

If you used to do stm32 programming, you will feel very familiar with CH32V SDK.

WCH provide evaluate source code package for every CH32V MCU model and licensed under Apache-2.0, you can download them from WCH official website according to your MCU model.

For ch32v003, https://www.wch.cn/downloads/CH32V003EVT_ZIP.html

For ch32v103, http://www.wch.cn/downloads/CH32V103EVT_ZIP.html.

For ch32v20x, https://www.wch.cn/downloads/CH32V20xEVT_ZIP.html

For ch32v30x, http://www.wch.cn/downloads/CH32V307EVT_ZIP.html

These evt source code packages contains core SDK and a lot of demo routines but lack Makefile support, in this repo, I provide a simple script and a Makefile template to help you convert it. after 'CH32VxxxEVT.ZIP' downloaded, the conversion process as below:

```
git clone https://github.com/cjacker/opensource-toolchain-ch32v.git
cd opensource-toolchain-ch32v
mkdir evt
unzip CH32VxxxEVT.ZIP -d evt
# copy core sdk to project_template dir
cp -r evt/EVT/EXAM/SRC/* project_template
# take some codes from GPIO_Toggle demo.
cp -r evt/EVT/EXAM/GPIO/GPIO_Toggle/User project_template

cd project_template
# replace it with our blink demo
rm -f User/main.c
mv main.c User/
# clean up
rm -rf evt

# if ch32v103 bluepill, run
./generate_makefile bluepill103

# if ch32v203g6 flappyboard, run
./generate_makefile flappyboard203

# if ch32v003, run
./generate_makefile ch32v003
```

Before type `make' to build the project, you need to:

* ensure choosing the correct 'Startup' asm file.
* ensure the RAM/FLASH settings in `Ld/Link.ld` is correct.
* edit 'User/main.c' to setup correct GPIO port.

For example, for [flappyboard](https://github.com/metro94/FlappyBoard) with CH32V203G6, you should use `Startup/startup_ch32v20x_D6.S` and the 'MEMORY' section in 'Link.ld' also need to be modified. All these changes had been done in `generate_makefile` script, if it is not suite for your MCU model, please modify it to match your MCU model.

Then type `make` to build the project.

After building complete, you will get 'build/CH32V.elf', 'build/CH32V.hex' and 'build/CH32V.bin', which can be used for debugging and programming later.


# Flashing and Debugging

There is 2 way to programming a CH32V MCU: ISP and RVSWD.

## ISP programming

ISP programming doesn't need a WCH-LINK or WCH-LINKE adapter, you can connect the board directly to PC USB port.

A forked version of [ch55xtool](https://github.com/karlp/ch552tool) can support program WCH CH55x 8051 MCUs and WCH CH32V103/307 RISC-V MCU.

There is another opensource project [wchisp](https://github.com/ch32-rs/wchisp), but up to v2.0, it hang when probing device. I will try it again when it has a new release.

**Installation:**

```
git clone https://github.com/karlp/ch552tool.git
cd ch552tool
git checkout for-upstream-2
sudo install -m0755 ch55xtool/ch55xtool.py /usr/bin/ch55xtool
```

**Programming:**

You need enter ISP mode first. please find the 'BOOT0' and 'RESET' button on your development board

- hold the 'BOOT0' button down and power on the device (connect the device to USB port).

or 

- connect your development board directly to Linux PC USB port.
- hold the BOOT0 button down, press the RESET button then release it, after a while (about 1 second), release BOOT0 button.

Then 

- run `lsusb`, you will find something like '4348:55e0 WinChipHead'.

After enter ISP mode, take above blink example as demo (and wire up a LED to A5 pin), the programming process as below:

```
sudo ch55xtool -f build/CH32V.bin
```

You may need to press 'RESET' button again after programming.

## OpenOCD programming and debugging

CH32V do NOT support JTAG/SWD programming / debugging interface, it had implemented a private interface named 'RVSWD' for CH32V103 and above, and a 1-wire interface named SDI for CH32V003. 

You can not use SWD/JTAG usb adapters to program/debug CH32V. and these proprietary interfaces also can not be supported by official OpenOCD (up to now, the changes WCH made to OpenOCD is not upstreamed).

You have to prepare a 'WCH-LINK' or 'WCH-LINKE'(to program CH32V003) usb adapter and build a forked version OpenOCD with 'wlink' enabled.

**Build and Install WCH OpenOCD:**

If you choose to use MRS prebuilt toolchain and WCH OpenOCD for Linux (as mention in Compiler section), you can ignore the building process.

```
git clone https://github.com/karlp/openocd-hacks/
cd openocd-hacks
git submodule update --init --progress
./configure --prefix=/opt/wch-openocd --program-prefix=wch- --enable-wlink
make
sudo make install
```

After installation finished, add '/opt/wch-openocd/bin' to PATH env.

**Programming:**

Please wire up you 'WCH-LINK' or 'WCH-LINKE' usb adapter with your development board (as same as SWD) and use 'wch-riscv.cfg' (for MRS Toolchain) provide in this repo. For CH32V003, only the SWDIO pin is needed.

```
# erase all
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "flash erase_sector wch_riscv 0 last " -c exit
# program
sudo wch-openocd -f wch-riscv.cfg  -c init -c halt  -c "program xxx.hex\bin\elf "  -c exit
# verify
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "verify_image xxx.hex\bin\elf"    -c exit
# reset/resume
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c wlink_reset_resume    -c exit
```

**Debugging**

```
sudo openocd -f wch-riscv.cfg
# open another term
riscv-none-embed-gdb ./build/CH32V.elf
(gdb) target remote :3333
Remote debugging using :3333
_start () at Startup/startup_ch32v10x.S:15
15              j       handle_reset
(gdb) load
Loading section .init, size 0x38 lma 0x0
Loading section .vector, size 0x108 lma 0x38
Loading section .text, size 0x211c lma 0x140
Loading section .data, size 0x84 lma 0x225c
Start address 0x00000000, load size 8928
Transfer rate: 3 KB/sec, 2232 bytes/write.
(gdb)
```

