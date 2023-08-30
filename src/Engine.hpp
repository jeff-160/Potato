#include <windows.h>
#include <string>


class Engine{
    private:
        std::string Name;
        const int Width = 800;
        const int Height = 600;
        void Close();
        void MainLoop();

    public: 
        SDL_Window* Window;
        SDL_Renderer* Renderer;
        Uint32 TextSpeed = 30;
        Uint32 FrameStart;
        int FrameTime, FPS = 100;
        void Run();
    
    Engine(std::string Name): Name(Name){
        this->Window = SDL_CreateWindow(
                                    this->Name.c_str(), 
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                    this->Width, this->Height, 
                                    SDL_WINDOW_SHOWN);
        this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);
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