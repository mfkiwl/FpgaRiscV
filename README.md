# Examples of bare metal RiscV programming with softcore on a fpga

## Cloning this repository ##

git clone --recurse-submodules https://github.com/infphyny/FpgaRiscV.git

## ** Installing the RiscV toolchain **
  * Clone https://github.com/riscv/riscv-gnu-toolchain.git
  * Then execute
  * ./configure --prefix=/opt/riscv --enable-multilib
  * sudo make -j N
  * Add /opt/riscv and quartus in $PATH

   * Install fusesoc

## ** Deca RiscV soc properties **
* VexRiscv cpu with Wishbone bus.
* Pinout in data/Deca/pinmap.tcl
* Frequency: 75 MHz
* Ram: 128k
* KEY0 is wired to reset.
* Uart RX need to be connected or pull high, otherwise Uart tx will be halted (SpinalHDL Uart implementation feature).
* To modify frequency, go in rtl/Deca/pll, execute quartus then choose pll for MAX10 and overwrite pll.v
* If frequency is changed, modify the fixed frequency in WbUart.scala and regenerate the WishboneUartCtrl.v to keep 115200 baud rate.        
* To modify the ram size: Modify RAM_SIZE in Makefiles. Adjust gp and sp in crt0.S. Modify mem size in Deca.core.  

## ** Trying an example **
* in sw/Deca/"example folder"/

* To compile software:  make
* To generate bitstream and upload:  make hw  Take ~5min
* To upload the previously generated bitstream: make upload
* To do a simulation: make sim  then ctrl-c after few seconds :  .vcd file grow fast  ~1 GiB/5s

## ** TODO **
* Quartus: Execute Synthesis and PnR once. Then update BRAM with a .mif from binary generated with the riscv toolchain.   
* Add documentation on how to generate the VexRiscv.v file from Vexriscv repository.   
