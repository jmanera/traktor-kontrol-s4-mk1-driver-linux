//
// Created by aspgems on 22/11/22.
//

#include "EvDevEvent.h"

using namespace std;

const map<__u16, string> EvDevEvent::types = {
        {0, "EV_SYN"},
        {3, "EV_ABS"},
        {1, "EV_KEY"}
};

const map<__u16, string> EvDevEvent::codes = {
        {0, "SYN_REPORT"},
        {51, "ABS_MT_WIDTH_MINOR"},
        {50, "ABS_MT_WIDTH_MAJOR"},
        {49, "ABS_MT_TOUCH_MINOR"},
        {48, "ABS_MT_TOUCH_MAJOR"},
        {52, "ABS_MT_ORIENTATION"},
        {53, "ABS_MT_POSITION_X"},
        {54, "ABS_MT_POSITION_Y"},
        {55, "ABS_MT_TOOL_TYPE"},
        {56, "ABS_MT_BLOB_ID"},
        {57, "ABS_MT_TRACKING_ID"},
        {58, "ABS_MT_PRESSURE"},
        {59, "ABS_MT_DISTANCE"},
        {60, "ABS_MT_TOOL_X"},
        {61, "ABS_MT_TOOL_Y"},
        {25, "ABS_DISTANCE"},
        {27, "ABS_TILT_Y"},

        // Sliders
        {21, "ABS_HAT2Y"},

        // Buttons
        {256, "BTN_0"},
        {257, "BTN_1"},
        {258, "BTN_2"},
        {259, "BTN_3"},
        {260, "BTN_4"},
        {261, "BTN_5"},
        {262, "BTN_6"},
        {263, "BTN_7"},
        {264, "BTN_8"},
        {264, "BTN_9"},
        {274, "BTN_MIDDLE"},
        {275, "BTN_SIDE"},
        {276, "BTN_RIGHT"},
        {272, "BTN_LEFT"},
        {288, "BTN_TRIGGER"},
        {289, "BTN_THUMB"},
        {290, "BTN_THUMB2"},
        {291, "BTN_TOP"},
        {292, "BTN_TOP2"},
        {297, "BTN_BASE4"},
        {296, "BTN_BASE3"},
        {322, "BTN_TOOL_BRUSH"},
        {326, "BTN_TOOL_MOUSE"},
        {327, "BTN_TOOL_LENS"}
};


EvDevEvent::EvDevEvent(__u16 in_type, __u16 in_code, __s32 in_value, timeval in_time, bool in_shift_ch1, bool in_shift_ch2, bool in_toggle_ac, bool in_toggle_bd) {
    type = in_type;
    code = in_code;
    value = in_value;
    time = in_time;
    shift_ch1 = in_shift_ch1;
    shift_ch2 = in_shift_ch2;
    toggle_ac = in_toggle_ac;
    toggle_bd = in_toggle_bd;
}

void EvDevEvent::handle_with(RtMidiOut *midi_out){
    spdlog::debug("[EvDevEvent::handle_with] Checking event to handle with...");
    string type_string = EvDevEvent::types.find(type)->second;
    if (type_string == "EV_KEY"){
        button_dev = Button::buttons_mapping[code];
        button_dev->value = value;
        spdlog::debug("[EvDevEvent::handle_with] Get Button. Code: {0}, Name: {1}, LED Code: {2}, Channel: {3}", to_string(button_dev->code), button_dev->name, to_string(button_dev->led_code), to_string(button_dev->channel));
        int status = button_dev->handle_event(midi_out, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
        if (status < 0){
            spdlog::error("[EvDevEvent::handle_with] Error handling button with code: {0} {1}", to_string(status), strerror(status));
        }
    }
    else if (type_string == "EV_ABS"){
        if (Slider::sliders_mapping.find(code) != Slider::sliders_mapping.end()){
            slider_dev = Slider::sliders_mapping[code];
            slider_dev->value = floor(value / 32);
            spdlog::debug("[EvDevEvent::handle_with] Get Slider. Code: {0}, Name: {1}, Value: {2}", to_string(slider_dev->code), slider_dev->name, to_string(slider_dev->value));
            int status = slider_dev->handle_event(midi_out, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
            if (status < 0){
              spdlog::error("[EvDevEvent::handle_with] Error handling slider with code: {0} {1}", to_string(status), strerror(status));
            }
        }
        else if (Knob::knob_mapping.find(code) != Knob::knob_mapping.end()){
            knob_dev = Knob::knob_mapping[code];
            knob_dev->value = value;
            spdlog::debug("[EvDevEvent::handle_with] Get Knob. Code: {0}, Name: {1}, Value: {2}", to_string(knob_dev->code), knob_dev->name, to_string(knob_dev->value));
            int status = knob_dev->handle_event(midi_out, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
            if (status < 0){
              spdlog::error("[EvDevEvent::handle_with] Error handling knob with code: {0} {1}", to_string(status), strerror(status));
            }
        }
        else if (Jog::jog_mapping.find(code) != Jog::jog_mapping.end()){
            jog_dev = Jog::jog_mapping[code];
            jog_dev->value = value;
            spdlog::debug("[EvDevEvent::handle_with] Get Knob. Code: {0}, Name: {1}, Value: {2}", to_string(jog_dev->code), jog_dev->name, to_string(jog_dev->value));
        }
        else{
            spdlog::warn("[EvDevEvent::handle_with] EV_ABS Event not handled: {0} {1} {2} {3}", to_string(type), to_string(code), to_string(value), to_string(time.tv_sec));
        }
    }
    else {
        spdlog::error("[EvDevEvent::handle_with] Event not handled: Type: {0} Code: {1} Value: {2} Time: {3}", EvDevEvent::types.find(type)->second, to_string(code), to_string(value), to_string(time.tv_sec));
    }
    spdlog::debug("[EvDevEvent::handle_with] FINISHED");
}
