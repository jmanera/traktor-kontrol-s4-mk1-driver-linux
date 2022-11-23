cmake_minimum_required(VERSION 3.23)
project(traktor_kontrol_s4_mk1_driver_linux)

set(CMAKE_CXX_STANDARD 17)

if(NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif()

include_directories(./classes)
include_directories(./includes)

add_executable(traktor_kontrol_s4_mk1_driver_linux
        classes/RtMidiHelper.cpp
        classes/EvDevHelper.cpp
        includes/EvDevHelper.h
        includes/RtMidiHelper.h
        main.cpp classes/Button.cpp includes/Button.h classes/Knob.cpp includes/Knob.h classes/Slider.cpp includes/Slider.h classes/Led.cpp includes/Led.h classes/EvDevEvent.cpp includes/EvDevEvent.h classes/Jog.cpp includes/Jog.h classes/RtMidiEvent.cpp includes/RtMidiEvent.h)

target_link_libraries(traktor_kontrol_s4_mk1_driver_linux rtmidi)
target_link_libraries(traktor_kontrol_s4_mk1_driver_linux libevdev.so.2)
target_link_libraries(traktor_kontrol_s4_mk1_driver_linux spdlog::spdlog)