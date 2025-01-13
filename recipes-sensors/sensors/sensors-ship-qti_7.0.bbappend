inherit deploy

do_deploy() {
    cp -r ${TOPDIR}/../layers/meta-rubikpi/recipes-sensors/sensors/files/* ${D}/etc/sensors/config/
}

FILES:${PN} += "/etc/sensors/config/json.lst"
FILES:${PN} += "/etc/sensors/config/qcm6490_ak991x_0.json"
FILES:${PN} += "/etc/sensors/config/qcm6490_icm4x6xa_0.json"

addtask deploy before do_build after do_install
