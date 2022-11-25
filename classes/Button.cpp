//
// Created by aspgems on 21/11/22.
//

#include "Button.h"

Button::Button(int in_code, string in_name, int in_led, int in_channel, int in_value) {
    code = in_code;
    name = in_name;
    led_code = in_led;
    channel = in_channel;
    value = 0;
}

Button::Button(){
    code = led_code = channel = value = -1;
    name = "";
}

map<int, Button *> Button::buttons_mapping = {
        // CHANNEL 1 BUTTONS
        { 256, new Button(256, "CH1_HOTCUE_1", 66, 1) },
        { 257, new Button(257, "CH1_SHIFT", 78, 1) },
        { 258, new Button(258, "CH1_HOTCUE_2", 68, 1) },
        { 259, new Button(259, "CH1_SYN", 79, 1) },
        { 260, new Button(260, "CH1_HOTCUE_3", 70, 1) },
        { 261, new Button(261, "CH1_CUE", 80, 1) },
        { 262, new Button(262, "CH1_HOTCUE_4", 72, 1) },
        { 263, new Button(263, "CH1_PLAY", 81, 1) },
        { 264, new Button(264, "CH1_CHANGE_DECK", 86, 1) },
        { 265, new Button(265, "CH1_SAMPLE_1", 90, 1) },
        { 266, new Button(266, "CH1_LOOP_IN", 76, 1) },
        { 267, new Button(267, "CH1_SAMPLE_2", 91, 1) },
        { 268, new Button(268, "CH1_LOOP_OUT", 77, 1)},
        { 269, new Button(269, "CH1_SAMPLE_3", 92, 1)},
        { 270, new Button(270, "CH1_LOAD", 74, 1)},
        { 271, new Button(271, "CH1_SAMPLE_4", 93, 1)},
        { 272, new Button(272, "CH1_TEMPO_UP", 82, 1)},
        { 273, new Button(273, "CH1_TEMPO_DOWN", 83, 1)},
        { 274, new Button(274, "CH1_KNOB_LOOP", -1, 1)},
        { 275, new Button(275, "CH1_KNOB_TEMPO", -1, 1)},

        { 280, new Button(280, "LOOP_EDITOR_SIZE", 9, 2) },
        { 282, new Button(282, "LOOP_EDITOR_UNDO", 13, 2) },
        { 281, new Button(281, "LOOP_EDITOR_REC", 8, 2) },

        { 283, new Button(283, "LOOP_EDITOR_PLAY", 122, 2) },
        { 284, new Button(284, "BROWSE", 11, 2) },
        { 285, new Button(285, "SNAP", 7, 2) },
        { 286, new Button(286, "MASTER", 6, 2) },
        { 287, new Button(287, "QUANT", 10, 2) },
        { 289, new Button(289, "CH1_EARPHONES", 24, 1) },
        { 290, new Button(290, "CH2_EARPHONES", 37, 2) },

        { 292, new Button(292, "BROWSE_KNOB", -1, 2) },

        { 296, new Button(296, "CH2_TEMPO_UP", 126, 2)},
        { 297, new Button(297, "CH2_TEMPO_DOWN", 127, 2) },
        { 298, new Button(298, "CH2_KNOB_LOOP", -1, 2) },
        { 299, new Button(299, "CH2_KNOB_TEMPO", -1, 2) },

        { 304, new Button(304, "CH2_CHANGE_DECK", 130, 2)},
        { 305, new Button(305, "CH2_SAMPLE_1", 134, 2)},
        { 306, new Button(306, "CH2_LOOP_IN", 120, 2)},
        { 307, new Button(307, "CH2_SAMPLE_2", 135, 2)},
        { 308, new Button(308, "CH2_LOOP_OUT", 121, 2)},
        { 309, new Button(309, "CH2_SAMPLE_3", 136, 2)},
        { 310, new Button(310, "CH2_LOAD", 118, 2)},
        { 311, new Button(311, "CH2_SAMPLE_4", 137, 2)},
        { 312, new Button(312, "CH2_HOTCUE_1", 110, 2)},
        { 313, new Button(313, "CH2_SHIFT", 122, 2)},
        { 314, new Button(314, "CH2_HOTCUE_2", 112, 2)},
        { 315, new Button(315, "CH2_SYNC", 123, 2)},
        { 316, new Button(316, "CH2_HOTCUE_3", 114, 2)},
        { 317, new Button(317, "CH2_CUE", 124, 2)},
        { 318, new Button(318, "CH2_HOTCUE_4", 116, 2)},
        { 319, new Button(319, "CH2_PLAY", 125, 2)},

        { 321, new Button(321, "FX1_DRY_WET", 154, 1)},
        { 322, new Button(322, "FX1_1", 155, 1)},
        { 323, new Button(323, "FX1_2", 156, 1)},
        { 324, new Button(324, "FX1_3", 157, 1)},
        { 325, new Button(325, "FX1_MODE", 158, 1)},

        { 327, new Button(327, "CH1_GAIN", -1, 1) },

        { 330, new Button(330, "CH1_FX_CHANNEL_1", 25, 1)},
        { 331, new Button(331, "CH1_FX_CHANNEL_2", 26, 1)},
        { 332, new Button(332, "CH2_FX_CHANNEL_1", 38, 2)},
        { 333, new Button(333, "CH2_FX_CHANNEL_2", 39, 2)},

        { 345, new Button(345, "FX2_DRY_WET", 159, 2) },
        { 346, new Button(346, "FX2_1", 160, 2) },
        { 347, new Button(347, "FX2_2", 161, 2) },
        { 348, new Button(348, "FX2_3", 162, 2) },
        { 349, new Button(349, "FX2_MODE", 163, 2) },
        { 350, new Button(296, "CH2_GAIN", -1, 2 )}
};

int Button::handle_event(){

    int output_value = Led::MIDDLE;
    if (value == 1){
        output_value = Led::ON;
    }

    string cmd = "amixer -c TraktorKontrolS cset numid=" + to_string(led_code) + " " + to_string(output_value) + " > /dev/null";
    return system(cmd.c_str());
}