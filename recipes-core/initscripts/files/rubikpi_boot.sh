#!/bin/sh

# usb config
echo 500 > /sys/bus/platform/devices/a600000.usb/power/autosuspend_delay_ms
echo 500 > /sys/bus/platform/devices/8c00000.usb/power/autosuspend_delay_ms
echo auto > /sys/bus/platform/devices/a600000.usb/power/control
echo auto > /sys/bus/platform/devices/8c00000.usb/power/control

# remount
echo "[RubikPi Boot Service]: remount /usr" > /dev/kmsg
mount -o remount,rw /
mount -o remount,rw /usr

#bluetooth init
hciattach -n -p ttyHS7 bcm43xx 3000000 noflow 0x0000



