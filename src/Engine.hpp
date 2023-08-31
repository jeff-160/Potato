namespace Potato{
    class Engine{
        private:
            SDL_Window* Window;
            SDL_Renderer* Renderer;
            
            std::string Name;
            std::string Background;
            
            Uint32 FrameStart;
            int FrameTime, FPS = 100;

            Uint32 TextSpeed = 30;

            int StoryIndex = 0;
            std::map<int, std::function<void()>> Story;
            std::vector<Character*> Scene;

            void Error(std::string e);
            void Close();
            void MainLoop();
            void Render(std::string is, int x, int y, int w, int h);
            void RunStory();

        public: 
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
        

        Engine(std::string Name, std::string WindowIcon=""): Name(Name), UISet(this->ScreenWidth, this->ScreenHeight){
            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->ScreenWidth, this->ScreenHeight, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

            if (WindowIcon=="") return;
            SDL_Surface* Icon = IMG_Load(WindowIcon.c_str());
            if (Icon==nullptr){
                this->Error("Invalid icon file path: " +  WindowIcon);
                return;
            }
            SDL_SetWindowIcon(this->Window, Icon);
            SDL_FreeSurface(Icon);
        }
    };

    // engine operating functions
    void Engine::Error(std::string err){
        std::cerr<<"Error: " << err << std::endl;
        return;
    }

    void Engine::Run(){
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_Event event;

        while (1){
            this->FrameStart = SDL_GetTicks();
            while (SDL_PollEvent(&event)){
                if (event.type==SDL_QUIT) return this->Close();
            }

            MainLoop();
        }
    }

    void Engine::Close(){
        SDL_DestroyRenderer(this->Renderer);
        SDL_DestroyWindow(this->Window);
        SDL_Quit();
    }

    void Engine::MainLoop(){
        SDL_SetRenderDrawColor(this->Renderer, 0,0,0,255);
        SDL_RenderClear(this->Renderer);

        this->Render(this->Background, 0, 0, this->ScreenWidth, this->ScreenHeight);
        for(auto c:this->Scene)
            this->Render(c->Images[c->CurrentFrame], c->X, c->Y, c->Width, c->Height);

        SDL_RenderPresent(this->Renderer);

        this->FrameTime = SDL_GetTicks()-this->FrameStart;
        if (this->FPS>this->FrameTime)
            SDL_Delay(this->FPS-this->FrameTime);
    }

    void Engine::Render(std::string ImgSrc, int X, int Y, int Width, int Height){
        SDL_Texture* CTexture = IMG_LoadTexture(this->Renderer, ImgSrc.c_str());
        SDL_Rect CBounds = {X, Y, Width, Height};
        SDL_RenderCopy(this->Renderer, CTexture, nullptr, &CBounds);
        SDL_DestroyTexture(CTexture);
    }

    // story management
    void Engine::RunStory(){
        this->Story[this->StoryIndex]();
    }
    void Engine::Step(int Inc){
        this->StoryIndex+=Inc;
        this->RunStory();
    }
    void Engine::Jump(int Dest){
        this->StoryIndex = Dest;
        this->RunStory();
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
            return this->Error("Failed to load background image");
        this->Background = BgSrc;
        SDL_DestroyTexture(BgTexture);
    }
}