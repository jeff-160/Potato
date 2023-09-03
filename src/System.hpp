#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

namespace Potato{
    class System{
        private:
            static void Error(std::string e);
            static std::map<std::string, int> GetDefaults();
            static std::map<std::string, int> DefaultSettings;

        public:
            friend class Engine;
            friend class Character;
            friend class UIElement;
            friend class UICreator;
    };
    
    void System::Error(std::string Err){
        std::cerr<<"Error: " << Err << std::endl;
        return;
    }

    std::map<std::string, int> System::GetDefaults(){
        auto ParseData = [](std::string Data)->std::map<std::string, int>{
            std::map<std::string, int> PData;
            std::vector<std::string> Toks;
            std::istringstream Iss(Data);
            std::string T;
            while (std::getline(Iss, T, ','))
                Toks.push_back(T);
            for (int i=0;i<Toks.size();i+=2)
                PData.insert({Toks[i], std::stoi(Toks[i+1])});
            return PData;
        };

        char Directory[PATH_MAX];
        _getcwd(Directory, sizeof(Directory));
        std::string Command = "python " + std::string(Directory) + "\\src\\LoadDef.py";
        FILE* Pipe = popen(Command.c_str(), "r");
        
        char Buffer[128];
        std::string Output;
        
        while (fgets(Buffer, sizeof(Buffer), Pipe) != nullptr)
            Output+=Buffer;
        pclose(Pipe);

        return ParseData(Output);
    }

    std::map<std::string, int> System::DefaultSettings = GetDefaults();
}

#endif