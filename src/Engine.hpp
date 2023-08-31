namespace Potato{
    class Engine{
        private:
            std::string Name;
            
            Uint32 FrameStart;
            int FrameTime, FPS = 100;

            Uint32 TextSpeed = 30;

            void Close();
            void MainLoop();

        public: 
            const int Width = 900;
            const int Height = 600;

            SDL_Window* Window;
            SDL_Renderer* Renderer;
            
            std::map<int, std::function<void()>> Story;
            std::vector<Character*> Scene;
            
            void Run();
            void SetTextSpeed(Uint32 ts);
            void ClearScene();
            void SceneAdd(Character* ca);
            void SceneRemove(Character* ca);
        

        Engine(std::string Name, std::string WindowIcon=""): Name(Name){
            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->Width, this->Height, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

            if (WindowIcon=="") return;
            SDL_Surface* Icon = IMG_Load(WindowIcon.c_str());
            if (Icon==nullptr){
                std::cerr<<"Invalid icon file path: " << WindowIcon << std::endl;
                return;
            }
            SDL_SetWindowIcon(this->Window, Icon);
            SDL_FreeSurface(Icon);
        }
    };


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

        for(auto c:this->Scene)
            c->Draw(this->Renderer);

        SDL_RenderPresent(this->Renderer);

        this->FrameTime = SDL_GetTicks()-this->FrameStart;
        if (this->FPS>this->FrameTime)
            SDL_Delay(this->FPS-this->FrameTime);
    }


    void Engine::SetTextSpeed(Uint32 TextSpeed){
        this->TextSpeed = TextSpeed;
    }

    void Engine::ClearScene(){
        this->Scene.clear();
    }
    void Engine::SceneAdd(Character* CharAddr){
        this->Scene.push_back(CharAddr);
    }
    void Engine::SceneRemove(Character* CharAddr){
        for (int i=0;i<this->Scene.size();i++){
            if (this->Scene[i]==CharAddr){
                this->Scene.erase(this->Scene.begin()+i);
                return;
            }
        }
    }
}