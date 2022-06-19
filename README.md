# Opensource toolchain for WCH ch32v RISC-V 32bit MCU

**NOTE: the MIT license of this repo means all individual resources made by myself, the content of the tutorial and the example codes is licensed under MIT. All third-party opensource projects, upstream source codes and patches to other opensource projects will/should follow their own LICENSE.**

CH32V Risc-V 32bit MCU series is a family of General-Purpose RISC-V MCU from WCH.

If you want to learn more about it, please refer to http://www.wch-ic.com/products/categories/47.html?pid=5.

These MCUs support a private debugging protocol named 'RVSWD' and requires a special (but not expensive) usb adapter named 'wchlink'. it implemented in WCH forked OpenOCD with 'wlink' interface. At first, the WCH forked OpenOCD is close source and only provide binaries compiled for Windows and Linux by MounRiver Studio (an IDE for ch32v developent based on eclipse).

Recently (2022-03), the private forked OpenOCD (ver 0.11.0-dev) is opensourced by request from opensource developers (https://github.com/kprasadvnsi/riscv-openocd-wch).

# Hardware requirements
* A development board with CH32V mcu

It's easy to find a ch32v103 or ch32v307 board.

* A 'wchlink' USB SWD/RVSWD adapter
 
Such adapters usually support dual mode (SWD for ARM and RVSWD for CH32V)and can be toggled by jumper or on-board keys.

# Toolchain overview
* Compiler: gcc
* Debugger: openocd/gdb
* SDK: official EVT source package
* Programmer: ch55xtool for ISP mode/openocd for RVSWD

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

MounRiver provide a prebuilt toolchain for linux, you can download it from [MounRiver Studio](http://file.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.40.tar.xz). up to this tutorial written, the lastest version is "MRS_Toolchain_Linux_x64_V1.40.tar.xz", it include a prebuilt gnu toolchain and prebuilt openocd with wlink support.

By the way, it's a little bit unprofessional since the dir name in this tarball for linux contains 'space'...

After download finished, extract it to somewhere, and change the unprofessinal dir name and modify the PATH env, for example:

```
sudo mkdir -p /opt/mrs-riscv-toolchain
sudo tar xf MRS_Toolchain_Linux_x64_V1.40.tar.xz -C /opt/mrs-riscv-toolchain --strip-components=1 
# correct the dir name
sudo mv /opt/mrs-riscv-toolchain/"RISC-V Embedded GCC" /opt/mrs-riscv-toolchain/risv-none-embed-gcc
```

And add `/opt/mrs-riscv-toolchain/riscv-none-embed-gcc/bin` to PATH env according to your shell.

You also need add `/opt/mrs-riscv-toolchain/OpenOCD/bin` to PATH env if you want to use this prebuild OpenOCD.


**NOTE**, the target triplet of mrs riscv toolchain is **`riscv-none-embed`**.

*   Xpack riscv toolchain

[xpack-dev-tools](https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack) provde a prebuilt toolchain for riscv. you can download it from https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack/releases/tag/v10.2.0-1.2. up to this tutorial written, the lastest version is '10.2.0', after download finished, extract it and add path to PATH env, for example:

```
sudo mkdir -p /opt/xpack-riscv-toolchain
sudo tar xf xpack-riscv-none-embed-gcc-10.2.0-1.2-linux-x64.tar.gz -C /opt/xpack-riscv-toolchain --strip-components=1
```

and add `/opt/xpack-riscv-toolchain/bin` to PATH env according to your shell.

**NOTE**, the target triplet of xpack riscv toolchain is **`riscv-none-embed`**.

# SDK
If you used to do stm32 programming, you will feel very familiar with CH32V SDK.

WCH provide evaluate source code package for every CH32V MCU model and licensed under Apache-2.0, you can download them from WCH official website according to your MCU model.

For ch32v103, you should download it from http://www.wch.cn/downloads/CH32V103EVT_ZIP.html.

For ch32v307, the link is http://www.wch.cn/downloads/CH32V307EVT_ZIP.html

These evt source code packages contains core SDK and a lot of demo routines but lack Makefile support, in this repo, I provide a simple script and a Makefile template to help you convert it. after 'CH32VxxxEVT.ZIP' downloaded, the conversion process as below:

```
git clone https://github.com/cjacker/opensource-toolchain-ch32v.git
mkdir evt
unzip CH32VxxxEVT.ZIP -d evt
# copy core sdk to myproject dir
cp -r ../evt/EVT/EXAM/SRC/* myproject
# take some codes from TOUCHKEY demo.
cp -r evt/EVT/EXAM/TOUCHKEY/User myproject

cd myproject
# replace it with our blink demo
rm -f User/main.c
mv main.c User/
# clean up
rm -rf evt
./generate_makefile
make
```

After building complete, you will get 'build/CH32V.elf', 'build/CH32V.hex' and 'build/CH32V.bin', which can be used for debugging and programming later.


# Flashing and Debugging
There is 2 way to programming a ch32v MCU, ISP and RVSWD.

## ISP programming
A forked version of [ch55xtool](https://github.com/karlp/ch552tool) can support program WCH CH55x 8051 MCUs and WCH CH32V103/307 Risc-V MCU.

**Installation:**

```
git clone https://github.com/karlp/ch552tool.git
cd ch552tool
git checkout for-upstream-2
sudo install -m0755 ch55xtool/ch55xtool.py /usr/bin/ch55xtool
```

**Programming:**

You need enter ISP mode first. 

- connect your development board directly to Linux PC USB port.
- find the 'boot0' and 'reset' key on your development board
- **hold the BOOT0 key down, press RESET key and release it, after a while (about 1 second), release BOOT0 key**
- run `lsusb`, you will find something like '4348:55e0 WinChipHead'.

After enter ISP mode, take above blink example as demo (and wire up a LED to A8 pin), the programming process as below:

```
sudo ch55xtool -f build/CH32V.bin
```

you may need to press 'reset' key again after programming.

## OpenOCD programming and debugging

CH32V did NOT support JTAG/SWD programming and debugging interface, it had implemented a private protocol named 'RVSWD'. that's to say, you can not use  your SWD/JTAG usb adapters to program/debug CH32V. and it also can not supported by official OpenOCD (up to now, the changes WCH made to OpenOCD is not upstreamed).

You have to prepare a 'wchlink' usb adapter with WCH RiscV mcu support and build a forkd version OpenOCD with 'wlink' interface supported.

**Build and Install WCH OpenOCD:**

If you choose to use MRS prebuilt toolchain and WCH OpenOCD for Linux (as mention in Compiler section), you can ignore the building process.

```
git clone https://github.com/kprasadvnsi/riscv-openocd-wch.git
cd riscv-openocd-wch
./configure --prefix=/opt/wch-openocd --program-prefix=wch- --enable-wlink
make
sudo make install
```

After installation finished, add '/opt/wch-openocd/bin' to PATH env.

**Programming:**

Please wire up you 'wchlink' usb adapter with development board (pins as same as SWD) first and use 'wch-riscv.cfg' (for WCH OpenOCD 0.11-dev) provide in this repo.

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

