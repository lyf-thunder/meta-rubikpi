SUMMARY = "Copy BT related"
LICENSE = "CLOSED"

FILESPATH =+ "${THISDIR}:"
SRC_URI = "file://files/"

SRC_DIR = "${THISDIR}"
INHIBIT_PACKAGE_DEBUG_SPLIT = '1'
INHIBIT_PACKAGE_STRIP = '1'

# FILES:${PN}-dev = ""
FILES_SOLIBSDEV = ""

do_install() {
	install -d ${D}/usr/src/rubikpi-bt-demo
	install -d ${D}/usr/src/rubikpi-bt-demo/3rdparty
	install -d ${D}/usr/src/rubikpi-bt-demo/test_files/av
	install -d ${D}/usr/src/rubikpi-bt-demo/test_files/ag

	cp -r ${WORKDIR}/files/3rdparty ${D}/usr/src/rubikpi-bt-demo/3rdparty
	cp -r ${WORKDIR}/files/app_manager ${D}/usr/src/rubikpi-bt-demo/app_manager
	cp -r ${WORKDIR}/files/app_av ${D}/usr/src/rubikpi-bt-demo/app_av
	cp -r ${WORKDIR}/files/app_ag ${D}/usr/src/rubikpi-bt-demo/app_ag
	cp -r ${WORKDIR}/files/app_ble ${D}/usr/src/rubikpi-bt-demo/app_ble
	cp -r ${WORKDIR}/files/bsa_server ${D}/usr/src/rubikpi-bt-demo/bsa_server
	cp -r ${WORKDIR}/files/app_opc ${D}/usr/src/rubikpi-bt-demo/app_opc
	cp -r ${WORKDIR}/files/app_ops ${D}/usr/src/rubikpi-bt-demo/app_ops
	cp -r ${WORKDIR}/files/BCM4345C5_003.006.006.1081.1154.hcd ${D}/usr/src/rubikpi-bt-demo/BCM4345C5_003.006.006.1081.1154.hcd
	cp -r ${WORKDIR}/files/2c.wav ${D}/usr/src/rubikpi-bt-demo/test_files/av/2c.wav
	cp -r ${WORKDIR}/files/Remix_48K_2ch_16bit.wav ${D}/usr/src/rubikpi-bt-demo/test_files/av/Remix_48K_2ch_16bit.wav


	chmod 777 -R ${D}/usr/src/rubikpi-bt-demo
}

FILES:${PN} += "/usr/src/rubikpi-bt-demo"
FILES:${PN} += "/usr/src/rubikpi-bt-demo/3rdparty"
