#include "Knob.h"

map<int, Knob *> Knob::knob_mapping = {
    {24, new Knob(24, "KNOB MIC VOL FRONT", 0)},
    {25, new Knob(25, "KNOB CUE MIX FRONT", 0)},
    {28, new Knob(28, "DECK D FILTER", 0)},
    {44, new Knob(44, "DECK C FILTER", 0)},
    {36, new Knob(36, "DECK B FILTER", 0)},
    {40, new Knob(40, "DECK A FILTER", 0)},
    {29, new Knob(29, "DECK D LOW", 0)},
    {45, new Knob(45, "DECK C LOW", 0)},
    {37, new Knob(37, "DECK B LOW", 0)},
    {41, new Knob(41, "DECK A LOW", 0)},
    {30, new Knob(30, "DECK D MID", 0)},
    {46, new Knob(46, "DECK C MID", 0)},
    {38, new Knob(38, "DECK B MID", 0)},
    {42, new Knob(42, "DECK A MID", 0)},
    {31, new Knob(31, "DECK D HI", 0)},
    {47, new Knob(47, "DECK C HI", 0)},
    {39, new Knob(39, "DECK B HI", 0)},
    {43, new Knob(43, "DECK A HI", 0)},
    {32, new Knob(32, "FX1 EFFECT 3 KNOB", 0)},
    {33, new Knob(33, "FX1 EFFECT 2 KNOB", 0)},
    {34, new Knob(34, "FX1 EFFECT 1 KNOB", 0)},
    {35, new Knob(35, "FX1 DRY WET KNOB", 0)},
    {48, new Knob(48, "FX2 EFFECT 3 KNOB", 0)},
    {49, new Knob(49, "FX2 EFFECT 2 KNOB", 0)},
    {50, new Knob(50, "FX2 EFFECT 1 KNOB", 0)},
    {51, new Knob(51, "FX2 DRY WET KNOB", 0)},
    {54, new Knob(54, "BROWSE KNOB", 0)},
    {55, new Knob(55, "CH1 / CH3 MOVE KNOB", 0)},
    {56, new Knob(56, "CH1 / CH3 SIZE KNOB", 0)},
    {57, new Knob(57, "CH2 / CH4 MOVE KNOB", 0)},
    {58, new Knob(58, "CH2 / CH4 SIZE KNOB", 0)},
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

int Knob::handle_event(RtMidiOut *midi_out, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd, ConfigHelper *config_helper) {
  shared_ptr<spdlog::logger> logger = spdlog::get(config_helper->get_string_value("traktor_s4_logger_name"));
  if (MidiEventOut::midi_mapping.find(code) != MidiEventOut::midi_mapping.end()) {
    MidiEventOut *midi_event = MidiEventOut::midi_mapping[code];
    logger->debug("[Knob::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    logger->debug("[Knob::handle_event] Creating message...");

    if (midi_event->controller_type == "BROWSE_ROT") {
      value = get_value_rot();
    }
    else if (midi_event->controller_type == "ROT"){
      value = get_value_rot();
    }
    else if (midi_event->controller_type == "ROT_MOVE"){
      value = get_value_rot_move();
    }
    else if (midi_event->controller_type == "GAIN_ROT"){
      value = get_value_gain_rot();
    }
    else if (midi_event->controller_type == "POT"){
      value = floor(value / 32);
    }

    if ((value < 0) && (midi_event->controller_type != "ROT_MOVE")){
      logger->debug("[Knob::handle_event] Message didn't sent!");
      return -1;
    }

    logger->debug("[Knob::handle_event] Knob named {0} performed with Code:{1} Value: {2}", name, code, value);
    logger->debug("[Knob::handle_event] Knob named {0} performed with Code:{1} Value: {2} parsed", name, code, (unsigned char)value);

    auto message = UtilsHelper::create_message(shift_ch1, shift_ch2, toggle_ac, toggle_bd, midi_event, value);

    logger->debug("[Knob::handle_event] Message created!");
    logger->debug("[Knob::handle_event] Sending to MIDI Outport....");
    logger->debug("[Knob::handle_event] Message {0} - {1} - {2}....", message[0], message[1], message[2]);

    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      logger->error("[Knob::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return -1;
    }

    logger->debug("[Knob::handle_event] Sent!");

    return 0;
  }
  return 0;
}

int Knob::get_value_rot_move(){
  int diff = 0;
  if (prev_control_value == -1){
    prev_control_value = value;
    updated = MidiEventOut::get_time();
    return 0;
  }

  updated = MidiEventOut::get_time();
  if (value > prev_control_value){
    prev_control_value = value;
    return 1;
  }
  else if (value < prev_control_value){
    prev_control_value = value;
    return -1;
  }
  return 0;
}

int Knob::get_value_gain_rot() {
  int diff = -1;
  if (prev_control_value == -1){
    prev_control_value = value;
    counter = 0x3f;
    updated = MidiEventOut::get_time();
    return 0x3f;
  }

  if (value <= 3 && prev_control_value >= 12){
    diff = 16 - prev_control_value + value;
  }
  else if (value >= 12 && prev_control_value <= 3){
    diff = value - 16 - prev_control_value;
  }
  else{
    diff = value - prev_control_value;
  }
  prev_control_value = value;

  if ((MidiEventOut::get_time() - updated) < 5){
    counter += diff;
    return -1;
  }
  else{
    counter += diff;
    updated = MidiEventOut::get_time();
    if (counter > 0x7f){
      counter = 0x7f;
    }
    else if (counter < 0){
      counter = 0;
    }
    return counter;
  }
}

int Knob::get_value_rot() {
  int diff = -1;
  if (prev_control_value == -1){
    prev_control_value = value;
    updated = MidiEventOut::get_time();
    return -1;
  }

  if (value <= 3 && prev_control_value >= 12){
    diff = 16 - prev_control_value + value;
  }
  else if (value >= 12 && prev_control_value <= 3){
    diff = value - 16 - prev_control_value;
  }
  else{
    diff = value - prev_control_value;
  }

  prev_control_value = value;

  if ((MidiEventOut::get_time() - updated) < 5){
    counter += diff;
    return -1;
  }
  else{
    value = 0x3f + counter + diff;
    counter = 0;
    updated = MidiEventOut::get_time();

    if (value > 0x7f){
      value = 0x7f;
    }
    else if (value < 0){
      value = 0;
    }
    return value;
  }
}
