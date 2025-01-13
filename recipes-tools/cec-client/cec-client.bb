SUMMARY = "cec-client tool"
LICENSE = "CLOSED"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${THISDIR}/files/cec-client ${D}${bindir}
}

deltask do_package_qa
