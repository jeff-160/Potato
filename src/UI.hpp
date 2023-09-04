#ifndef UI_HEADER
#define UI_HEADER

namespace Potato{
    struct UIElement{
        int X, Y;
        int Width, Height;
        
        double Opacity = 1;
        bool BackgroundIsColor = true;
        std::tuple<int, int, int> 
            BackgroundColor = std::make_tuple(System::DefaultSettings["BGR"], System::DefaultSettings["BGG"], System::DefaultSettings["BGB"]),
            TextColor = std::make_tuple(System::DefaultSettings["TCR"], System::DefaultSettings["TCG"], System::DefaultSettings["TCB"]);
        std::string BackgroundImage = "";
        
        std::optional<std::tuple<int, int, int>> Outline = std::make_tuple(0,0,0);
        int OutlineThickness = System::DefaultSettings["DefaultOutline"]; 
        
        int Margin = System::DefaultSettings["Margin"];
        std::string TextContent = "";
        std::optional<std::string> Font = std::nullopt;
        int FontSize = System::DefaultSettings["FontSize"];
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
                DBDim = std::make_pair(
                        static_cast<int>(ScreenWidth*System::DefaultSettings["DBW"]), 
                        static_cast<int>(ScreenHeight*System::DefaultSettings["DBH"])
                    ),
                NBDim = std::make_pair(
                        static_cast<int>(DBDim.first*System::DefaultSettings["NBW"]), 
                        static_cast<int>(DBDim.second*System::DefaultSettings["NBH"])
                    );

            this->DialogueBox.X = ScreenWidth/2-DBDim.first/2, this->DialogueBox.Y = ScreenHeight-DBDim.second-System::DefaultSettings["DBYMargin"];
            this->DialogueBox.Width = DBDim.first, this->DialogueBox.Height = DBDim.second;

            this->NameBox.X = this->DialogueBox.X+System::DefaultSettings["NBXMargin"], this->NameBox.Y = this->DialogueBox.Y-NBDim.second;
            this->NameBox.Width = NBDim.first, this->NameBox.Height = NBDim.second;
            this->NameBox.TextAlignMode = 1;
            this->NameBox.OutlineThickness = System::DefaultSettings["NameBoxOutline"];
            this->NameBox.Visible = false;
        }
    };
}

#endif