SUMMARY = "WiringPi for RUBIK Pi"
LICENSE = "CLOSED"

DEPENDS += "libxcrypt"

SRCPROJECT = "git://github.com/rubikpi-ai/WiringRP.git;protocol=https"
SRCBRANCH  = "main"
SRCREV = "4bfb0de9f6605978e55ee2e89374b2eb2a84358d"

SRC_URI =  "${SRCPROJECT};branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

INSANE_SKIP:${PN} += "ldflags"

do_compile() {
	oe_runmake -C ${S}/wiringPi
	oe_runmake -C ${S}/devLib EXTRA_CFLAGS="-I${S}/wiringPi"
}

do_install() {
	install -d ${D}/usr/lib
	install -d ${D}/usr/include

	cp -r ${S}/wiringPi/libwiringPi.so.${PV} ${D}/usr/lib/libwiringPi.so
	cp -r ${S}/wiringPi/*.h ${D}/usr/include/

	cp -r ${S}/devLib/libwiringPiDev.so.${PV} ${D}/usr/lib/libwiringPiDev.so
	cp -r ${S}/devLib/*.h ${D}/usr/include/
}

FILES:${PN}-dev = ""

FILES:${PN} += " \
	/usr/lib/libwiringPi.so \
	/usr/lib/libwiringPiDev.so \
"
FILES:${PN} += "/usr/include/*.h"
