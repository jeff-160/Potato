#ifndef ENGINE_HEADER
#define ENGINE_HEADER

namespace Potato{
    class Engine{
        private:
            SDL_Window* Window;
            SDL_Renderer* Renderer;
            
            std::string Name;
            std::optional<std::string> Background = std::nullopt;
            
            Uint32 FrameStart;
            int FrameTime, FPS = 100;

            std::optional<std::string> CurrentText = std::nullopt;
            Uint32 TextSpeed = 50;
            std::thread TextThread;

            std::optional<int> StoryIndex = 0;
            std::map<int, std::function<void()>> Story;
            std::vector<Character*> Scene;

            void Close();
            void MainLoop();
            void Render(std::string is, int x, int y, int w, int h);
            void RenderUI();
            void RunStory();

            static void OutputText(const std::string& str) {
                for (char c : str) {
                    if (CurrentEngine->CurrentText!=str) return;
                    CurrentEngine->UISet.DialogueBox.TextContent+=c;
                    std::this_thread::sleep_for(std::chrono::milliseconds(CurrentEngine->TextSpeed));
                }
            }

        public: 
            friend class Character;
            friend class UIElement;

            const int ScreenWidth = 900;
            const int ScreenHeight = 600;
            UICreator UISet;
            
            void Run();
            void Step(int i);
            void Jump(int d);
            void SetStory(std::map<int, std::function<void()>> sm);
            void SetTextSpeed(Uint32 ts);
            void ClearScene();
            void SceneAddCharacter(Character* ca);
            void SceneRemoveCharacter(Character* ca);
            void SceneSetBackground(std::string bs);
            void SceneClearBackground();
        

        Engine(std::string Name, std::string WindowIcon=""): Name(Name), UISet(this->ScreenWidth, this->ScreenHeight){
            TTF_Init();
            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->ScreenWidth, this->ScreenHeight, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

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
                    if (
                        mx>=this->UISet.DialogueBox.X &&
                        mx<=this->UISet.DialogueBox.X+this->UISet.DialogueBox.Width &&
                        my>=this->UISet.DialogueBox.Y &&
                        my<=this->UISet.DialogueBox.Y+this->UISet.DialogueBox.Height &&
                        this->Story.count(this->StoryIndex.value())>0
                    ) this->RunStory();
                }
            }

            MainLoop();
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

        if (this->Background.has_value())
            this->Render(this->Background.value(), 0, 0, this->ScreenWidth, this->ScreenHeight);
        for(auto c:this->Scene)
            this->Render(c->Images[c->CurrentFrame], c->X, c->Y, c->Width, c->Height);
        Engine::RenderUI();

        SDL_RenderPresent(this->Renderer);

        this->FrameTime = SDL_GetTicks()-this->FrameStart;
        if (this->FPS>this->FrameTime)
            SDL_Delay(this->FPS-this->FrameTime);
    }

    // rendering
    void Engine::Render(std::string ImgSrc, int X, int Y, int Width, int Height){
        SDL_Texture* CTexture = IMG_LoadTexture(this->Renderer, ImgSrc.c_str());
        if (CTexture==nullptr)
            return System::Error("Failed to load image: "+ImgSrc);
        SDL_Rect CBounds = {X, Y, Width, Height};
        SDL_RenderCopy(this->Renderer, CTexture, nullptr, &CBounds);
        SDL_DestroyTexture(CTexture);
    }

    void Engine::RenderUI(){
        std::vector<UIElement> UIElems = {this->UISet.DialogueBox, this->UISet.NameBox};
        for (auto elem: UIElems){
            if (!elem.Visible) continue;

            switch (elem.BackgroundIsColor){
                case true:
                    {
                        SDL_Rect EBounds = {elem.X, elem.Y, elem.Width, elem.Height};
                        int r, g, b;
                        std::tie(r, g, b) = elem.BackgroundColor;
                        SDL_SetRenderDrawColor(this->Renderer, r, g, b, static_cast<int>(elem.Opacity*255));
                        SDL_RenderFillRect(this->Renderer, &EBounds);
                    }
                break;

                case false:
                    Engine::Render(elem.BackgroundImage, elem.X, elem.Y, elem.Width, elem.Height);
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
        this->StoryIndex.value()+=Inc;
    }
    void Engine::Jump(int Dest){
        this->StoryIndex.value() = Dest;
    }


    // engine modifications
    void Engine::SetStory(std::map<int, std::function<void()>> SMap){
        this->Story = SMap;
    }

    void Engine::SetTextSpeed(Uint32 TextSpeed){
        this->TextSpeed = TextSpeed;
    }


    // scene setting
    void Engine::ClearScene(){
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

    void Engine::SceneSetBackground(std::string BgSrc){
        SDL_Texture* BgTexture = IMG_LoadTexture(this->Renderer, BgSrc.c_str());
        if (BgTexture==nullptr)
            return System::Error("Failed to load background image");
        this->Background = BgSrc;
        SDL_DestroyTexture(BgTexture);
    }
    void Engine::SceneClearBackground(){
        this->Background = std::nullopt;
    }
}

#endif