OpenMAOR
========
Open source firmware for programming MAOR-12T robot in C++

Main goals of the project:
* Allow programming the robot in C++
* Make the program persist after turning the robot off and on
* Fix some issues related to program size (the original firmware crashed in weird way with program size >1023 bytes)

Quick start
-----------
Install dependencies:
```sh
sudo apt-get install git cmake gcc-avr binutils-avr avr-libc avrdude
```

Connect the robot using an AVR programmer (you may have to adjust program.sh if you use something else than usbasp), then run:
```sh
git clone https://github.com/krzys-h/OpenMAOR
cd OpenMAOR
mkdir build
cd build
cmake ..
make
../program.sh -b OpenMAOR-bootloader.hex
```
This will install the OpenMAOR bootloader that allows uploading programs via UART and remote controlling the robot while the program is not active.

If you ever want to go back to original firmware, run:
```sh
wget http://www.mobot.pl/download/MAOR-12v2_12T_v17.zip
unzip MAOR-12v2_12T_v17.zip
../program.sh MAOR-12T_v17.hex
```

How to create project using OpenMAOR
------------------------------------
See [CMakeLists.txt file in sample directory](sample/CMakeLists.txt)

Uploading programs to the robot
-------------------------------
After installing the bootloader, you can upload programs to the robot using any AVR programming software that supports AVR109 protocol. For example, using avrdude (assuming that UART is connected to COM5):
```sh
avrdude -c avr109 -P COM5 -B 57600 -p m32 -U flash:w:filename.hex:i
```
The program starts when you press the button on the robot, just like when programming the robot with the default SPAR-TA firmware.

API
---
Run ```make doc``` to generate API documentation, or just browse the code in [src/common/](src/common) directory