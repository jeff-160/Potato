#ifndef ENGINE_HEADER
#define ENGINE_HEADER

namespace Potato{
    struct Engine{
        friend struct System;
        friend struct SceneCreator;
        friend struct Character;
        friend struct UISet;
        friend struct UIElement;
        friend struct Effects;
        friend struct Transitions;
        
        private:
            SDL_Window* Window;
            SDL_Renderer* Renderer;
            
            std::string Name;
            
            Uint32 FrameStart;
            int FrameTime, FPS = static_cast<int>(1000/System::DefaultSettings["FPS"]);

            std::optional<std::string> CurrentText = std::nullopt;
            Uint32 TextSpeed = System::DefaultSettings["TextSpeed"];
            std::thread TextThread;

            std::optional<int> StoryIndex = std::nullopt;
            std::map<int, std::function<void()>> Story;

            std::pair<std::string, std::variant<std::string, std::tuple<int, int, int>>> StartScreen = 
                std::make_pair("", std::make_tuple(0,0,0));
            UIElement StartButton;
            UIElement EndScreen, EndMask, EndText;
            std::vector<UIElement*> EndUI;

            std::optional<std::string> Font = std::nullopt;            

            void Close();
            void MainLoop();
            void SetRenderColor(std::tuple<int, int, int> rgb, float o);
            void RenderColor(std::tuple<int, int, int> c, int x, int y, float w, float h, float o);
            void RenderImage(std::string is, int x, int y, int w, int h, float o);
            void RenderUI(UIElement e);
            void RunStory();
            void CheckUIClick(int mx, int my);

            static void OutputText(const std::string& str) {
                for (char c : str) {
                    if (CurrentEngine->CurrentText!=str) {
                        CurrentEngine->UI.DialogueBox.TextContent = "";
                        return;
                    }
                    CurrentEngine->UI.DialogueBox.TextContent+=c;
                    Threading::Delay(CurrentEngine->TextSpeed);
                }
            }

        public:
            const int ScreenWidth = System::DefaultSettings["ScreenWidth"];
            const int ScreenHeight = System::DefaultSettings["ScreenHeight"];
            UISet UI;
            std::vector<UIElement*> UIElements;

            SceneCreator Scene;

            int CharacterFadeRate = static_cast<int>(System::DefaultSettings["CharacterFadeRate"]);
            int CharacterSlideRate = static_cast<int>(System::DefaultSettings["CharacterSlideRate"]);

            int TransitionRate = static_cast<int>(System::DefaultSettings["TransitionRate"]);
            
            void Run();
            void Step(int i);
            void Jump(int d);
            
            void SetStory(std::map<int, std::function<void()>> sm);
            void SetTextSpeed(Uint32 ts);
            void SetFont(std::string f);

            void SetTitleScreen(std::string t, int f, std::variant<std::string, std::tuple<int, int, int>> b);
            void DisplayStartScreen();
            void End(std::vector<std::string> t, std::variant<std::string, std::tuple<int, int, int>> b);
        

