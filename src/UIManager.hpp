struct UIElement{
    int X, Y;
    int Width, Height;
    bool BackgroundIsColor = true;
    std::tuple<int, int, int> BackgroundColor = {255, 255, 255};
    std::string BackgroundImage = "";
    double Opacity = 1;
    bool Visible = true;
};

namespace Potato{
    class UICreator{
        private:
            UIElement DialogueBox;
            UIElement NameBox;
        public:
            friend class Engine;
        
        UICreator(int ScreenWidth, int ScreenHeight){
            std::pair<int, int> DBDim = {static_cast<int>(ScreenWidth*0.8), static_cast<int>(ScreenHeight*0.4)};
            this->DialogueBox = {
                ScreenWidth/2-DBDim.first/2, ScreenHeight-DBDim.second-10,
                DBDim.first, DBDim.second
            };

            std::pair<int, int> NBDim = {static_cast<int>(DBDim.first*0.3), static_cast<int>(DBDim.second*0.2)};
            this->NameBox = {
                this->DialogueBox.X+20, this->DialogueBox.Y-NBDim.second,
                NBDim.first, NBDim.second
            };
            this->NameBox.Visible = false;
        }
    };
}