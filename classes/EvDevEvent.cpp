#include "EvDevEvent.h"

using namespace std;

Knob *knob_dev;
Slider *slider_dev;
Jog *jog_dev;
Button *button_dev;

const map<__u16, string> EvDevEvent::types = {
        {0, "EV_SYN"},
        {3, "EV_ABS"},
        {1, "EV_KEY"}
};

EvDevEvent::EvDevEvent(__u16 in_type, __u16 in_code, __s32 in_value, timeval in_time) {
    type = in_type;
    code = in_code;
    value = in_value;
    time = in_time;
}

void EvDevEvent::handle_with(RtMidiOut *midi_out, int controller_id, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd){
    spdlog::debug("[EvDevEvent::handle_with] Checking event to handle with...");
    string type_string = EvDevEvent::types.find(type)->second;
    spdlog::debug("[EvDevEvent::handle_with] Type: {0} Code: {1}", type_string, code);
    if (type_string == "EV_KEY"){
        button_dev = Button::buttons_mapping[code];
        if (button_dev == nullptr){
          spdlog::debug("[EvDevEvent::handle_with] BUTTON not recognized with code: {0}", code);
          return;
        }

        button_dev->value = value;
        spdlog::debug("[EvDevEvent::handle_with] Get BUTTON. Code: {0}, Name: {1}, LED Code: {2}, Channel: {3}", to_string(button_dev->code), button_dev->name, to_string(button_dev->led_code), to_string(button_dev->channel));
        int status = button_dev->handle_event(midi_out, controller_id, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
        if (status < 0){
            spdlog::debug("[EvDevEvent::handle_with] Error handling BUTTON with Code: {0} Name: {1} Status: {2}", to_string(button_dev->code), button_dev->name,  strerror(status));
        }
    }
    else if (type_string == "EV_ABS"){
        if (Slider::sliders_mapping.find(code) != Slider::sliders_mapping.end()){
            slider_dev = Slider::sliders_mapping[code];
            if (slider_dev == nullptr){
              spdlog::debug("[EvDevEvent::handle_with] SLIDER not recognized with code: {0}", code);
              return;
            }

            slider_dev->value = floor(value / 32);
            spdlog::debug("[EvDevEvent::handle_with] Get SLIDER. Code: {0}, Name: {1}, Value: {2}", to_string(slider_dev->code), slider_dev->name, to_string(slider_dev->value));
            int status = slider_dev->handle_event(midi_out, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
            if (status < 0){
              spdlog::debug("[EvDevEvent::handle_with] Error handling SLIDER with Code: {0} Name: {1} Status: {2}", to_string(slider_dev->code), slider_dev->name,  strerror(status));
            }
        }
        else if (Knob::knob_mapping.find(code) != Knob::knob_mapping.end()){
            knob_dev = Knob::knob_mapping[code];
            if (knob_dev == nullptr){
              spdlog::debug("[EvDevEvent::handle_with] KNOB not recognized with code: {0}", code);
              return;
            }

            knob_dev->value = value;
            spdlog::debug("[EvDevEvent::handle_with] Get KNOB. Code: {0}, Name: {1}, Value: {2}", to_string(knob_dev->code), knob_dev->name, to_string(knob_dev->value));
            int status = knob_dev->handle_event(midi_out, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
            if (status < 0){
              spdlog::debug("[EvDevEvent::handle_with] Error handling KNOB with Code: {0} Name: {1} Status: {2}", to_string(knob_dev->code), knob_dev->name,  strerror(status));
            }
        }
        else if (Jog::jog_mapping.find(code) != Jog::jog_mapping.end()){
            jog_dev = Jog::jog_mapping[code];
            if (jog_dev == nullptr){
              spdlog::debug("[EvDevEvent::handle_with] JOG WHEEL not recognized with code: {0}", code);
              return;
            }

            jog_dev->value = value;
            spdlog::debug("[EvDevEvent::handle_with] Get JOG HWEEL. Code: {0}, Name: {1}, Value: {2}", to_string(jog_dev->code), jog_dev->name, to_string(jog_dev->value));
            int status = jog_dev->handle_event(midi_out, shift_ch1, shift_ch2, toggle_ac, toggle_bd);
            if (status < 0){
              spdlog::debug("[EvDevEvent::handle_with] Error handling JOG WHEEL with Code: {0} Name: {1} Status: {2}", to_string(button_dev->code), button_dev->name,  strerror(status));
            }
        }
        else{
            spdlog::debug("[EvDevEvent::handle_with] EV_ABS Event not handled: {0} {1} {2} {3}", to_string(type), to_string(code), to_string(value), to_string(time.tv_sec));
        }
    }
    else {
        spdlog::debug("[EvDevEvent::handle_with] Event not recognized: Type: {0} Code: {1} Value: {2} Time: {3}", EvDevEvent::types.find(type)->second, to_string(code), to_string(value), to_string(time.tv_sec));
    }
    spdlog::debug("[EvDevEvent::handle_with] Finished");
}
