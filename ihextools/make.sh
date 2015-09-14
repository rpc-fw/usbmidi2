#!/bin/bash

echo Converting master binary

cp ../usbmidi2_master/Debug/usbmidi2_master.hex master.hex
BUILDID=$(md5sum master.hex|cut -c -8)
hextoc/hextoc master.hex -i 0x$BUILDID -P 2400:$(./sense_master_id.sh master.hex):$BUILDID > master.h
cp master.h ../usbmidi2_slave/Sources/masterbinary.h

echo Converting slave binary
cp ../usbmidi2_slave/Debug/usbmidi2_slave.hex slave.hex
hextosyx/hextosyx slave.hex

cp slave.hex.syx usbmidi2.syx



echo Converting 4x4 master binary

cp ../usbmidi2_4x4_master/Debug/usbmidi2_master.hex master4x4.hex
BUILDID=$(md5sum master4x4.hex|cut -c -8)
hextoc/hextoc master4x4.hex -i 0x$BUILDID -P 2400:$(./sense_master_id.sh master4x4.hex):$BUILDID > master4x4.h
cp master4x4.h ../usbmidi2_4x4_slave/Sources/masterbinary.h

echo Converting 4x4 slave binary
cp ../usbmidi2_4x4_slave/Debug/usbmidi2_slave.hex slave4x4.hex
hextosyx/hextosyx slave4x4.hex

cp slave4x4.hex.syx usbmidi2_4x4.syx
