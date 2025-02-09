SUMMARY = "rubikpi wifi config"
LICENSE = "CLOSED"

FILESPATH =+ "${WORKSPACE}/layers/meta-rubikpi/recipes-rubikpi-wifi/rubikpi-wifi:"
SRC_URI = "file://files/"
SRC_DIR = "${THISDIR}"

do_install() {
	install -d ${D}/etc
	install -d ${D}/etc/systemd/network
	cp -r ${WORKDIR}/files/00-wireless-dhcp.network ${D}/etc/systemd/network/00-wireless-dhcp.network
}

FILES:${PN} += "/etc/systemd/network/00-wireless-dhcp.network"
