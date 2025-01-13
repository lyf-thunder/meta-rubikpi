SUMMARY = "Basic programs and scripts"
DESCRIPTION = "Package group to bring in all basic packages"

LICENSE = "BSD-3-Clause-Clear"

PACKAGE_ARCH = "${TUNE_PKGARCH}"

inherit packagegroup

PROVIDES = "${PACKAGES}"

PACKAGES = ' \
    ${PN} \
    '

RDEPENDS:${PN} = "\
    packagegroup-rubikpi-devicetree \
    "

