var TraktorKontrolS4mk1 = {};

// Global variables
var browseKnobRotated = false; // Flag for detecting if browse knob was rotated
var browseKnobPressed = false; // Flag for browse Knob pressing status
var PFLs = [false, false, false, false] // Array flag for PFL status on each channel
var libraryStatus = false
var jumpPrev = false // Flag for beat jump on pushing and rotating knob
var browseButtonFlag = false // Flag for Browse button pressed detection
var maximizeLibrary = true // Flag for inactivate maximize Library
var quantizeVal; // Flag for turning on/off quantize
var syncSamplers = true; // Flag for turning on/off sync on all samplers
var deck_ac_current = "A";
var deck_bd_current = "B";
const beatJumps = [0.03125, 0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64] //Available beat loop

function TraktorKontrolS4mk1() {}

// Object declaration for pfl / load track double function button
TraktorKontrolS4mk1.pflLoadTrack = {};

// init function
TraktorKontrolS4mk1.init = function(id, debugging) {
    //midi.sendShortMsg(0xB1, 0x0B, 0);

    //Set browse rotating knob to center value
    //midi.sendShortMsg(0xB4, 2, 63);

    //Set loop rotating knob to center value
    /*for (var i = 0; i <= 3; i++) {
        midi.sendShortMsg(0xB0 + i, 0x15, 63);
    }*/

    // Turn off LEDs
    /*for (var chan = 0; chan <= 3; chan++) {
        for (var i = 1; i <= 127; i++) {
            midi.sendShortMsg(0xB0 + chan, i, 0x00);
        }
    }*/
    engine.makeConnection("[Channel1]", 'VuMeter', TraktorKontrolS4mk1.vuCallback);
    engine.makeConnection("[Channel2]", 'VuMeter', TraktorKontrolS4mk1.vuCallback2);
    engine.makeConnection("[Channel3]", 'VuMeter', TraktorKontrolS4mk1.vuCallback3);
    engine.makeConnection("[Channel4]", 'VuMeter', TraktorKontrolS4mk1.vuCallback4);


    // Set gain knobs to center value
    /*for (var i = 0; i <= 3; i++) {
        midi.sendShortMsg(0xB0 + i, 0x3C, 63);
        engine.setValue('[Channel' + (i + 1) + ']', "pregain", true);
    }*/

    // Set quantize value on for all channels;
    quantizeVal = false;

    //midi.sendShortMsg(0xB4, 0x05, true);
    for (var i = 0; i <= 3; i++) {
        engine.setValue('[Channel' + (i + 1) + ']', "quantize", quantizeVal);
        engine.setValue('[Channel' + (i + 1) + ']', 'beatjump_size', beatJumps[8]);
        engine.setValue('[Channel' + (i + 1) + ']', 'beatloop_size', beatJumps[8]);
    }
    for (var i = 0; i <= 15; i++) engine.setValue('[Sampler' + (i + 1) + ']', "quantize", quantizeVal);
    //midi.sendShortMsg(0xB4, 0x51, 100);
};

TraktorKontrolS4mk1.enableBeatLoop = function(channel, control, value, status, group) {
    if (value <= 0) return;
    const actualBeatLoop = engine.getValue(group, 'beatloop_size');
    if (engine.getValue(group, "loop_enabled")) {
        engine.setValue(group, "beatloop_" + actualBeatLoop + "_toggle", true);
    } else {
        engine.setValue(group, "beatloop_" + actualBeatLoop + "_toggle", true);
    }
}

TraktorKontrolS4mk1.manageSampler = function(channel, control, value, status, group) {
    const sampler_playing = engine.getValue(group, 'play');
    if (value <= 0)
        return;

    if (sampler_playing > 0) {
        engine.setValue(group, 'stop', true);
    } else {
        engine.setValue(group, 'start_play', true);
    }


}

