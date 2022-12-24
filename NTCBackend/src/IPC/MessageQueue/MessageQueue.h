#pragma once
#include "NTCpch.h"

#include "Containers/ThreadSafeQueue/ThreadSafeQueue.h"


namespace NTC
{
    using MessageQueue_t = containers::ThreadSafeQueue<std::string>;
    
    class MessageQueue
    {
    public:
        MessageQueue() = delete;

        static void PopMessage(MessageQueue_t::value_type& message);
        static void AddMessage(const MessageQueue_t::value_type& message);
        
    protected:
        static Scope<MessageQueue_t> MessageQueue_;
    };
}