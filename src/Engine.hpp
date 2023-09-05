#ifndef ENGINE_HEADER
#define ENGINE_HEADER

namespace Potato{
    class Engine{
        private:
            SDL_Window* Window;
            SDL_Renderer* Renderer;
            
            std::string Name;
            // std::optional<std::string> Background = std::nullopt;
            std::pair<bool, std::variant<std::string, std::tuple<int, int, int>>> 
                Background = std::make_pair(true, std::make_tuple(
                        System::DefaultSettings["SBR"],
                        System::DefaultSettings["SBB"],
                        System::DefaultSettings["SBG"]
                    ));
            
            Uint32 FrameStart;
            int FrameTime, FPS = System::DefaultSettings["FPS"];

            std::optional<std::string> CurrentText = std::nullopt;
            Uint32 TextSpeed = System::DefaultSettings["TextSpeed"];
            std::thread TextThread;

            std::optional<int> StoryIndex = std::nullopt;
            std::map<int, std::function<void()>> Story;
            std::vector<Character*> Scene;

            void Close();
            void MainLoop();
            void SetRenderColor(std::tuple<int, int, int> rgb, float o);
            void RenderColor(std::tuple<int, int, int> c, int x, int y, int w, int h, float o);
            void RenderImage(std::string is, int x, int y, int w, int h, float o);
            void RenderUI();
            void RenderBackground();
            void RunStory();
            void CheckUIClick(int mx, int my);

            static void OutputText(const std::string& str) {
                for (char c : str) {
                    if (CurrentEngine->CurrentText!=str) return;
                    CurrentEngine->UISet.DialogueBox.TextContent+=c;
                    std::this_thread::sleep_for(std::chrono::milliseconds(CurrentEngine->TextSpeed));
                }
            }

        public: 
            friend class Character;
            friend class UICreator;
            friend class UIElement;

            const int ScreenWidth = System::DefaultSettings["ScreenWidth"];
            const int ScreenHeight = System::DefaultSettings["ScreenHeight"];
            UICreator UISet;
            
            void Run();
            void Step(int i);
            void Jump(int d);
            
            void SetStory(std::map<int, std::function<void()>> sm);
            void SetTextSpeed(Uint32 ts);
            
            void SceneClearCharacter();
            void SceneAddCharacter(Character* ca);
            void SceneRemoveCharacter(Character* ca);
            
            void SceneSetBackgroundColor(std::tuple<int, int, int> c);
            void SceneSetBackgroundImage(std::string bs);
            void SceneClearBackground();
        

        Engine(std::string Name, std::string WindowIcon=""): Name(Name), UISet(this->ScreenWidth, this->ScreenHeight){
            TTF_Init();
            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->ScreenWidth, this->ScreenHeight, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

            this->UISet.DialogueBox.Callback = [&](){
                if (this->Story.count(this->StoryIndex.value())>0)
                    this->RunStory();
            };

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
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_Event event;

        this->RunStory();
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
        UIElement Elems[] = {this->UISet.DialogueBox, this->UISet.NameBox};
        for (auto elem: Elems){
            if (
                MouseX>=elem.X &&
                MouseX<=elem.X+elem.Width &&
                MouseY>=elem.Y &&
                MouseY<=elem.Y+elem.Height
            ) return elem.Callback();
        }
    }

    void Engine::Close(){
        CurrentEngine = nullptr;

        SDL_DestroyRenderer(this->Renderer);
        SDL_DestroyWindow(this->Window);
        SDL_Quit();
    }

    void Engine::MainLoop(){
        SDL_SetRenderDrawColor(this->Renderer, 0,0,0,255);
        SDL_RenderClear(this->Renderer);

        this->RenderBackground();
        for(auto c:this->Scene){
            if (c->Images.size()>1) c->ChangeFrame();
            this->RenderImage(c->Images[c->CurrentFrame], c->X, c->Y, c->Width, c->Height, c->Opacity);
        }
        Engine::RenderUI();

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
            return System::Error("Failed to load image: "+ImgSrc);
        SDL_SetTextureAlphaMod(CTexture, static_cast<int>(Opacity*255));
        SDL_Rect CBounds = {X, Y, Width, Height};
        SDL_RenderCopy(this->Renderer, CTexture, nullptr, &CBounds);
        SDL_DestroyTexture(CTexture);
    }

    void Engine::RenderColor(std::tuple<int, int, int> Color, int X, int Y, int Width, int Height, float Opacity){
        SDL_Rect CBounds = {X, Y, Width, Height};
        this->SetRenderColor(Color, Opacity);
        SDL_RenderFillRect(this->Renderer, &CBounds);
    }

    void Engine::RenderBackground(){
        if (this->Background.first)
            return this->RenderColor(
                    std::get<std::tuple<int, int, int>>(this->Background.second), 
                    0, 0, this->ScreenWidth, this->ScreenHeight, 1);
        this->RenderImage(
            std::get<std::string>(this->Background.second), 
            0, 0, this->ScreenWidth, this->ScreenHeight, 1);
    }

    void Engine::RenderUI(){
        std::vector<UIElement> UIElems = {this->UISet.DialogueBox, this->UISet.NameBox};
        for (auto elem: UIElems){
            if (!elem.Visible) continue;

            switch (elem.BackgroundIsColor){
                case true:
                    this->RenderColor(elem.BackgroundColor, elem.X, elem.Y, elem.Width, elem.Height, elem.Opacity);
                break;

                case false:
                    this->RenderImage(elem.BackgroundImage, elem.X, elem.Y, elem.Width, elem.Height, elem.Opacity);
                break;
            }

            if (elem.Outline.has_value())
                elem.RenderOutline();

            if (elem.TextContent.length()>0)
                elem.DisplayText();
        }
    }

    // story management
    void Engine::RunStory(){
        if (!this->StoryIndex.has_value() || this->Story.find(this->StoryIndex.value())==this->Story.end()) 
            return;
        this->Story[this->StoryIndex.value()]();
    }
    void Engine::Step(int Inc){
        if (!this->StoryIndex.has_value()) return;
        this->StoryIndex.value() +=Inc;
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


    // scene setting
    void Engine::SceneClearCharacter(){
        this->Scene.clear();
    }

    void Engine::SceneAddCharacter(Character* CharAddr){
        this->Scene.push_back(CharAddr);
    }

    void Engine::SceneRemoveCharacter(Character* CharAddr){
        for (int i=0;i<this->Scene.size();i++){
            if (this->Scene[i]==CharAddr){
                this->Scene.erase(this->Scene.begin()+i);
                return;
            }
        }
    }

    void Engine::SceneSetBackgroundColor(std::tuple<int, int, int> Color){
        this->Background = {true, Color};
    }
    void Engine::SceneSetBackgroundImage(std::string BgSrc){
        SDL_Texture* BgTexture = IMG_LoadTexture(this->Renderer, BgSrc.c_str());
        if (BgTexture==nullptr)
            return System::Error("Failed to load background image");
        this->Background = {false, BgSrc};
        SDL_DestroyTexture(BgTexture);
    }
    void Engine::SceneClearBackground(){
        this->Background = {
            true, std::make_tuple(
                System::DefaultSettings["SBR"],
                System::DefaultSettings["SBB"],
                System::DefaultSettings["SBG"]
            )
        };
    }
}

#endif