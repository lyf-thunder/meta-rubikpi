DESCRIPTION = "USB Storage Mounting Scripts and Systemd Service."

HOMEPAGE         = "https://www.codeaurora.org/"
LICENSE          = "BSD-3-Clause-Clear"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/BSD-3-Clause-Clear;md5=7a434440b651f4a472ca93716d01033a"

SRC_URI = "file://usbstorage-mount.sh \
           file://usbstorage-mount@.service \
           file://10-usbstorage-mount.rules"

S = "${WORKDIR}"

do_install() {
    install -d ${D}/usr/bin
    install -m 0755 ${S}/usbstorage-mount.sh ${D}/usr/bin/usbstorage-mount.sh

    install -d ${D}/etc/systemd/system
    install -m 0644 ${S}/usbstorage-mount@.service ${D}/etc/systemd/system/usbstorage-mount@.service

    install -d ${D}/etc/udev/rules.d
    install -m 0644 ${S}/10-usbstorage-mount.rules ${D}/etc/udev/rules.d/10-usbstorage-mount.rules
}

RDEPENDS_${PN} += "systemd"
