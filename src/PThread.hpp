namespace Potato{
    class PThread{
        private:
            friend class Effects;
            friend class Transitions;
            static void RunAsync(std::function<void()> Callback){
                {
                    std::thread T(Callback);
                    T.detach();
                }
            }
    };
}