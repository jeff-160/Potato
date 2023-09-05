#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

namespace Potato{
    class System{
        private:
            static void Error(std::string e);
            static std::map<std::string, double> GetDefaults();
            static std::map<std::string, double> DefaultSettings;

        public:
            friend class Engine;
            friend class Character;
            friend class UIElement;
            friend class UICreator;
            friend class Effects;
            friend class Transitions;
    };
    
    void System::Error(std::string Err){
        std::cerr<<"Error: " << Err << std::endl;
        return;
    }

    std::map<std::string, double> System::GetDefaults(){
        auto ParseData = [](std::string Data)->std::map<std::string, double>{
            std::map<std::string, double> PData;
            std::vector<std::string> Toks;
            std::istringstream Iss(Data);
            std::string T;
            while (std::getline(Iss, T, ','))
                Toks.push_back(T);
            for (int i=0;i<Toks.size();i+=2)
                PData.insert({Toks[i], std::stod(Toks[i+1])});
            return PData;
        };

        FILE* Pipe = popen("python src\\LoadDef.py", "r");
        
        char Buffer[128];
        std::string Output;
        
        while (fgets(Buffer, sizeof(Buffer), Pipe) != nullptr)
            Output+=Buffer;
        pclose(Pipe);

        return ParseData(Output);
    }

    std::map<std::string, double> System::DefaultSettings = GetDefaults();
}

#endif