namespace Potato{
    class SceneCreator{
        private:
            std::vector<Character*> Characters;
            std::pair<bool, std::variant<std::string, std::tuple<int, int, int>>> 
                    Background = std::make_pair(true, std::make_tuple(
                            System::DefaultSettings["SBR"],
                            System::DefaultSettings["SBB"],
                            System::DefaultSettings["SBG"]
                        ));
            std::function<void()> Transition = [](){};

            void RenderBackground();

        public:
            friend class Engine;

            void ClearCharacter();
            void AddCharacter(Character* ca);
            void RemoveCharacter(Character* ca);
            
            void SetBackgroundColor(std::tuple<int, int, int> c);
            void SetBackgroundImage(std::string bs);
            void ClearBackground();

            void SetTransition(void (*tc)());
            void ClearTransition();
    };

    // scene modifications
    void SceneCreator::SetBackgroundColor(std::tuple<int, int, int> Color){
        this->Background = {true, Color};
    }
    void SceneCreator::ClearBackground(){
        this->Background = {
            true, std::make_tuple(
                System::DefaultSettings["SBR"],
                System::DefaultSettings["SBB"],
                System::DefaultSettings["SBG"]
            )
        };
    }

    // character handling
    void SceneCreator::ClearCharacter(){
        this->Characters.clear();
    }

    void SceneCreator::AddCharacter(Character* CharAddr){
        this->Characters.push_back(CharAddr);
    }

    void SceneCreator::RemoveCharacter(Character* CharAddr){
        for (int i=0;i<this->Characters.size();i++){
            if (this->Characters[i]==CharAddr){
                this->Characters.erase(this->Characters.begin()+i);
                return;
            }
        }
    }

    // transition handling
    void SceneCreator::SetTransition(void (*TransitionCallback)()){
        this->Transition = TransitionCallback;
    }
    
    void SceneCreator::ClearTransition(){
        this->Transition = [](){};
    }
}