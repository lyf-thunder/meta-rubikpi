SUMMARY = "First login password change script"
LICENSE          = "BSD-3-Clause-Clear"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/BSD-3-Clause-Clear;md5=7a434440b651f4a472ca93716d01033a"
SRC_URI = " \
    file://first-login.sh \
    file://first-login.service \
"
SYSTEMD_SERVICE:${PN} = "first-login.service"

do_install() {
    install -d ${D}${sbindir}
    install -m 0755 ${WORKDIR}/first-login.sh ${D}${sbindir}/first-login

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/first-login.service ${D}${systemd_system_unitdir}/
}

FILES:${PN} = " \
    ${sbindir}/first-login \
    ${systemd_system_unitdir}/first-login.service \
"