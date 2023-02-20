# Opensource toolchain for WCH CH32V RISC-V 32bit MCU

WCH CH32V series is a family of General-Purpose 32bit RISC-V MCU based on QingKe core.

If you want to learn more about it, please refer to http://www.wch-ic.com/products/categories/47.html?pid=5.

CH32V103/203/208/305/307 use a proprietary debugging interface named 'RVSWD' (similar to SWD) and requires a special (but not expensive) usb adapter named 'WCH-LINK' or 'WCH-LINKE' to program/debug. it was implemented in WCH forked OpenOCD as 'wlink' interface. 

At first, the WCH forked OpenOCD is close sourced and only provide binaries compiled for Windows and Linux by MounRiver Studio (an IDE based on eclipse for CH32V developent). Recently (2022-03), the private forked OpenOCD (ver 0.11.0-dev) is opensourced by the request of opensource developers (https://github.com/kprasadvnsi/riscv-openocd-wch).

When CH32V003 released, A new 1-wire proprietary interface named 'SDI' was introduced with CH32V003, it need a 'WCH-LINKE' adapter instead old 'WCH-LINK', 'WCH-LINK'(without E) adatper can not support this 1-wire debugging interface. Up to now, only this OpenOCD fork (https://github.com/karlp/openocd-hacks/) can support the 1-wire interface.

By the way, WCH CH571/573 and CH581/582/583 are series of 32-bit RISC-V core microcontroller integrated with BLE wireless communication, these parts also covered by this tutorial.

# Table of contents
- [Hardware prerequist](https://github.com/cjacker/opensource-toolchain-ch32v#hardware-prerequist)
- [Toolchain overview](https://github.com/cjacker/opensource-toolchain-ch32v#toolchain-overview)
- [RISC-V GNU Toolchain](https://github.com/cjacker/opensource-toolchain-ch32v#risc-v-gnu-toolchain)
  + [Building from source](https://github.com/cjacker/opensource-toolchain-ch32v#building-from-source)
  + [Use prebuilt toolchain](https://github.com/cjacker/opensource-toolchain-ch32v#use-prebuilt-toolchain)
    - [MounRiver studio toolchain](https://github.com/cjacker/opensource-toolchain-ch32v#mounriver-studio-toolchain)
    - [Xpack riscv toolchain](https://github.com/cjacker/opensource-toolchain-ch32v#xpack-riscv-toolchain)
- [SDK](https://github.com/cjacker/opensource-toolchain-ch32v#sdk)
  + [For CH32V](https://github.com/cjacker/opensource-toolchain-ch32v#for-ch32v)
  + [For CH5XX RISC-V BLE](https://github.com/cjacker/opensource-toolchain-ch32v#for-ch5xx-risc-v-ble)
- [Programming](https://github.com/cjacker/opensource-toolchain-ch32v#programming)
  + [ISP programming](https://github.com/cjacker/opensource-toolchain-ch32v#isp-programming)
  + [OpenOCD programming](https://github.com/cjacker/opensource-toolchain-ch32v#openocd-programming)
- [Debugging](https://github.com/cjacker/opensource-toolchain-ch32v#debugging)
- [Project templates](https://github.com/cjacker/opensource-toolchain-ch32v/edit/main/README.md#project-templates)
  + ch32v003evt
  + ch32v103evt
  + ch32v20xevt
  + ch32v307evt
  + ch573evt
  + ch583evt
- [How to switch between dual modes of WCH-LinkE](https://github.com/cjacker/opensource-toolchain-ch32v/blob/main/README.md#how-to-switch-modes-of-wch-linke)

# Hardware prerequist

* A CH32V board or WCH CH5xx RISC-V BLE board
* A 'WCH-LINKE' adapter (WCH-LINK without E is deprecated)
  - old 'WCH-LINK' (without E) do not support programming CH32V003 with 1-wire SDI interface.
  - WCH official utilities will flash a new firmware to 'WCH-LINK' (without E) every time when switch between DAP and RV mode.
  - The new firmware do not support using on-board button to toggle 'WCH-LINK' (without E) mode anymore.

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

There are a lot of prebuilt riscv toolchains, Here are two choices with well support.

### MounRiver studio toolchain

MounRiver provide a prebuilt toolchain for linux, you can download it from [MounRiver Studio website](http://file.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.60.tar.xz). up to this tutorial written, the lastest version is "MRS_Toolchain_Linux_x64_V1.60.tar.xz", it include a prebuilt gnu toolchain (gcc 8.2.0) and prebuilt openocd with RVSWD and SDI support.

After download and extract it to somewhere, Please change the toolchain's bad dir name:

```
sudo mkdir -p /opt/mrs-riscv-toolchain
sudo tar xf MRS_Toolchain_Linux_x64_V1.60.tar.xz -C /opt/mrs-riscv-toolchain --strip-components=1 
# correct the dir name with space
sudo mv /opt/mrs-riscv-toolchain/"RISC-V Embedded GCC" /opt/mrs-riscv-toolchain/risv-none-embed-gcc
```

And add `/opt/mrs-riscv-toolchain/riscv-none-embed-gcc/bin` to PATH env according to your shell.

You also need add `/opt/mrs-riscv-toolchain/OpenOCD/bin` to PATH env if you want to use this prebuilt OpenOCD.

**NOTE**, the target triplet of mrs riscv toolchain is **`riscv-none-embed`**.

#### Xpack riscv toolchain

[xpack-dev-tools](https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack) provde a prebuilt toolchain for riscv. you can download it from [here](https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack). although the riscv-none-embed-gcc-xpack had been marked as deprecated, but you'd better stay with riscv gcc v10.2 due to the riscv '-march' changes happened in gcc v12.0 and above.

After download:

```
sudo mkdir -p /opt/xpack-riscv-toolchain
sudo tar xf xpack-riscv-none-embed-gcc-10.2.0-1.2-linux-x64.tar.gz -C /opt/xpack-riscv-toolchain --strip-components=1
```

and add `/opt/xpack-riscv-toolchain/bin` to PATH env according to your shell.

**NOTE**, the triplet of xpack prebuilt toolchain is **`riscv-none-embed`**.


# SDK

If you used to do stm32 programming, you will feel very familiar with CH32V SDK.

WCH provide evaluate source code package for every CH32V MCU model and licensed under Apache-2.0, you can download them from WCH official website according to your MCU model.

For ch32v003, https://www.wch.cn/downloads/CH32V003EVT_ZIP.html

For ch32v103, http://www.wch.cn/downloads/CH32V103EVT_ZIP.html.

For ch32v20x, https://www.wch.cn/downloads/CH32V20xEVT_ZIP.html

For ch32v30x, http://www.wch.cn/downloads/CH32V307EVT_ZIP.html

For ch571/573, https://www.wch.cn/downloads/CH573EVT_ZIP.html

For ch581/582/583, https://www.wch.cn/downloads/CH583EVT_ZIP.html

These evt packages contains core SDK and a lot of demo routines but lack Makefile support, here provide [ch32v evt project template and convertor](https://github.com/cjacker/ch32v_evt_makefile_gcc_project_template) and [ch5xx evt project template and convertor](https://github.com/cjacker/ch5xx_riscv_ble_evt_makefile_gcc_project_template) to convert EVT packages to makefile project.

## For CH32V EVT Packages

```
git clone https://github.com/cjacker/ch32v_evt_makefile_gcc_project_template
cd ch32v_evt_makefile_gcc_project_template
./generate_project_from_evt.sh <part>
```
the `<part>` can be obtained with running `./generate_project_from_evt.sh` without any args.

Then type `make` to build the project.

After building complete, you will get `build/<part>.elf`, `build/<part>.hex` and `build/part.bin`, which can be used for debugging and programming later.


## For CH5XX RISC-V BLE

```
git clone https://github.com/cjacker/ch5xx_riscv_ble_evt_makefile_gcc_project_template
cd ch5xx_riscv_ble_evt_makefile_gcc_project_template
./generate_project_from_evt.sh <part>
```

the `<part>` can be obtained with running `./generate_project_from_evt.sh` without any args.

Then type `make` to build the project.

After building complete, you will get `build/<part>.elf`, `build/<part>.hex` and `build/part.bin`, which can be used for debugging and programming later.

**Note 1:** These demo only blink LED connect to PA8.

**Note 2:** 'CH573EVT.ZIP' and 'CH583EVT.ZIP' is partial opensourced, the static library named 'libISP573.a' and 'libISP583.a' is in binary format.

# Programming

There is 2 way to programming a CH32V MCU: ISP and RVSWD.

## ISP programming

ISP programming doesn't need a WCH-LINKE adapter, it program the target device via USB port directly.

The best opensource WCH ISP tool is [wchisp](https://github.com/ch32-rs/wchisp), which support more parts than other solutions, it is written in rust lang.

**Installation:**

```
git clone https://github.com/ch32-rs/wchisp.git
cd wchisp
cargo build --release
sudo install -m0755 target/release/wchisp /usr/bin/wchisp
```

**Programming:**

You need enter ISP mode first. please find the 'BOOT0' and 'RESET' button on your development board

- hold the 'BOOT0' button down and power on the device (connect the device to USB port).

or 

- connect your development board directly to Linux PC USB port.
- hold the BOOT0 button down, press the RESET button then release it, after a while (about 1 second), release BOOT0 button.

Then 

- run `lsusb`, you will find something like '4348:55e0 WinChipHead'.

After enter ISP mode, take above blink example as demo (change the LED port according to your board), the programming process as below:

```
sudo wchisp flash build/ch32v.bin
```

You may need to press 'RESET' button to reset the board after programming.

A forked version of [ch55xtool](https://github.com/karlp/ch552tool) can also support program WCH CH32V103/307, you can have a try yourself.

## OpenOCD programming

CH32V do NOT support JTAG and SWD programming / debugging interface, You can not use SWD/JTAG usb adapters to program/debug CH32V. It had a private interface named 'RVSWD' for CH32V103 and above, and a 1-wire interface named 'SDI' for CH32V003. 

And these proprietary interfaces also can not be supported by upstream OpenOCD (up to now, the changes WCH made to OpenOCD is not upstreamed).

You have to prepare a 'WCH-LINKE' usb adapter and build a forked version OpenOCD with 'wlink' interface enabled.

**Build and Install WCH OpenOCD:**

If you choose to use MRS prebuilt toolchain and WCH OpenOCD for Linux (as mention in Compiler section), just ignore the building process.

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

Please wire up 'WCH-LINKE' adapter with your development board (pins as same as SWD) and use 'wch-riscv.cfg' (from MRS Toolchain) provide in this repo. For CH32V003, only the 'PD1 / SWDIO' pin is needed.

```
# erase all
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "flash erase_sector wch_riscv 0 last " -c exit
# program and verify
sudo wch-openocd -f wch-riscv.cfg  -c init -c halt  -c "program xxx.hex\bin\elf verify" -c exit
# verify
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "verify_image xxx.hex\bin\elf" -c exit
# reset/resume
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "wlink_reset_resume" -c exit
```

For all makefile demos in this repo, you can use 'make program' to program the target device.

# Debugging

Launch OpenOcd to connect to the target device:
```
sudo openocd -f wch-riscv.cfg
```
Open another terminal and run:
```
riscv-none-embed-gdb ./build/ch32v103.elf
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

Then you can set breakpoints, run, step, continue as usual gdb.

# Project templates
The pre-converted project templates from WCH official EVT packages and supported parts:
- [ch32v003evt_gcc_makefile](https://github.com/cjacker/ch32v003evt_gcc_makefile)
  + ch32v003j4m6
  + ch32v003a4m6
  + ch32v003f4u6
  + ch32v003f4p6
- [ch32v103evt_gcc_makefile](https://github.com/cjacker/ch32v103evt_gcc_makefile)
  + ch32v103c6t6
  + ch32v103c8u6
  + ch32v103c8t6
  + ch32v103r8t6
- [ch32v20xevt_gcc_makefile](https://github.com/cjacker/ch32v20xevt_gcc_makefile)
  + ch32v203f6p6
  + ch32v203g6u6
  + ch32v203k6t6
  + ch32v203c6t6
  + ch32v203f8p6
  + ch32v203f8u6
  + ch32v203g8r6
  + ch32v203k8t6
  + ch32v203c8t6
  + ch32v203c8u6
  + ch32v203rbt6
  + ch32v208gbu6
  + ch32v208cbu6
  + ch32v208rbt6
  + ch32v208wbu6
- [ch32v307evt_gcc_makefile](https://github.com/cjacker/ch32v307evt_gcc_makefile)
  + ch32v303cbt6
  + ch32v303rbt6
  + ch32v303rct6
  + ch32v303vct6
  + ch32v305fbp6
  + ch32v305rbt6
  + ch32v307rct6
  + ch32v307wcu6
  + ch32v307vct6
- [ch573evt](https://github.com/cjacker/ch573evt_gcc_makefile)
  + ch573
  + ch571
- [ch583evt](https://github.com/cjacker/ch583evt_gcc_makefile)
  + ch583
  + ch582
  + ch581

# How to switch modes of WCH-LinkE

Usually, there are buttons on WCH-LinkE adapter, hold the 'mode' button download and plugin, it will switch to another mode.

WCH-LinkE also support switching modes by software, Please refer to https://github.com/cjacker/wchlinke-mode-switch, a command line tool support switching WCH-LinkE modes between DAP / RV from Host.

