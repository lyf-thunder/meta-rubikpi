#include <stdio.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdbool.h>
#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "notify.h"

#define ALOGE(fmt, arg...) syslog (LOG_ERR, fmt, ##arg)
#define ALOGI(fmt, arg...) syslog (LOG_INFO, fmt, ##arg)
#define ALOGD(fmt, arg...) syslog (LOG_DEBUG, fmt, ##arg)
#define ALOGV(fmt, arg...) syslog (LOG_NOTICE, fmt, ##arg)

#define NETLINK_ES8316 29
#define NETLINK_LT9611 30
#define MSG_LEN 125

typedef struct _user_msg_info {
    struct nlmsghdr hdr;
    char msg[MSG_LEN];
} user_msg_info;

int Notify::addDevices(audio_output_type_t id)
{
    std::map<audio_output_type_t, std::string>::iterator it = devices.find(id);
    if (it == devices.end())
        return -22;

    for (auto it = deviceList.begin(); it != deviceList.end(); ++it) {
        if (it->id == id) {
            return 0;
        }
    }

    Device newDevice;
    newDevice.id = id;
    newDevice.name = it->second;
    deviceList.push_back(newDevice);

    ALOGD("%s: new device(%s)", __func__, newDevice.name);
    return 0;
}

int Notify::delDevices(audio_output_type_t id)
{
    for (auto it = deviceList.begin(); it != deviceList.end(); ++it) {
        if (it->id == id) {
            deviceList.erase(it);
            ALOGD("%s: del device(%s)", __func__, it->name.c_str());
            return 0;
        }
    }
    return 0;
}

bool Notify::isDevicesexist(audio_output_type_t id)
{
    for (auto it = deviceList.begin(); it != deviceList.end(); ++it) {
        if (it->id == id) {
            ALOGD("%s: device exist(%s)", __func__, it->name.c_str());
            return true;
        }
    }
    ALOGD("%s: device not find(%d)", __func__, id);
    return false;
}


int Notify::switchDevices()
{
    if (this->isDevicesexist(AUDIO_DEVICE_OUT_HEADSET)) {
        if (getDefaultDevice() != AUDIO_DEVICE_OUT_HEADSET) {
            system("pactl set-sink-port 0 headset");
            system("pactl set-source-port 4 headset-mic");
            setDefaultDevice(AUDIO_DEVICE_OUT_HEADSET);
            ALOGD("%s: connect headset", __func__);
        }
    } else if (this->isDevicesexist(AUDIO_DEVICE_OUT_HDMI)) {
        if (getDefaultDevice() != AUDIO_DEVICE_OUT_HDMI) {
            system("pactl set-sink-port 0 hdmi");
            setDefaultDevice(AUDIO_DEVICE_OUT_HDMI);
            ALOGD("%s: connect hdmi", __func__);
        }
    } else {
        if (getDefaultDevice() != AUDIO_DEVICE_OUT_SPEAKER) {
            system("pactl set-sink-port 0 speaker");
            system("pactl set-source-port 4 speaker-mic");
            setDefaultDevice(AUDIO_DEVICE_OUT_SPEAKER);
            ALOGD("%s: connect speaker", __func__);
        }
    }

    usleep(500 * 1000);

    return 0;
}

void *notify_handle(void *argv) {
    Notify *p = static_cast<Notify*>(argv);
    int s = 0, n;
    user_msg_info u_msg;
    struct sockaddr_nl addr;
    audio_output_type_t type;

    ALOGD("%s: sockId = %d", __func__, p->sockId);
    s = socket(AF_NETLINK, SOCK_RAW, p->sockId);
    if (s < 0) {
        ALOGE("%s: create socket failed", __func__);
        return NULL;
    }

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = 200;
    addr.nl_groups = 0;

    ALOGD("%s: socket = %d", __func__, s);
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ALOGE("%s: bind socket failed", __func__);
        close(s);
        return NULL;
    }

    for (;;) {
        memset(&u_msg, 0, sizeof(u_msg));
        socklen_t len = sizeof(struct sockaddr_nl);
        n = recvfrom(s, &u_msg, sizeof(user_msg_info), 0,
                        (struct sockaddr *)&addr, &len);
        if (strstr(u_msg.msg, "Connection")) {
            if (strstr(u_msg.msg, "Headset"))
                type = AUDIO_DEVICE_OUT_HEADSET;
            else if (strstr(u_msg.msg, "Hdmi"))
                type = AUDIO_DEVICE_OUT_HDMI;
            p->addDevices(type);
        } else if (strstr(u_msg.msg, "Disconnect")) {
            if (strstr(u_msg.msg, "Headset"))
                type = AUDIO_DEVICE_OUT_HEADSET;
            else if (strstr(u_msg.msg, "Hdmi"))
                type = AUDIO_DEVICE_OUT_HDMI;
            p->delDevices(type);
        } else if (!strncmp(u_msg.msg, "Restart Adsp", strlen("Restart Adsp"))) {
            ALOGD("%s: Attempt to restart ADSP", __func__);
            system("systemctl restart adsprpcd_audiopd");
            continue;
        }
        p->switchDevices();
    }
    close(s);

    return NULL;
}

int Notify::initDeviceStatus(std::shared_ptr<Notify> p)
{
    const char *path1 = "/sys/devices/platform/soc@0/9c0000.geniqup/980000.i2c/i2c-0/0-0011/detect_info";
    const char *path2 = "/sys/devices/platform/soc@0/ac0000.geniqup/a84000.i2c/i2c-9/9-0039/detect_info";
    int fd = -1;
    char buf = 0;

    if ((fd = open(path1, O_RDONLY)) < 0) {
        ALOGD("%s: open fail path(%s) fd(%d)", __func__, "es8316", fd);
    } else {
        if (read(fd, &buf, sizeof(buf)) < 0)
            ALOGE("%s: read fail\n", __func__);
        close(fd);
        fd = -1;
        ALOGD("%s: buf(%d)", __func__, buf);
        if (buf == 49) {
            p->addDevices(AUDIO_DEVICE_OUT_HEADSET);
            p->switchDevices();
            buf = 0;
        }
    }

    if ((fd = open(path2, O_RDONLY)) < 0) {
        ALOGD("%s: open fail path(%s) fd(%d)", __func__, "lt9611", fd);
    } else {
        if (read(fd, &buf, sizeof(buf)) < 0)
            ALOGE("%s: read fail", __func__);
        close(fd);
        fd = -1;
        ALOGD("%s: buf(%d)", __func__, "es8316", buf);
        if (buf == 49) {
            p->addDevices(AUDIO_DEVICE_OUT_HDMI);
            p->switchDevices();
            buf = 0;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    std::shared_ptr<Notify> p = std::make_shared<Notify>();

    ALOGD("%s: enter", __func__);
    p->initDeviceStatus(p);

    p->sockId = NETLINK_ES8316;
    if (pthread_create(&thread1, NULL, notify_handle, p.get())) {
        ALOGE("%s: Thread creation failed", __func__);
        exit(EXIT_FAILURE);
    }

    usleep(10 * 1000);
    p->sockId = NETLINK_LT9611;
    if (pthread_create(&thread2, NULL, notify_handle, p.get())) {
        ALOGE("%s: Thread creation failed", __func__);
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
