SRC_URI:remove:qcm6490:qcom-custom-bsp = " \
              file://0001-weston-add-sdm-option.patch \
              file://0001-drm-backend-power-off-during-hotplug-disconnect.patch \
              "

SRC_URI:append:qcm6490:qcom-custom-bsp = "  \
              file://weston.ini \
              file://0001-Add-sdm-backend.patch \
              file://0001-weston-export-shared-headers.patch \
              file://0001-weston-add-protocol-extension-for-power-and-brightne.patch \
              file://0001-weston-add-surface-position-and-power-key.patch \
              file://0001-weston-add-support-color-calibration.patch \
              "

DEPENDS:append:qcm6490 = " qcom-display-hal-linux virtual/libgbm seatd"

PACKAGECONFIG:append:qcm6490 = " sdm disablepowerkey"

# Enable when sdm downstream is needed
LDFLAGS:append:qcm6490  = " -ldrmutils -ldisplaydebug -lglib-2.0 -ldmabufheap"

#meson script's CPP flags
CXXFLAGS:append:qcm6490  = " -I${STAGING_INCDIR}/sdm"
CXXFLAGS:append:qcm6490  = " -I${STAGING_INCDIR}/display/display"

do_install:append:qcm6490() {
    install -m 0644 ${WORKDIR}/weston.ini -D ${D}${sysconfdir}/xdg/weston/weston.ini
}

PACKAGECONFIG:remove:qcm6490 = "kms"

FILES:${PN} += "${bindir}/*"
FILES:${PN} += " ${libdir}/libweston-13/*.so"
FILES:${PN} += " ${libdir}/*.so"
FILES:${PN} += "${sysconfdir}/xdg/weston/weston.ini"
