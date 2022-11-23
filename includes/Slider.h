//
// Created by aspgems on 21/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_SLIDER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_SLIDER_H
#include <string>
#include <map>

using namespace std;

class Slider {

public:
    Slider(int code, string name, int value);
    Slider();
    int code;
    string name;
    int value;
    static map<int, Slider *> sliders_mapping;
};



#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_SLIDER_H
