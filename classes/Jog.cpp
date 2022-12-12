#include "Jog.h"

map<int, Jog *> Jog::jog_mapping = {
        {52, new Jog(52, "JOG WHEEL CH1 / CH3", 0)},
        {53, new Jog(53, "JOG WHEEL CH2 / CH4", 0)},
        {26, new Jog(26, "JOG WHEEL CH1 / CH3 PRESSED", 0)},
        {27, new Jog(27, "JOG WHEEL CH1 / CH3 PRESSED", 0)}
};

Jog::Jog(){
  code = value = counter = updated = -1;
  sensitivity = 5;
  prev_control_value = -1000;
  name = "";
}

Jog::Jog(int in_code, string in_name, int in_value){
  code = in_code;
  name = in_name;
  value = in_value;
  sensitivity = 10;
  prev_control_value = -1000;
}

unsigned int Jog::handle_event(RtMidiOut *midi_out, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd){
  if (MidiEventOut::midi_mapping.find(code) != MidiEventOut::midi_mapping.end()) {
    MidiEventOut *midi_event = MidiEventOut::midi_mapping[code];
    spdlog::debug("[Jog::handle_event] Jog Wheel {0} with Code: {1} and Value {2}", name, code, value);
    spdlog::debug("[Jog::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    spdlog::debug("[Jog::handle_event] Creating message...");
    std::vector<unsigned char> message;
    unsigned char channel = midi_event->channel_byte;
    unsigned char status = midi_event->status_byte;
    if ((midi_event->channel_byte == 0xb0) || (midi_event->channel_byte == 0xb2)){
      if (shift_ch1 == true && toggle_ac == false){
        channel = midi_event->tgl_off_shf_on_channel_byte;
        status = midi_event->tgl_off_shf_on_status_byte;
      }
      if (shift_ch1 == false && toggle_ac == true){
        channel = midi_event->tgl_on_shf_off_channel_byte;
        status = midi_event->tgl_on_shf_off_status_byte;
      }
      if (shift_ch1 == true && toggle_ac == true){
        channel = midi_event->tgl_on_shf_on_channel_byte;
        status = midi_event->tgl_on_shf_on_status_byte;
      }
    }

    else if ((midi_event->channel_byte == 0xb1) || (midi_event->channel_byte == 0xb3)){
      if (shift_ch2 == true && toggle_bd == false){
        channel = midi_event->tgl_off_shf_on_channel_byte;
        status = midi_event->tgl_off_shf_on_status_byte;
      }
      if (shift_ch2 == false && toggle_bd == true){
        channel = midi_event->tgl_on_shf_off_channel_byte;
        status = midi_event->tgl_on_shf_off_status_byte;
      }
      if (shift_ch2 == true && toggle_bd == true){
        channel = midi_event->tgl_on_shf_on_channel_byte;
        status = midi_event->tgl_on_shf_on_status_byte;
      }
    }

    if (channel == -1){
      channel = midi_event->channel_byte;
    }

    if (status == -1){
      status = midi_event->status_byte;
    }
    message.push_back(channel);
    message.push_back(status);
    int midi_value = 0;
    if (midi_event->controller_type == "JOG_TOUCH"){
      if (value >= 3050){
        midi_value = 0x7f;
      }
    }
    else{
      midi_value = get_value_jog();
    }

    if (midi_value == -1000){
      return 0;
    }

    message.push_back(midi_value);
    spdlog::debug("[Jog::handle_event] Message Channel: {0} Status: {1} Value: {2}", message[0], message[1], message[2]);
    spdlog::debug("[Jog::handle_event] Message created!");
    spdlog::debug("[Jog::handle_event] Sending to MIDI Outport....");
    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      spdlog::error("[Jog::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return -1;
    }
    spdlog::debug("[Jog::handle_event] Sent!");
  }

  return 0;
}

int Jog::get_value_jog(){
  int diff = -1;
  if (prev_control_value == -1000){
    prev_control_value = value;
    updated = MidiEventOut::get_time();
    spdlog::debug("[Jog::get_value_jog] First occurrence Updated: {0} Value: {1}", updated, prev_control_value);
    return -1000;
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

  if ((MidiEventOut::get_time() - updated) < sensitivity){
    counter += diff;
    return -1000;
  }
  else{
    int midi_value = counter + diff;

    counter = 0;
    updated = MidiEventOut::get_time();

    if ((midi_value >= -64) && (midi_value < 0)){
      midi_value = 128 + midi_value;
    }
    else if(midi_value < -64){
      midi_value = 64;
    }
    else if (midi_value >= 63){
      midi_value = 63;
    }

    return midi_value;
  }
}
