inherit cmake

SUMMARY = "NETWORK"
HOMEPAGE = "https://source.codeaurora.org"
SECTION = "multimedia"

LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/${LICENSE};md5=550794465ba0ec5312d6919e203a55f9"

# Dependencies.

DEPENDS += ""

FILESPATH =+ "${TOPDIR}/../layers/meta-rubikpi/recipes-multimedia/audio:"
SRC_URI = " \
	file://notify \
	file://notify/notify.service \
	file://notify/notify.sh \
	"

S = "${WORKDIR}/notify"

FILES:${PN} += "${INSTALL_BINDIR}"
FILES:${PN} += "${INSTALL_LIBDIR}"
FILES:${PN} += "${systemd_unitdir}"

SYSTEMD_SERVICE_${PN} = "notify.service"

do_install:append () {
    install -d ${D}${systemd_system_unitdir}
    install -d ${D}${sysconfdir}/initscripts/

    install -m 0644 ${WORKDIR}/notify/notify.service ${D}${systemd_system_unitdir}
    install -m 0755 ${WORKDIR}/notify/notify.sh ${D}${sysconfdir}/initscripts/notify.sh
    install -d ${D}${systemd_system_unitdir}/multi-user.target.wants/
    ln -sf ${systemd_system_unitdir}/notify.service \
           ${D}${systemd_system_unitdir}/multi-user.target.wants/notify.service
}

SOLIBS = ".so"
FILES_SOLIBSDEV = ""
