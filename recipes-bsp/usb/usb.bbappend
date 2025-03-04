FILESEXTRAPATHS:prepend := "${WORKSPACE}/layers/meta-rubikpi/recipes-bsp/usb/files:"

do_install:append() {
	rm -f ${D}${systemd_unitdir}/system/var-usbfw.mount
	rm -f ${D}${systemd_unitdir}/system/local-fs.target.wants/var-usbfw.mount
	rm -f ${D}${nonarch_base_libdir}/firmware/renesas_usb_fw.mem
}

FILES:${PN} += "${nonarch_base_libdir}/firmware/"
