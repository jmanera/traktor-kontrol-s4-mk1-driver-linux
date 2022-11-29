#include "Knob.h"

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
        {59, new Knob(59, "CH1 GAIN KNOB", 0)},
        {60, new Knob(60, "CH2 GAIN KNOB", 0)},
        {61, new Knob(61, "CH3 GAIN KNOB", 0)},
        {62, new Knob(62, "CH4 GAIN KNOB", 0)}
};

Knob::Knob(){
  code = value = -1;
  name = "";
}

Knob::Knob(int in_code, string in_name, int in_value){
  code = in_code;
  name = in_name;
  value = in_value;
  counter = prev_control_value = updated = -1;
}

unsigned int Knob::handle_event(RtMidiOut *midi_out, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd) {
  if (MidiEvent::midi_mapping.find(code) != MidiEvent::midi_mapping.end()) {
    MidiEvent *midi_event = MidiEvent::midi_mapping[code];
    spdlog::debug("[Knob::handle_event] Button named {0} performed with Code:{1} Value: {2}", name, code, value);
    spdlog::debug("[Knob::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    spdlog::debug("[Knob::handle_event] Creating message...");
    std::vector<unsigned char> message;
    message.push_back(midi_event->channel_byte);
    message.push_back(midi_event->status_byte);
    string rot;
    /*if (midi_event->controller_type == "ROT"){
      string rots[4] = {"move_rot_", "size_rot_", "move_rot_", "size_rot_"};
      rot = rots[code - 55];
      if (code <= 56) {
        if (toggle_ac == true) {
          rot = rot + "c";
        } else {
          rot = rot + "a";
        }
      }
      else{
        if (toggle_bd == true) {
          rot = rot + "d";
        } else {
          rot = rot + "b";
        }
      }
    }*/

    if ((midi_event->controller_type == "BROWSE_ROT") || (midi_event->controller_type == "ROT")){
      int diff = -1;
      if (prev_control_value == -1){
        prev_control_value = value;
        updated = get_time();
        return EXIT_SUCCESS;
      }
      if (value <= 3 && prev_control_value >= 12){
        diff = 16 - prev_control_value + value;
      }
      else if(value >= 12 && prev_control_value <= 3){
        diff = value - 16 - prev_control_value;
      }
      else{
        diff = value - prev_control_value;
      }

      prev_control_value = value;

      if ((get_time() - updated) < 0.005){
        counter += diff;
        return EXIT_SUCCESS;
      }
      else{
        value = 0x3F + counter + diff;
        counter = 0;
        updated = get_time();
        if (value > 0x7F){
          value = 0x7F;
        }
        else if (value < 0){
          value = 0;
        }
      }
    }
    message.push_back(value);
    spdlog::debug("[Knob::handle_event] Message created!");
    spdlog::debug("[Knob::handle_event] Sending to MIDI Outport....");
    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      spdlog::error("[Knob::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return EXIT_FAILURE;
    }
    spdlog::debug("[Knob::handle_event] Sent!");

    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}

int Knob::get_time(){
  return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}