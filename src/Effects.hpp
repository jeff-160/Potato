#ifndef EFFECTS_HEADER
#define EFFECTS_HEADER

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
            std::cos(angle)*System::DefaultSettings["CharacterSlideSpeed"], 
            std::sin(angle)*System::DefaultSettings["CharacterSlideSpeed"]
        };
    }
    
    bool Effects::CheckDest(Character Char, std::pair<float, float> Destination){
        std::vector<float> Dists = {
            std::abs(Destination.first-Char.X), 
            std::abs(Destination.second-Char.Y)
        };
        return std::all_of(Dists.begin(), Dists.end(), [&](float d){return d<=System::DefaultSettings["CharacterSlideSpeed"];});
    }

    void Effects::FadeIn(Character &Char){
        Threading::RunAsync(
            [&Char](){
                while (Char.Opacity<1){
                    Char.Opacity+=System::DefaultSettings["CharacterFadeSpeed"];
                    Threading::Delay(CurrentEngine->CharacterFadeRate);
                }
                Char.Opacity = 1;
            }
        );
    }
    void Effects::FadeOut(Character &Char){
        Threading::RunAsync(
            [&Char](){
                while (Char.Opacity>0){
                    Char.Opacity-=System::DefaultSettings["CharacterFadeSpeed"];
                    Threading::Delay(CurrentEngine->CharacterFadeRate);
                }
                Char.Opacity = 0;
            }
        );
    }

    void Effects::Slide(Character &Char, float X, float Y){
        std::pair<float, float> Dest = {X, Y};
        std::pair<float, float> Velocity = ComputeVelocity(Char, Dest);
        Threading::RunAsync(
            [&Char, Dest, Velocity](){
                while (!CheckDest(Char, Dest)){
                    Char.X+=Velocity.first;
                    Char.Y+=Velocity.second;
                    Threading::Delay(CurrentEngine->CharacterSlideRate);
                }
                Char.X = Dest.first, Char.Y = Dest.second;
            }
        );
    }
}

#endif