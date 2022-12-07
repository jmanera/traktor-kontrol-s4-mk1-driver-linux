//
// Created by aspgems on 2/12/22.
//

#include "MidiEventIn.h"

using namespace std;

MidiEventIn::MidiEventIn(string in_control_channel_1,
                          string in_control_channel_2,
                          string in_control_channel_3,
                          string in_control_channel_4,
                          string in_control_channel_5){
  control_channel_1 = in_control_channel_1;
  control_channel_2 = in_control_channel_2;
  control_channel_3 = in_control_channel_3;
  control_channel_4 = in_control_channel_4;
  control_channel_5 = in_control_channel_5;
}

map<char, MidiEventIn *> MidiEventIn::midi_in_mapping = {
      {0x01, new MidiEventIn( "74","118","74","118","-") },
      {0x05, new MidiEventIn( "83","127","83","127","9" ) },
      {0x06, new MidiEventIn( "82","126","82","126","13" ) },
      {0x07, new MidiEventIn( "78","122","78","122","8" ) },
      {0x08, new MidiEventIn( "79","123","79","123","12" ) },
      {0x09, new MidiEventIn( "80","124","80","124","11" ) },
      {0x0A, new MidiEventIn( "81","125","81","125","-" ) },
      {0x0B, new MidiEventIn( "66","110","66","110","-" ) },
      {0x0C, new MidiEventIn( "68","112","68","112","-" ) },
      {0x0D, new MidiEventIn( "70","114","70","114","-" ) },
      {0x0E, new MidiEventIn( "72","116","72","116","-" ) },
      {0x0F, new MidiEventIn( "90","134","90","134","-" ) },
      {0x10, new MidiEventIn( "91","135","91","135","-" ) },
      {0x11, new MidiEventIn( "92","136","92","136","-" ) },
      {0x12, new MidiEventIn( "93","137","93","137","-" ) },
      {0x17, new MidiEventIn( "76","120","76","120","-" ) },
      {0x18, new MidiEventIn( "77","121","77","121","154" ) },
      {0x19, new MidiEventIn( "-","-","-","-","155" ) },
      {0x1A, new MidiEventIn( "-","-","-","-","156" ) },
      {0x1B, new MidiEventIn( "-","-","-","-","157" ) },
      {0x1C, new MidiEventIn( "-","-","-","-","158" ) },
      {0x22, new MidiEventIn( "-","-","-","-","159" ) },
      {0x23, new MidiEventIn( "-","-","-","-","160" ) },
      {0x24, new MidiEventIn( "-","-","-","-","161" ) },
      {0x25, new MidiEventIn( "-","-","-","-","162" ) },
      {0x26, new MidiEventIn( "-","-","-","-","163" ) },
      {0x3E, new MidiEventIn( "25","38","51","64","-" ) },
      {0x3F, new MidiEventIn( "26","39","52","65","-" ) },
      {0x44, new MidiEventIn( "24","37","50","63","-" ) },
      {0x46, new MidiEventIn( "16 17 18 19 20 21","29 30 31 32 33 34","42 43 44 45 46 47","55 56 57 58 59 60","-" ) }
};

string MidiEventIn::check_channel_value(unsigned char channel){
  if (channel == 0xb0)
    return control_channel_1;
  if (channel == 0xb1)
    return control_channel_2;
  if (channel == 0xb2)
    return control_channel_3;
  if (channel == 0xb3)
    return control_channel_4;
  if (channel == 0xb4)
    return control_channel_5;

  return "-";
}