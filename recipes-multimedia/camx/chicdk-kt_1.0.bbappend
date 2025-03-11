FILESPATH =+ "${TOPDIR}/../src/vendor/qcom/proprietary:"

# SRC_URI += "git://qpm-git.qualcomm.com/home2/git/revision-history/platform/vendor/qcom-proprietary/camx-kt.git;protocol=https;rev=93be056914ce5b253191de7fc1addcb74a259c7f;branch=${CUST_ID}-camx.qclinux.1.0.r1-rel;destsuffix=camx-kt \
#            git://qpm-git.qualcomm.com/home2/git/revision-history/platform/vendor/qcom-proprietary/camx-api-kt.git;protocol=https;rev=3bb46e3b1cd261dacffd512919c86a1244a0be31;branch=${CUST_ID}-camx.qclinux.1.0.r1-rel;destsuffix=camx-api-kt \
#           "
SRC_URI  = "file://chi-cdk-kt/"

S = "${WORKDIR}/chi-cdk-kt"

BASE_SRC_PATH = "${WORKDIR}"
