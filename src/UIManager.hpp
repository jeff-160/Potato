struct UIBox{
    int X, Y;
    int Width, Height;
    bool visible = true;
};

namespace Potato{
    class UICreator{
        private:
            UIBox DialogueBox;
            UIBox NameBox;
        public:
            friend class Engine;
        
        UICreator(int ScreenWidth, int ScreenHeight){
            std::pair<int, int> DBDim = {static_cast<int>(ScreenWidth*0.8), static_cast<int>(ScreenHeight*0.5)};
            this->DialogueBox = {
                ScreenWidth/2-DBDim.first/2, ScreenHeight/2-DBDim.second/2,
                DBDim.first, DBDim.second
            };

            std::pair<int, int> NBDim = {static_cast<int>(DBDim.first*0.3), static_cast<int>(DBDim.second*0.4)};
            this->NameBox = {
                this->DialogueBox.X, this->DialogueBox.Y-NBDim.second,
                NBDim.first, NBDim.second
            };
        }
    };
}