# Opensource toolchain for WCH ch32v RISC-V 32bit MCU

CH32V Risc-V 32bit MCU series is a family of General-Purpose RISC-V MCU from WCH.

If you want to learn more about it, please refer to http://www.wch-ic.com/products/categories/47.html?pid=5.

These MCUs support a private debugging protocol named 'RVSWD' and requires a special (but not expensive) usb adapter named 'wchlink'. it implemented in WCH forked OpenOCD with 'wlink' interface. At first, the WCH forked OpenOCD is close source and only provide binaries compiled for Windows and Linux by MounRiver Studio (an IDE for ch32v developent based on eclipse).

Recently (2022-03), the private forked OpenOCD is opensourced by request from opensource developers (https://github.com/kprasadvnsi/riscv-openocd-wch).

# Hardware requirements
* A development board with CH32V mcu

It's easy to find agd32v103 or gd32v307 board.

* A 'wchlink' USB SWD/RVSWD adapter
 
Such adapters usually support dual mode (SWD for ARM and RVSWD for CH32V)and can be toggled by jumper or on-board keys.

# Toolchain overview
* Compiler: gcc
* Debugger: openocd/gdb
* SDK:
* Programmer: ch55xtool for ISP mode/openocd for RVSWD



