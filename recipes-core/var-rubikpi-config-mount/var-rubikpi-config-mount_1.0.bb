SUMMARY = "Mount files related to configuration tools"
LICENSE = "CLOSED"

FILESPATH =+ "${WORKSPACE}/layers/meta-rubikpi/recipes-core/var-rubikpi-config-mount:"
SRC_URI = "file://files "

S = "${WORKDIR}/files"

do_install() {
	install -d ${D}/etc/rubikpi_config
	install -d ${D}/var/devcfg
	install -d ${D}/var/rubikpi_config
	install -d ${D}/var/rubikpi_dtso

	install -d ${D}${systemd_unitdir}/system/local-fs.target.wants
	install -m 0644  ${WORKDIR}/files/var-devcfg.mount ${D}${systemd_unitdir}/system/var-devcfg.mount
	ln -sf ${systemd_unitdir}/system/var-devcfg.mount ${D}${systemd_unitdir}/system/local-fs.target.wants/var-devcfg.mount

	install -d ${D}${systemd_unitdir}/system/local-fs.target.wants
	install -m 0644  ${WORKDIR}/files/var-rubikpi_config.mount ${D}${systemd_unitdir}/system/var-rubikpi_config.mount
	ln -sf ${systemd_unitdir}/system/var-rubikpi_config.mount ${D}${systemd_unitdir}/system/local-fs.target.wants/var-rubikpi_config.mount

	install -d ${D}${systemd_unitdir}/system/local-fs.target.wants
	install -m 0644  ${WORKDIR}/files/var-rubikpi_dtso.mount ${D}${systemd_unitdir}/system/var-rubikpi_dtso.mount
	ln -sf ${systemd_unitdir}/system/var-rubikpi_dtso.mount ${D}${systemd_unitdir}/system/local-fs.target.wants/var-rubikpi_dtso.mount
}

FILES:${PN} += " ${systemd_unitdir}/*"
FILES:${PN} += "/etc/rubikpi_config"
FILES:${PN} += "/var/devcfg"
FILES:${PN} += "/var/rubikpi_config"
FILES:${PN} += "/var/rubikpi_dtso"
