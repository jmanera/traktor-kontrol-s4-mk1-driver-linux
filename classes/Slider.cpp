#include "Slider.h"

map<int, Slider *> Slider::sliders_mapping = {
        {16, new Slider(16, "SLIDER VOLUME CH4", 0)},
        {17, new Slider(17, "SLIDER VOLUME CH2", 0)},
        {18, new Slider(18, "SLIDER VOLUME CH1", 0)},
        {19, new Slider(19, "SLIDER VOLUME CH3", 0)},
        {21, new Slider(21, "SLIDER PITCH CH1 / CH3", 0)},
        {22, new Slider(22, "SLIDER PITCH CH2 / CH4", 0)},
        {23, new Slider(23, "SLIDER CROSS-FADER", 0)}

};

Slider::Slider(int in_code, string in_name, int in_value){
  code = in_code;
  name = in_name;
  value = in_value;
}

Slider::Slider(){
  code = value = -1;
  name = "";
}

int Slider::handle_event(RtMidiOut *midi_out, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd, ConfigHelper *config_helper){
  shared_ptr<spdlog::logger> logger = spdlog::get(config_helper->get_string_value("traktor_s4_logger_name"));
  if (MidiEventOut::midi_mapping.find(code) != MidiEventOut::midi_mapping.end()) {
    MidiEventOut *midi_event = MidiEventOut::midi_mapping[code];
    logger->debug("[Slider::handle_event] Slider named {0} performed with Code:{1} Value: {2}", name, code, value);
    logger->debug("[Slider::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    logger->debug("[Slider::handle_event] Creating message...");

    auto message = UtilsHelper::create_message(shift_ch1, shift_ch2, toggle_ac, toggle_bd, midi_event, value);

    logger->debug("[Slider::handle_event] Message created!");
    logger->debug("[Slider::handle_event] Sending to MIDI out port....");

    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      logger->error("[Slider::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return -1;
    }

    logger->debug("[Slider::handle_event] Sent!");
  }
  return 0;
}
