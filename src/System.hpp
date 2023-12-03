#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

namespace Potato{
    struct System{
        friend struct Engine;
        friend struct SceneCreator;
        friend struct Character;
        friend struct UIElement;
        friend struct UISet;
        friend struct Effects;
        friend struct Transitions;
        friend struct Audio;
        friend struct Choice;

        private:
            static void Error(std::string e);
            static std::map<std::string, double> GetDefaults();
            static std::map<std::string, double> DefaultSettings;
    };

    std::map<std::string, double> System::GetDefaults(){
        auto ParseData = [](std::string Data)->std::map<std::string, double> {
            std::map<std::string, double> PData;
            std::vector<std::string> Toks;
            std::istringstream Iss(Data);
            std::string T;
            while (std::getline(Iss, T, ','))
                Toks.push_back(T);
            for (int i=0;i<Toks.size();i+=2)
                PData.insert({Toks[i], std::stof(Toks[i+1])});
            return PData;
        };

        FILE* Pipe = popen("src\\LoadDef.exe", "r");
        
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