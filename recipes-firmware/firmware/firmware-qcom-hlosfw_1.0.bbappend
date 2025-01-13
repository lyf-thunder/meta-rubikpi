do_deploy:append() {
    install -d ${D}/usr/lib/firmware
    install -m 0644 ${TOPDIR}/../layers/meta-rubikpi/recipes-firmware/firmware/files/* ${D}/usr/lib/firmware/
}

FILES:${PN} += "/usr/lib/firmware"
FILES:${PN} += "/usr/lib/firmware/nvram.txt"
FILES:${PN} += "/usr/lib/firmware/lt9611uxc_fw.bin"
FILES:${PN} += "/usr/lib/firmware/aw882xx_acf.bin"
FILES:${PN} += "/usr/lib/firmware/sduart8987_combo.bin"
FILES:${PN} += "/usr/lib/firmware/config.txt"
FILES:${PN} += "/usr/lib/firmware/fw_bcm43456c5_ag.bin"
FILES:${PN} += "/usr/lib/firmware/renesas_usb_fw.mem"
