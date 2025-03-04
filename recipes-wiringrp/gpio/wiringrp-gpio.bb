SUMMARY = "WiringPi gpio tool for RUBIK Pi"
LICENSE = "CLOSED"

DEPENDS += "libxcrypt wiringrp"

SRCPROJECT = "git://github.com/rubikpi-ai/WiringRP.git;protocol=https"
SRCBRANCH  = "main"
SRCREV = "4bfb0de9f6605978e55ee2e89374b2eb2a84358d"

SRC_URI =  "${SRCPROJECT};branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

do_configure() {
}

INSANE_SKIP:${PN} += "ldflags"

do_compile() {
	oe_runmake -C ${S}/gpio EXTRA_CFLAGS="-I${S}/wiringPi -I${S}/devLib"
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/gpio/gpio ${D}${bindir}
}

FILES_${PN} += "${bindir}"