TraktorKontrolS4mk1.changeDeckAC = function(channel, control, value, status, group) {
    print("Change Deck AC");
    if (value == 0)
        return;
    if (deck_ac_current == "A") {
        deck_ac_current = "C";
        // Set HotCues to Deck C
        for (var i = 1; i <= 4; i++) {
            if (engine.getValue("[Channel3]", "hotcue_" + i + "_enabled")) {
                midi.sendShortMsg(0xB2, 0x50 + i, 0x7F);
            } else {
                midi.sendShortMsg(0xB2, 0x50 + i, 0x0);
            }
        }

        // Set Loop Size to Deck C
        var actualBeatLoop = engine.getValue("[Channel3]", 'beatloop_size');
        print('Beat loop size: ' + actualBeatLoop);
        var beatId = 0
        for (var i = 0; i <= 11; i++) {
            if (actualBeatLoop == beatJumps[i]) beatId = i;
        }
        midi.sendShortMsg(0xB2, 0x50, beatId);
    } else {
        deck_ac_current = "A";
        // Set HotCues to Deck A
        for (var i = 1; i <= 4; i++) {
            if (engine.getValue("[Channel1]", "hotcue_" + i + "_enabled")) {
                midi.sendShortMsg(0xB0, 0x50 + i, 0x7F);
            } else {
                midi.sendShortMsg(0xB0, 0x50 + i, 0x0);
            }
        }
        // Set Loop Size to Deck A
        var actualBeatLoop = engine.getValue("[Channel1]", 'beatloop_size');
        print('Beat loop size: ' + actualBeatLoop);
        var beatId = 0
        for (var i = 0; i <= 11; i++) {
            if (actualBeatLoop == beatJumps[i]) beatId = i;
        }
        midi.sendShortMsg(0xB0, 0x50, beatId);
    }



    print("Current Deck is: " + deck_ac_current);
}

TraktorKontrolS4mk1.changeDeckBD = function(channel, control, value, status, group) {
    print("Change Deck BD");
    if (value == 0)
        return;
    if (deck_bd_current == "B") {
        deck_bd_current = "D";
        // Set HotCues to Deck D
        for (var i = 1; i <= 4; i++) {
            if (engine.getValue("[Channel4]", "hotcue_" + i + "_enabled")) {
                midi.sendShortMsg(0xB3, 0x50 + i, 0x7F);
            } else {
                midi.sendShortMsg(0xB3, 0x50 + i, 0x0);
            }
        }
        // Set Loop Size to Deck D
        var actualBeatLoop = engine.getValue("[Channel4]", 'beatloop_size');
        print('Beat loop size: ' + actualBeatLoop);
        var beatId = 0
        for (var i = 0; i <= 11; i++) {
            if (actualBeatLoop == beatJumps[i]) beatId = i;
        }
        midi.sendShortMsg(0xB3, 0x50, beatId);
    } else {
        deck_bd_current = "B";
        // Set HotCues to Deck B
        for (var i = 1; i <= 4; i++) {
            if (engine.getValue("[Channel2]", "hotcue_" + i + "_enabled")) {
                midi.sendShortMsg(0xB1, 0x50 + i, 0x7F);
            } else {
                midi.sendShortMsg(0xB1, 0x50 + i, 0x0);
            }
        }
        // Set Loop Size to Deck B
        var actualBeatLoop = engine.getValue("[Channel2]", 'beatloop_size');
        print('Beat loop size: ' + actualBeatLoop);
        var beatId = 0
        for (var i = 0; i <= 11; i++) {
            if (actualBeatLoop == beatJumps[i]) beatId = i;
        }
        midi.sendShortMsg(0xB1, 0x50, beatId);
    }
    print("Current Deck is: " + deck_bd_current);
}

TraktorKontrolS4mk1.fineRate = function(channel, control, value, status, group) {
    var actualRate = engine.getValue(group, 'rate');
    print('Actal rate: ' + actualRate);
    if (value > 1) engine.setValue(group, 'rate', actualRate + 0.001);
    else engine.setValue(group, 'rate', actualRate - 0.001);
}

TraktorKontrolS4mk1.toggleRecord = function(channel, control, value, status, group) {
    engine.setValue('[Recording]', 'toggle_recording', true);
    if (engine.getValue('[Recording]', 'status') != 0) {
        midi.sendShortMsg(0xB4, 0x03, 1);
    } else {
        midi.sendShortMsg(0xB4, 0x03, 0);
    }

}

TraktorKontrolS4mk1.rate = function(channel, control, value, status, group) {
    var rateSkip = 0.0;
    if (value < 64) rateSkip = (value - 64) / 64;
    else rateSkip = (value - 64) / 63;
    engine.setValue(group, 'rate', rateSkip);

    if (value % 2 && value > 64) midi.sendShortMsg(0xB0 + channel, 0x06, true);
    else midi.sendShortMsg(0xB0 + channel, 0x06, false);

    if (value % 2 && value < 64) midi.sendShortMsg(0xB0 + channel, 0x05, true);
    else midi.sendShortMsg(0xB0 + channel, 0x05, false);

    if (value === 64) {
        midi.sendShortMsg(0xB0 + channel, 0x06, true);
        midi.sendShortMsg(0xB0 + channel, 0x05, true);
    }
}

