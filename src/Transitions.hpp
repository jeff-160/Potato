#ifndef TRANSITIONS_HEADER
#define TRANSITIONS_HEADER

namespace Potato{
    struct Transitions{
        static void FadeInOut();         
        static void Pop();
        static void Slide();

        private: 
            static void Pause(){
                Threading::Delay(1000);
            }
    };

    void Transitions::FadeInOut(){ 
        CurrentEngine->DialogueUI.TransitionScreen.Opacity = 0;
        CurrentEngine->DialogueUI.TransitionScreen.Visible = true;
        CurrentEngine->DialogueUI.TransitionScreen.X = CurrentEngine->DialogueUI.TransitionScreen.Y = 0;
        
        Threading::RunAsync(
            [](){
                int Dir = 1;
                while(1){
                    if (CurrentEngine->DialogueUI.TransitionScreen.Opacity<=0 && Dir==-1) break;
                    CurrentEngine->DialogueUI.TransitionScreen.Opacity+=Dir*System::DefaultSettings["TransitionFadeInOutSpeed"];
                    if (CurrentEngine->DialogueUI.TransitionScreen.Opacity>=1){
                        CurrentEngine->DialogueUI.TransitionScreen.Opacity = 1;
                        Pause();
                        Dir = -1;
                    }
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }
    
    void Transitions::Pop(){
        CurrentEngine->DialogueUI.TransitionScreen.Opacity = 1;
        CurrentEngine->DialogueUI.TransitionScreen.Visible = true;
        CurrentEngine->DialogueUI.TransitionScreen.Width = CurrentEngine->DialogueUI.TransitionScreen.Height = 0;

        Threading::RunAsync(
            [](){
                float Perc = 0;
                int Dir = 1;
                while(1){
                    if (Perc<=0 &&Dir==-1) break;
                    Perc+=Dir*System::DefaultSettings["TransitionPopSpeed"];
                    CurrentEngine->DialogueUI.TransitionScreen.Width = Perc*CurrentEngine->ScreenWidth;
                    CurrentEngine->DialogueUI.TransitionScreen.Height = Perc*CurrentEngine->ScreenHeight;
                    CurrentEngine->DialogueUI.TransitionScreen.X = 
                        (int)CurrentEngine->ScreenWidth/2-CurrentEngine->DialogueUI.TransitionScreen.Width/2;
                    CurrentEngine->DialogueUI.TransitionScreen.Y = 
                        (int)CurrentEngine->ScreenHeight/2-CurrentEngine->DialogueUI.TransitionScreen.Height/2;
                    if (Perc>=1){Pause(); Dir=-1;}
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }

    void Transitions::Slide(){
        CurrentEngine->DialogueUI.TransitionScreen.Opacity = 1;
        CurrentEngine->DialogueUI.TransitionScreen.Visible = true;
        CurrentEngine->DialogueUI.TransitionScreen.Width = CurrentEngine->ScreenWidth;
        CurrentEngine->DialogueUI.TransitionScreen.Height = CurrentEngine->ScreenHeight;
        CurrentEngine->DialogueUI.TransitionScreen.X = -CurrentEngine->DialogueUI.TransitionScreen.Width;
        CurrentEngine->DialogueUI.TransitionScreen.Y = 0;

        Threading::RunAsync(
            [](){
                while (1){
                    if (CurrentEngine->DialogueUI.TransitionScreen.X==CurrentEngine->ScreenWidth) 
                        break;
                    CurrentEngine->DialogueUI.TransitionScreen.X+=static_cast<int>(System::DefaultSettings["TransitionSlideSpeed"]);
                    if (CurrentEngine->DialogueUI.TransitionScreen.X==0)
                        Pause();
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }
}

#endif