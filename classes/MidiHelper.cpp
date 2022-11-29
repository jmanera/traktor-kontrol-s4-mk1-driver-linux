#include "MidiHelper.h"

using namespace std;

MidiHelper::MidiHelper(){
    try {
        pMidiIn = new RtMidiIn(RtMidi::UNSPECIFIED, "TRAKTORS4");
        pMidiIn->openVirtualPort();
        pMidiIn->setCallback(&midi_in_callback);

        pMidiOut = new RtMidiOut(RtMidi::UNSPECIFIED,"TRAKTORS4");
        pMidiOut->openVirtualPort();
    }
    catch ( RtMidiError &error ) {
        spdlog::error("[RtMidiHelper::RtMidiHelper] RtMidi Error: {0}", error.getMessage());
        exit( EXIT_FAILURE );
    }
}

bool MidiHelper::close_input_port()
{
    pMidiIn->closePort();
    return true;
}

bool MidiHelper::close_output_port()
{
    pMidiOut->closePort();
    return true;
}

void MidiHelper::midi_in_callback(double deltatime, std::vector<unsigned char > *message, void *userData){
    spdlog::debug("ENTER");
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        std::cout << "stamp = " << deltatime << std::endl;
}

void MidiHelper::show_midi_information(MidiHelper *rtmidi_helper){
    unsigned int nPorts = rtmidi_helper->pMidiIn->getPortCount();
    spdlog::debug("[RtMidiHelper::show_midi_information] There are {0} MIDI input sources available", nPorts);
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = rtmidi_helper->pMidiIn->getPortName(i);
        }
        catch ( RtMidiError &error ) {
            spdlog::error("[RtMidiHelper::show_midi_information] {0}", error.getMessage());
        }
        spdlog::debug("[RtMidiHelper::show_midi_information]    Input Port #{0}: {1}", i+1, portName);
    }

    nPorts = rtmidi_helper->pMidiOut->getPortCount();
    spdlog::debug("[RtMidiHelper::show_midi_information] There are {0} MIDI output sources available", nPorts);
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = rtmidi_helper->pMidiOut->getPortName(i);
        }
        catch (RtMidiError &error) {
            spdlog::error("[RtMidiHelper::show_midi_information] {0}", error.getMessage());
        }
        spdlog::debug("[RtMidiHelper::show_midi_information]    Output Port #{0}: {1}", i+1, portName);
    }
}
