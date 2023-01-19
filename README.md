## Traktor Kontrol S4 Mk1 v0.1 Driver for Linux

Linux driver based on the awesome work from blaxpot and Javi Fonseca on the Traktor Kontrol S4 Mk1 for work with Mixxx.

[traktor-s4-mk1-midify](https://github.com/blaxpot/traktor-s4-mk1-midify)

[Thread from the Mixxx forum](https://mixxx.discourse.group/t/native-instruments-kontrol-s4-v0-1/11942)

## Requirements
- **[RtMidi Library](https://www.music.mcgill.ca/~gary/rtmidi/)**
- **[ALSA Library](https://www.alsa-project.org/wiki/Main_Page)**
- **snd-usb-caiaq** Kernel Module

## Usage

As a command line driver, you can use these parameters:
- **--help** Show help information
- **--logMode** Set the log mode: console | logfile (default) currently
- **--logLevel** Set the log verbose level: info | debug currently
- **--configFile** Set the path where the config file is. Based on JSON format and included in this repo: **config.json**

## Build
1. Install dependencies. The package names will depend on your Linux distribution.

**Ubuntu**
```
sudo apt install cmake cmake-extras libasound2 librtaudio-dev librtmidi-dev libevdev-dev
```

**Arch Linux**
```
pacman -Syu base-devel cmake alsa-lib rtaudio rtmidi libevdev
```

2. Build with `cmake`:
```
mkdir build
cd build
cmake ..
cmake --build .
```

If all goes well, you should now be able to start the application like so:
```
# ./traktor_kontrol_s4_mk1_driver_linux --logMode console
[2023-01-20 09:51:40.493] [traktor_kontrol_s4_logger] [info] [main::init_application] Traktor Kontrol S4 Mk1 Driver for Linux started
[2023-01-20 09:51:40.493] [traktor_kontrol_s4_logger] [info] [main:show_main_configuration] Current arguments:
[2023-01-20 09:51:40.493] [traktor_kontrol_s4_logger] [info] [main:show_main_configuration] Log level: INFO
[2023-01-20 09:51:40.493] [traktor_kontrol_s4_logger] [info] [main:show_main_configuration] Log mode: LOG IN CONSOLE
[2023-01-20 09:51:40.493] [traktor_kontrol_s4_logger] [info] [main::init_application] Starting helpers....
[2023-01-20 09:51:40.503] [traktor_kontrol_s4_logger] [info] [main::init_application] Get MIDI information....
[2023-01-20 09:51:40.505] [traktor_kontrol_s4_logger] [info] [main::init_application] Initializing EvDev device....
```

## Installation
Installation process is pending, currently you only can download or clone the repo and compile with CMake (>3.16)

## IN PROCESS
