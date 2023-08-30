namespace Potato{
    class Character{
        public:
            std::string Name;
            int X, Y;
            int Width, Height;
            Uint32 Opacity = 1;

            void Speak(std::string t);
        
        Character(std::string Name, int X, int Y, int Width, int Height): Name(Name), X(X), Y(Y), Width(Width), Height(Height){

        }
    };

    void Character::Speak(std::string Text){
        
    }
}