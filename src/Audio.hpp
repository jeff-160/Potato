#ifndef AUDIO_HEADER
#define AUDIO_HEADER

namespace Potato{
    struct AudioManager{
        friend class Audio;

        static std::tuple<SDL_AudioSpec, Uint8*, Uint32, SDL_AudioDeviceID> Load(std::string as);
    };

    std::tuple<SDL_AudioSpec, Uint8*, Uint32, SDL_AudioDeviceID> AudioManager::Load(std::string AudioSource){
        std::string ErrMsg = "Failed to load audio: " + AudioSource;

        SDL_AudioSpec Spec;
        Uint8* Buffer = NULL;
        Uint32 Length = 0;

        if (SDL_LoadWAV(AudioSource.c_str(), &Spec, &Buffer, &Length)==NULL)
            System::Error(ErrMsg);

        SDL_AudioDeviceID Device = SDL_OpenAudioDevice(NULL, 0, &Spec, NULL, 0);
        if (Device==0){
            SDL_FreeWAV(Buffer);
            SDL_CloseAudioDevice(Device);
            System::Error(ErrMsg);
        }

        return {Spec, Buffer, Length, Device};
    }

    class Audio{
        friend class AudioManager;

        private:
            std::string Source;

            SDL_AudioSpec Spec;
            Uint8* Buffer = nullptr;
            Uint32 Length = 0;
            SDL_AudioDeviceID Device = 0;

        public:
            float Volume = 1;
            bool Loop = false;

            void Play();
            void Pause();
            void Restart();

        Audio(std::string Source): Source(Source){
            std::tie(this->Spec, this->Buffer, this->Length, this->Device) = AudioManager::Load(Source);

            this->Spec.freq = 44100;
            this->Spec.format = AUDIO_S16SYS;
            this->Spec.channels = 2;
            this->Spec.samples = 1024;
            Spec.callback = nullptr;
        }
    };

    void Audio::Play(){
        Threading::RunAsync(
            [&](){
                SDL_QueueAudio(this->Device, this->Buffer, this->Length);
                SDL_PauseAudioDevice(this->Device, 0);
                while (SDL_GetQueuedAudioSize(this->Device) > 0)
                    Threading::Delay(1);
                if (this->Loop) return this->Play();
            }
        );
    }

    void Audio::Pause(){
        SDL_PauseAudioDevice(this->Device, 1);
    }

    void Audio::Restart(){
        this->Pause();
        Audio NewAudio(this->Source);
        this->Device = NewAudio.Device;
        this->Play();
    }
}

#endif