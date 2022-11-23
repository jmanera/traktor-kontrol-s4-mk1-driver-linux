//
// Created by aspgems on 21/11/22.
//

#include "Knob.h"

Knob::Knob(){
    code = value = -1;
    name = "";
}

Knob::Knob(int in_code, string in_name, int in_value){
    code = in_code;
    name = in_name;
    value = in_value;
}

map<int, Knob *> Knob::knob_mapping = {
        {32, new Knob(32, "FX1 EFFECT 3 KNOB", 0)},
        {33, new Knob(33, "FX1 EFFECT 2 KNOB", 0)},
        {34, new Knob(34, "FX1 EFFECT 1 KNOB", 0)},
        {35, new Knob(35, "FX1 DRY WET KNOB", 0)},

        {48, new Knob(48, "FX1 EFFECT 3 KNOB", 0)},
        {49, new Knob(49, "FX1 EFFECT 2 KNOB", 0)},
        {50, new Knob(50, "FX1 EFFECT 1 KNOB", 0)},
        {51, new Knob(51, "FX1 DRY WET KNOB", 0)},

        {54, new Knob(54, "BROWSE KNOB", 0)},
        {55, new Knob(55, "CH1 / CH3 MOVE KNOB", 0)},
        {56, new Knob(56, "CH1 / CH3 LOOP SIZE KNOB", 0)},
        {57, new Knob(57, "CH2 / CH4 MOVE KNOB", 0)},
        {58, new Knob(58, "CH2 / CH4 LOOP SIZE KNOB", 0)},

        {59, new Knob(48, "CH1 GAIN KNOB", 0)},
        {60, new Knob(60, "CH2 GAIN KNOB", 0)},
        {61, new Knob(35, "CH3 GAIN KNOB", 0)},
        {62, new Knob(62, "CH4 GAIN KNOB", 0)}
};