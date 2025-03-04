SUMMARY = "rubikpi config"
DESCRIPTION = "rubik PI Device Configuration Tool"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRCPROJECT = "git://github.com/rubikpi-ai/tools.git;protocol=https"
SRCBRANCH  = "rubikpi_config"
SRCREV = "0b73c4c25abe5ee13ea091fc9d7491f4ca6135ff"

SRC_URI =  "${SRCPROJECT};branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

do_install() {
	install -d ${D}${bindir}

	install -m 0755 ${S}/rubikpi_config ${D}${bindir}/
	cp -r ${S}/rubikpi_config.ini ${D}${bindir}/rubikpi_config.ini
	cp -r ${S}/rubikpi.dtso ${D}${bindir}/rubikpi.dtso
}

