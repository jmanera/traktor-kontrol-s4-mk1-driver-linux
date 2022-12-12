#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_LED_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_LED_H

#include <string>
#include <map>

using namespace std;

class Led {
private:
public:
    static const int OFF = 0;
    static const int LOW = 2;
    static const int MIDDLE = 10;
    static const int ON = 31;
    Led(int code, string name, int value, bool is_meter, bool by_default = true);
    Led();
    int code;
    string name;
    int value;
    bool is_meter;
    bool by_default;
    static map<int, Led *> leds_mapping;
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_LED_H
