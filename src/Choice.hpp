namespace Potato{
    struct Choice{
        public:
            std::string Text;
            int StoryIndex;
            std::variant<std::string, std::tuple<int, int, int>> Background;
            std::tuple<int, int, int> FontColor;

        Choice(
            std::string Text, 
            int StoryIndex, 
            std::variant<std::string, std::tuple<int, int, int>> Background=std::make_tuple(
                System::DefaultSettings["BGR"],
                System::DefaultSettings["BGG"],
                System::DefaultSettings["BGB"]
            ), 
            std::tuple<int, int, int> FontColor={
                System::DefaultSettings["TCR"],
                System::DefaultSettings["TCG"],
                System::DefaultSettings["TCB"],
            }
        ): Text(Text), StoryIndex(StoryIndex), Background(Background), FontColor(FontColor){};
    };
}