#include "NTCpch.h"
#include "ThreadPool.h"

namespace NTC
{
    Scope<ThreadPool> ThreadPool::instance_;
    
    thread_local containers::ThreadSafeQueue<FunctionWrapper>* ThreadPool::localWorkQueue_;
    thread_local unsigned ThreadPool::index_;

    void ThreadPool::Init()
    {
        NTC_PROFILE_FUNCTION();

        if(!bIsInitialized)
        {
            instance_ = CreateScope<ThreadPool>();
            bIsInitialized = true;
        }

        NTC_TRACE("Thread pool initialized");
    }

    ThreadPool::ThreadPool()
        : done_(false), thread_joiner_(threads_)
    {
        const unsigned threadCount = std::thread::hardware_concurrency() - 1;

        NTC_TRACE("Thread count in pool: " + std::to_string(threadCount));

        try
        {
            for(unsigned i = 0; i < threadCount; ++i)
                queues_.emplace_back(new containers::ThreadSafeQueue<FunctionWrapper>);
            
            for(unsigned i = 0; i < threadCount; ++i)
                threads_.emplace_back(&ThreadPool::worker_thread, this, i);
        }
        catch (...)
        {
            done_ = true;
            throw;
        }
    }

    ThreadPool::~ThreadPool()
    {
        done_ = true;
    }

    void ThreadPool::worker_thread(unsigned index)
    {
        index_ = index;
        localWorkQueue_ = queues_[index_].get();
        
        while (!done_)
            run_pending_task_();
    }

    void ThreadPool::run_pending_task_()
    {
        FunctionWrapper task;

        if (pop_task_from_local_queue(task) ||
            pop_task_from_pool_queue(task) ||
            pop_task_from_other_thread_queue(task))
                task();
        else
            std::this_thread::yield();
    }

    bool ThreadPool::pop_task_from_local_queue(FunctionWrapper& task)
    {
        return localWorkQueue_ && localWorkQueue_->try_pop(task);
    }

    bool ThreadPool::pop_task_from_pool_queue(FunctionWrapper& task)
    {
        return poolWorkQueue_.try_pop(task);
    }

    bool ThreadPool::pop_task_from_other_thread_queue(FunctionWrapper& task)
    {
        for (std::size_t i = 0; i < queues_.size(); ++i)
        {
            const std::size_t index = (index_ + i + 1) % queues_.size();
            if(queues_.at(index)->try_steal(task))
                return true;
        }

        return false;
    }
}
