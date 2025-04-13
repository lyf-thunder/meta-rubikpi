SUMMARY = "rubikpi config"
DESCRIPTION = "rubik PI Device Configuration Tool"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRCPROJECT = "git://github.com/rubikpi-ai/tools.git;protocol=https"
SRCBRANCH  = "rubikpi_config"
SRCREV = "9a3b15b51de826dcd32d763fbf3f09de2eb3a409"

SRC_URI =  "${SRCPROJECT};branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

do_install() {
	install -d ${D}${bindir}

	install -m 0755 ${S}/rubikpi_config ${D}${bindir}/
}
