//
// Created by aspgems on 22/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_JOG_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_JOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class Jog {
public:
    Jog(int code, string name, int value);
    Jog();
    int code;
    string name;
    int value;
    static map<int, Jog *> jog_mapping;
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_JOG_H
