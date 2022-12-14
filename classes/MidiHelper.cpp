#include "MidiHelper.h"

using namespace std;

class callbackData{
 public:
  int traktor_device_id = 0;
  bool shift_ch1 = false;
  bool shift_ch2 = false;
  bool toggle_ac = false;
  bool toggle_bd = false;
  ConfigHelper *config{};
};

MidiHelper::MidiHelper(ConfigHelper *config){
  this->config_helper = config;
  try {
      traktor_device_id = AlsaHelper::get_traktor_device(config);
      if (traktor_device_id == -1){
        spdlog::error("[EvDevHelper:EvDevHelper] Traktor Kontrol S4 Device not found.... Bye!");
        exit(EXIT_FAILURE);
      }
      pMidiIn = new RtMidiIn(RtMidi::UNSPECIFIED, config->get_string_value("midi_client_name"), config->get_int_value("midi_queue_size_limit"));
      pMidiIn->openVirtualPort(config->get_string_value("midi_virtual_port_name"));


      auto *data = new callbackData();
      data->traktor_device_id = traktor_device_id;
      data->config = config;

      pMidiIn->setErrorCallback(reinterpret_cast<RtMidiErrorCallback>(midi_in_error_callback), (void *) data);
      pMidiIn->setCallback(reinterpret_cast<RtMidiIn::RtMidiCallback>(midi_in_callback), (void *) data);

      pMidiOut = new RtMidiOut(RtMidi::UNSPECIFIED, config->get_string_value("midi_client_name"));
      pMidiOut->openVirtualPort(config->get_string_value("midi_virtual_port_name"));
  }
  catch ( RtMidiError &error ) {
      spdlog::error("[RtMidiHelper::RtMidiHelper] RtMidi Error: {0}", error.getMessage());
      exit(EXIT_FAILURE);
  }
}

[[maybe_unused]] bool MidiHelper::close_input_port() const
{
    pMidiIn->closePort();
    return true;
}

[[maybe_unused]] bool MidiHelper::close_output_port() const
{
    pMidiOut->closePort();
    return true;
}

RtMidiErrorCallback MidiHelper::midi_in_error_callback(RtMidiError::Type type, const string &error_message, void *userData){
  auto* data = reinterpret_cast<callbackData *>(userData);
  auto sharedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(data->config->get_string_value("midi_out_callback_error_log_file"));
  auto logger = std::make_shared<spdlog::logger>(data->config->get_string_value("midi_out_callback_error_logger_name"), sharedFileSink);
  logger->info("[MidiHelper::midi_in_error_callback] Error MIDI Error callback.... {0}", error_message);
  return nullptr;
}

RtMidiIn::RtMidiCallback MidiHelper::midi_in_callback(double deltatime, std::vector<unsigned char> *message, void *userData){
    auto* data = reinterpret_cast<callbackData *>(userData);
    unsigned char channel = message->at(0);
    unsigned char status = message->at(1);
    unsigned char value = message->at(2);

    auto sharedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(data->config->get_string_value("midi_in_callback_log_file"));
    auto logger = std::make_shared<spdlog::logger>(data->config->get_string_value("midi_in_callback_logger_name"), sharedFileSink);
    logger->set_level(spdlog::level::debug);
    logger->debug("[MidiHelper::midi_in_callback] MIDI In callback received with message: Channel: {0} Status: {1} Value: {2}", channel, status, value);

    try {
      auto it = MidiEventIn::midi_in_mapping.find((char)status);
      if ((it != MidiEventIn::midi_in_mapping.end()) && (channel >= 0xb0) && (channel <= 0xb4)){
        if ((status == 0x50) && (value >= 0x0) && (value <= 0xc)){
          if (!UtilsHelper::show_beat_loop_display(channel, status, value, data->traktor_device_id, data->config))
          {
            logger->debug("[MidiHelper::midi_in_callback] Error processing beat loop size: Channel: {0} Status: {1} Value: {2}", channel, status, value);
            return nullptr;
          }
          logger->debug("[MidiHelper::midi_in_callback] MIDI In callback performed");
          return nullptr;
        }
        string control_id = it->second->check_channel_value(channel);

        if ((control_id != "-") && control_id.length() > 3){
          if (!UtilsHelper::show_vumeters_leds(value, data->traktor_device_id, control_id)){
            logger->debug("[MidiHelper::midi_in_callback] Error processing vu meters: Channel: {0} Status: {1} Value: {2}", channel, status, value);
            return nullptr;
          }
          logger->debug("[MidiHelper::midi_in_callback] MIDI In callback performed");
          return nullptr;
        }
        else{
          if (control_id != "-"){
            if (!UtilsHelper::show_static_leds(value, data->traktor_device_id, control_id)){
              logger->debug("[MidiHelper::midi_in_callback] Error processing static Led: Channel: {0} Status: {1} Value: {2}", channel, status, value);
              return nullptr;
            }
            logger->debug("[MidiHelper::midi_in_callback] MIDI In callback performed");
            return nullptr;
          }
        }
      }
    }
    catch (...){ }
  logger->debug("[MidiHelper::midi_in_callback] MIDI In callback performed");
  return nullptr;
}

void MidiHelper::show_midi_information(MidiHelper *midi_helper){
    unsigned int nPorts = midi_helper->pMidiIn->getPortCount();
    spdlog::debug("[RtMidiHelper::show_midi_information] There are {0} MIDI input sources available", nPorts);
    std::string portName;
    for ( unsigned int i=0; i < nPorts; i++ ) {
        try {
            portName = midi_helper->pMidiIn->getPortName(i);
        }
        catch ( RtMidiError &error ) {
            spdlog::error("[RtMidiHelper::show_midi_information] {0}", error.getMessage());
        }
        spdlog::debug("[RtMidiHelper::show_midi_information]    Input Port #{0}: {1}", i+1, portName);
    }

    nPorts = midi_helper->pMidiOut->getPortCount();
    spdlog::debug("[RtMidiHelper::show_midi_information] There are {0} MIDI output sources available", nPorts);
    for ( unsigned int i=0; i < nPorts; i++ ) {
        try {
            portName = midi_helper->pMidiOut->getPortName(i);
        }
        catch (RtMidiError &error) {
            spdlog::error("[RtMidiHelper::show_midi_information] {0}", error.getMessage());
        }
        spdlog::debug("[RtMidiHelper::show_midi_information]    Output Port #{0}: {1}", i+1, portName);
    }
}
