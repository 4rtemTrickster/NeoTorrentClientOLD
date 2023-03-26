#pragma once
#include "NTCpch.h"
#include "Containers/ThreadSafeQueue/ThreadSafeQueue.h"
#include "FunctionWrapper/FunctionWrapper.h"
#include "ThreadJoiner/ThreadJoiner.h"

namespace NTC
{
    //TODO: YOU NEED TO IMPROVE THAT PIECE OF SHIT!!!!!!!!!!!!!!!!!
    class ThreadPool
    {
    public:
        static void Init();
            
        template<typename FunctionType>
        static inline std::future<std::invoke_result_t<FunctionType>> submit(FunctionType f) { return instance_->submit_(f); }

        static void run_pending_task() { instance_->run_pending_task_(); }

        ThreadPool();
        virtual ~ThreadPool();
        
    protected:
        
        template <typename FunctionType>
        std::future<std::invoke_result_t<FunctionType>> submit_(FunctionType f);
        void run_pending_task_();
        
        static Scope<ThreadPool> instance_;
        inline static bool bIsInitialized = false;
        
        std::atomic_bool done_;
        containers::ThreadSafeQueue<FunctionWrapper> poolWorkQueue_;
        std::vector<Scope<containers::ThreadSafeQueue<FunctionWrapper>>> queues_;
        std::vector<std::thread> threads_;
        ThreadJoiner thread_joiner_;
        static thread_local containers::ThreadSafeQueue<FunctionWrapper>* localWorkQueue_;
        static thread_local unsigned index_;

        void worker_thread(unsigned index);

        bool pop_task_from_local_queue(FunctionWrapper& task);
        bool pop_task_from_pool_queue(FunctionWrapper& task);
        bool pop_task_from_other_thread_queue(FunctionWrapper& task);
    };

    template <typename FunctionType>
    std::future<std::invoke_result_t<FunctionType>> ThreadPool::submit_(FunctionType f)
    {
        using ResultType = std::invoke_result_t<FunctionType>;

        std::packaged_task<ResultType()> task(std::move(f));
        std::future<ResultType> res(task.get_future());

        if(localWorkQueue_)
            localWorkQueue_->push(std::move(task));
        else
            poolWorkQueue_.push(std::move(task));
        
        return res;
    }
}