// Turn quantize on/off for all channels and samplers
TraktorKontrolS4mk1.quantize = function(channel, control, value, status, group) {
    if (value) {
        quantizeVal = !quantizeVal;
        if (quantizeVal)
            midi.sendShortMsg(0xB4, 0x02, 1);
        else
            midi.sendShortMsg(0xB4, 0x02, 0);
        for (var i = 0; i <= 3; i++) engine.setValue('[Channel' + (i + 1) + ']', "quantize", quantizeVal);
        for (var i = 0; i <= 15; i++) engine.setValue('[Sampler' + (i + 1) + ']', "quantize", quantizeVal);
    }
}

TraktorKontrolS4mk1.setHeadGain = function(channel, control, value, status, group) {
    print("Set Head Gain");
}

// Change loop size on knob rotation
TraktorKontrolS4mk1.beatLoopSize = function(channel, control, value, status, group) {
    var actualBeatLoop = engine.getValue(group, 'beatloop_size');
    print('Beat loop size: ' + actualBeatLoop);
    var actualBeatJump = engine.getValue(group, 'beatjump_size');
    var beatIdLoop = 0;
    var beatIdJump = 0;
    for (var i = 0; i <= 11; i++) {
        if (actualBeatLoop == beatJumps[i]) beatIdLoop = i;
        if (actualBeatJump == beatJumps[i]) beatIdJump = i;
    }
    print('beatId: ' + beatIdLoop)
    var currentBeatLoop = 0;
    if (value < 63) {
        print('Beat loop size backWard: ' + engine.getValue(group, 'beatloop_size'));
        if (beatIdLoop) {
            engine.setValue(group, 'beatloop_size', beatJumps[beatIdLoop - 1]);
            midi.sendShortMsg(0xB0 + channel, 0x50, beatIdLoop - 1);
        }
        if (beatIdJump) engine.setValue(group, 'beatjump_size', beatJumps[beatIdJump - 1]);
    }
    if (value > 63) {
        if (beatIdLoop < 11) {
            engine.setValue(group, 'beatloop_size', beatJumps[beatIdLoop + 1]);
            midi.sendShortMsg(0xB0 + channel, 0x50, beatIdLoop + 1);
        }
        if (beatIdJump < 11) engine.setValue(group, 'beatjump_size', beatJumps[beatIdJump + 1]);
    }
    midi.sendShortMsg(0xB0 + channel, 0x13, 63);
    midi.sendShortMsg(0xB0 + channel, 0x15, 63);

}

// Beat jump size on knob rotation
/*TraktorKontrolS4mk1.beatJumpSize = function(channel, control, value, status, group) {
    var actualBeatJump = engine.getValue(group, 'beatjump_size');
    var beatId = 0
    for (var i = 0; i <= 11; i++) {
        if (actualBeatJump == beatJumps[i]) beatId = i;
    }
    if (value < 63) {
        if (beatId) engine.setValue(group, 'beatjump_size', beatJumps[beatId - 1]);
        midi.sendShortMsg(0xB0 + channel, 0x13, 63);
    }
    if (value > 63) {
        if (beatId < 11) engine.setValue(group, 'beatjump_size', beatJumps[beatId + 1]);
        midi.sendShortMsg(0xB0 + channel, 0x13, 63);
    }
}*/

TraktorKontrolS4mk1.MoveFocusForward = function(channel, control, value, status, group) {
    if (value) {
        engine.setValue('[Library]', 'MoveFocusForward', true);
        print('MoveFocus' + engine.getValue('[Library]', 'MoveFocus'));
    }
}


