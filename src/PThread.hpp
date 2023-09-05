namespace Potato{
    class PThread{
        public:
            static void RunAsync(std::function<void()> Callback){
                {
                    std::thread T(Callback);
                    T.detach();
                }
            }
    };
}