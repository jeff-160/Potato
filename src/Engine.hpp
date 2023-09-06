#ifndef ENGINE_HEADER
#define ENGINE_HEADER

namespace Potato{
    class Engine{
        private:
            SDL_Window* Window;
            SDL_Renderer* Renderer;
            SDL_Renderer* TRenderer;
            
            std::string Name;
            
            Uint32 FrameStart;
            int FrameTime, FPS = static_cast<int>(1000/System::DefaultSettings["FPS"]);

            std::optional<std::string> CurrentText = std::nullopt;
            Uint32 TextSpeed = System::DefaultSettings["TextSpeed"];
            std::thread TextThread;

            std::optional<int> StoryIndex = std::nullopt;
            std::map<int, std::function<void()>> Story;

            void Close();
            void MainLoop();
            void SetRenderColor(SDL_Renderer* r, std::tuple<int, int, int> rgb, float o);
            void RenderColor(SDL_Renderer* r, std::tuple<int, int, int> c, int x, int y, int w, int h, float o);
            void RenderImage(SDL_Renderer* r, std::string is, int x, int y, int w, int h, float o);
            void RenderUI();
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
            friend class SceneCreator;
            friend class Character;
            friend class UICreator;
            friend class UIElement;
            friend class Effects;
            friend class Transitions;

            const int ScreenWidth = System::DefaultSettings["ScreenWidth"];
            const int ScreenHeight = System::DefaultSettings["ScreenHeight"];
            UICreator UISet;

            SceneCreator Scene;

            int FadeRate = static_cast<int>(System::DefaultSettings["FadeRate"]);
            int SlideRate = static_cast<int>(System::DefaultSettings["SlideRate"]);
            float FadeSpeed = System::DefaultSettings["FadeSpeed"];
            
            void Run();
            void Step(int i);
            void Jump(int d);
            
            void SetStory(std::map<int, std::function<void()>> sm);
            void SetTextSpeed(Uint32 ts);
        

        Engine(std::string Name, std::string WindowIcon=""): Name(Name), UISet(this->ScreenWidth, this->ScreenHeight){
            TTF_Init();
            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->ScreenWidth, this->ScreenHeight, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = this->TRenderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

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
        SDL_Init(SDL_INIT_VIDEO);

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
        SDL_DestroyRenderer(this->Renderer);
        SDL_DestroyWindow(this->Window);
        SDL_Quit();
        
        CurrentEngine = nullptr;
    }

    void Engine::MainLoop(){
        SDL_SetRenderDrawColor(this->Renderer, 0,0,0,255);
        SDL_RenderClear(this->Renderer);

        this->Scene.RenderBackground();
        for(auto c:this->Scene.Characters){
            if (c->Images.size()>1) c->ChangeFrame();
            this->RenderImage(this->Renderer, c->Images[c->CurrentFrame], c->X, c->Y, c->Width, c->Height, c->Opacity);
        }
        Engine::RenderUI();

        SDL_RenderPresent(this->Renderer);
        SDL_RenderPresent(this->TRenderer);

        this->FrameTime = SDL_GetTicks()-this->FrameStart;
        if (this->FPS>this->FrameTime)
            SDL_Delay(this->FPS-this->FrameTime);
    }

    // rendering
    void Engine::SetRenderColor(SDL_Renderer* Renderer, std::tuple<int, int, int> RGB, float Opacity){
        int r, g, b;
        std::tie(r, g, b) = RGB;
        SDL_SetRenderDrawColor(Renderer, r, g, b, static_cast<int>(Opacity*255));
    }

    void Engine::RenderImage(SDL_Renderer* Renderer, std::string ImgSrc, int X, int Y, int Width, int Height, float Opacity){
        SDL_Texture* CTexture = IMG_LoadTexture(this->Renderer, ImgSrc.c_str());
        if (CTexture==nullptr)
            return System::Error("Failed to load image: "+ImgSrc);
        SDL_SetTextureAlphaMod(CTexture, static_cast<int>(Opacity*255));
        SDL_FRect CBounds = {X, Y, Width, Height};
        SDL_RenderCopyF(Renderer, CTexture, nullptr, &CBounds);
        SDL_DestroyTexture(CTexture);
    }

    void Engine::RenderColor(SDL_Renderer* Renderer, std::tuple<int, int, int> Color, int X, int Y, int Width, int Height, float Opacity){
        SDL_Rect CBounds = {X, Y, Width, Height};
        this->SetRenderColor(this->Renderer, Color, Opacity);
        SDL_RenderFillRect(Renderer, &CBounds);
    }

    void Engine::RenderUI(){
        std::vector<UIElement> UIElems = {this->UISet.DialogueBox, this->UISet.NameBox};
        for (auto elem: UIElems){
            if (!elem.Visible) continue;

            switch (elem.BackgroundIsColor){
                case true:
                    this->RenderColor(this->Renderer, elem.BackgroundColor, elem.X, elem.Y, elem.Width, elem.Height, elem.Opacity);
                break;

                case false:
                    this->RenderImage(this->Renderer, elem.BackgroundImage, elem.X, elem.Y, elem.Width, elem.Height, elem.Opacity);
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
        this->Scene.Transition();
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
}

#endif