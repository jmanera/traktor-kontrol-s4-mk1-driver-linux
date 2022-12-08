#include "MidiEventOut.h"

MidiEventOut::MidiEventOut(unsigned char in_status_type,
                           unsigned char in_channel_byte,
                           string in_controller_type,
                           string in_name,
                           unsigned char in_tgl_off_shf_on_status_byte = -1,
                           unsigned char in_tgl_off_shf_on_channel_byte = -1,
                           unsigned char in_tgl_on_shf_off_status_byte = -1,
                           unsigned char in_tgl_on_shf_off_channel_byte = -1,
                           unsigned char in_tgl_on_shf_on_status_byte = -1,
                           unsigned char in_tgl_on_shf_on_channel_byte = -1){

    status_byte = in_status_type;
    channel_byte = in_channel_byte;
    controller_type = in_controller_type;

    name = in_name;

    tgl_off_shf_on_status_byte = in_tgl_off_shf_on_status_byte;
    tgl_off_shf_on_channel_byte = in_tgl_off_shf_on_channel_byte;

    tgl_on_shf_off_status_byte = in_tgl_on_shf_off_status_byte;
    tgl_on_shf_off_channel_byte = in_tgl_on_shf_off_channel_byte;

    tgl_on_shf_on_status_byte = in_tgl_on_shf_on_status_byte;
    tgl_on_shf_on_channel_byte = in_tgl_on_shf_on_channel_byte;

}

