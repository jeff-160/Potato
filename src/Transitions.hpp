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
        CurrentEngine->UISet.TransitionScreen.Opacity = 0;
        CurrentEngine->UISet.TransitionScreen.Visible = true;
        CurrentEngine->UISet.TransitionScreen.X = CurrentEngine->UISet.TransitionScreen.Y = 0;
        
        Threading::RunAsync(
            [](){
                int Dir = 1;
                while(1){
                    if (CurrentEngine->UISet.TransitionScreen.Opacity<=0 && Dir==-1) break;
                    CurrentEngine->UISet.TransitionScreen.Opacity+=Dir*System::DefaultSettings["TransitionFadeInOutSpeed"];
                    if (CurrentEngine->UISet.TransitionScreen.Opacity>=1){
                        CurrentEngine->UISet.TransitionScreen.Opacity = 1;
                        Pause();
                        Dir = -1;
                    }
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }
    
    void Transitions::Pop(){
        CurrentEngine->UISet.TransitionScreen.Opacity = 1;
        CurrentEngine->UISet.TransitionScreen.Visible = true;
        CurrentEngine->UISet.TransitionScreen.Width = CurrentEngine->UISet.TransitionScreen.Height = 0;

        Threading::RunAsync(
            [](){
                float Perc = 0;
                int Dir = 1;
                while(1){
                    if (Perc<=0 &&Dir==-1) break;
                    Perc+=Dir*System::DefaultSettings["TransitionPopSpeed"];
                    CurrentEngine->UISet.TransitionScreen.Width = Perc*CurrentEngine->ScreenWidth;
                    CurrentEngine->UISet.TransitionScreen.Height = Perc*CurrentEngine->ScreenHeight;
                    CurrentEngine->UISet.TransitionScreen.X = 
                        (int)CurrentEngine->ScreenWidth/2-CurrentEngine->UISet.TransitionScreen.Width/2;
                    CurrentEngine->UISet.TransitionScreen.Y = 
                        (int)CurrentEngine->ScreenHeight/2-CurrentEngine->UISet.TransitionScreen.Height/2;
                    if (Perc>=1){Pause(); Dir=-1;}
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }

    void Transitions::Slide(){
        CurrentEngine->UISet.TransitionScreen.Opacity = 1;
        CurrentEngine->UISet.TransitionScreen.Visible = true;
        CurrentEngine->UISet.TransitionScreen.Width = CurrentEngine->ScreenWidth;
        CurrentEngine->UISet.TransitionScreen.Height = CurrentEngine->ScreenHeight;
        CurrentEngine->UISet.TransitionScreen.X = -CurrentEngine->UISet.TransitionScreen.Width;
        CurrentEngine->UISet.TransitionScreen.Y = 0;

        Threading::RunAsync(
            [](){
                while (1){
                    if (CurrentEngine->UISet.TransitionScreen.X==CurrentEngine->ScreenWidth) 
                        break;
                    CurrentEngine->UISet.TransitionScreen.X+=static_cast<int>(System::DefaultSettings["TransitionSlideSpeed"]);
                    if (CurrentEngine->UISet.TransitionScreen.X==0)
                        Pause();
                    Threading::Delay(CurrentEngine->TransitionRate);
                }
            }
        );
    }
}