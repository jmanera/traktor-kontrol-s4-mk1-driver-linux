#include "UtilsHelper.h"

using namespace std;

vector<string> UtilsHelper::explode(string& string_to_explode,
                              const char& separator)
{
  string next;
  vector<string> result;
  for (char it : string_to_explode) {
    if (it == separator) {
      if (!next.empty()) {
        result.push_back(next);
        next.clear();
      }
    }
    else {
      next += it;
    }
  }
  if (!next.empty())
    result.push_back(next);
  return result;
}

void UtilsHelper::capture_signals()
{
  signal(SIGTERM,shutdown_application);
  signal(SIGKILL,shutdown_application);
  signal(SIGSEGV,shutdown_application);
  signal(SIGABRT,shutdown_application);
  signal(SIGQUIT,shutdown_application);
  signal(SIGTSTP,shutdown_application);
  signal(SIGINT,shutdown_application);
}

void UtilsHelper::shutdown_application(int signum)
{
  spdlog::info("[main::shutdown_application] Shutting down application....");
  EvDevHelper::shutdown_buttons_leds();
  spdlog::info("[main::shutdown_application] Exiting application with code: {0}", signum);
  spdlog::info("[main::shutdown_application] Good bye!");
  exit(signum);
}

std::vector<unsigned char> UtilsHelper::create_message(bool shift1, bool shift2, bool toggle_ac, bool toggle_bd, MidiEventOut *midi_event, unsigned char value)
{
  int channel, status;
  channel = status = -1;
  std::vector<unsigned char> message;

  if ((midi_event->channel_byte == 0xb0) || (midi_event->channel_byte == 0xb2)) {
    if (shift1 && !toggle_ac) {
      channel = midi_event->tgl_off_shf_on_channel_byte;
      status = midi_event->tgl_off_shf_on_status_byte;
    }
    if (!shift1 && toggle_ac) {
      channel = midi_event->tgl_on_shf_off_channel_byte;
      status = midi_event->tgl_on_shf_off_status_byte;
    }
    if (shift1 && toggle_ac) {
      channel = midi_event->tgl_on_shf_on_channel_byte;
      status = midi_event->tgl_on_shf_on_status_byte;
    }
  }
  else if ((midi_event->channel_byte == 0xb1) || (midi_event->channel_byte == 0xb3)){
    if (shift2 == true && toggle_bd == false){
      channel = midi_event->tgl_off_shf_on_channel_byte;
      status = midi_event->tgl_off_shf_on_status_byte;
    }
    if (shift2 == false && toggle_bd == true){
      channel = midi_event->tgl_on_shf_off_channel_byte;
      status = midi_event->tgl_on_shf_off_status_byte;
    }
    if (shift2 == true && toggle_bd == true){
      channel = midi_event->tgl_on_shf_on_channel_byte;
      status = midi_event->tgl_on_shf_on_status_byte;
    }
  }

  if ((int)channel == -1){
    channel = midi_event->channel_byte;
  }

  if ((int)status == -1){
    status = midi_event->status_byte;
  }

  message.push_back(channel);
  message.push_back(status);
  message.push_back(value);

  return message;
}

