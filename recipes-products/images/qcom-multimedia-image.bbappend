inherit populate_sdk_qt5

CORE_IMAGE_BASE_INSTALL += " \
	packagegroup-rubikpi \
	rubikpi-bt-staticdev \
	rubikpi-sense-hat \
	rwreservepartition \
	ax88179bprogrammer \
	packagegroup-qt5-toolchain-target \
"
IMAGE_INSTALL:append = " i2c-tools"
IMAGE_INSTALL:append = " minicom"
IMAGE_INSTALL:append = " make cmake"
IMAGE_INSTALL:append = " iperf3 iperf2"
IMAGE_INSTALL:append = " tcpdump lmbench wget lighttpd"
IMAGE_INSTALL:append = " adduser iproute2 python3-pip sudo"
IMAGE_INSTALL:append = " rwreservepartition"
IMAGE_INSTALL:append = " ax88179bprogrammer"
IMAGE_INSTALL:append = " libcec"
IMAGE_INSTALL:append = " cec-client"
IMAGE_INSTALL:append = " qcom-adreno"
IMAGE_INSTALL:append = " sip3 python3-pyqt5 python3-pyqtchart python3-pytest-qt"

CORE_IMAGE_EXTRA_INSTALL += " ros-core turtlesim  "

EXTRA_USERS_PARAMS = "\
    useradd -r -s /bin/false system; \
    usermod -p '\$6\$FIumPDif04\$xNtcC1aRH.k0FnCrzUH807bD6uND43RMUWPzIDnXgp0JDrC86mCVFfp1o7jH/6qCRXGPpStTcZUo4zkJkcSE31' root; \
    "

EXTRA_IMAGE_FEATURES += "tools-sdk"
