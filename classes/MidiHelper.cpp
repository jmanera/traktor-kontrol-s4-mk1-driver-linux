#include "MidiHelper.h"

using namespace std;

class callbackData{
 public:
  int traktor_device_id = 0;
  bool shift_ch1 = false;
  bool shift_ch2 = false;
  bool toggle_ac = false;
  bool toggle_bd = false;
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
  auto sharedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("/tmp/traktor_kontrol_s4_logger_error.log");
  auto logger = std::make_shared<spdlog::logger>("traktor_kontrol_s4_logger_error", sharedFileSink);
  logger->info("[MidiHelper::midi_in_error_callback] Error MIDI Error callback.... {0}", error_message);
  return NULL;
}

RtMidiIn::RtMidiCallback MidiHelper::midi_in_callback(double deltatime, std::vector<unsigned char> *message, void *userData){
    callbackData* data = reinterpret_cast<callbackData *>(userData);
    unsigned char channel = message->at(0);
    unsigned char status = message->at(1);
    unsigned char value = message->at(2);

    auto sharedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("/tmp/traktor_kontrol_s4_logger.log");
    auto logger = std::make_shared<spdlog::logger>("traktor_kontrol_s4_logger", sharedFileSink);
    logger->set_level(spdlog::level::debug);
    logger->debug("[MidiHelper::midi_in_callback] MIDI In callback received with message: Channel: {0} Status: {1} Value: {2}", channel, status, value);

    try {
      auto it = MidiEventIn::midi_in_mapping.find(status);
      if ((it != MidiEventIn::midi_in_mapping.end()) && (channel >= 0xb0) && (channel <= 0xb4)){
        if ((status == 0x50) && (value >= 0x0) && (value <= 0xc)){
          int beat_equivalences[12] = {32, 16, 8, 4, 2, 1, 2, 4, 8, 16, 32, 64};
          int loop_value = beat_equivalences[value];
          int units = (int)loop_value % 10;
          int tens = ((int)loop_value / 10) %10;
          int segments_to_show_units[7], segments_to_show_tens[7];
          logger->debug("[MidiHelper::midi_in_callback] Beat loop size received, Channel: {0} Status: {1} Value: {2} Loop Value: {3}", channel, status, value, loop_value);
          if ((channel == 0xb0) || (channel == 0xb2)){
            if (value <= 0x4){
              AlsaHelper::set_led_value(data->traktor_device_id, Led::ch1_digit1_led_dot, Led::ON);
            }
            else{
              AlsaHelper::set_led_value(data->traktor_device_id, Led::ch1_digit1_led_dot, Led::OFF);
            }
            for (int i = Led::ch1_digit1_led_numbers[0]; i <= Led::ch1_digit2_led_numbers[Led::total_segments - 1]; i++){
              if (i != Led::ch1_digit1_led_dot)
                AlsaHelper::set_led_value(data->traktor_device_id, i, Led::OFF);
            }
            for (int i = 0; i < 7; i++){
              segments_to_show_units[i] = Led::numbers[units][i] * Led::ch1_digit2_led_numbers[i];
            }
            for (int i = 0; i < 7; i++){
              segments_to_show_tens[i] = Led::numbers[tens][i] * Led::ch1_digit1_led_numbers[i];
            }
          }
          else if ((channel == 0xb1) || (channel == 0xb3)){
            if (value <= 0x4){
              AlsaHelper::set_led_value(data->traktor_device_id, Led::ch2_digit1_led_dot, Led::ON);
            }
            else{
              AlsaHelper::set_led_value(data->traktor_device_id, Led::ch2_digit1_led_dot, Led::OFF);
            }
            for (int i = Led::ch2_digit1_led_numbers[0]; i <= Led::ch2_digit2_led_numbers[Led::total_segments - 1]; i++){
              if (i != Led::ch2_digit1_led_dot)
                AlsaHelper::set_led_value(data->traktor_device_id, i, Led::OFF);
            }
            for (int i = 0; i < Led::total_segments; i++){
              segments_to_show_units[i] = Led::numbers[units][i] * Led::ch2_digit2_led_numbers[i];
            }
            for (int i = 0; i < Led::total_segments; i++){
              segments_to_show_tens[i] = Led::numbers[tens][i] * Led::ch2_digit1_led_numbers[i];
            }
          }

          logger->debug("[MidiHelper::midi_in_callback] Beat loop size received, Channel: {0} Units Values: {1} {2} {3} {4} {5} {6} {7}", channel, segments_to_show_units[0], segments_to_show_units[1], segments_to_show_units[2], segments_to_show_units[3], segments_to_show_units[4], segments_to_show_units[5], segments_to_show_units[6]);
          logger->debug("[MidiHelper::midi_in_callback] Beat loop size received, Channel: {0} Tens Values:  {1} {2} {3} {4} {5} {6} {7}", channel, segments_to_show_tens[0], segments_to_show_tens[1], segments_to_show_tens[2], segments_to_show_tens[3], segments_to_show_tens[4], segments_to_show_tens[5], segments_to_show_tens[6]);
          AlsaHelper::bulk_led_value(data->traktor_device_id, segments_to_show_units, Led::ON, Led::total_segments);
          AlsaHelper::bulk_led_value(data->traktor_device_id, segments_to_show_tens, Led::ON, Led::total_segments);
          return NULL;
        }
        string control_id = it->second->check_channel_value(channel);
        int full_brightness = 0;
        if ((control_id != "-") && control_id.length() > 3){
          vector<string> control_array = UtilsHelper::explode(control_id, ' ');
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
        else{
          if (control_id != "-"){
            int control_id_num = stoi(control_id);
            if ((control_id_num >= 1) && (control_id_num <= 163)){
              if (value >= 1)
                AlsaHelper::set_led_value(data->traktor_device_id, control_id_num, Led::ON);
              else
                AlsaHelper::set_led_value(data->traktor_device_id, control_id_num, Led::MIDDLE);
            }
          }
        }
      }
    }
    catch (...){ }
  logger->info("[MidiHelper::midi_in_callback] MIDI In callback performed");
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
