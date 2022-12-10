#include "MidiHelper.h"
#include "MidiEventIn.h"

using namespace std;

class callbackData{
 public:
  int traktor_device_id = 0;
};

MidiHelper::MidiHelper(){
    try {
        traktor_device_id = AlsaHelper::get_traktor_device();
        if (traktor_device_id == -1){
          spdlog::error("[EvDevHelper:EvDevHelper] Traktor Kontrol S4 Device not found.... Bye!");
          exit(EXIT_FAILURE);
        }
        pMidiIn = new RtMidiIn(RtMidi::UNSPECIFIED, "TRAKTORS4", 50);
        pMidiIn->openVirtualPort("Traktor Kontrol S4 MK1");


        callbackData *data = new callbackData();
        data->traktor_device_id = traktor_device_id;

        pMidiIn->setErrorCallback(reinterpret_cast<RtMidiErrorCallback>(midi_in_error_callback));
        pMidiIn->setCallback(reinterpret_cast<RtMidiIn::RtMidiCallback>(midi_in_callback), (void *) data);

        pMidiOut = new RtMidiOut(RtMidi::UNSPECIFIED,"TRAKTORS4");
        pMidiOut->openVirtualPort("Traktor Kontrol S4 MK1");
    }
    catch ( RtMidiError &error ) {
        spdlog::error("[RtMidiHelper::RtMidiHelper] RtMidi Error: {0}", error.getMessage());
        exit(EXIT_FAILURE);
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
    unsigned char channel = message->at(0);
    unsigned char status = message->at(1);
    unsigned char value = message->at(2);

    try {
      auto it = MidiEventIn::midi_in_mapping.find(status);
      callbackData* data = reinterpret_cast<callbackData *>(userData);
      if ((it != MidiEventIn::midi_in_mapping.end()) && (channel >= 0xb0) && (channel <= 0xb4)){
        string control_id = it->second->check_channel_value(channel);
        int full_brightness = 0;
        if ((control_id != "-") && control_id.length() > 3){
          vector<string> control_array = explode(control_id, ' ');
          if (value > 1){
            int light = value - 1;
            full_brightness = floor(light / 21);
            int partial = light % 21;
            for (int i = 0; i < full_brightness; i++){
              AlsaHelper::set_led_value(data->traktor_device_id, stoi(control_array[i]), Led::ON);
            }
            if (partial > 0){
              int alsa_values[20] = {1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26, 28, 29};
              AlsaHelper::set_led_value(data->traktor_device_id, stoi(control_array[full_brightness]), alsa_values[partial - 1]);
            }
          }
          else if (value == 1){
            AlsaHelper::set_led_value(data->traktor_device_id, stoi(control_array[0]), Led::MIDDLE);
          }
          else{
            AlsaHelper::set_led_value(data->traktor_device_id, stoi(control_array[0]), Led::OFF);
          }
        }
      }
    }
    catch (...){ }
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

  for (string::const_iterator it = str.begin(); it != str.end(); it++) {
    if (*it == ch) {
      if (!next.empty()) {
        result.push_back(next);
        next.clear();
      }
    }
    else {
      next += *it;
    }
  }
  if (!next.empty())
    result.push_back(next);
  return result;
}