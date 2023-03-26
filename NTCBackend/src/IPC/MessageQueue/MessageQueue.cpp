#include "NTCpch.h"
#include "MessageQueue.h"

namespace NTC
{
    Scope<MessageQueue_t> MessageQueue::MessageQueue_ = CreateScope<MessageQueue_t>();
    
    void MessageQueue::WaitAndPopMessage(MessageQueue_t::value_type& message)
    {
        MessageQueue_->wait_and_pop(message);
    }

    bool MessageQueue::TryPopMessage(MessageQueue_t::value_type& message)
    {
        return MessageQueue_->try_pop(message);
    }

    void MessageQueue::AddMessage(const MessageQueue_t::value_type& message)
    {
        MessageQueue_->push(message);
    }
}