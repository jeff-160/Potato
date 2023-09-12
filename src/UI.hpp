#ifndef UI_HEADER
#define UI_HEADER

namespace Potato{
    struct UIElement{
        float X, Y;
        float Width, Height;
        
        float Opacity = 1;
        
        std::optional<std::variant<std::string, std::tuple<int, int, int>>> Background = 
                std::make_tuple(System::DefaultSettings["BGR"], System::DefaultSettings["BGG"], System::DefaultSettings["BGB"]);
        std::tuple<int, int, int> 
            TextColor = std::make_tuple(System::DefaultSettings["TCR"], System::DefaultSettings["TCG"], System::DefaultSettings["TCB"]);
        
        std::optional<std::tuple<int, int, int>> OutlineColor = std::make_tuple(0,0,0);
        int OutlineThickness = System::DefaultSettings["DefaultOutline"]; 
        
        int Margin = System::DefaultSettings["Margin"];
        std::string TextContent = "";
        int FontSize = System::DefaultSettings["FontSize"];
        int TextAlignMode = 0;
        
        bool Visible = true;

        std::function<void()> Callback = [](){};

        void DisplayText();
        void RenderOutline();
        void Show(){this->Visible=true;};
        void Hide(){this->Visible=false;};

        UIElement(int X, int Y, int Width, int Height): X(X), Y(Y), Width(Width), Height(Height){}
    };

    class UISet{
        friend struct Engine;
        friend struct Transitions;
        
        private:
            UIElement TransitionScreen;
        
        public:
            UIElement DialogueBox;
            UIElement NameBox;
        
        UISet(float ScreenWidth, float ScreenHeight): DialogueBox(0,0,0,0), NameBox(0,0,0,0), TransitionScreen(0,0,0,0){
            std::pair<int, int>
                DBDim = std::make_pair(
                        ScreenWidth*System::DefaultSettings["DBW"], 
                        ScreenHeight*System::DefaultSettings["DBH"]
                    ),
                NBDim = std::make_pair(
                        DBDim.first*System::DefaultSettings["NBW"], 
                        DBDim.second*System::DefaultSettings["NBH"]
                    );

            this->DialogueBox.X = ScreenWidth/2-DBDim.first/2, this->DialogueBox.Y = ScreenHeight-DBDim.second-System::DefaultSettings["DBYMargin"];
            this->DialogueBox.Width = DBDim.first, this->DialogueBox.Height = DBDim.second;

            this->NameBox.X = this->DialogueBox.X+System::DefaultSettings["NBXMargin"], this->NameBox.Y = this->DialogueBox.Y-NBDim.second;
            this->NameBox.Width = NBDim.first, this->NameBox.Height = NBDim.second;
            this->NameBox.TextAlignMode = 1;
            this->NameBox.OutlineThickness = System::DefaultSettings["NameBoxOutline"];
            this->NameBox.Visible = false;

            this->TransitionScreen.X = this->TransitionScreen.Y = 0;
            this->TransitionScreen.Background = std::make_tuple(0,0,0);
            this->TransitionScreen.OutlineThickness = 0;
            this->TransitionScreen.Width = ScreenWidth, this->TransitionScreen.Height = ScreenHeight;
            this->TransitionScreen.Visible = false;
        }
    };
}

#endif