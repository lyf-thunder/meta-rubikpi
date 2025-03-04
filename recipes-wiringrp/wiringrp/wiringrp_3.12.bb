SUMMARY = "WiringPi for RUBIK Pi"
LICENSE = "CLOSED"

DEPENDS += "libxcrypt"

SRCPROJECT = "git://github.com/rubikpi-ai/WiringRP.git;protocol=https"
SRCBRANCH  = "main"
SRCREV = "4bfb0de9f6605978e55ee2e89374b2eb2a84358d"

SRC_URI =  "${SRCPROJECT};branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

do_configure() {
}

INSANE_SKIP:${PN} += "ldflags"

do_compile() {
	oe_runmake -C ${S}/wiringPi
	oe_runmake -C ${S}/devLib EXTRA_CFLAGS="-I${S}/wiringPi"
}

do_install() {
	install -d ${D}${libdir}
	install -d ${D}${includedir}

	install -m 0755 ${S}/wiringPi/libwiringPi.so.* ${D}${libdir}
	install -m 0644 ${S}/wiringPi/*.h ${D}${includedir}

	install -m 0755 ${S}/devLib/libwiringPiDev.so.* ${D}${libdir}
	install -m 0644 ${S}/devLib/*.h ${D}${includedir}

	cd ${D}${libdir}
	ln -sf libwiringPi.so.${PV} libwiringPi.so
	ln -sf libwiringPiDev.so.${PV} libwiringPiDev.so
}

FILES_${PN} += "${libdir}"
FILES_${PN} += "${includedir}"
