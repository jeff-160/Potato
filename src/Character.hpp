#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

namespace Potato{
    struct Character{
        friend struct Engine;
        
        private:
            int CurrentFrame = 0;
            int FrameElapsed = 0;
            void ChangeFrame();

        public:
            std::string Name;
            float X, Y;
            float Width, Height;
            float Opacity = 1;

            std::vector<std::string> Images;
            int FrameRate;

            void Draw(SDL_Renderer* r);
        
        Character(std::string Name, 
                int X, int Y, 
                int Width, int Height, 
                std::vector<std::string> Images, int FrameRate=System::DefaultSettings["CharacterFrameRate"]): 
            Name(Name), X(X), Y(Y), Width(Width), Height(Height), Images(Images), FrameRate(FrameRate){}
    };

    void Character::ChangeFrame(){
        this->FrameElapsed++;
        if (this->FrameElapsed>=this->FrameRate){
            this->FrameElapsed = 0;
            this->CurrentFrame = this->CurrentFrame>=this->Images.size()-1 ? 0 : this->CurrentFrame+1;
        }
    }
}

#endif