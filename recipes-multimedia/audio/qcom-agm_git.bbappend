FILESPATH =+ "${TOPDIR}/../layers/meta-rubikpi/recipes-multimedia/audio/agm:"
SRC_URI += "\
    file://0002_Modify_the_backend_conf_xml_file.patch \
    file://0003_Change_the_capture_format_of_ES8316_from_1ch_to_2ch.patch \
    file://0004_Change_the_HDMI_OUT_AUDIO_format_from_16_to_32bit.patch \
    file://0005_Enable_the_third_i2s.patch \
    file://0006_Enable_BTHS_record.patch \
    file://0007_Add_agmhostless_tool_to_Linux.patch \
"
