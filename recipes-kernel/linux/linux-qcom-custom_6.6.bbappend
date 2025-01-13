
LIC_FILES_CHKSUM = "file://${TOPDIR}/../src/vendor/qcom/opensource/kernel-6.6/COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

FILESPATH =+ "${TOPDIR}/../src/vendor/qcom/opensource:"
SRC_URI = "file://kernel-6.6;protocol=file;name=git \
           ${@bb.utils.contains('DISTRO_FEATURES', 'selinux', ' file://selinux.cfg', '', d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'selinux', ' file://selinux_debug.cfg', '', d)} \
           "
S = "${WORKDIR}/kernel-6.6"
