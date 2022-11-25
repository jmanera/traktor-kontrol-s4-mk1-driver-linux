//
// Created by aspgems on 22/11/22.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Jog.h"
#define CONFIG_FILE "mappings/traktor-kontrol-s4-mk1/jogs.conf"

Jog::Jog(){
    code = value = -1;
    name = "";
}

Jog::Jog(int in_code, string in_name, int in_value){
    code = in_code;
    name = in_name;
    value = in_value;
}

map<int, Jog *> Jog::jog_mapping = {
        {52, new Jog(52, "JOG WHEEL CH1 / CH3", 0)},
        {53, new Jog(53, "JOG WHEEL CH2 / CH4", 0)}
};