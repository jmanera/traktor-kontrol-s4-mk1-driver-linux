//
// Created by aspgems on 21/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_KNOB_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_KNOB_H

#include <string>
#include <map>

using namespace std;

class Knob {
private:

public:
    Knob(int code, string name, int value);
    Knob();
    int code;
    string name;
    int value;
    static map<int, Knob *> knob_mapping;
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_KNOB_H
