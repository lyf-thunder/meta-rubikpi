SUMMARY = "RUBIKPi devicetree package groups"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

LICENSE = "BSD-3-Clause & \
           BSD-3-Clause-Clear \
           "

PROVIDES = "${PACKAGES}"

PACKAGES = "${PN}"

RDEPENDS:${PN} = " \
    rubikpi3-dtbo \
    "
