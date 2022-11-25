//
// Created by aspgems on 23/11/22.
//

#include "RtAudioHelper.h"

#define CARD_NAME "Traktor Kontrol S4"

void RtAudioHelper::show_control_id(snd_ctl_elem_id_t *id)
{
    char *str;
    str = snd_ctl_ascii_elem_id_get(id);
    if (str)
        spdlog::debug("[RtAudioHelper::show_control_id] Show Control ID: {0}", str);
    free(str);
}

void RtAudioHelper::check(int err, const char *f)
{
    if (err < 0) {
        spdlog::error("[RtAudioHelper::check] {0} failed: {1}", f, snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}
#define CHECK(f) check(f, #f)

int RtAudioHelper::set_led_value(int card_id, int control_id, int led_value){
    spdlog::debug("[RtAudioHelper::set_led_value] Setting value {0} to Led id {1} with value {2}...", to_string(card_id), to_string(control_id), to_string(led_value));
    snd_ctl_t *ctl;
    snd_ctl_elem_value_t *value;

    string control_name = "hw:" + to_string(card_id);
    spdlog::debug("[RtAudioHelper::set_led_value] Controller name: {0}", control_name);
    CHECK(snd_ctl_open(&ctl, control_name.c_str(), 0));

    snd_ctl_elem_value_alloca(&value);
    snd_ctl_elem_value_set_interface(value, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_value_set_numid(value, control_id);
    snd_ctl_elem_value_set_integer(value, 0, led_value);
    CHECK(snd_ctl_elem_write(ctl, value));

    snd_ctl_close(ctl);
    spdlog::debug("FINISHED");
    return 0;
}

int RtAudioHelper::bulk_led_value(int card_id, int control_ids[], int led_value, int num_controls){
    string control_name = "hw:" + to_string(card_id);
    spdlog::debug("[RtAudioHelper::bulk_led_value] Initiating bulk_led_value in device {0}...", control_name);
    snd_ctl_t *ctl;
    snd_ctl_elem_value_t *value;

    CHECK(snd_ctl_open(&ctl, control_name.c_str(), 0));

    snd_ctl_elem_value_alloca(&value);
    snd_ctl_elem_value_set_interface(value, SND_CTL_ELEM_IFACE_MIXER);

    for (int ctl_id = 0; ctl_id < num_controls; ctl_id++){
        spdlog::debug("[RtAudioHelper::bulk_led_value] Setting Led code {0}", to_string(control_ids[ctl_id]) );
        snd_ctl_elem_value_set_numid(value, control_ids[ctl_id]);
        snd_ctl_elem_value_set_integer(value, 0, led_value);
        CHECK(snd_ctl_elem_write(ctl, value));
    }
    snd_ctl_close(ctl);

    spdlog::debug("[RtAudioHelper::bulk_led_value] FINISHED");
    return 0;
}

int RtAudioHelper::get_traktor_device(){
    spdlog::debug("[RtAudioHelper::get_traktor_device] Looking for Traktor Kontrol S4 ALSA Device....");
    int card, dev;
    card = dev = -1;
    char *name = NULL;

    snd_ctl_t *handle = NULL;
    snd_ctl_card_info_t *info = NULL;
    snd_pcm_info_t *pcminfo = NULL;
    snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;
    string card_id;

    snd_ctl_card_info_alloca (&info);
    snd_pcm_info_alloca (&pcminfo);

    if (snd_card_next (&card) < 0 || card < 0){
        spdlog::error("[RtAudioHelper::get_traktor_device] ERROR Reading audio cards!");
    }
    while(card >= 0){
        card_id = "hw:" + to_string(card);
        if (snd_ctl_open (&handle, card_id.c_str(), 0) == 0){
            snd_ctl_card_info (handle, info);
            while (true){
                snd_ctl_pcm_next_device (handle, &dev);
                spdlog::debug("[RtAudioHelper::get_traktor_device] Analyzing Device: {0}....", card_id.c_str());
                if (dev < 0){
                    spdlog::warn("[RtAudioHelper::get_traktor_device] Error reading device from: {0}. Continuing", card_id);
                    break;
                }
                snd_pcm_info_set_device (pcminfo, dev);
                snd_pcm_info_set_subdevice (pcminfo, 0);
                snd_pcm_info_set_stream (pcminfo, stream);
                if (snd_ctl_pcm_info (handle, pcminfo) >= 0){
                    snd_card_get_name (card, &name);
                    spdlog::debug("[RtAudioHelper::get_traktor_device] Card Name: {0}", name);
                    if(strncmp(name, CARD_NAME, 32) == 0){
                        snd_ctl_close(handle);
                        spdlog::debug("[RtAudioHelper::get_traktor_device] Found on hw:{0},{1} with name: {2}!!!", card, dev, name);
                        free (name);
                        spdlog::debug("[RtAudioHelper::get_traktor_device] FINISHED");
                        return card;
                    }
                }
                spdlog::debug("[RtAudioHelper::get_traktor_device] Device {0} is not Traktor Kontrol S4 device", card_id.c_str());
            }
            snd_ctl_close(handle);
        }
        snd_card_next (&card);
    }
    spdlog::debug("[RtAudioHelper::get_traktor_device] NO DEVICES FOUND!");
    return EXIT_FAILURE;
}