map<int, MidiEventOut *> MidiEventOut::midi_mapping = {
    { 16, new MidiEventOut(0x45, 0xb3, "POT", "DECK D VOLUME")},
    { 19, new MidiEventOut(0x45, 0xb2, "POT", "DECK C VOLUME")},
    { 17, new MidiEventOut(0x45, 0xb1, "POT", "DECK B VOLUME")},
    { 18, new MidiEventOut(0x45, 0xb0, "POT", "DECK A VOLUME")},
    { 291, new MidiEventOut(0x44, 0xb3, "BTN", "DECK D EARPHONES TOGGLE")},
    { 288, new MidiEventOut(0x44, 0xb2, "BTN", "DECK C EARPHONES TOGGLE")},
    { 290, new MidiEventOut(0x44, 0xb1, "BTN", "DECK B EARPHONES TOGGLE")},
    { 289, new MidiEventOut(0x44, 0xb0, "BTN", "DECK A EARPHONES TOGGLE")},
    { 28, new MidiEventOut(0x43, 0xb3, "POT", "DECK D FILTER")},
    { 44, new MidiEventOut(0x43, 0xb2, "POT", "DECK C FILTER")},
    { 36, new MidiEventOut(0x43, 0xb1, "POT", "DECK B FILTER")},
    { 40, new MidiEventOut(0x43, 0xb0, "POT", "DECK A FILTER")},
    { 29, new MidiEventOut(0x42, 0xb3, "POT", "DECK D LOW")},
    { 45, new MidiEventOut(0x42, 0xb2, "POT", "DECK C LOW")},
    { 37, new MidiEventOut(0x42, 0xb1, "POT", "DECK B LOW")},
    { 41, new MidiEventOut(0x42, 0xb0, "POT", "DECK A LOW")},
    { 30, new MidiEventOut(0x41, 0xb3, "POT", "DECK D MID")},
    { 46, new MidiEventOut(0x41, 0xb2, "POT", "DECK C MID")},
    { 38, new MidiEventOut(0x41, 0xb1, "POT", "DECK B MID")},
    { 42, new MidiEventOut(0x41, 0xb0, "POT", "DECK A MID")},
    { 31, new MidiEventOut(0x40, 0xb3, "POT", "DECK D HI")},
    { 47, new MidiEventOut(0x40, 0xb2, "POT", "DECK C HI")},
    { 39, new MidiEventOut(0x40, 0xb1, "POT", "DECK B HI")},
    { 43, new MidiEventOut(0x40, 0xb0, "POT", "DECK A HI")},
    { 335, new MidiEventOut(0x3f, 0xb3, "POT", "DECK D FX2 ON/OFF")},
    { 329, new MidiEventOut(0x3f, 0xb2, "POT", "DECK C FX2 ON/OFF")},
    { 333, new MidiEventOut(0x3f, 0xb1, "POT", "DECK B FX2 ON/OFF")},
    { 331, new MidiEventOut(0x3f, 0xb0, "POT", "DECK A FX2 ON/OFF")},
    { 334, new MidiEventOut(0x3e, 0xb3, "POT", "DECK D FX1 ON/OFF")},
    { 328, new MidiEventOut(0x3e, 0xb2, "POT", "DECK C FX1 ON/OFF")},
    { 332, new MidiEventOut(0x3e, 0xb1, "POT", "DECK B FX1 ON/OFF")},
    { 330, new MidiEventOut(0x3e, 0xb0, "POT", "DECK A FX1 ON/OFF")},
    { 62, new MidiEventOut(0x3c, 0xb3, "GAIN_ROT", "DECK D GAIN")},
    { 61, new MidiEventOut(0x3c, 0xb2, "GAIN_ROT", "DECK C GAIN")},
    { 60, new MidiEventOut(0x3c, 0xb1, "GAIN_ROT", "DECK B GAIN")},
    { 59, new MidiEventOut(0x3c, 0xb0, "GAIN_ROT", "DECK A GAIN")},
    { 349, new MidiEventOut(0x26, 0xb4, "BTN", "FX2 MENU")},
    { 348, new MidiEventOut(0x25, 0xb4, "BTN", "FX2 EFFECT 3 ON/OFF")},
    { 347, new MidiEventOut(0x24, 0xb4, "BTN", "FX2 EFFECT 2 ON/OFF")},
    { 346, new MidiEventOut(0x23, 0xb4, "BTN", "FX2 EFFECT 1 ON/OFF")},
    { 345, new MidiEventOut(0x22, 0xb4, "BTN", "FX2 UNIT ON/OFF")},
    { 32, new MidiEventOut(0x21, 0xb4, "POT", "FX2 EFFECT KNOB 3")},
    { 33, new MidiEventOut(0x20, 0xb4, "POT", "FX2 EFFECT KNOB 2")},
    { 34, new MidiEventOut(0x1f, 0xb4, "POT", "FX2 EFFECT KNOB 1")},
    { 35, new MidiEventOut(0x1e, 0xb4, "POT", "FX2 DRY / WET")},
    { 325, new MidiEventOut(0x1c, 0xb4, "BTN", "FX1 MENU")},
    { 324, new MidiEventOut(0x1b, 0xb4, "BTN", "FX1 EFFECT 3 ON/OFF")},
    { 323, new MidiEventOut(0x1a, 0xb4, "BTN", "FX1 EFFECT 2 ON/OFF")},
    { 322, new MidiEventOut(0x19, 0xb4, "BTN", "FX1 EFFECT 1 ON/OFF")},
    { 321, new MidiEventOut(0x18, 0xb4, "BTN", "FX1 UNIT ON/OFF")},
    { 48, new MidiEventOut(0x17, 0xb4, "POT", "FX1 EFFECT KNOB 3")},
    { 49, new MidiEventOut(0x16, 0xb4, "POT", "FX1 EFFECT KNOB 2")},
    { 50, new MidiEventOut(0x15, 0xb4, "POT", "FX1 EFFECT KNOB 1")},
    { 51, new MidiEventOut(0x14, 0xb4, "POT", "FX1 DRY / WET")},
    //{ , new MidiEventOut(0x04, 0xb4, "POT", "LOOP REC DRY / WET")},
    { 23, new MidiEventOut(0x0a, 0xb4, "POT", "CROSSFADER")},
    { 284, new MidiEventOut(0x09, 0xb4, "BTN", "BROWSE")},
    { 283, new MidiEventOut(0x08, 0xb4, "BTN", "PLAY")},
    { 281, new MidiEventOut(0x07, 0xb4, "BTN", "REC")},
    { 282, new MidiEventOut(0x06, 0xb4, "BTN", "UNDO")},
    { 280, new MidiEventOut(0x05, 0xb4, "BTN", "SIZE")},
    { 292, new MidiEventOut(0x03, 0xb4, "BTN", "BROWSER KNOB PRESS")},
    { 54, new MidiEventOut(0x02, 0xb4, "BROWSE_ROT", "BROWSER KNOB TURN")},
    { 21, new MidiEventOut(0x4, 0xb0, "POT", "DECK A TEMPO", 0x22, 0xb0, 0x4, 0xb2, 0x22, 0xb2)},
    { 22, new MidiEventOut(0x4, 0xb1, "POT", "DECK B TEMPO", 0x22, 0xb1, 0x4, 0xb3, 0x22, 0xb3)},
    { 26, new MidiEventOut(0x3, 0xb0, "JOG_TOUCH", "DECK A JOG WHEEL PRESS", 0x21, 0xb0, 0x3, 0xb2, 0x21, 0xb2)},
    { 27, new MidiEventOut(0x3, 0xb1, "JOG_TOUCH", "DECK B JOG WHEEL PRESS", 0x21, 0xb1, 0x3, 0xb3, 0x21, 0xb3)},
    { 52, new MidiEventOut(0x2, 0xb0, "JOG_ROT", "DECK A JOG WHEEL TURN", 0x20, 0xb0, 0x2, 0xb2, 0x20, 0xb2)},
    { 53, new MidiEventOut(0x2, 0xb1, "JOG_ROT", "DECK B JOG WHEEL TURN", 0x20, 0xb1, 0x2, 0xb3, 0x20, 0xb3)},
    { 55, new MidiEventOut(0x13, 0xb0, "ROT", "DECK A LOOP MOVE KNOB TURN", 0x31, 0xb0, 0x13, 0xb2, 0x31, 0xb2)},
    { 56, new MidiEventOut(0x15, 0xb0, "ROT", "DECK A LOOP SIZE KNOB TURN", 0x33, 0xb0, 0x15, 0xb2, 0x33, 0xb2)},
    { 57, new MidiEventOut(0x13, 0xb1, "ROT", "DECK B LOOP MOVE KNOB TURN", 0x31, 0xb1, 0x13, 0xb3, 0x31, 0xb2)},
    { 58, new MidiEventOut(0x15, 0xb1, "ROT", "DECK B LOOP SIZE KNOB TURN", 0x33, 0xb1, 0x15, 0xb3, 0x33, 0xb3)},
    { 256, new MidiEventOut(0xb, 0xb0, "BTN", "DECK A HOTCUE 1", 0x29, 0xb0, 0xb, 0xb2, 0x29, 0xb2)},
    { 258, new MidiEventOut(0xc, 0xb0, "BTN", "DECK A HOTCUE 2", 0x2a, 0xb0, 0xc, 0xb2, 0x2a, 0xb2)},
    { 259, new MidiEventOut(0x8, 0xb0, "BTN", "DECK A SYNC", 0x26, 0xb0, 0x8, 0xb2, 0x26, 0xb2)},
    { 260, new MidiEventOut(0xd, 0xb0, "BTN", "DECK A HOTCUE 3", 0x2b, 0xb0, 0xd, 0xb2, 0x2b, 0xb2)},
    { 261, new MidiEventOut(0x9, 0xb0, "BTN", "DECK A CUE", 0x27, 0xb0, 0x9, 0xb2, 0x27, 0xb2)},
    { 262, new MidiEventOut(0xe, 0xb0, "BTN", "DECK A HOTCUE 4", 0x2c, 0xb0, 0xe, 0xb2, 0x2c, 0xb2)},
    { 263, new MidiEventOut(0xa, 0xb0, "BTN", "DECK A PLAY", 0x28, 0xb0, 0xa, 0xb2, 0x28, 0xb2)},
    { 265, new MidiEventOut(0xf, 0xb0, "BTN", "DECK A SAMPLE 1", 0x2d, 0xb0, 0xf, 0xb2, 0x2d, 0xb2)},
    { 266, new MidiEventOut(0x17, 0xb0, "BTN", "DECK A LOOP IN", 0x35, 0xb0, 0x17, 0xb2, 0x35, 0xb2)},
    { 267, new MidiEventOut(0x10, 0xb0, "BTN", "DECK A SAMPLE 2", 0x2e, 0xb0, 0x10, 0xb2, 0x2e, 0xb2)},
    { 268, new MidiEventOut(0x18, 0xb0, "BTN", "DECK A LOOP OUT", 0x36, 0xb0, 0x18, 0xb2, 0x36, 0xb2)},
    { 269, new MidiEventOut(0x11, 0xb0, "BTN", "DECK A SAMPLE 3", 0x2f, 0xb0, 0x11, 0xb2, 0x2f, 0xb2)},
    { 270, new MidiEventOut(0x1, 0xb0, "BTN", "DECK A LOAD", 0x1f, 0xb0, 0x1, 0xb2, 0x1f, 0xb2)},
    { 271, new MidiEventOut(0x12, 0xb0, "BTN", "DECK A SAMPLE 4", 0x30, 0xb0, 0x12, 0xb2, 0x30, 0xb2)},
    { 272, new MidiEventOut(0x6, 0xb0, "BTN", "DECK A TEMPO UP", 0x24, 0xb0, 0x6, 0xb2, 0x24, 0xb2)},
    { 273, new MidiEventOut(0x5, 0xb0, "BTN", "DECK A TEMPO DOWN", 0x23, 0xb0, 0x5, 0xb2, 0x23, 0xb2)},
    { 274, new MidiEventOut(0x15, 0xb0, "BTN", "DECK A LOOP SIZE KNOB PRESS", 0x33, 0xb0, 0x15, 0xb2, 0x33, 0xb2)},
    { 275, new MidiEventOut(0x13, 0xb0, "BTN", "DECK A LOOP MOVE KNOB PRESS", 0x31, 0xb0, 0x13, 0xb2, 0x31, 0xb2)},
    { 296, new MidiEventOut(0x6, 0xb1, "BTN", "DECK B TEMPO RANGE ADJUST UP", 0x24, 0xb1, 0x6, 0xb3, 0x24, 0xb3)},
    { 297, new MidiEventOut(0x5, 0xb1, "BTN", "DECK B TEMPO RANGE ADJUST DOWN", 0x23, 0xb1, 0x5, 0xb3, 0x23, 0xb3)},
    { 298, new MidiEventOut(0x15, 0xb1, "BTN", "DECK B LOOP SIZE KNOB PRESS", 0x33, 0xb1, 0x15, 0xb3, 0x33, 0xb3)},
    { 299, new MidiEventOut(0x13, 0xb1, "BTN", "DECK B LOOP MOVE KNOB PRESS", 0x31, 0xb1, 0x13, 0xb3, 0x31, 0xb3)},
    { 305, new MidiEventOut(0xf, 0xb1, "BTN", "DECK B SAMPLE 1", 0x2d, 0xb1, 0xf, 0xb3, 0x2d, 0xb3)},
    { 306, new MidiEventOut(0x17, 0xb1, "BTN", "DECK B LOOP IN", 0x35, 0xb1, 0x17, 0xb3, 0x35, 0xb3)},
    { 307, new MidiEventOut(0x10, 0xb1, "BTN", "DECK B SAMPLE 2", 0x2e, 0xb1, 0x10, 0xb3, 0x2e, 0xb3)},
    { 308, new MidiEventOut(0x18, 0xb1, "BTN", "DECK B LOOP OUT", 0x36, 0xb1, 0x18, 0xb3, 0x36, 0xb3)},
    { 309, new MidiEventOut(0x11, 0xb1, "BTN", "DECK B SAMPLE 3", 0x2f, 0xb1, 0x11, 0xb3, 0x2f, 0xb3)},
    { 310, new MidiEventOut(0x1, 0xb1, "BTN", "DECK B LOAD", 0x1f, 0xb1, 0x1, 0xb3, 0x1f, 0xb3)},
    { 311, new MidiEventOut(0x12, 0xb1, "BTN", "DECK B SAMPLE 4", 0x30, 0xb1, 0x12, 0xb3, 0x30, 0xb3)},
    { 312, new MidiEventOut(0xb, 0xb1, "BTN", "DECK B HOTCUE 1", 0x29, 0xb1, 0xb, 0xb3, 0x29, 0xb3)},
    { 314, new MidiEventOut(0xc, 0xb1, "BTN", "DECK B HOTCUE 2", 0x2a, 0xb1, 0xc, 0xb3, 0x2a, 0xb3)},
    { 315, new MidiEventOut(0x8, 0xb1, "BTN", "DECK B SYNC", 0x26, 0xb1, 0x8, 0xb3, 0x26, 0xb3)},
    { 316, new MidiEventOut(0xd, 0xb1, "BTN", "DECK B HOTCUE 3", 0x2b, 0xb1, 0xd, 0xb3, 0x2b, 0xb3)},
    { 317, new MidiEventOut(0x9, 0xb1, "BTN", "DECK B CUE", 0x27, 0xb1, 0x9, 0xb3, 0x27, 0xb3)},
    { 318, new MidiEventOut(0xe, 0xb1, "BTN", "DECK B HOTCUE 4", 0x2c, 0xb1, 0xe, 0xb3, 0x2c, 0xb3)},
    { 319, new MidiEventOut(0xa, 0xb1, "BTN", "DECK B PLAY", 0x28, 0xb1, 0xa, 0xb3, 0x28, 0xb3)}
};


int MidiEventOut::get_time(){
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}