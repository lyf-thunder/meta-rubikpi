SUMMARY = "config tool mount devcfg"
LICENSE = "CLOSED"

FILESPATH =+ "${WORKSPACE}/layers/meta-rubikpi/recipes-core/var-devcfg-mount:"
SRC_URI = "file://files "

S = "${WORKDIR}/files"

do_install() {
    install -d ${D}/var/devcfg

    install -d ${D}${systemd_unitdir}/system/local-fs.target.wants
    install -m 0644  ${WORKDIR}/files/var-devcfg.mount ${D}${systemd_unitdir}/system/var-devcfg.mount

    ln -sf ${systemd_unitdir}/system/var-devcfg.mount ${D}${systemd_unitdir}/system/local-fs.target.wants/var-devcfg.mount
}

FILES:${PN} += " ${systemd_unitdir}/*"
FILES:${PN} += "/var/devcfg"