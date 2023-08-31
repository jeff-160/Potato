namespace Potato{
    class Character{
        private:
            int CurrentFrame = 0;

        public:
            std::string Name;
            double X, Y;
            int Width, Height;
            Uint32 Opacity = 1;

            std::vector<std::string> Images;
            int FrameRate;

            void Draw(SDL_Renderer* r);
            void Speak(std::string t);
        
        Character(std::string Name, int X, int Y, int Width, int Height, std::vector<std::string> Images, int FrameRate=30): 
            Name(Name), X(X), Y(Y), Width(Width), Height(Height), Images(Images), FrameRate(FrameRate){

        }
    };

    void Character::Speak(std::string Text){
        
    }

    void Character::Draw(SDL_Renderer* Renderer){
        SDL_Texture* CTexture = IMG_LoadTexture(Renderer, this->Images[this->CurrentFrame].c_str());
        SDL_Rect CBounds = {this->X, this->Y, this->Width, this->Height};
        SDL_RenderCopy(Renderer, CTexture, nullptr, &CBounds);
        SDL_DestroyTexture(CTexture);
    }
}