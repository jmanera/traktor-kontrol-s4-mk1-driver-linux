//
// Created by aspgems on 21/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H

#include <string>
#include <map>
#include "Led.h"

using namespace std;

class Button {
private:

public:
    Button(int code, string name, int led_code, int channel, int value = 0);
    Button();
    int code;
    string name;
    int led_code;
    int channel;
    int value;
    bool hold;
    static map<int, Button *> buttons_mapping;
    int handle_event();
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H
