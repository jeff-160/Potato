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
        CurrentEngine->UI.TransitionScreen.Opacity = 0;
        CurrentEngine->UI.TransitionScreen.Visible = true;
        CurrentEngine->UI.TransitionScreen.X = CurrentEngine->UI.TransitionScreen.Y = 0;
        
        Threading::RunAsync(
            [](){
                int Dir = 1;
                while(1){
                    if (CurrentEngine->UI.TransitionScreen.Opacity<=0 && Dir==-1) break;
                    CurrentEngine->UI.TransitionScreen.Opacity+=Dir*System::DefaultSettings["TransitionFadeInOutSpeed"];
                    if (CurrentEngine->UI.TransitionScreen.Opacity>=1){
                        CurrentEngine->UI.TransitionScreen.Opacity = 1;
                        Pause();
                        Dir = -1;
                    }
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }
    
    void Transitions::Pop(){
        CurrentEngine->UI.TransitionScreen.Opacity = 1;
        CurrentEngine->UI.TransitionScreen.Visible = true;
        CurrentEngine->UI.TransitionScreen.Width = CurrentEngine->UI.TransitionScreen.Height = 0;

        Threading::RunAsync(
            [](){
                float Perc = 0;
                int Dir = 1;
                while(1){
                    if (Perc<=0 &&Dir==-1) break;
                    Perc+=Dir*System::DefaultSettings["TransitionPopSpeed"];
                    CurrentEngine->UI.TransitionScreen.Width = Perc*CurrentEngine->ScreenWidth;
                    CurrentEngine->UI.TransitionScreen.Height = Perc*CurrentEngine->ScreenHeight;
                    CurrentEngine->UI.TransitionScreen.X = 
                        (int)CurrentEngine->ScreenWidth/2-CurrentEngine->UI.TransitionScreen.Width/2;
                    CurrentEngine->UI.TransitionScreen.Y = 
                        (int)CurrentEngine->ScreenHeight/2-CurrentEngine->UI.TransitionScreen.Height/2;
                    if (Perc>=1){Pause(); Dir=-1;}
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }

    void Transitions::Slide(){
        CurrentEngine->UI.TransitionScreen.Opacity = 1;
        CurrentEngine->UI.TransitionScreen.Visible = true;
        CurrentEngine->UI.TransitionScreen.Width = CurrentEngine->ScreenWidth;
        CurrentEngine->UI.TransitionScreen.Height = CurrentEngine->ScreenHeight;
        CurrentEngine->UI.TransitionScreen.X = -CurrentEngine->UI.TransitionScreen.Width;
        CurrentEngine->UI.TransitionScreen.Y = 0;

        Threading::RunAsync(
            [](){
                while (1){
                    if (CurrentEngine->UI.TransitionScreen.X==CurrentEngine->ScreenWidth) 
                        break;
                    CurrentEngine->UI.TransitionScreen.X+=static_cast<int>(System::DefaultSettings["TransitionSlideSpeed"]);
                    if (CurrentEngine->UI.TransitionScreen.X==0)
                        Pause();
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }
}

#endif