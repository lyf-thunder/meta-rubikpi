DESCRIPTION = "rubikpi devicetree overlay"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/\
${LICENSE};md5=550794465ba0ec5312d6919e203a55f9"

inherit module deploy

FILESPATH =+ "${TOPDIR}/../src/vendor/qcom/opensource/kernel-6.6/arch/arm64/boot/dts:"

SRC_URI = "file://thundercomm/rubikpi3;protocol=file;name=rubikpi3"

S = "${WORKDIR}/thundercomm/rubikpi3"

DTC := "${KBUILD_OUTPUT}/scripts/dtc/dtc"
CAMERA_INCLUDE := "${STAGING_KERNEL_DIR}/techpack/camera-kernel/camera/"
KERNEL_INCLUDE := "${STAGING_KERNEL_DIR}/include/  -I ${CAMERA_INCLUDE}"
EXTRA_OEMAKE += "DTC='${DTC}' KERNEL_INCLUDE='${KERNEL_INCLUDE}'"

do_install() {
   :
}

do_compile() {
    oe_runmake ${EXTRA_OEMAKE} rubikpi3-overlay
}

do_deploy() {
    install -d ${DEPLOYDIR}/tech_dtbs
    install -m 0644 \
    ${S}/*.dtbo \
    ${DEPLOYDIR}/tech_dtbs/
}
addtask do_deploy after do_install
