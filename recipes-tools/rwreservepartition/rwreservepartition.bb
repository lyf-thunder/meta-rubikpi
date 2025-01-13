SUMMARY = "Rw_reservepartition tool"
LICENSE = "CLOSED"

FILESPATH =+ "${THISDIR}:"
SRC_URI = "file://files "

S = "${WORKDIR}/files"

TARGET_CXX_ARCH += "${LDFLAGS}"

do_compile() {
    make
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 rw_reservepartition ${D}${bindir}
}
