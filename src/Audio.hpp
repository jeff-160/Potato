#ifndef AUDIO_HEADER
#define AUDIO_HEADER

namespace Potato{
    struct Audio{
        private:
            Mix_Chunk* Chunk = NULL;
            std::optional<int> Channel = std::nullopt;
            bool Paused = false;
            
            void PlayChunk(int ChannelNo=-1, bool Resume=false){
                Threading::RunAsync(
                    [&, ChannelNo, Resume](){
                        Mix_Volume(ChannelNo, MIX_MAX_VOLUME*this->Volume);
                        if (Resume){
                            Mix_Resume(ChannelNo);
                            this->Paused = false;
                        }
                        else
                            this->Channel = Mix_PlayChannel(ChannelNo, this->Chunk, 0);
                        while (Mix_Playing(this->Channel.value()))
                            Threading::Delay(1);
                        this->Channel = std::nullopt;
                        if (this->Loop)
                            return this->PlayChunk(ChannelNo);
                    }
                );
            }

        public:
            bool Loop = false;
            float Volume = 1;

            void Play();
            void Pause();
            void Restart();

            Audio(std::string Source){
                this->Chunk = Mix_LoadWAV(Source.c_str());
                if (this->Chunk==NULL)
                    System::Error("Failed to load audio: " + Source);
            }
    };

    void Audio::Play(){
        if (!this->Channel.has_value() || this->Paused)
            this->PlayChunk(-1, this->Paused);
    }

    void Audio::Pause(){
        if (this->Channel.has_value()){
            this->Paused = true;
            Mix_Pause(this->Channel.value());
        }
    }

    void Audio::Restart(){
        this->PlayChunk(this->Channel.has_value() ? this->Channel.value() : -1);
    }
}

#endif