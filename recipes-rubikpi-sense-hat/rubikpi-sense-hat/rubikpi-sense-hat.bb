SUMMARY = "Copy sense hat related"
LICENSE = "CLOSED"

FILESPATH =+ "${THISDIR}:"
SRC_URI = "file://files/"

SRC_DIR = "${THISDIR}"
INHIBIT_PACKAGE_DEBUG_SPLIT = '1'
INHIBIT_PACKAGE_STRIP = '1'

# FILES:${PN}-dev = ""
FILES_SOLIBSDEV = ""

do_install() {
	install -d ${D}/usr/src/rubikpi-sense-hat-demo
	install -d ${D}/usr/src/rubikpi-sense-hat-demo/snake
	install -d ${D}/usr/src/rubikpi-sense-hat-demo/compass

	cp -r ${WORKDIR}/files/compass.c ${D}/usr/src/rubikpi-sense-hat-demo/compass/compass.c
	cp -r ${WORKDIR}/files/snake.c ${D}/usr/src/rubikpi-sense-hat-demo/snake/snake.c
}

FILES:${PN} += "/usr/src/rubikpi-sense-hat-demo"
FILES:${PN} += "/usr/src/rubikpi-sense-hat-demo/snake"
FILES:${PN} += "/usr/src/rubikpi-sense-hat-demo/compass"
