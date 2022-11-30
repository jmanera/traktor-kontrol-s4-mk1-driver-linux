#include "Slider.h"

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

Slider::Slider(int in_code, string in_name, int in_value){
  code = in_code;
  name = in_name;
  value = in_value;
}

Slider::Slider(){
  code = value = -1;
  name = "";
}

unsigned int Slider::handle_event(RtMidiOut *midi_out, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd){
  if (MidiEvent::midi_mapping.find(code) != MidiEvent::midi_mapping.end()) {
    MidiEvent *midi_event = MidiEvent::midi_mapping[code];
    spdlog::debug("[Slider::handle_event] Button named {0} performed with Code:{1} Value: {4}", name, code, value);
    spdlog::debug("[Slider::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    spdlog::debug("[Slider::handle_event] Creating message...");
    std::vector<unsigned char> message;
    message.push_back(midi_event->channel_byte);
    message.push_back(midi_event->status_byte);
    message.push_back(value);
    spdlog::debug("[Slider::handle_event] Message created!");
    spdlog::debug("[Slider::handle_event] Sending to MIDI Outport....");
    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      spdlog::error("[Slider::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return EXIT_FAILURE;
    }
    spdlog::debug("[Slider::handle_event] Sent!");
  }

  return EXIT_SUCCESS;
}