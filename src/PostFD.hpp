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
        if (this->ChoiceBoxes.size()>0) return;

        if (Char!=NULL){
            CurrentEngine->UI.NameBox.TextContent = Char->Name;
            CurrentEngine->UI.NameBox.Show(); 
        }
        
        CurrentEngine->UI.DialogueBox.Show();
        CurrentEngine->UI.DialogueBox.TextContent = "";
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

        std::string FontSrc = CurrentEngine->Font.has_value() ? CurrentEngine->Font.value() : "default/font.ttf";
        TTF_Font* TextFont = TTF_OpenFont(FontSrc.c_str(), this->FontSize);
        if (TextFont==nullptr)
            return System::Error("Failed to load font: " + FontSrc);
        
        SDL_Surface* TextSurface = TTF_RenderText_Blended_Wrapped(TextFont, this->TextContent.c_str(), Color, this->Width-this->Margin*2);
        SDL_FRect TBounds = {this->X+this->Margin, this->Y+this->Margin, TextSurface->w, TextSurface->h};
        SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(CurrentEngine->Renderer, TextSurface);

        // center alignment
        if (this->TextAlignMode==1)
            TBounds.x = this->X+(this->Width-TextSurface->w)/2;
        
        SDL_RenderCopyF(CurrentEngine->Renderer, TextTexture, nullptr, &TBounds);

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

    // choices
    void SceneCreator::Choice(std::vector<Potato::Choice> Choices){
        CurrentEngine->UI.DialogueBox.Hide();
        CurrentEngine->UI.NameBox.Hide();

        int CY = -static_cast<int>(Choices.size()/2);
        int Margin = 10;

        for (auto Choice : Choices){
            UIElement ChoiceBox(
                0,0,
                CurrentEngine->ScreenWidth*0.7,
                CurrentEngine->ScreenHeight/10
            );
            ChoiceBox.X = static_cast<int>((CurrentEngine->ScreenWidth-ChoiceBox.Width)/2);
            ChoiceBox.Y = CurrentEngine->ScreenHeight/2+CY*ChoiceBox.Height+CY*Margin;
            
            ChoiceBox.TextContent = Choice.Text;
            ChoiceBox.TextAlignMode = 1;
            ChoiceBox.TextColor = Choice.FontColor;
            ChoiceBox.FontSize = Choice.FontSize;

            if (std::holds_alternative<std::string>(Choice.Background))
                ChoiceBox.Background = std::get<std::string>(Choice.Background);
            else
                ChoiceBox.Background = std::get<std::tuple<int, int, int>>(Choice.Background);

            ChoiceBox.Callback = [&, Choice](){
                this->ChoiceBoxes.clear();
                CurrentEngine->Jump(Choice.StoryIndex);
                CurrentEngine->RunStory();
            };

            this->ChoiceBoxes.push_back(ChoiceBox);
            CY++;
        }
    }
}

#endif