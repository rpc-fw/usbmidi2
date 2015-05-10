#!/bin/bash

echo Converting master binary

cp ../usbmidi2_master/Debug/kl26test.hex master.hex
BUILDID=$(md5sum master.hex|cut -c -8)
hextoc/hextoc master.hex -i 0x$BUILDID -P 4400:$(./sense_master_id.sh master.hex):$BUILDID > master.h
cp master.h ../usbmidi2_slave/Sources/masterbinary.h

echo Converting slave binary
cp ../usbmidi2_slave/Debug/usbmidi2_slave.hex slave.hex
hextosyx/hextosyx slave.hex

cp slave.hex.syx usbmidi2.syx

