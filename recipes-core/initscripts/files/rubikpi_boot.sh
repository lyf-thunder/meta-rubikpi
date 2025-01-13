#!/bin/sh

# usb config
echo 500 > /sys/bus/platform/devices/a600000.usb/power/autosuspend_delay_ms
echo 500 > /sys/bus/platform/devices/8c00000.usb/power/autosuspend_delay_ms
echo auto > /sys/bus/platform/devices/a600000.usb/power/control
echo auto > /sys/bus/platform/devices/8c00000.usb/power/control

# heartbeat led
echo heartbeat > /sys/class/leds/green/trigger
echo 5 > /sys/class/leds/green/brightness