        Engine(std::string Name, std::string WindowIcon=""): 
        Name(Name), UI(this->ScreenWidth, this->ScreenHeight), 
        EndScreen(0,0,0,0), EndMask(0,0,0,0), EndText(0,0,0,0), StartButton(0,0,0,0){
            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->ScreenWidth, this->ScreenHeight, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawBlendMode(this->Renderer, SDL_BLENDMODE_BLEND);

            this->UIElements = {&this->UI.DialogueBox, &this->UI.NameBox, &this->UI.TransitionScreen};
            this->UI.DialogueBox.Callback = [&](){
                if (this->StoryIndex.has_value() &&
                    this->Story.count(this->StoryIndex.value())>0
                ) this->RunStory();
            };
            
            // end screen
            this->EndScreen.Width = this->EndMask.Width = this->ScreenWidth;
            this->EndScreen.Height = this->EndMask.Height = this->ScreenHeight;
            this->EndMask.Opacity = 0.5; this->EndMask.Background = std::make_tuple(0,0,0);
            this->EndText.Width = this->ScreenWidth*0.7, this->EndText.Height = this->ScreenHeight*0.7;
            this->EndText.Background = std::nullopt;
            this->EndText.TextColor = {255, 255, 255};
            this->EndText.X = static_cast<int>((this->ScreenWidth-this->EndText.Width)/2);
            this->EndText.Y = static_cast<int>((this->ScreenHeight-this->EndText.Height)/2);
            this->EndUI = {&this->EndScreen, &this->EndMask, &this->EndText};
            for (auto e:this->EndUI) {
                e->Visible = false;
                e->OutlineThickness = 0;
            }

            this->StartButton.Width = this->ScreenWidth/System::DefaultSettings["StartButtonWidth"];
            this->StartButton.Height = this->ScreenHeight/System::DefaultSettings["StartButtonHeight"];
            this->StartButton.Background = std::make_tuple(237, 231, 225);
            this->StartButton.TextContent = "Start"; 
            this->StartButton.FontSize = this->StartButton.Height/2; 
            this->StartButton.TextAlignMode = 1;
            this->StartButton.X = static_cast<int>((this->ScreenWidth-this->StartButton.Width)/2);
            this->StartButton.Y = static_cast<int>((this->ScreenHeight-this->StartButton.Height)/2);
            this->StartButton.Callback = [&, this](){
                for (auto e:this->EndUI) e->Visible = false;
                this->EndUI.pop_back();
                this->StartButton.Visible = false;
                this->RunStory();
            };
            this->EndUI.push_back(&this->StartButton);

            if (WindowIcon=="") return;
            SDL_Surface* Icon = IMG_Load(WindowIcon.c_str());
            if (Icon==nullptr){
                System::Error("Invalid icon file path: " +  WindowIcon);
                return;
            }
            SDL_SetWindowIcon(this->Window, Icon);
            SDL_FreeSurface(Icon);
        }
    };

    // engine operating functions
    void Engine::Run(){
        CurrentEngine = this;

        ShowWindow(GetConsoleWindow(), SW_HIDE);
        SDL_Event event;

        this->DisplayStartScreen();

        while (1){
            this->FrameStart = SDL_GetTicks();
            while (SDL_PollEvent(&event)){
                if (event.type==SDL_QUIT) return this->Close();
                else if (event.type==SDL_MOUSEBUTTONUP){
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    this->CheckUIClick(mx, my);
                }
            }

            MainLoop();
        }
    }

    void Engine::CheckUIClick(int MouseX, int MouseY){
        std::vector<UIElement> elems = this->Scene.ChoiceBoxes;
        for (auto e:this->UIElements) elems.push_back(*e);
        elems.push_back(this->StartButton);
        for (auto elem: elems){
            if (
                elem.Visible &&
                MouseX>=elem.X &&
                MouseX<=elem.X+elem.Width &&
                MouseY>=elem.Y &&
                MouseY<=elem.Y+elem.Height
            ) return elem.Callback();
        }
    }

    void Engine::Close(){
        SDL_DestroyRenderer(this->Renderer);
        SDL_DestroyWindow(this->Window);
        SDL_Quit();
        Mix_Quit();
        
        CurrentEngine = nullptr;
    }

    void Engine::MainLoop(){
        SDL_SetRenderDrawColor(this->Renderer, 0,0,0,255);
        SDL_RenderClear(Renderer);

        this->Scene.RenderBackground();
        for(auto c:this->Scene.Characters){
            if (c->Images.size()>1) c->ChangeFrame();
            this->RenderImage(c->Images[c->CurrentFrame], c->X, c->Y, c->Width, c->Height, c->Opacity);
        }
        for (auto e:this->UIElements) this->RenderUI(*e);
        for (auto c:this->Scene.ChoiceBoxes) this->RenderUI(c);
        for (auto e:this->EndUI) this->RenderUI(*e);

        SDL_RenderPresent(this->Renderer);

        this->FrameTime = SDL_GetTicks()-this->FrameStart;
        if (this->FPS>this->FrameTime)
            SDL_Delay(this->FPS-this->FrameTime);
    }

    // rendering
    void Engine::SetRenderColor(std::tuple<int, int, int> RGB, float Opacity){
        int r, g, b;
        std::tie(r, g, b) = RGB;
        SDL_SetRenderDrawColor(this->Renderer, r, g, b, static_cast<int>(Opacity*255));
    }

