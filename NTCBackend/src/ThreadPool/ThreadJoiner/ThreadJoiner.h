#pragma once

namespace NTC
{
    class ThreadJoiner
    {
    public:
        explicit ThreadJoiner(std::vector<std::thread>& threads)
            : threads_(threads) {}

        ~ThreadJoiner()
        {
            for (auto& thread : threads_)
                if (thread.joinable())
                    thread.join();
        }
    protected:
        std::vector<std::thread>& threads_;
    };
}