#!/bin/sh

CONFIG_FILE="/etc/rubikpi_config/rubikpi_config.ini"
# usb config
echo 500 > /sys/bus/platform/devices/a600000.usb/power/autosuspend_delay_ms
echo 500 > /sys/bus/platform/devices/8c00000.usb/power/autosuspend_delay_ms
echo auto > /sys/bus/platform/devices/a600000.usb/power/control
echo auto > /sys/bus/platform/devices/8c00000.usb/power/control

# remount
echo "[RubikPi Boot Service]: remount /usr" > /dev/kmsg
mount -o remount,rw /
mount -o remount,rw /usr

# rubikpi_config
ln -sf /var/rubikpi_dtso/rubikpi.dtso /etc/rubikpi_config/rubikpi.dtso
ln -sf /var/rubikpi_config/rubikpi_config.ini /etc/rubikpi_config/rubikpi_config.ini

# heartb led
heartbeat_color=$(grep "heartbeat_color=" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d '"')
heartbeat_brightness=$(grep "heartbeat_brightness=" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d '"')
heartbeat_switch=$(grep "heartbeat_switch=" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d '"')
if [ ${heartbeat_switch} = "on" ]; then
	echo heartbeat > /sys/class/leds/${heartbeat_color}/trigger
	echo ${heartbeat_brightness} > /sys/class/leds/green/brightness
fi

# bluetooth init
bt_protocol_stack=$(grep "bt_protocol_stack=" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d '"')
if [ ${bt_protocol_stack} = "bsa" ]; then
	systemctl start bt
else
	hciattach -n -p ttyHS7 bcm43xx 3000000 noflow 0x0000
fi



