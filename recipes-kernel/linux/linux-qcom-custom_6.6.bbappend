
LIC_FILES_CHKSUM = "file://${TOPDIR}/../src/vendor/qcom/opensource/kernel-6.6/COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

FILESPATH =+ "${TOPDIR}/../src/vendor/qcom/opensource:"
SRC_URI = "file://kernel-6.6;protocol=file;name=git \
           ${@bb.utils.contains('DISTRO_FEATURES', 'selinux', ' file://selinux.cfg', '', d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'selinux', ' file://selinux_debug.cfg', '', d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'smack', ' file://smack.cfg', '', d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'smack', ' file://smack_debug.cfg', '', d)} \
           file://0001-QCLINUX-Add-support-to-compile-msm_display.ko.patch \
           file://0002-QCLINUX-lt9611uxc-changes-to-include-htotal-and-vtot.patch \
           "
S = "${WORKDIR}/kernel-6.6"

do_configure:append() {
    kernel_conf_variable LOCALVERSION ""
    kernel_conf_variable LOCALVERSION_AUTO n
    oe_runmake -C ${S} O=${B} savedefconfig && cp ${B}/defconfig ${WORKDIR}/defconfig.saved
}

KERNEL_CONFIG_FRAGMENTS:append = " ${S}/arch/arm64/configs/rubikpi3.config"
