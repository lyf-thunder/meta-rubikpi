#ifndef NOTIFY_H
#define NOTIFY_H

#include <iostream>
#include <list>
#include <memory>
#include <typeinfo>
#include <map>

typedef enum {
    AUDIO_DEVICE_OUT_SPEAKER = 0x0u,
    AUDIO_DEVICE_OUT_HEADSET = 0x1u,
    AUDIO_DEVICE_OUT_HDMI = 0x2u,
    AUDIO_DEVICE_MAX = AUDIO_DEVICE_OUT_HDMI,
} audio_output_type_t;

std::map<audio_output_type_t, std::string> devices = {
    {AUDIO_DEVICE_OUT_HEADSET, "Headset"},
    {AUDIO_DEVICE_OUT_HDMI, "Hdmi"},
};

class Notify {
public:
    Notify() {}
    ~Notify() {}
    int initDeviceStatus(std::shared_ptr<Notify> p);
    int addDevices(audio_output_type_t id);
    int delDevices(audio_output_type_t id);
    int switchDevices();
    int setDefaultDevice(audio_output_type_t device) {
        defaultDevice = device;
        return 0;
    }
    audio_output_type_t getDefaultDevice() { return defaultDevice; }
    int sockId;
private:
    bool isDevicesexist(audio_output_type_t id);
    struct Device {
        audio_output_type_t id;
        std::string name;
    };
    std::list<Device> deviceList;
    audio_output_type_t defaultDevice = AUDIO_DEVICE_OUT_SPEAKER;
};

#endif