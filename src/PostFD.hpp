#ifndef POSTFD_HEADER
#define POSTFD_HEADER

namespace Potato{
    // system error
    void System::Error(std::string Err){
        if (CurrentEngine!=nullptr) 
            CurrentEngine->Close();
        throw std::runtime_error(Err);
        return;
    }

    // character
    void SceneCreator::Speak(Character* Char, std::string Text){
        if (Char!=NULL){
            CurrentEngine->UISet.NameBox.TextContent = Char->Name;
            CurrentEngine->UISet.NameBox.Show(); 
        }
        
        CurrentEngine->UISet.DialogueBox.Show();
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
        SDL_FRect OBounds = {this->X, this->Y, this->Width, this->Height};
        CurrentEngine->SetRenderColor(this->OutlineColor.value(), this->Opacity);
        for (int i = 0; i < this->OutlineThickness; ++i) {
            SDL_RenderDrawRectF(CurrentEngine->Renderer, &OBounds);
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
                std::get<std::tuple<int, int, int>>(this->Background.second), 
                0, 0, CurrentEngine->ScreenWidth, CurrentEngine->ScreenHeight, 1);
        CurrentEngine->RenderImage(
            std::get<std::string>(this->Background.second), 
            0, 0, CurrentEngine->ScreenWidth, CurrentEngine->ScreenHeight, 1);
    }
}

#endif