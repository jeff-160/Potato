#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

namespace Potato{
    class Character{
        private:
            int CurrentFrame = 0;

        public:
            friend class Engine;
            
            std::string Name;
            double X, Y;
            int Width, Height;
            Uint32 Opacity = 1;

            std::vector<std::string> Images;
            int FrameRate;

            void Draw(SDL_Renderer* r);
            void Speak(std::string t);
        
        Character(std::string Name, int X, int Y, int Width, int Height, std::vector<std::string> Images, int FrameRate=30): 
            Name(Name), X(X), Y(Y), Width(Width), Height(Height), Images(Images), FrameRate(FrameRate){}
    };
}

#endif