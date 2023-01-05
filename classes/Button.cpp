#include "Button.h"

#include <utility>

map<int, Button *> Button::buttons_mapping = {
        { 256, new Button(256, "CH1_HOT-CUE_1", 66, 1, 0) },
        { 257, new Button(257, "CH1_SHIFT", 78, 1, 0) },
        { 258, new Button(258, "CH1_HOT-CUE_2", 68, 1, 0) },
        { 259, new Button(259, "CH1_SYN", 79, 1, 0) },
        { 260, new Button(260, "CH1_HOT-CUE_3", 70, 1, 0) },
        { 261, new Button(261, "CH1_CUE", 80, 1, 0) },
        { 262, new Button(262, "CH1_HOT-CUE_4", 72, 1, 0) },
        { 263, new Button(263, "CH1_PLAY", 81, 1, 0) },
        { 264, new Button(264, "CH1_CHANGE_DECK", 86, 1, 0) },
        { 265, new Button(265, "CH1_SAMPLE_1", 90, 1, 0) },
        { 266, new Button(266, "CH1_LOOP_IN", 76, 1, 0) },
        { 267, new Button(267, "CH1_SAMPLE_2", 91, 1, 0) },
        { 268, new Button(268, "CH1_LOOP_OUT", 77, 1, 0) },
        { 269, new Button(269, "CH1_SAMPLE_3", 92, 1, 0) },
        { 270, new Button(270, "CH1_LOAD", 74, 1, 0) },
        { 271, new Button(271, "CH1_SAMPLE_4", 93, 1, 0) },
        { 272, new Button(272, "CH1_TEMPO_UP", 82, 1, 0) },
        { 273, new Button(273, "CH1_TEMPO_DOWN", 83, 1, 0) },
        { 274, new Button(274, "CH1_KNOB_LOOP", -1, 1, 0) },
        { 275, new Button(275, "CH1_KNOB_TEMPO", -1, 1, 0) },
        { 280, new Button(280, "LOOP_EDITOR_SIZE", 9, 5, 0) },
        { 282, new Button(282, "LOOP_EDITOR_UNDO", 13, 5, 0) },
        { 281, new Button(281, "LOOP_EDITOR_REC", 8, 5, 0) },
        { 283, new Button(283, "LOOP_EDITOR_PLAY", 122, 5, 0) },
        { 284, new Button(284, "BROWSE", 11, 5, 0) },
        { 285, new Button(285, "SNAP", 7, 5, 0) },
        { 286, new Button(286, "MASTER", 6, 5, 0) },
        { 287, new Button(287, "QUANT", 10, 5, 0) },
        { 288, new Button(288, "CH3_EARPHONES", 50, 3, 0) },
        { 289, new Button(289, "CH1_EARPHONES", 24, 1, 0) },
        { 290, new Button(290, "CH2_EARPHONES", 37, 2, 0) },
        { 291, new Button(291, "CH4_EARPHONES", 63, 4, 0) },
        { 292, new Button(292, "BROWSE_KNOB", -1, 5, 0) },
        { 296, new Button(296, "CH2_TEMPO_UP", 126, 2, 0) },
        { 297, new Button(297, "CH2_TEMPO_DOWN", 127, 2, 0) },
        { 298, new Button(298, "CH2_KNOB_LOOP", -1, 2, 0) },
        { 299, new Button(299, "CH2_KNOB_TEMPO", -1, 2, 0) },
        { 304, new Button(304, "CH2_CHANGE_DECK", 130, 2, 0) },
        { 305, new Button(305, "CH2_SAMPLE_1", 134, 2, 0) },
        { 306, new Button(306, "CH2_LOOP_IN", 120, 2, 0) },
        { 307, new Button(307, "CH2_SAMPLE_2", 135, 2, 0) },
        { 308, new Button(308, "CH2_LOOP_OUT", 121, 2, 0) },
        { 309, new Button(309, "CH2_SAMPLE_3", 136, 2, 0) },
        { 310, new Button(310, "CH2_LOAD", 118, 2, 0) },
        { 311, new Button(311, "CH2_SAMPLE_4", 137, 2, 0) },
        { 312, new Button(312, "CH2_HOT-CUE_1", 110, 2, 0) },
        { 313, new Button(313, "CH2_SHIFT", 122, 2, 0) },
        { 314, new Button(314, "CH2_HOT-CUE_2", 112, 2, 0) },
        { 315, new Button(315, "CH2_SYNC", 123, 2, 0) },
        { 316, new Button(316, "CH2_HOT-CUE_3", 114, 2, 0) },
        { 317, new Button(317, "CH2_CUE", 124, 2, 0) },
        { 318, new Button(318, "CH2_HOT-CUE_4", 116, 2, 0) },
        { 319, new Button(319, "CH2_PLAY", 125, 2, 0) },
        { 321, new Button(321, "FX1_DRY_WET", 154, 1, 0) },
        { 322, new Button(322, "FX1_1", 155, 1, 0) },
        { 323, new Button(323, "FX1_2", 156, 1, 0) },
        { 324, new Button(324, "FX1_3", 157, 1, 0) },
        { 325, new Button(325, "FX1_MODE", 158, 1, 0) },
        { 326, new Button(326, "CH3_GAIN", -1, 3, 0) },
        { 327, new Button(327, "CH1_GAIN", -1, 1, 1) },
        { 328, new Button(328, "BTN_TOOL_QUINT_TAP", -1, 1, 0) },
        { 329, new Button(329, "BTN_STYLUS3", -1, 1, 0) },
        { 330, new Button(330, "CH1_FX_CHANNEL_1", 25, 1, 0) },
        { 331, new Button(331, "CH1_FX_CHANNEL_2", 26, 1, 0) },
        { 332, new Button(332, "CH2_FX_CHANNEL_1", 38, 2, 0) },
        { 333, new Button(333, "CH2_FX_CHANNEL_2", 39, 2, 0) },
        { 334, new Button(334, "BTN_TOOL_TRIPLE_TAP", 39, 2, 0) },
        { 335, new Button(335, "BTN_TOOL_QUAD_TAP", 39, 2, 0) },
        { 345, new Button(345, "FX2_DRY_WET", 159, 2, 0) },
        { 346, new Button(346, "FX2_1", 160, 2, 0) },
        { 347, new Button(347, "FX2_2", 161, 2, 0) },
        { 348, new Button(348, "FX2_3", 162, 2, 0) },
        { 349, new Button(349, "FX2_MODE", 163, 2, 0) },
        { 350, new Button(350, "CH2_GAIN", -1, 2, 0)},
        { 351, new Button(351, "CH4_GAIN", -1, 4, 0)}
};