    void Engine::RenderImage(std::string ImgSrc, int X, int Y, int Width, int Height, float Opacity){
        SDL_Texture* CTexture = IMG_LoadTexture(this->Renderer, ImgSrc.c_str());
        if (CTexture==nullptr)
            System::Error("Failed to load image: "+ImgSrc);
        SDL_SetTextureAlphaMod(CTexture, static_cast<int>(Opacity*255));
        SDL_FRect CBounds = {X, Y, Width, Height};
        SDL_RenderCopyF(this->Renderer, CTexture, nullptr, &CBounds);
        SDL_DestroyTexture(CTexture);
    }

    void Engine::RenderColor(std::tuple<int, int, int> Color, int X, int Y, float Width, float Height, float Opacity){
        SDL_FRect CBounds = {X, Y, Width, Height};
        this->SetRenderColor(Color, Opacity);
        SDL_RenderFillRectF(this->Renderer, &CBounds);
    }

    void Engine::RenderUI(UIElement elem){
        if (!elem.Visible) return;

        if (elem.Background.has_value()){
            switch (std::holds_alternative<std::string>(elem.Background.value())){
                case true:
                    this->RenderImage(
                        std::get<std::string>(elem.Background.value()),
                        elem.X, elem.Y, elem.Width, elem.Height, elem.Opacity
                    );
                break;

                case false:
                    this->RenderColor(
                        std::get<std::tuple<int, int, int>>(elem.Background.value()),
                        elem.X, elem.Y, elem.Width, elem.Height, elem.Opacity
                    );
                break;
            }
        }

        if (elem.OutlineColor.has_value())
            elem.RenderOutline();

        if (elem.TextContent.length()>0)
            elem.DisplayText();
    }

    // story management
    void Engine::RunStory(){
        if (!this->StoryIndex.has_value() || this->Story.find(this->StoryIndex.value())==this->Story.end()) 
            return;
        
        this->Scene.Transition();
        CurrentEngine->UI.DialogueBox.Hide(); 
        CurrentEngine->UI.NameBox.Hide();
        
        this->Story[this->StoryIndex.value()]();
    }

    void Engine::Step(int Inc){
        if (!this->StoryIndex.has_value()) return;
        this->StoryIndex.value() += Inc;
    }
    void Engine::Jump(int Dest){
        this->StoryIndex = Dest;
    }

    // engine modifications
    void Engine::SetStory(std::map<int, std::function<void()>> SMap){
        this->Story = SMap;
    }

    void Engine::SetTextSpeed(Uint32 TextSpeed){
        this->TextSpeed = TextSpeed;
    }

    void Engine::SetFont(std::string Font){
        TTF_Font* EngineFont = TTF_OpenFont(Font.c_str(), 0);
        if (EngineFont==nullptr)
            return System::Error("Failed to load font: " + Font);
        this->Font = Font;
        TTF_CloseFont(EngineFont);
    }

    // engine start and end
    void Engine::SetTitleScreen(
        std::string Title, 
        int FontSize=System::DefaultSettings["StartScreenFontSize"],
        std::variant<std::string, std::tuple<int, int, int>> Background=std::make_tuple(0,0,0)){
        this->StartScreen = std::make_pair(Title, Background);
        this->EndText.FontSize = FontSize;
    }
    void Engine::DisplayStartScreen(){
        for (auto u:this->UIElements) u->Visible = false;
        for (auto e:this->EndUI) e->Visible = true;
        this->EndScreen.Background = this->StartScreen.second;

        this->EndText.TextAlignMode = 1;
        this->EndText.TextContent = this->StartScreen.first;
    }

    void Engine::End(std::vector<std::string> Texts, std::variant<std::string, std::tuple<int, int, int>> Background=std::make_tuple(0,0,0)){
        this->Scene.SetBackground(Background);

        for (auto e: this->EndUI) e->Visible = true;
        this->EndScreen.Background = Background;
        this->EndText.TextAlignMode = 0;
        this->EndText.FontSize = this->EndText.Height/System::DefaultSettings["EndScreenFontSize"];

        Threading::RunAsync(
            [&, Texts](){
                for (std::string Text:Texts){
                    CurrentEngine->EndText.TextContent = "";
                    for (char c : Text) {
                        CurrentEngine->EndText.TextContent+=c;
                        Threading::Delay(CurrentEngine->TextSpeed);
                    }
                    Threading::Delay(3000);
                }
            }
        );
    }
}

#endif