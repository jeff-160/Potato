#ifndef VFX_HEADER
#define VFX_HEADER

namespace Potato{
    struct Effects {
        private:
            static std::pair<float, float> ComputeVelocity(Character c, std::pair<float, float> d);
            static bool CheckDest(Character c, std::pair<float, float> d);
        
        public:
            static void FadeIn(Character &c);
            static void FadeOut(Character &c);
            static void Slide(Character &c, float x, float y);
    };

    std::pair<float, float> Effects::ComputeVelocity(Character Char, std::pair<float, float> Destination){
        double angle = std::atan2(Destination.second-Char.Y, Destination.first-Char.X);
        return {
            std::cos(angle)*Char.SlideSpeed, 
            std::sin(angle)*Char.SlideSpeed
        };
    }
    
    bool Effects::CheckDest(Character Char, std::pair<float, float> Destination){
        std::vector<float> Dists = {
            std::abs(Destination.first-Char.X), 
            std::abs(Destination.second-Char.Y)
        };
        return std::all_of(Dists.begin(), Dists.end(), [&](float d){return d<=Char.SlideSpeed;});
    }

    void Effects::FadeIn(Character &Char){
        PThread::RunAsync(
            [&Char](){
                while (Char.Opacity<1){
                    Char.Opacity+=CurrentEngine->FadeSpeed;
                    std::this_thread::sleep_for(std::chrono::milliseconds(CurrentEngine->FadeRate));
                }
                Char.Opacity = 1;
            }
        );
    }
    void Effects::FadeOut(Character &Char){
        PThread::RunAsync(
            [&Char](){
                while (Char.Opacity>0){
                    Char.Opacity-=CurrentEngine->FadeSpeed;
                    std::this_thread::sleep_for(std::chrono::milliseconds(CurrentEngine->FadeRate));
                }
                Char.Opacity = 0;
            }
        );
    }

    void Effects::Slide(Character &Char, float X, float Y){
        std::pair<float, float> Dest = {X, Y};
        std::pair<float, float> Velocity = ComputeVelocity(Char, Dest);
        PThread::RunAsync(
            [&Char, Dest, Velocity](){
                while (!CheckDest(Char, Dest)){
                    Char.X+=Velocity.first;
                    Char.Y+=Velocity.second;
                    std::this_thread::sleep_for(std::chrono::milliseconds(CurrentEngine->SlideRate));
                }
                Char.X = Dest.first, Char.Y = Dest.second;
            }
        );
    }


    struct Transitions{
        static void FadeInOut();
    };

    void Transitions::FadeInOut(){
        std::cout<<"fuckign nigger";
    }
}

#endif