Button::Button(int in_code, string in_name, int in_led, int in_channel, int in_value = 0) {
  code = in_code;
  name = std::move(in_name);
  led_code = in_led;
  channel = in_channel;
  value = 0;
}

Button::Button(){
  code = led_code = channel = value = -1;
  name = "";
}

int Button::handle_event(RtMidiOut *midi_out, int controller_id, bool shift_ch1, bool shift_ch2, bool toggle_ac, bool toggle_bd, ConfigHelper *config_helper){
  shared_ptr<spdlog::logger> logger = spdlog::get(config_helper->get_string_value("traktor_s4_logger_name"));
  if (MidiEventOut::midi_mapping.find(code) != MidiEventOut::midi_mapping.end()) {
    MidiEventOut *midi_event = MidiEventOut::midi_mapping[code];
    logger->debug("[Button::handle_event] Button named {0} performed with Code:{1} Led Code: {2} Channel: {3} Value: {4}", name, code, led_code, channel, value);
    logger->debug("[Button::handle_event] Sending to MIDI with: Name: {0} Controller Type: {1} Status: {2} Channel: {3}", midi_event->name, midi_event->controller_type, midi_event->status_byte, midi_event->channel_byte);
    logger->debug("[Button::handle_event] Creating message...");

    auto message = UtilsHelper::create_message(shift_ch1, shift_ch2, toggle_ac, toggle_bd, midi_event, value);

    logger->debug("[Button::handle_event] Message created!");
    logger->debug("[Button::handle_event] Sending to MIDI out port....");

    try{
      midi_out->sendMessage(&message);
    }
    catch (exception &e){
      logger->error("[Button::handle_event] Error sending message to MIDI out port: {0}", e.what());
      return -1;
    }
    logger->debug("[Button::handle_event] Sent!");
  }

  return 0;
}