
#CROSS_TOOLS_DIR = ~/VR_V10/ambalink_sdk_4_4/output.oem/h2_ambalink/host/usr/bin/
#CROSS_COMPILER  = $(CROSS_TOOLS_DIR)aarch64-linux-gnu-
GCC = $(CROSS_COMPILER)gcc

all : app_vsc

CFLAGS += -D LINUX_VERSION

app_vsc.o : app_vsc.c
	$(GCC) -c -g $(CFLAGS)  app_vsc.c
brcm_patchram_plus.o : brcm_patchram_plus.c
	$(GCC) -c -g $(CFLAGS) brcm_patchram_plus.c
app_vsc : app_vsc.o brcm_patchram_plus.o
	$(GCC) app_vsc.o brcm_patchram_plus.o -o $@ -g $(CFLAGS) -lpthread -lrt
	#$(GCC) app_vsc.o brcm_patchram_plus.o -o $@ -g $(CFLAGS) -pthread -lpthread -lrt

clean:
	rm -f app_vsc app_vsc.o brcm_patchram_plus.o
