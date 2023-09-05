#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

namespace Potato{
    class Character{
        private:
            int CurrentFrame = 0;
            int FrameElapsed = 0;
            void ChangeFrame();

        public:
            friend class Engine;
            
            std::string Name;
            int X, Y;
            int Width, Height;
            float Opacity = 1;

            std::vector<std::string> Images;
            int FrameRate;

            void Draw(SDL_Renderer* r);
            void Speak(std::string t);
        
        Character(std::string Name, 
                int X, int Y, 
                int Width, int Height, 
                std::vector<std::string> Images, int FrameRate=System::DefaultSettings["FrameRate"]): 
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