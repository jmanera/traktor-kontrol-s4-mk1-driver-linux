//
// Created by aspgems on 21/11/22.
//

#include "Slider.h"

Slider::Slider(int in_code, string in_name, int in_value){
    code = in_code;
    name = in_name;
    value = in_value;
}

Slider::Slider(){
    code = value = -1;
    name = "";
}

map<int, Slider *> Slider::sliders_mapping = {
        {16, new Slider(16, "SLIDER VOLUME CH4", 0)},
        {17, new Slider(17, "SLIDER VOLUME CH2", 0)},
        {18, new Slider(18, "SLIDER VOLUME CH1", 0)},
        {19, new Slider(19, "SLIDER VOLUME CH3", 0)},
        {21, new Slider(21, "SLIDER PITCH CH1 / CH3", 0)},
        {22, new Slider(22, "SLIDER PITCH CH2 / CH4", 0)},
        {23, new Slider(23, "SLIDER CROSSFADER", 0)},
        {24, new Slider(24, "KNOB MIC VOL FRONT", 0)},
        {25, new Slider(25, "KNOB CUE MIX FRONT", 0)}
};