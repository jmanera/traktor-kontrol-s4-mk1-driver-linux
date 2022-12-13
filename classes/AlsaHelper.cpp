#include "AlsaHelper.h"

#define CARD_NAME "Traktor Kontrol S4"

void AlsaHelper::show_control_id(snd_ctl_elem_id_t *id)
{
    char *str;
    str = snd_ctl_ascii_elem_id_get(id);
    if (str)
        spdlog::debug("[AlsaHelper::show_control_id] Show Control ID: {0}", str);
    free(str);
}

void AlsaHelper::check(int err, const char *f)
{
    if (err < 0) {
        spdlog::error("[AlsaHelper::show_control_id] {0} failed: {1}", f, snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}
#define CHECK(f) check(f, #f)

int AlsaHelper::set_led_value(int card_id, int control_id, int led_value){
    spdlog::debug("[AlsaHelper::set_led_value] Setting value {0} to Led id {1} with value {2}...", to_string(card_id), to_string(control_id), to_string(led_value));
    if (control_id == 0){
      spdlog::debug("[AlsaHelper::set_led_value] Control ID: {0} not found", control_id);
      return -1;
    }

    if (Led::leds_mapping.find(control_id) == Led::leds_mapping.end()) {
      spdlog::debug("[AlsaHelper::set_led_value] Control ID: {0} not found", control_id);
      return -1;
    }

    snd_ctl_t *ctl;
    snd_ctl_elem_value_t *value;

    string control_name = "hw:" + to_string(card_id);
    spdlog::debug("[AlsaHelper::set_led_value] Controller name: {0}", control_name);
    CHECK(snd_ctl_open(&ctl, control_name.c_str(), SND_CTL_NONBLOCK));

    snd_ctl_elem_value_alloca(&value);
    snd_ctl_elem_value_set_interface(value, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_value_set_numid(value, control_id);
    snd_ctl_elem_value_set_integer(value, 0, led_value);
    spdlog::debug("[AlsaHelper::set_led_value] snd_ctl_elem_write returns {0}", snd_ctl_elem_write(ctl, value));

    snd_ctl_close(ctl);
    spdlog::debug("[AlsaHelper::set_led_value] FINISHED");
    return 0;
}

int AlsaHelper::bulk_led_value(int card_id, int control_ids[], int led_value, int num_controls){
    string control_name = "hw:" + to_string(card_id);
    spdlog::debug("[AlsaHelper::show_control_id] Initiating bulk_led_value in device {0}...", control_name);

    for (int ctl_id = 0; ctl_id < num_controls; ctl_id++){
      if (control_ids[ctl_id] != 0)
        set_led_value(card_id, control_ids[ctl_id], led_value);
    }
    spdlog::debug("[AlsaHelper::show_control_id] FINISHED");
    return 0;
}

int AlsaHelper::get_traktor_device(){
    spdlog::debug("[AlsaHelper::show_control_id] Looking for Traktor Kontrol S4 ALSA Device....");
    int card = -1, dev = -1;
    char *name = NULL;

    snd_ctl_t *handle = NULL;
    snd_ctl_card_info_t *info = NULL;
    snd_pcm_info_t *pcminfo = NULL;
    snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;
    string card_id;

    snd_ctl_card_info_alloca (&info);
    snd_pcm_info_alloca (&pcminfo);

    if (snd_card_next (&card) < 0 || card < 0){
        spdlog::error("[AlsaHelper::show_control_id] ERROR Reading audio cards!");
    }
    while(card >= 0){
        card_id = "hw:" + to_string(card);
        if (snd_ctl_open (&handle, card_id.c_str(), 0) == 0){
            snd_ctl_card_info (handle, info);
            while (true){
                snd_ctl_pcm_next_device (handle, &dev);
                spdlog::debug("[AlsaHelper::show_control_id] Analyzing Device: {0}....", card_id.c_str());
                if (dev < 0){
                    spdlog::debug("[AlsaHelper::show_control_id] Error reading device from: {0}. Continuing", card_id);
                    break;
                }
                snd_pcm_info_set_device (pcminfo, dev);
                snd_pcm_info_set_subdevice (pcminfo, 0);
                snd_pcm_info_set_stream (pcminfo, stream);
                if (snd_ctl_pcm_info (handle, pcminfo) >= 0){
                    snd_card_get_name (card, &name);
                    spdlog::debug("[AlsaHelper::show_control_id] Card Name: {0}", name);
                    if(strncmp(name, CARD_NAME, 32) == 0){
                        snd_ctl_close(handle);
                        spdlog::debug("[AlsaHelper::show_control_id] Found on hw:{0},{1} with name: {2}!!!", card, dev, name);
                        free (name);
                        spdlog::debug("[AlsaHelper::show_control_id] FINISHED");
                        return card;
                    }
                }
                spdlog::debug("[AlsaHelper::show_control_id] Device {0} is not Traktor Kontrol S4 device", card_id.c_str());
            }
            snd_ctl_close(handle);
        }
        snd_card_next (&card);
    }
    spdlog::debug("[AlsaHelper::show_control_id] NO DEVICES FOUND!");
    return -1;
}
