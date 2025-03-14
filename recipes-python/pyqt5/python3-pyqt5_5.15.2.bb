require ${TOPDIR}/../layers/meta-qt5/recipes-python/pyqt5/python3-pyqt5.inc

SRC_URI[md5sum] = "b94576e9e013210dc5aba061913e4bd4"
SRC_URI[sha256sum] = "372b08dc9321d1201e4690182697c5e7ffb2e0770e6b4a45519025134b12e4fc"

PYQT_MODULES += "QtNetwork QtNetwork QtQml ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'QtQuick QtWidgets QtQuickWidgets', '', d)}"
