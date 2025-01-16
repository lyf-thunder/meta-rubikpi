#!/bin/sh

ACTION="$1"
DEVICE="$2"

MOUNT_DIR="/mnt/$DEVICE"
BOARD_DIR="/dev/$DEVICE"

echo "ACTION: $ACTION"

case "$ACTION" in
	add)

		mkdir -p "$MOUNT_DIR" 
		echo " MOUNT_DIR: $MOUNT_DIR"
		echo " BOARD_DIR= $BOARD_DIR"
		mount -t auto "$BOARD_DIR" "$MOUNT_DIR"
		;;
	remove)
		echo " BOARD_DIR: $BOARD_DIR"
		umount "$BOARD_DIR"
		rm -rf "$MOUNT_DIR"
		;;
esac
