SUMMARY = "rubikpi btapp"
DESCRIPTION = "rubikpi bluetooth control application"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

FILESPATH =+ "${THISDIR}:"
SRC_URI = "file://files "

S = "${WORKDIR}/files/src"

TARGET_CXX_ARCH += "${LDFLAGS}"



do_compile() {
	make CPU=arm64 
}

do_install() {
	install -d ${D}/usr/src/rubikpi-bt-demo
	install -d ${D}/usr/src/rubikpi-bt-demo/3rdparty
	install -d ${D}/usr/src/rubikpi-bt-demo/test_files/av
	install -d ${D}/usr/src/rubikpi-bt-demo/test_files/ag
	install -d ${D}/usr/src/rubikpi-btapp
	install -d ${D}/usr/src/rubikpi-btapp/src
	install -d ${D}/usr/src/rubikpi-btapp/test_files/av
	install -d ${D}/usr/src/rubikpi-btapp/test_files/ag

	cp -r ${WORKDIR}/files/3rdparty ${D}/usr/src/rubikpi-bt-demo/3rdparty
	cp -r ${WORKDIR}/files/app_manager ${D}/usr/src/rubikpi-btapp/app_manager
	cp -r ${WORKDIR}/files/app_av ${D}/usr/src/rubikpi-btapp/app_av
	cp -r ${WORKDIR}/files/app_ag ${D}/usr/src/rubikpi-btapp/app_ag
	cp -r ${WORKDIR}/files/app_ble ${D}/usr/src/rubikpi-btapp/app_ble
	cp -r ${WORKDIR}/files/app_opc ${D}/usr/src/rubikpi-btapp/app_opc
	cp -r ${WORKDIR}/files/app_ops ${D}/usr/src/rubikpi-btapp/app_ops
        
	cp -r ${WORKDIR}/files/src ${D}/usr/src/rubikpi-btapp/src
	cp -r ${WORKDIR}/files/rubikpi_btapp ${D}/usr/src/rubikpi-btapp/rubikpi_btapp
	cp -r ${WORKDIR}/files/rubikpi_btapp.conf ${D}/usr/src/rubikpi-btapp/rubikpi_btapp.conf
	cp -r ${WORKDIR}/files/bsa_server ${D}/usr/src/rubikpi-btapp/bsa_server
	cp -r ${WORKDIR}/files/BCM4345C5_003.006.006.1081.1154.hcd ${D}/usr/src/rubikpi-btapp/BCM4345C5_003.006.006.1081.1154.hcd
	cp -r ${WORKDIR}/files/2c.wav ${D}/usr/src/rubikpi-btapp/test_files/av/2c.wav
	cp -r ${WORKDIR}/files/Remix_48K_2ch_16bit.wav ${D}/usr/src/rubikpi-btapp/test_files/av/Remix_48K_2ch_16bit.wav

	chmod 777 -R ${D}/usr/src/rubikpi-bt-demo
	chmod 777 -R ${D}/usr/src/rubikpi-btapp
}

FILES:${PN} += "/usr/src/rubikpi-bt-demo"
FILES:${PN} += "/usr/src/rubikpi-bt-demo/3rdparty"
FILES:${PN} += "/usr/src/rubikpi-btapp"
FILES:${PN} += "/usr/src/rubikpi-btapp/src"


deltask do_package_qa

