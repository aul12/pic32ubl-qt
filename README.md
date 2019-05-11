# pic32ubl-qt
Port of the PIC32-Bootloader-Application for Linux and MacOs.

This is fork of pic32ubl-qt ([https://code.google.com/archive/p/pic32ubl-qt/](https://code.google.com/archive/p/pic32ubl-qt/)), which is not under development anymore.
This version incorporates small fixes for QT5 and better documentation.

## How to install the program.

### Prerequisites
 * QT [https://www.qt.io/developers/]
 * LibSerial
 * LibUsb
 * HID API

### Building the application
First generate the Makefile using qmake:
```
qmake
```
next compile the program using make:
```
make
```
