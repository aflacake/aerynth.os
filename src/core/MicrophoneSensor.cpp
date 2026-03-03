#include "MicrophoneSensor.h"
#include <cmath>

void audioCallback(void* userdata, Uint8* stream, int len)
{
    MicrophoneSensor* self = static_cast<MicrophoneSensor*>(userdata);

    float sum = 0.0f;
    int samples = len / sizeof(Sint16);
    Sint16* data = (Sint16*)stream;

    for (int i = 0; i < samples; ++i) {
        float s = data[i] / 32768.0f;
        sum += s * s;
    }

    self->volume = sqrt(sum / samples);
}

bool MicrophoneSensor::init()
{
    SDL_AudioSpec want{};
    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 1024;
    want.callback = audioCallback;
    want.userdata = this;

    device = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &want, nullptr, 0);
    if (!device) return false;

    SDL_PauseAudioDevice(device, 0);
    return true;
}

void MicrophoneSensor::update()
{
    // tidak ada logika aktif (pasif)
}

bool MicrophoneSensor::isSoundDetected() const
{
    return volume > 0.05f; // threshold pasif
}

void MicrophoneSensor::shutdown()
{
    if (device) SDL_CloseAudioDevice(device);
}