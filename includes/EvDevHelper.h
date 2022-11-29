#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVHELPER_H

#include <cstring>
#include <stdlib.h>
#include <filesystem>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <typeinfo>
#include <variant>
#include <libevdev/libevdev.h>
#include <map>
#include <tuple>
#include <signal.h>
#include "spdlog/spdlog.h"
#include "evdevw.hpp"
#include "Button.h"
#include "Knob.h"
#include "Led.h"
#include "EvDevEvent.h"
#include "AlsaHelper.h"

using namespace std;

class EvDevHelper{
private:
    static vector<string> get_evdev_device();
    static tuple<int, struct libevdev *> get_traktor_controller_device();
    static void shutdown_application(int);
    static int traktor_device_id;
public:
    static void read_events_from_device(RtMidiOut *);
    static void initialize_buttons_leds();
    static void shutdown_buttons_leds();
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVHELPER_H
