inherit deploy

do_deploy() {
    cp -r ${TOPDIR}/../layers/meta-rubikpi/recipes-multimedia/audio/acdbdata/files/* ${D}${sysconfdir}/acdbdata/qcm6490_idp/
}

FILES:${PN} += "/etc/acdbdata/qcm6490_idp/acdb_cal.acdb"
FILES:${PN} += "/etc/acdbdata/qcm6490_idp/workspaceFileXml.qwsp"

addtask deploy before do_build after do_install