// Song preview on/off pushing Browse knob button
// If nothing happends pushing Browse knob function as double click.
//    This is a trick for opening/closing the Playlist, Procesar and History menues
TraktorKontrolS4mk1.playStopPreview = function(channel, control, value, status, group) {
    print('!jumpPrev: ' + !jumpPrev + ', !value: ' + !value + ', PrevDeckStop: ' + engine.getParameter('[PreviewDeck1]', "stop"))
    print('LoadSelectedTrackAndPlay: ' + engine.getParameter('[PreviewDeck1]', "LoadSelectedTrackAndPlay"))
    if (value) browseKnobPressed = true;
    else browseKnobPressed = false;
    // Preview and on/off on browse knob pressing
    if (!value && !jumpPrev) {
        change = true;
        if (engine.getParameter('[PreviewDeck1]', "stop") || browseKnobRotated) {
            engine.setValue('[PreviewDeck1]', "LoadSelectedTrackAndPlay", true);
            if (engine.getParameter('[PreviewDeck1]', "stop")) engine.setValue('[Library]', 'GoToItem', true);
        } else engine.setValue('[PreviewDeck1]', "stop", true);
        browseKnobRotated = false
    }
    if (!value && jumpPrev) {
        change = true
        jumpPrev = false;
        browseKnobRotated = false;
    }
}

// Library browse up/down while rotating Browse knob
// or beat jump if knob was pressed
TraktorKontrolS4mk1.browseUpDown = function(channel, control, value, status, group) {
    browseKnobRotated = true
    if (value > 63) {
        if (browseKnobPressed) {
            engine.setValue("[PreviewDeck1]", "beatjump_8_forward", true);
            jumpPrev = true;
        } else {
            engine.setValue('[Library]', "MoveDown", true);
            jumpPrev = false;
        }
        midi.sendShortMsg(0xB4, 2, 63);
    }
    if (value < 63) {
        if (browseKnobPressed) {
            engine.setValue("[PreviewDeck1]", "beatjump_8_backward", true);
            jumpPrev = true;
        } else {
            engine.setValue('[Library]', "MoveUp", true);
            jumpPrev = false;
        }
        midi.sendShortMsg(0xB4, 2, 63);
    }
}

TraktorKontrolS4mk1.maximize_library = function(channel, control, value, status, group) {
    if (value) browseButtonFlag = true;
    else browseButtonFlag = false;
    print("browseButtonFlag: " + browseButtonFlag);
    if (!value) {
        // Turn off library and resume CUES
        if (libraryStatus) {
            engine.setValue("[Master]", 'maximize_library', false);
            libraryStatus = false;
            browseButtonFlag = false;
            // Reset CUE and LEDs
            for (var i = 0; i <= 3; i++) {
                midi.sendShortMsg(0xB0 + i, 0x44, PFLs[i]);
                engine.setParameter('[Channel' + (i + 1) + ']', "pfl", PFLs[i]);
                print("PFL" + i + ": " + PFLs[i])
            }
            // Turn off headphones preview
            engine.setValue('[PreviewDeck1]', "stop", true);
        }
        // Turn on library and mute CUES
        else if (maximizeLibrary) {
            engine.setValue("[Master]", 'maximize_library', true);
            libraryStatus = true;
            browseButtonFlag = true;
            for (var i = 0; i <= 3; i++) {
                midi.sendShortMsg(0xB0 + i, 0x44, false);
                engine.setParameter('[Channel' + (i + 1) + ']', "pfl", false);
            }
        }
    }
    maximizeLibrary = true;
}

// PFL on/off or Load track in library on or Browse button on
TraktorKontrolS4mk1.PFL = function(channel, control, value, status, group) {
    // assign PFL[Channel] actual status (on/off)
    var pfl = engine.getParameter(group, "pfl");
    // Si está activa "Big Library" (o presionado el boton Browse)
    // al presionar el botón pfl ("CUE" en Kontrol S4) 
    // se carga el track seleccionado en el canal correspondiente
    print("Browse value: " + browseButtonFlag)
    if ((engine.getParameter('[Master]', 'maximize_library') || browseButtonFlag) && value) {
        engine.setValue(group, "LoadSelectedTrack", true);
        // Set gain to center value
        engine.setValue(group, "pregain", true);
        midi.sendShortMsg(0xB0 + channel, 0x3C, 63);
        //Turn on "LOAD" LED button
        midi.sendShortMsg(channel + 0xB0, 0x01, 0x7F);
        engine.setValue('[PreviewDeck1]', "stop", true); // Stop headphoes preview
        maximizeLibrary = false;
    }
    // En caso contrario se activa función 'pfl' al presionar "CUE" en Kontrol S4
    // del canal correspondiente
    else {
        if (pfl && !value) {
            engine.setValue(group, "pfl", false)
            PFLs[channel] = false
        }
        if (!pfl && !value) {
            engine.setValue(group, "pfl", true)
            PFLs[channel] = true
        }
    }
}


