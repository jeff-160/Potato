namespace Potato{
    class Engine{
        private:
            std::string Name;
            
            const int Width = 900;
            const int Height = 600;

            Uint32 FrameStart;
            int FrameTime, FPS = 100;

            Uint32 TextSpeed = 30;

            void Close();
            void MainLoop();

        public: 
            SDL_Window* Window;
            SDL_Renderer* Renderer;
            
            
            std::map<int, std::function<void()>> Story;
            
            void Run();
            void SetTextSpeed(Uint32 ts);
        
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
            FrameStart = SDL_GetTicks();
            while (SDL_PollEvent(&event)){
                if (event.type==SDL_QUIT) return this->Close();
            }

            MainLoop();
        }
    }

    void Engine::Close(){
        SDL_DestroyRenderer(Renderer);
        SDL_DestroyWindow(Window);
        SDL_Quit();
    }

    void Engine::MainLoop(){
        SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
        SDL_RenderClear(Renderer);

        SDL_RenderPresent(Renderer);

        FrameTime = SDL_GetTicks()-FrameStart;
        if (FPS>FrameTime)
            SDL_Delay(FPS-FrameTime);
    }


    void Engine::SetTextSpeed(Uint32 TextSpeed){
        this->TextSpeed = TextSpeed;
    }
}