#include "MidiHelper.h"
#include "MidiEventIn.h"

using namespace std;

MidiHelper::MidiHelper(){
    try {
        pMidiIn = new RtMidiIn(RtMidi::UNSPECIFIED, "TRAKTORS4", 50);
        pMidiIn->openVirtualPort("Traktor Kontrol S4 MK1");

        pMidiIn->setErrorCallback(reinterpret_cast<RtMidiErrorCallback>(midi_in_error_callback));
        pMidiIn->setCallback(reinterpret_cast<RtMidiIn::RtMidiCallback>(midi_in_callback));

        pMidiOut = new RtMidiOut(RtMidi::UNSPECIFIED,"TRAKTORS4");
        pMidiOut->openVirtualPort("Traktor Kontrol S4 MK1");
    }
    catch ( RtMidiError &error ) {
        spdlog::error("[RtMidiHelper::RtMidiHelper] RtMidi Error: {0}", error.getMessage());
        exit( EXIT_FAILURE );
    }
}

bool MidiHelper::close_input_port()
{
    pMidiIn->closePort();
    return true;
}

bool MidiHelper::close_output_port()
{
    pMidiOut->closePort();
    return true;
}
RtMidiErrorCallback MidiHelper::midi_in_error_callback(RtMidiError::Type type, const string &error_message, void *userData){
  spdlog::info("[MidiHelper::midi_in_error_callback] Error MIDI In callback.... {0}", error_message);
  return NULL;
}
RtMidiIn::RtMidiCallback MidiHelper::midi_in_callback(double deltatime, std::vector<unsigned char> *message, void *userData){
    auto nBytes = message->size();
    //spdlog::info("[MidiHelper::midi_in_callback] Performing MIDI In callback....");

    unsigned char channel = message->at(0);
    unsigned char status = message->at(1);
    unsigned char value = message->at(2);
    double timestamp = deltatime;
    try {
      auto it = MidiEventIn::midi_in_mapping.find(status);
      if ((it != MidiEventIn::midi_in_mapping.end()) && (channel >= 0xb0) && (channel <= 0xb4)){
        //spdlog::info("[MidiHelper::midi_in_callback] Found event in MIDI In Mapping");
        unsigned char channel_map = channel & 0x4f;
        MidiEventIn * control_data = it->second;
        //spdlog::info("[MidiHelper::midi_in_callback] Control received: {0} {1} {2} {3}", atoi(control_data->control_channel_1), atoi(control_data->control_channel_2), atoi(control_data->control_channel_3), atoi(control_data->control_channel_4), atoi(control_data->control_channel_5));


        string control_id = it->second->check_channel_value(channel);
        if ((control_id != "-") && control_id.length() > 3){
          int full_brightness = 0;
          int i = 0;
          vector<string> control_array = explode(control_id, ' ');

          if (value > 1){
            int light = value - 1;
            full_brightness = floor(light / 21);
            int partial = light % 21;

            for (i = 0; i < full_brightness; i++){
              AlsaHelper::set_led_value(2, stoi(control_array[i]), 31);
            }

            if (partial > 0){
              int alsa_values[20] = {1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26, 28, 29};
              AlsaHelper::set_led_value(2, stoi(control_array[full_brightness]), alsa_values[partial - 1]);

            }
            else if (value == 1){
              AlsaHelper::set_led_value(2, stoi(control_array[0]), 1);
            }
          }
          AlsaHelper::set_led_value(2, stoi(control_id), floor(value % 31));
        }
        //cout << "-" << it->second->control_channel_1 << "-" << endl;
      }
    }
    catch (...){ }
    /*
      cout << "NOT FOUND" << endl;
      /*
      if (MidiEventIn::midi_in_mapping.find(status) != MidiEventIn::midi_in_mapping.end()){


        if (control_id != "-"){
          //
          spdlog::info("[MidiHelper::midi_in_callback] Control received {0} Value: {1} Channel: { 2}", control_id, value, channel);
        }
      }*/
/*
    }
    else{
      cout << "FOUND" << endl;
      spdlog::warn("[MidiHelper::midi_in_callback] Error in incoming MIDI Message");
    }*/
/*


      spdlog::info("[MidiHelper::midi_in_callback] Received Channel: {0} Status: {1} Value: {2} Timestamp {3}", channel, status, value, timestamp);
      struct libevdev *dev = NULL;
      int rc = 1;

      tie(rc, dev) = EvDevHelper::get_traktor_controller_device();
    }
    */
  return NULL;
}

void MidiHelper::show_midi_information(MidiHelper *rtmidi_helper){
    unsigned int nPorts = rtmidi_helper->pMidiIn->getPortCount();
    spdlog::debug("[RtMidiHelper::show_midi_information] There are {0} MIDI input sources available", nPorts);
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = rtmidi_helper->pMidiIn->getPortName(i);
        }
        catch ( RtMidiError &error ) {
            spdlog::error("[RtMidiHelper::show_midi_information] {0}", error.getMessage());
        }
        spdlog::debug("[RtMidiHelper::show_midi_information]    Input Port #{0}: {1}", i+1, portName);
    }

    nPorts = rtmidi_helper->pMidiOut->getPortCount();
    spdlog::debug("[RtMidiHelper::show_midi_information] There are {0} MIDI output sources available", nPorts);
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = rtmidi_helper->pMidiOut->getPortName(i);
        }
        catch (RtMidiError &error) {
            spdlog::error("[RtMidiHelper::show_midi_information] {0}", error.getMessage());
        }
        spdlog::debug("[RtMidiHelper::show_midi_information]    Output Port #{0}: {1}", i+1, portName);
    }
}

vector<string> MidiHelper::explode(string& str, const char& ch) {
  string next;
  vector<string> result;

  // For each character in the string
  for (string::const_iterator it = str.begin(); it != str.end(); it++) {
    // If we've hit the terminal character
    if (*it == ch) {
      // If we have some characters accumulated
      if (!next.empty()) {
        // Add them to the result vector
        result.push_back(next);
        next.clear();
      }
    } else {
      // Accumulate the next character into the sequence
      next += *it;
    }
  }
  if (!next.empty())
    result.push_back(next);
  return result;
}