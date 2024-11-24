# Opensource toolchain for WCH CH32V RISC-V 32bit MCU

WCH CH32V series is a family of General-Purpose 32bit RISC-V MCU, please refer to [official website](http://www.wch-ic.com/products/categories/47.html?pid=5) for more infomations.

# Table of contents
- [Hardware prerequist](https://github.com/cjacker/opensource-toolchain-ch32v#hardware-prerequist)
- [Toolchain overview](https://github.com/cjacker/opensource-toolchain-ch32v#toolchain-overview)
- [RISC-V GNU Toolchain](https://github.com/cjacker/opensource-toolchain-ch32v#risc-v-gnu-toolchain)
  + [Building from source](https://github.com/cjacker/opensource-toolchain-ch32v#building-from-source)
  + [Use prebuilt toolchain](https://github.com/cjacker/opensource-toolchain-ch32v#use-prebuilt-toolchain)
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

# Hardware prerequist

* A CH32V board or WCH CH5xx RISC-V BLE board
* A 'WCH-LinkE' adapter
  - either WCH-LinkE r0-1v2 or 1v3 supported by my forked openocd.
  - future version of WCH-LinkE may also be supported.
  - old WCH-Link (without E) adapter can NOT program CH32V003 series.

WCH have an official online store on AliExpress, you can buy EVT board and WCH-LinkE adapter from it.

# Toolchain overview

* Compiler: gcc
* SDK: WCH official EVT source package
* Programmer:
  - [patched OpenOCD](https://github.com/cjacker/wch-openocd) for RVSWD(2 wire) and SDI(1 wire, used by CH32V003)
    * [wlink](https://github.com/ch32-rs/wlink) to switch WCH-LinkE adapter to RV mode.
  - official [WCHISPTool_CMD](http://wch-ic.com/downloads/WCHISPTool_CMD_ZIP.html) for ISP mode (close source)
  - [wchisp](https://github.com/ch32-rs/wchisp) for ISP mode
* Debugger: patched OpenOCD and gdb

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
**NOTE**, for ch32v003, you should build gcc with `--with-abi=ilp32e --with-arch=rv32ec_zicsr`.

It will be installed to "/opt/riscv-gnu-toolchain" dir, and the [target triplet](https://wiki.osdev.org/Target_Triplet) of gcc should be 'riscv32-unknown-elf-'.

Since the prefix is not set to standard dir, you need add '/opt/riscv-gnu-toolchain/bin' to PATH env. for example, for bash, add it to ~/.bashrc:
```
export PATH=/opt/riscv-gnu-toolchain/bin:$PATH
```

## Use prebuilt toolchain

[xpack-dev-tools](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/) provde a prebuilt toolchain for riscv. you can download it from https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/. The lastest version is '14.2.0', download and extract it as:

```
sudo mkdir -p /opt/xpack-riscv-toolchain
sudo tar xf xpack-riscv-none-elf-gcc-14.2.0-2-linux-x64.tar.gz -C /opt/xpack-riscv-toolchain --strip-components=1
```

And add `/opt/xpack-riscv-toolchain/bin` to PATH env according to your shell.

**NOTE**, the target triplet of xpack riscv toolchain is **`riscv-none-elf`** and it can support ch32v003 development.

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

**Note 2:** 'CH573EVT.ZIP' and 'CH583EVT.ZIP' is partial opensourced, there is no sources of static library 'libISP573.a' and 'libISP583.a'.

# Programming

## ISP programming

ISP programming doesn't need a WCH-LinkE adapter, it program the target device via USB port directly.

WCH officially provides `WCHISPTool_CMD` tool, it is close-source but provide prebuilt cli binaries for windows/macosx/linux platform, you can download it from [wch official website](https://wch-ic.com/downloads/WCHISPTool_CMD_ZIP.html).

The best opensource WCH ISP tool is [wchisp](https://github.com/ch32-rs/wchisp), which support more parts than other opensource solutions.

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

A forked version of [ch55xtool](https://github.com/karlp/ch552tool) can also support program WCH CH32V103/307, please have a try yourself.

## OpenOCD programming

CH32V103/203/208/305/307 use a proprietary debugging interface named 'RVSWD' (similar to SWD) and requires a special (but not expensive) usb adapter named 'WCH-Link' or 'WCH-LinkE' to program/debug. it was implemented in WCH forked OpenOCD as 'wlink' interface. 

At first, WCH private-forked OpenOCD is close sourced and only provide binaries compiled for Windows and Linux by MounRiver Studio (an IDE based on eclipse for CH32V developent). Later (2022-03), the [private-forked OpenOCD](https://github.com/kprasadvnsi/riscv-openocd-wch) is opensourced by the request of opensource developers, but no update after that, and it can not support ch32v003 which released later.

When CH32V003 released, A new 1-wire proprietary interface named 'SDI' was introduced with CH32V003, it need a 'WCH-LinkE' adapter instead old 'WCH-Link', 'WCH-Link'(without E) adapter can not support this 1-wire debugging interface and the old version forked OpenOCD can not support WCH-LinkE.

Another developer got the updated WCH OpenOCD sources and create [this OpenOCD fork](https://github.com/karlp/openocd-hacks/), this fork is able to support the 1-wire SDI interface. but as reported by some users, it can not support WCH-LinkE r0 1v3. I create a [openocd fork](https://github.com/cjacker/wch-openocd), and add WCH-LinkE r0 1v3 and maybe future version support.

**Build and Install WCH OpenOCD:**

```
git clone https://github.com/cjacker/wch-openocd
cd wch-openocd

./configure --prefix=/opt/wch-openocd --program-prefix=wch- --enable-wlink
make
sudo make install
```
After installation finished, add '/opt/wch-openocd/bin' to PATH env.


**Programming:**

Please wire up 'WCH-LinkE' adapter with your development board (pins as same as SWD) and use 'wch-riscv.cfg' (from MRS Toolchain) provide in this repo. For CH32V003, only 'PD1 / SWDIO' pin is needed.

Since WCH-LinkE support RV/DAP dual mode, please make sure your WCH-LinkE adapter is in RV mode, you can use [wlink](https://github.com/ch32-rs/wlink) to switch mode of WCH-LinkE, to switch to RV mode:
```
wlink mode-switch --rv

```

After switch to RV mode


```
# to erase all
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "flash erase_sector wch_riscv 0 last " -c exit
# to program and verify
sudo wch-openocd -f wch-riscv.cfg  -c init -c halt  -c "program xxx.hex\bin\elf verify" -c exit
# to verify
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "verify_image xxx.hex\bin\elf" -c exit
# to reset/resume
sudo wch-openocd -f wch-riscv.cfg -c init -c halt -c "wlink_reset_resume" -c exit
```

Actually, the 'wch-riscv.cfg' is a combination of 'scripts/interface/wlink.cfg' and 'scripts/target/wch-riscv.cfg' of the patched OpenOCD, you can also use these two config files.

For all examples in this repo, you can use 'make program' to program the target device.

# Debugging

Launch OpenOcd to connect to the target device:
```
sudo openocd -f wch-riscv.cfg
```
Open another terminal and run:
```
riscv-none-elf-gdb ./build/ch32v103.elf
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
