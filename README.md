# AR488-XFR-GUI
Software control of the laboratory power supplies Ametek / Sorensen / Xantrex XFR 6-200, XFR 7.5-140, XFR 12-100, XFR 20-60, XFR 35-35, XFR 40-30, XFR 60-20, XFR 100-12, XFR 150-8, XFR 300-4, XFR 600-2 via AR488 GPIB controller (Arduino).

## Required
AR488 Arduino GPIB Interface is an Arduino-based controller for interfacing with IEEE488 GPIB devices via USB.
To use this software, the simple adapter is required. So only the wiring without IC.

Building instructions and firmware are available here and are also very well documented:

[link AR488](https://github.com/Twilight-Logic/AR488)

## Note
I currently only have one device to test, which is a Xantrex XFR 60-20.
But since the instructions are for all these devices, it must also work there. I would be very pleased to receive feedback.

## Required libs

**For Serial:**
`apt install libqt5serialport5-dev`

**For Linechart:**
`apt install libqt5charts5 libqt5charts5-dev qml-module-qtcharts`

## Alternative
There is a ready-made APPIMAGE that works like this without linux. See last release.

## Control
![Main](https://github.com/paranoid64/AR488-XFR-GUI/tree/main/FRX1.png "IMAGE 1")

## Terminal
![Terminal](https://github.com/paranoid64/AR488-XFR-GUI/tree/main/FRX2.png "IMAGE 2")

If the error LED lights up, you can query the error here by clicking on ERROR.

## Device Info
![Device Info](https://github.com/paranoid64/AR488-XFR-GUI/tree/main/FRX3.png "IMAGE 3")

Here is the answer from the ROM (M = Master, S = Slave ROM)

The maximum volts and amperes are determined via ID Command
