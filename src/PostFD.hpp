#ifndef POSTFD_HEADER
#define POSTFD_HEADER

namespace Potato{
    // character
    void Character::Speak(std::string Text){
        CurrentEngine->UISet.NameBox.TextContent = this->Name;
        CurrentEngine->UISet.NameBox.Visible = true;
        CurrentEngine->UISet.DialogueBox.TextContent = "";
        CurrentEngine->CurrentText = Text;
        {
            CurrentEngine->TextThread = std::thread(&Engine::OutputText, Text);
            CurrentEngine->TextThread.detach();
        }
    }

    // ui
    void UIElement::DisplayText(){
        int r, g, b;
        std::tie(r, g, b) = this->TextColor;
        SDL_Color Color = {r, g, b, 255};

        std::string FontSrc = this->Font.has_value() ? this->Font.value() : "default/font.ttf";
        TTF_Font* TextFont = TTF_OpenFont(FontSrc.c_str(), this->FontSize);
        if (TextFont==nullptr)
            return System::Error("Failed to load font: " + FontSrc);
        
        SDL_Surface* TextSurface = TTF_RenderText_Blended_Wrapped(TextFont, this->TextContent.c_str(), Color, this->Width-this->Margin);
        SDL_Rect TBounds = {this->X+this->Margin, this->Y+this->Margin, TextSurface->w, TextSurface->h};
        SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(CurrentEngine->Renderer, TextSurface);

        int w, h;
        TTF_SizeText(TextFont, this->TextContent.c_str(), &w, &h);
        switch (this->TextAlignMode){
            case 1:
                TBounds.x = this->X+(this->Width-w)/2;
                break;
            case 2:
                TBounds.x = this->X+this->Width-w-this->Margin;
                break;
        }
        
        SDL_RenderCopy(CurrentEngine->Renderer, TextTexture, nullptr, &TBounds);

        SDL_FreeSurface(TextSurface);
        SDL_DestroyTexture(TextTexture);
    }

    void UIElement::RenderOutline(){
        SDL_Rect OBounds = {this->X, this->Y, this->Width, this->Height};
        CurrentEngine->SetRenderColor(CurrentEngine->Renderer, this->Outline.value(), this->Opacity);
        for (int i = 0; i < this->OutlineThickness; ++i) {
            SDL_RenderDrawRect(CurrentEngine->Renderer, &OBounds);
            OBounds.x += 1; OBounds.y += 1;
            OBounds.w -= 2; OBounds.h -= 2;
        }
    }

    // scene
    void SceneCreator::SetBackgroundImage(std::string BgSrc){
        SDL_Texture* BgTexture = IMG_LoadTexture(CurrentEngine->Renderer, BgSrc.c_str());
        if (BgTexture==nullptr)
            return System::Error("Failed to load background image");
        this->Background = {false, BgSrc};
        SDL_DestroyTexture(BgTexture);
    }
    void SceneCreator::RenderBackground(){
        if (this->Background.first)
            return CurrentEngine->RenderColor(
                    CurrentEngine->Renderer,
                    std::get<std::tuple<int, int, int>>(this->Background.second), 
                    0, 0, CurrentEngine->ScreenWidth, CurrentEngine->ScreenHeight, 1);
        CurrentEngine->RenderImage(
            CurrentEngine->Renderer,
            std::get<std::string>(this->Background.second), 
            0, 0, CurrentEngine->ScreenWidth, CurrentEngine->ScreenHeight, 1);
    }

    // transitions
    void Transitions::FadeInOut(){
        SDL_Rect MBounds = {0,0,CurrentEngine->ScreenWidth, CurrentEngine->ScreenWidth};
        CurrentEngine->SetRenderColor(CurrentEngine->TRenderer, {0,0,0}, 1);
    }
}

#endif