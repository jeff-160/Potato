namespace Potato{
    class System{
        private:
            static void Error(std::string e);

        public:
            friend class Engine;
            friend class Character;
            friend class UIElement;
    };
    
    void System::Error(std::string Err){
        std::cerr<<"Error: " << Err << std::endl;
        return;
    }
}