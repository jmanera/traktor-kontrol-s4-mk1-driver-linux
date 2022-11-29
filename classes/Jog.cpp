#include "Jog.h"

map<int, Jog *> Jog::jog_mapping = {
        {52, new Jog(52, "JOG WHEEL CH1 / CH3", 0)},
        {53, new Jog(53, "JOG WHEEL CH2 / CH4", 0)}
};

Jog::Jog(){
  code = value = counter = prev_control_value = sensitivity = updated = -1;
  name = "";
}

Jog::Jog(int in_code, string in_name, int in_value){
  code = in_code;
  name = in_name;
  value = in_value;
}


unsigned int Jog::handle_event(RtMidiOut *midi_out, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd){
  if (MidiEvent::midi_mapping.find(code) != MidiEvent::midi_mapping.end()) {
    MidiEvent *midi_event = MidiEvent::midi_mapping[code];
    spdlog::debug("[Jog::handle_event] Button named {0} performed with Code: {1} Led Code: {2} Channel: {3} Value: {4}", name, code, value);
    spdlog::debug("[Jog::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    spdlog::debug("[Jog::handle_event] Creating message...");
    std::vector<unsigned char> message;
    message.push_back(midi_event->channel_byte);
    message.push_back(midi_event->status_byte);
    message.push_back(get_value_jog());
    spdlog::debug("[Jog::handle_event] Message created!");
    spdlog::debug("[Jog::handle_event] Sending to MIDI Outport....");
    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      spdlog::error("[Jog::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return EXIT_FAILURE;
    }
    spdlog::debug("[Jog::handle_event] Sent!");
  }

  return EXIT_SUCCESS;
}

int Jog::get_value_jog(){
  int diff = -1;
  if (prev_control_value == -1){
    prev_control_value = value;
    updated = MidiEvent::get_time();
    return -1;
  }

  if (value <= 255 && prev_control_value >= 767){
    diff = 1024 - prev_control_value + value;
  }
  else if (value >= 767 && prev_control_value <= 255){
    diff = value - 1024 - prev_control_value;
  }
  else{
    diff = value - prev_control_value;
  }

  prev_control_value = value;

  if ((MidiEvent::get_time() - updated) < sensitivity){
    counter = diff;
    return -1;
  }
  else{
    value = counter + diff;
    counter = 0;
    updated = MidiEvent::get_time();

    if (-64 <= value < 0){
      value = 128 + value;
    }
    else if(value < -64){
      value = 64;
    }
    else if (value >= 63){
      value = 63;
    }

    return value;
  }
  return EXIT_SUCCESS;
}