bool UtilsHelper::show_beat_loop_display(unsigned char channel, unsigned char status, unsigned char value, int traktor_device_id, ConfigHelper *config)
{
  auto sharedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(config->get_string_value("midi_in_callback_log_file"));
  auto logger = std::make_shared<spdlog::logger>(config->get_string_value("midi_in_callback_logger_name"), sharedFileSink);
  logger->set_level(spdlog::level::debug);

  int beat_equivalences[12] = {32, 16, 8, 4, 2, 1, 2, 4, 8, 16, 32, 64};
  int loop_value = beat_equivalences[value];
  int units = (int)loop_value % 10;
  int tens = ((int)loop_value / 10) %10;
  int segments_to_show_units[Led::total_segments], segments_to_show_tens[Led::total_segments];
  logger->debug("[MidiHelper::midi_in_callback] Beat loop size received, Channel: {0} Status: {1} Value: {2} Loop Value: {3}", channel, status, value, loop_value);
  if ((channel == 0xb0) || (channel == 0xb2)){
    if (value <= 0x4){
      AlsaHelper::set_led_value(traktor_device_id, Led::ch1_digit1_led_dot, Led::ON);
    }
    else{
      AlsaHelper::set_led_value(traktor_device_id, Led::ch1_digit1_led_dot, Led::OFF);
    }
    for (int i = Led::ch1_digit1_led_numbers[0]; i <= Led::ch1_digit2_led_numbers[Led::total_segments - 1]; i++){
      if (i != Led::ch1_digit1_led_dot)
        AlsaHelper::set_led_value(traktor_device_id, i, Led::OFF);
    }
    for (int i = 0; i < Led::total_segments; i++){
      segments_to_show_units[i] = Led::numbers[units][i] * Led::ch1_digit2_led_numbers[i];
    }
    for (int i = 0; i < Led::total_segments; i++){
      segments_to_show_tens[i] = Led::numbers[tens][i] * Led::ch1_digit1_led_numbers[i];
    }
  }
  else if ((channel == 0xb1) || (channel == 0xb3)){
    if (value <= 0x4){
      AlsaHelper::set_led_value(traktor_device_id, Led::ch2_digit1_led_dot, Led::ON);
    }
    else{
      AlsaHelper::set_led_value(traktor_device_id, Led::ch2_digit1_led_dot, Led::OFF);
    }
    for (int i = Led::ch2_digit1_led_numbers[0]; i <= Led::ch2_digit2_led_numbers[Led::total_segments - 1]; i++){
      if (i != Led::ch2_digit1_led_dot)
        AlsaHelper::set_led_value(traktor_device_id, i, Led::OFF);
    }
    for (int i = 0; i < Led::total_segments; i++){
      segments_to_show_units[i] = Led::numbers[units][i] * Led::ch2_digit2_led_numbers[i];
    }
    for (int i = 0; i < Led::total_segments; i++){
      segments_to_show_tens[i] = Led::numbers[tens][i] * Led::ch2_digit1_led_numbers[i];
    }
  }

  AlsaHelper::bulk_led_value(traktor_device_id, segments_to_show_units, Led::ON, Led::total_segments);
  AlsaHelper::bulk_led_value(traktor_device_id, segments_to_show_tens, Led::ON, Led::total_segments);
  return true;
}

bool UtilsHelper::show_vumeters_leds(unsigned char value, int traktor_device_id, string control_id)
{
  int full_brightness = 0;
  vector<string> control_array = UtilsHelper::explode(control_id, ' ');
  if (value > 1){
    int light = value - 1;
    full_brightness = floor(light / 21);
    int partial = light % 21;
    for (int i = 0; i < full_brightness; i++){
      AlsaHelper::set_led_value(traktor_device_id, stoi(control_array[i]), Led::ON);
    }
    if (partial > 0){
      int alsa_values[20] = {1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26, 28, 29};
      AlsaHelper::set_led_value(traktor_device_id, stoi(control_array[full_brightness]), alsa_values[partial - 1]);
    }
  }
  else if (value == 1){
    AlsaHelper::set_led_value(traktor_device_id, stoi(control_array[0]), Led::MIDDLE);
  }
  else{
    AlsaHelper::set_led_value(traktor_device_id, stoi(control_array[0]), Led::OFF);
  }
  return true;
}

bool UtilsHelper::show_static_leds(unsigned char value, int traktor_device_id, string control_id)
{
  int control_id_num = stoi(control_id);
  if ((control_id_num >= 1) && (control_id_num <= 163)){
    if (value >= 1)
      AlsaHelper::set_led_value(traktor_device_id, control_id_num, Led::ON);
    else{
      if ((control_id_num != 24) && (control_id_num != 37) && (control_id_num != 50) && (control_id_num != 63)){
        AlsaHelper::set_led_value(traktor_device_id, control_id_num, Led::MIDDLE);
      }
      else{
        AlsaHelper::set_led_value(traktor_device_id, control_id_num, Led::OFF);
      }
    }
  }

  return true;
}