# Opensource toolchain for WCH CH32V RISC-V 32bit MCU

WCH CH32V series is a family of General-Purpose 32bit RISC-V MCU, please refer to [official website](http://www.wch-ic.com/products/categories/47.html?pid=5) for more infomations.

# Table of contents
- [Hardware prerequist](#hardware-prerequist)
- [Toolchain overview](#toolchain-overview)
- [RISC-V GNU Toolchain](#risc-v-gnu-toolchain)
  + [Building from source](#building-from-source)
  + [Use prebuilt toolchain](#use-prebuilt-toolchain)
- [SDK](#sdk)
  + [For CH32V](#for-ch32v-evt-packages)
  + [For CH5XX RISC-V BLE](#for-ch5xx-risc-v-ble)
- [Programming](#programming)
  + [ISP programming](#isp-programming)
  + [WCH-LinkE programming](#wch-linke-programming)
- [Debugging](#debugging)
- [Project templates](#project-templates)
  + ch32v003evt
  + ch32x035evt
  + ch32v103evt
  + ch32l103evt
  + ch32v20xevt
  + ch32v307evt
  + ch573evt
  + ch583evt
  + ch585evt
  + ch592evt
- [How to update firmware of WCH-Link/E]()
- [How to rescure bricked CH32V and WCH-Link/E]()

# Hardware prerequist

* CH32V/X/L board
* WCH-LinkE(ch32v305fbp6 based) or WCH-Link(ch549 based), with latest firmware

**Note:** WCH-Link does not support 1-wire SDI interface used by ch32v003, you need a WCH-LinkE to program/debug ch32v003.

WCH have an official online store on AliExpress, you can buy the EVT boards and WCH-LinkE from it.

# Toolchain overview

* Compiler: gcc
* SDK: WCH official EVT source package
* Programmer:
  - [wchisp](https://github.com/ch32-rs/wchisp) for ISP mode
  - [wlink](https://github.com/ch32-rs/wlink) to work with WCH-LinkE.
  - [WCH OpenOCD](https://github.com/cjacker/wch-openocd) to work with WCH-LinkE.
  - official [WCHISPTool_CMD](http://wch-ic.com/downloads/WCHISPTool_CMD_ZIP.html) for ISP mode (close source)
* Debugger: WCH OpenOCD and gdb

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

### MRS toolchain
MounRiver Studio (WCH official IDE) provide a standalone linux toolchain that you can use directly, especially for some chips with WCH private RISCV instructions, for example CH584/585, you have to use [MRS toolchain](http://www.mounriver.com/download), up to now, the currect version is v1.92.1, you can download it from [here](http://file-oss.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.92.1.tar.xz).

Download and extract it as:
```
wget http://file-oss.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.92.1.tar.xz
sudo mkdir -p /opt/MRS_toolchain
sudo tar xf MRS_Toolchain_Linux_x64_V1.92.1.tar.xz -C /opt/MRS_toolchain --strip-components=1
```

And add `/opt/MRS_toolchain/RISC-V_Embedded_GCC12/bin` to your PATH env.

**NOTE**, the target triplet of MRS toolchain is **`riscv-none-elf`**.


### xpack riscv toolchain
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

For ch32x035, https://www.wch.cn/downloads/CH32X035EVT_ZIP.html

For ch32v103, http://www.wch.cn/downloads/CH32V103EVT_ZIP.html.

For ch32l103, https://www.wch.cn/downloads/CH32L103EVT_ZIP.html

For ch32v20x, https://www.wch.cn/downloads/CH32V20xEVT_ZIP.html

For ch32v30x, http://www.wch.cn/downloads/CH32V307EVT_ZIP.html

For ch571/573, https://www.wch.cn/downloads/CH573EVT_ZIP.html

For ch581/582/583, https://www.wch.cn/downloads/CH583EVT_ZIP.html

For ch584/585, https://www.wch.cn/downloads/CH585EVT_ZIP.html

For ch591/592, https://www.wch.cn/downloads/CH592EVT_ZIP.html

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

After building complete, you will get `build/<part>.elf`, `build/<part>.hex` and `build/<part>.bin`, which can be used for debugging and programming later.

**Note 1:** These demo only blink LED connect to PA8.

**Note 2:** These evt packages are partial opensourced, there is no source of ISP static library.

**Note 3:** **For ch584/585, you have to use [MRS toolchain](http://file-oss.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.92.1.tar.xz) to support 'mcpy' instruction.**

# Programming

## ISP programming

ISP programming doesn't need a WCH-LinkE adapter, it program the target device via USB port directly.

### with wchisp

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

After enter ISP mode, you can program the target board as:

```
sudo wchisp flash build/ch32v.bin
```

You may need to press 'RESET' button to reset the board after programming.

A forked version of [ch55xtool](https://github.com/karlp/ch552tool) can also support program WCH CH32V103/307, you can have a try yourself.

### with official WCHISPTool_CMD (clsoe source)

WCH officially provides `WCHISPTool_CMD` tool, it is close-source but provide prebuilt cli binaries for windows/macosx/linux platform, you can download it from [wch official website](https://wch-ic.com/downloads/WCHISPTool_CMD_ZIP.html).

```
=====ISP_Command_Tool=====

TOOL VERSION:  V3.70

Usage: WCHISPTool_CMD [-pbvecofh]
  -p --device    device to operate
  -b --speed     uart speed
  -v --version   boot/tool version
  -c --configure configure file path
  -o --operation operation type
  -f --flash       flash file path
  -r --operation write protect option
```

## WCH-LinkE programming

CH32V103/203/208/305/307 use a proprietary 2-wire debugging interface named 'RVSWD' and requires
 a special (but very cheap) usb programmer/debugger named 'WCH-Link' or 'WCH-LinkE', and the software support was implemented in WCH forked OpenOCD as 'wlink' interface.

When CH32V003 released, A new 1-wire interface named 'SDI' was introduced with CH32V003, Only 'WCH-LinkE' can support SDI interface.

Before programming with WCH-LinkE, please wire up 'WCH-LinkE' with target board (pins as same as SWD). for CH32V003, only 'PD1 / SWDIO' pin is needed.

Since WCH-LinkE support dual mode (RV and DAP), please make sure your WCH-LinkE adapter is in RV mode. refer to next section to learn how to switch mode of WCH-LinkE with [wlink](https://github.com/ch32-rs/wlink).

### update WCH-LinkE firmware to latest version

If you encounter any error when using WCH-LinkE, you should try to update its firmware first. 

Download [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html), when connect to target, it will popup a window to ask you update firmware or not.

**NOTE:** WCH-LinkUtility can only run under windows.

### with wlink


[wlink](https://github.com/ch32-rs/wlink/) is a command line tool work with WCH-LinkE programmer/debugger.

wlink features:
- Flash firmware, support Intel HEX, ELF and raw binary format
- Erase chip
- Halt, resume, reset support
- Read chip info
- Read chip memory(flash)
- Read/write chip register - very handy for debugging
- Code-Protect & Code-Unprotect for supported chips
- Enable or Disable 3.3V, 5V output
- SDI print support, requires 2.10+ firmware
- Serial port watching for a smooth development experience

**Installation:**
```
cargo install --git https://github.com/ch32-rs/wlink
```

**WCH-LinkE mode switch:**

Switch WCH-LinkE to RV mode:

```
wlink mode-switch --rv
```

**Programming:**
```
wlink flash firmware.bin
```

### with OpenOCD

I put the latest source of WCH official OpenOCD [here](https://github.com/cjacker/wch-openocd). It can work with all known version of WCH-LinkE and support all WCH CH32V/L/X series MCUs.

**Installation:**

```
git clone https://github.com/cjacker/wch-openocd
cd wch-openocd

./configure --prefix=/opt/wch-openocd --program-prefix=wch- --enable-wlinke --disable-ch347 --disable-linuxgpiod --disable-werror

make
sudo make install
```

After installation finished, add '/opt/wch-openocd/bin' to PATH env.


**Programming:**

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

For all examples and project templates in this repo, type `make program` to program the target device.

# Debugging

Launch OpenOcd to connect to the target device:
```
sudo wch-openocd -f wch-riscv.cfg
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
- [ch32x035evt_gcc_makefile](https://github.com/cjacker/ch32x035evt_gcc_makefile)
  + ch32x035r8t6
  + ch32x035c8t6
  + ch32x035g8u6
  + ch32x035g8r6
  + ch32x035f8u6
  + ch32x035f7p6
  + ch32x033f8p6
- [ch32v103evt_gcc_makefile](https://github.com/cjacker/ch32v103evt_gcc_makefile)
  + ch32v103c6t6
  + ch32v103c8u6
  + ch32v103c8t6
  + ch32v103r8t6
- [ch32l103evt_gcc_makefile](https://github.com/cjacker/ch32l103evt_gcc_makefile)
  + ch32l103f8p6
  + ch32l103f8u6
  + ch32l103g8r6
  + ch32l103k8u6
  + ch32l103c8t6
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
- [ch585evt](https://github.com/cjacker/ch583evt_gcc_makefile)
  + ch585
  + ch584
- [ch592evt](https://github.com/cjacker/ch592evt_gcc_makefile)
  + ch592
  + ch591

# How to update firmware of WCH-Link/E

## download latest firmwares
Official firmwares from WCH can be extracted from [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html). Download and extract it, find the `Firmware_Link` dir:
```
Firmware_Link/
├── WCH-Link_APP_IAP_ARM.bin   # DAP firmware for WCH-Link
├── WCH-Link_APP_IAP_RV.bin    # RVSWD firmware for WCH-Link
├── WCH-LinkE-APP-IAP.bin      # firmware for WCH-LinkE

```

At first, WCH-Link can also toggle DAP/RV mode by a button or software like WCH-LinkE, but due to the firmware size increased, it doesn't support dual mode anymore. MounRiver Studio also flash the corresponding firmware everytime when you toggle the mode of WCH-Link.

## update firmware of WCH-Link

WCH-Link use CH549 mcu, it's a 8051 MCU and can be programmed by ISP under linux.

To program CH549 mcu, we need install [ch552tool](https://github.com/MarsTechHAN/ch552tool) first. And short DP pin (P5.1) and 3v3 VCC pin when power up to enter ISP mode.

After enter ISP mode, `lsusb` like:

```
Bus 001 Device 020: ID 4348:55e0 WinChipHead
```

Then erase the flash as:
```
# sudo ch55xtool -e -c
Found CH549 with SubId:18
BTVER:02.40
UID:64-63-49-43-00-00-AD-A6
Erasing chip data flash. Done.
Erasing chip flash. Done.
Finalize communication. Done.
```

Then program `WCH-Link_APP_IAP_RV.bin` or `WCH-Link_APP_IAP_ARM.bin` as:
```
sudo ch55xtool -f WCH-Link_APP_IAP_RV.bin
Found CH549 with SubId:18
BTVER:02.40
UID:64-63-49-43-00-00-AD-A6
Erasing chip flash. Done.
Flashing chip. Done.
Finalize communication. Done.
```

## update firmware of WCH-LinkE

WCH-LinkE use CH32V305fbp6, you need another workable WCH-LinkE to program it.

Wire up WCH-LinkE and the target WCH-LinkE as:
```
 +-----------+            +-----------+
 |           |----3v3-----|           |
 |           |            |           |
 |           |----gnd-----|  Target   |
 | WCH LinkE |            | WCH LinkE |
 |           |----dio-----|           |
 |           |            |           |
 |           |----clk-----|           |
 +-----------+            +-----------+
```                                           


Hold the "IAP" button on target WCH LinkE and plug WCH LinkE to PC USB port, using wlink to program it as:

```
wlink flash WCH-LinkE-APP-IAP.bin 
```

# How to rescue bricked CH32V

**NOTE: it can also rescue a bricked WCH-LinkE.**

If accidently programing the wrong firmware to target board, the SWDIO/SWCLK pins may be occupied for other purpose, the ch32v will be bricked. And it can not be probed and programmed by WCH-LinkE or ISP mode anymore.

To rescue a bricked CH32V, we should erase the code flash totally. 

You need prepare a workable WCH-LinkE (not WCH-Link) and download [WCH-LinkUtility](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html).

## Clear all code flash by pin NRST

Wire up WCH-LinkE and bricked CH32V as:
```
 +------------+            +---------------+
 |            |----3v3-----|               |
 |            |            |               |
 |            |----gnd-----|               |
 |            |            |               |
 | WCH-LinkE  |----dio-----| bricked CH32V |
 |            |            |               |
 |            |----clk-----|               |
 |            |            |               |
 |            |----nrst----|               |
 +------------+            +---------------+
```                                           


Open WCH LinkUtility and select the correct MCU series as:

<p align="center">
<img src="https://raw.githubusercontent.com/cjacker/opensource-toolchain-ch32v/refs/heads/main/assets/select_ch32v30x.png" width="70%"/></p>

Then click 'Clear All Code Flash By Pin NRST' will erase code flash of bricked CH32V:

<p align="center">
<img src="https://raw.githubusercontent.com/cjacker/opensource-toolchain-ch32v/refs/heads/main/assets/erase_by_nrst.png" width="70%"/></p>

If the bricked board did not export NRST pin, you can solder a wire to MCU NRST pin directly.

## Clear all code flash by power off

This way don't require NRST pin, wire up WCH-LinkE and bricked CH32V as:
```
 +------------+            +---------------+
 |            |            |               |
 |            |            |               |
 |            |----dio-----|               |
 |            |            |               |
 | WCH-LinkE  |            | bricked CH32V |
 |            |            |               |
 |            |----clk-----|               |
 |            |            |               |
 |            |            |               |
 +------------+            +---------------+
```

Open WCH LinkUtility and click 'Clear All Code Flash By Power Off'. 

<p align="center">
<img src="https://raw.githubusercontent.com/cjacker/opensource-toolchain-ch32v/refs/heads/main/assets/erase_by_poweroff.png" width="70%"/></p>

After clicked, power up the bricked CH32V as quick as possible, for example, plug the usb cable to PC host quickly. You may try several times to succeed and it will erase all code flash of bricked CH32V.
