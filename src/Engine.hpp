#include <windows.h>
#include <string>


class Engine{
    private:
        std::string Name;
        const int Width = 800;
        const int Height = 600;
        void Close();

    public: 
        SDL_Window* Window;
        SDL_Renderer* Renderer;

        uint32_t TextSpeed = 30;
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
        while (SDL_PollEvent(&event)){
            if (event.type==SDL_QUIT) return this->Close();
        }
    }
}

void Engine::Close(){
    SDL_DestroyRenderer(this->Renderer);
    SDL_DestroyWindow(this->Window);
    SDL_Quit();
}