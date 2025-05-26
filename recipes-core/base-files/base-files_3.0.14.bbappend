hostname = "rubikpi"

do_install_basefilesissue:append() {
	echo "Welcome to RUBIK Pi!" >> ${D}${sysconfdir}/issue
	echo "This pre-installed version is a Beta Version designed for your quick out-of-the-box experience." >> ${D}${sysconfdir}/issue
	echo "If you encounter any issues, please visit https://github.com/rubikpi-ai to download the latest software version." >> ${D}${sysconfdir}/issue
	echo >> ${D}${sysconfdir}/issue

	echo "Welcome to RUBIK Pi!" >> ${D}${sysconfdir}/issue.net
	echo "This pre-installed version is a Beta Version designed for your quick out-of-the-box experience." >> ${D}${sysconfdir}/issue.net
	echo "If you encounter any issues, please visit https://github.com/rubikpi-ai to download the latest software version." >> ${D}${sysconfdir}/issue.net
	echo >> ${D}${sysconfdir}/issue.net
}

do_install:append() {
    echo "[ -f /usr/sbin/first-login ] && /usr/sbin/first-login" >> ${D}${sysconfdir}/profile
}