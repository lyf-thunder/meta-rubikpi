inherit systemd externalsrc

FILESEXTRAPATHS:prepend := "${WORKSPACE}/layers/meta-rubikpi/recipes-core/initscripts/files:"
SRC_URI:append = " \
	file://rubikpi_boot.sh \
	file://rubikpi-boot.service \
	file://hctosys.sh \
	file://hctosys.service \
	file://bt.sh \
	file://bt.service \
	file://wifi.sh \
	file://wifi.service \
"

S = "${WORKDIR}"

do_install:append:qcom() {
	install -d ${D}${systemd_unitdir}/system/
	install -d ${D}${systemd_unitdir}/system/multi-user.target.wants/
	install -d ${D}${sysconfdir}/initscripts/

	# rubikpi boot config
	install -m 0755 ${WORKDIR}/rubikpi_boot.sh ${D}${sysconfdir}/initscripts/rubikpi_boot.sh
	install -m 0644 ${WORKDIR}/rubikpi-boot.service -D ${D}${systemd_unitdir}/system/rubikpi-boot.service
	ln -sf ${systemd_unitdir}/system/rubikpi-boot.service ${D}${systemd_unitdir}/system/multi-user.target.wants/rubikpi-boot.service

	# rtc
	install -m 0755 ${WORKDIR}/hctosys.sh ${D}${sysconfdir}/initscripts/hctosys.sh
	install -m 0644 ${WORKDIR}/hctosys.service -D ${D}${systemd_unitdir}/system/hctosys.service
	ln -sf ${systemd_unitdir}/system/hctosys.service ${D}${systemd_unitdir}/system/multi-user.target.wants/hctosys.service

	# bt
	install -m 0755 ${WORKDIR}/bt.sh ${D}${sysconfdir}/initscripts/bt.sh
	install -m 0644 ${WORKDIR}/bt.service -D ${D}${systemd_unitdir}/system/bt.service
	# ln -sf ${systemd_unitdir}/system/bt.service ${D}${systemd_unitdir}/system/multi-user.target.wants/bt.service

	# wifi
	install -m 0755 ${WORKDIR}/wifi.sh ${D}${sysconfdir}/initscripts/wifi.sh
	install -m 0644 ${WORKDIR}/wifi.service -D ${D}${systemd_unitdir}/system/wifi.service
	ln -sf ${systemd_unitdir}/system/wifi.service ${D}${systemd_unitdir}/system/multi-user.target.wants/wifi.service
}

# rubikpi boot config
INITSCRIPT_PACKAGES =+ "${PN}-rubikpi-boot"
INITSCRIPT_NAME:${PN}-rubikpi-boot = "rubikpi_boot.sh"

PACKAGES =+ "${PN}-rubikpi-boot"
FILES:${PN}-rubikpi-boot += "${systemd_unitdir}/system/rubikpi-boot.service ${systemd_unitdir}/system/multi-user.target.wants/rubikpi-boot.service ${sysconfdir}/initscripts/rubikpi_boot.sh"

# rtc
INITSCRIPT_PACKAGES =+ "${PN}-rubikpi-rtc"
INITSCRIPT_NAME:${PN}-rubikpi-rtc = "hctosys.sh"
INITSCRIPT_PARAMS:${PN}-rubikpi-rtc = "start 99 2 3 4 5 ."
INITSCRIPT_PARAMS:${PN}-rubikpi-rtc += "stop 1 0 1 6 ."

PACKAGES =+ "${PN}-rubikpi-rtc"
FILES:${PN}-rubikpi-rtc += "${systemd_unitdir}/system/hctosys.service ${systemd_unitdir}/system/multi-user.target.wants/hctosys.service ${sysconfdir}/initscripts/hctosys.sh"

# bt
INITSCRIPT_PACKAGES =+ "${PN}-rubikpi-bt"
INITSCRIPT_NAME:${PN}-rubikpi-bt = "bt.sh"

PACKAGES =+ "${PN}-rubikpi-bt"
FILES:${PN}-rubikpi-bt += "${systemd_unitdir}/system/bt.service ${systemd_unitdir}/system/multi-user.target.wants/bt.service ${sysconfdir}/initscripts/bt.sh"

# wifi
INITSCRIPT_PACKAGES =+ "${PN}-rubikpi-wifi"
INITSCRIPT_NAME:${PN}-rubikpi-wifi = "wifi.sh"

PACKAGES =+ "${PN}-rubikpi-wifi"
FILES:${PN}-rubikpi-wifi += "${systemd_unitdir}/system/wifi.service ${systemd_unitdir}/system/multi-user.target.wants/wifi.service ${sysconfdir}/initscripts/wifi.sh"

ALLOW_EMPTY:${PN}-functions = "1"
