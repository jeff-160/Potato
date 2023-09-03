#ifndef UI_HEADER
#define UI_HEADER

namespace Potato{
    struct UIElement{
        int X, Y;
        int Width, Height;
        
        double Opacity = 1;
        bool BackgroundIsColor = true;
        std::tuple<int, int, int> BackgroundColor = {255, 255, 255}, TextColor = {0,0,0};
        std::string BackgroundImage = "";
        
        std::optional<std::tuple<int, int, int>> Outline = std::make_tuple(0,0,0);
        int OutlineThickness = 2; 
        
        int Margin = 10;
        std::string TextContent = "";
        std::optional<std::string> Font = std::nullopt;
        int FontSize = 20;
        int TextAlignMode = 0;
        
        bool Visible = true;

        void DisplayText();
        void RenderOutline();


        UIElement(int X, int Y, int Width, int Height): X(X), Y(Y), Width(Width), Height(Height){}
    };

    class UICreator{
        private:
            UIElement DialogueBox;
            UIElement NameBox;
        public:
            friend class Engine;
            friend class Character;
        
        UICreator(int ScreenWidth, int ScreenHeight): DialogueBox(0,0,0,0), NameBox(0,0,0,0){
            std::pair<int, int> 
                DBDim = std::make_pair(static_cast<int>(ScreenWidth*0.8), static_cast<int>(ScreenHeight*0.4)),
                NBDim = std::make_pair(static_cast<int>(DBDim.first*0.3), static_cast<int>(DBDim.second*0.2));

            this->DialogueBox.X = ScreenWidth/2-DBDim.first/2, this->DialogueBox.Y = ScreenHeight-DBDim.second-10;
            this->DialogueBox.Width = DBDim.first, this->DialogueBox.Height = DBDim.second;

            this->NameBox.X = this->DialogueBox.X+20, this->NameBox.Y = this->DialogueBox.Y-NBDim.second;
            this->NameBox.Width = NBDim.first, this->NameBox.Height = NBDim.second;
            this->NameBox.TextAlignMode = 1;
            this->NameBox.Visible = false;
        }
    };
}

#endif