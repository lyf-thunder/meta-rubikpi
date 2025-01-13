FILESPATH =+ "${TOPDIR}/../layers/meta-rubikpi/recipes-qt/qt5/qtbase:"

SRC_URI += "\
	file://0027-Resolve-compilation-errors.patch \
"

# Disable desktop OpenGL. Enable OpenGL ES and EGFLS.
PACKAGECONFIG_GL = "gles2 eglfs gl"

# Enable fontconfig to get system freetype fonts
PACKAGECONFIG_FONTS += "fontconfig"

PACKAGECONFIG:append = " eglfs examples accessibility "