TraktorKontrolS4mk1.loadSelectedTrack = function(channel, control, value, status, group) {
    engine.setValue(group, "LoadSelectedTrack", true);
    //Turn on "LOAD" LED button
    midi.sendShortMsg(channel + 0xB0, 0x01, 0x7F);

    // Check the Hot Cues
    for (var i = 1; i <= 4; i++) {
        if (engine.getValue(group, "hotcue_" + i + "_enabled")) {
            midi.sendShortMsg(channel + 0xB0, 0x50 + i, 0x7F);
        } else {
            midi.sendShortMsg(channel + 0xB0, 0x50 + i, 0x0);
        }
    }


}

TraktorKontrolS4mk1.setHotCue = function(channel, control, value, status, group) {
    if (value == 0)
        return;

    print("setHotCue - Channel: " + channel + " Value: " + value + " Control: " + (control - 10) + " Status: " + status + " Group: " + group + "-");
    if (engine.getValue(group, "hotcue_" + (control - 10) + "_enabled")) {
        engine.setValue(group, "hotcue_" + (control - 10) + "_goto", true);
    } else {
        engine.setValue(group, "hotcue_" + (control - 10) + "_activate", true);
        midi.sendShortMsg(channel + 0xb0, 0x50 + (control - 10), 0x7F);
    }
}

TraktorKontrolS4mk1.clearHotCue = function(channel, control, value, status, group) {
    if (value == 0)
        return;

    print("clearHotCue - Channel: " + channel + " Value: " + value + " Control: " + (control - 40) + " Status: " + status + " Group: " + group + "-");
    if (engine.getValue(group, "hotcue_" + (control - 40) + "_enabled")) {
        engine.setValue(group, "hotcue_" + (control - 40) + "_clear", true);
        midi.sendShortMsg(channel + 0xb0, 0x50 + (control - 40), 0x0);
    }
}

TraktorKontrolS4mk1.eject = function(channel, control, value, status, group) {
    engine.setValue(group, "eject", true);
    engine.setValue(group, "LoadSelectedTrack", 0x00);
    //Turn off "LOAD" LED button
    midi.sendShortMsg(channel + 0xB0, 0x01, 0x00);
}

// VuMeter LEDs
TraktorKontrolS4mk1.vuCallback = function(channel, control, value, status, group) {
    midi.sendShortMsg(0xB0, 0x46, engine.getParameter('[Channel1]', 'VuMeter') * 0x7F * 0.93);
}
TraktorKontrolS4mk1.vuCallback2 = function(channel, control, value, status, group) {
    midi.sendShortMsg(0xB1, 0x46, engine.getParameter('[Channel2]', 'VuMeter') * 0x7F * 0.93);
}
TraktorKontrolS4mk1.vuCallback3 = function(channel, control, value, status, group) {
    midi.sendShortMsg(0xB2, 0x46, engine.getParameter('[Channel3]', 'VuMeter') * 0x7F * 0.93);
}
TraktorKontrolS4mk1.vuCallback4 = function(channel, control, value, status, group) {
    midi.sendShortMsg(0xB3, 0x46, engine.getParameter('[Channel4]', 'VuMeter') * 0x7F * 0.93);
}

// The button that enables/disables scratching
TraktorKontrolS4mk1.wheelTouch = function(channel, control, value, status, group) {
    var deckNumber = script.deckFromGroup(group);
    if ((status & 0xF0) === 0x90) { // If button down
        //if (value === 0x7F) { // Some wheels send 0x90 on press and release, so you need to check the value
        var alpha = 1.0 / 8;
        var beta = alpha / 32;
        engine.scratchEnable(deckNumber, 128, 8, alpha, beta);
    } else { // If button up
        engine.scratchDisable(deckNumber);
    }
}

// The wheel that actually controls the scratching
TraktorKontrolS4mk1.wheelTurn = function(channel, control, value, status, group) {
    // --- Choose only one of the following!

    // A: For a control that centers on 0:
    var newValue;
    if (value < 64) {
        newValue = value;
    } else {
        newValue = value - 128;
    }

    // B: For a control that centers on 0x40 (64):
    //var newValue = value - 64;

    // --- End choice

    // In either case, register the movement
    var deckNumber = script.deckFromGroup(group);
    if (engine.isScratching(deckNumber)) {
        engine.scratchTick(deckNumber, newValue); // Scratch!
    } else {
        engine.setValue(group, 'jog', newValue); // Pitch bend
    }
}