#ifndef ENGINE_HEADER
#define ENGINE_HEADER

namespace Potato{
    struct Engine{
        friend struct System;
        friend struct SceneCreator;
        friend struct Character;
        friend struct DialogueUISet;
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

            void Close();
            void MainLoop();
            void SetRenderColor(std::tuple<int, int, int> rgb, float o);
            void RenderColor(std::tuple<int, int, int> c, int x, int y, float w, float h, float o);
            void RenderImage(std::string is, int x, int y, int w, int h, float o);
            void RenderUI();
            void RunStory();
            void CheckUIClick(int mx, int my);

            static void OutputText(const std::string& str) {
                for (char c : str) {
                    if (CurrentEngine->CurrentText!=str) return;
                    CurrentEngine->DialogueUI.DialogueBox.TextContent+=c;
                    Threading::Delay(CurrentEngine->TextSpeed);
                }
            }

        public:
            const int ScreenWidth = System::DefaultSettings["ScreenWidth"];
            const int ScreenHeight = System::DefaultSettings["ScreenHeight"];
            DialogueUISet DialogueUI;
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
        

        Engine(std::string Name, std::string WindowIcon=""): Name(Name), DialogueUI(this->ScreenWidth, this->ScreenHeight){
            if (
                TTF_Init()<0 || 
                SDL_Init(SDL_INIT_VIDEO)<0 || 
                SDL_Init(SDL_INIT_AUDIO)<0 ||
                Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0
            ) System::Error("Failed to create engine");

            this->Window = SDL_CreateWindow(
                                        this->Name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        this->ScreenWidth, this->ScreenHeight, 
                                        SDL_WINDOW_SHOWN);
            this->Renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawBlendMode(this->Renderer, SDL_BLENDMODE_BLEND);

            this->UIElements = {&this->DialogueUI.DialogueBox, &this->DialogueUI.NameBox, &this->DialogueUI.TransitionScreen};
            this->DialogueUI.DialogueBox.Callback = [&](){
                if (this->StoryIndex.has_value() &&
                    this->Story.count(this->StoryIndex.value())>0
                ) this->RunStory();
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
        for (auto elem: this->UIElements){
            if (
                MouseX>=elem->X &&
                MouseX<=elem->X+elem->Width &&
                MouseY>=elem->Y &&
                MouseY<=elem->Y+elem->Height
            ) return elem->Callback();
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

    void Engine::RenderUI(){
        for (auto elem: this->UIElements){
            if (!elem->Visible) continue;

            switch (std::holds_alternative<std::string>(elem->Background)){
                case true:
                    this->RenderImage(
                        std::get<std::string>(elem->Background),
                        elem->X, elem->Y, elem->Width, elem->Height, elem->Opacity
                    );
                break;

                case false:
                    this->RenderColor(
                        std::get<std::tuple<int, int, int>>(elem->Background),
                        elem->X, elem->Y, elem->Width, elem->Height, elem->Opacity
                    );
                break;
            }

            if (elem->OutlineColor.has_value())
                elem->RenderOutline();

            if (elem->TextContent.length()>0)
                elem->DisplayText();
        }
    }

    // story management
    void Engine::RunStory(){
        if (!this->StoryIndex.has_value() || this->Story.find(this->StoryIndex.value())==this->Story.end()) 
            return;
        
        this->Scene.Transition();
        CurrentEngine->DialogueUI.DialogueBox.Hide(); 
        CurrentEngine->DialogueUI.NameBox.Hide();
        
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