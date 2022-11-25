cmake_minimum_required(VERSION 3.23)
project(traktor_kontrol_s4_mk1_driver_linux)

set(CMAKE_CXX_STANDARD 17)

include_directories(./classes)
include_directories(./includes)

add_executable(traktor_kontrol_s4_mk1_driver_linux
        includes/Button.h
        includes/EvDevHelper.h
        includes/RtMidiHelper.h
        includes/Knob.h
        includes/Slider.h
        includes/Led.h
        includes/EvDevEvent.h
        includes/Jog.h
        includes/RtMidiEvent.h
        includes/RtAudioHelper.h
        classes/RtMidiHelper.cpp
        classes/EvDevHelper.cpp
        classes/Button.cpp
        classes/Knob.cpp
        classes/Slider.cpp
        classes/Led.cpp
        classes/EvDevEvent.cpp
        classes/Jog.cpp
        classes/RtMidiEvent.cpp
        classes/RtAudioHelper.cpp
        main.cpp)

target_link_libraries(traktor_kontrol_s4_mk1_driver_linux rtmidi)
target_link_libraries(traktor_kontrol_s4_mk1_driver_linux libevdev.so.2)
target_link_libraries(traktor_kontrol_s4_mk1_driver_linux libasound.so.2)