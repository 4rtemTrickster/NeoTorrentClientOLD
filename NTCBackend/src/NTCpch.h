#pragma once

#include <Windows.h>

#include <iostream>

#include <string>
#include <queue>
#include <list>

#include <thread>
#include <mutex>

#include <future>
#include <chrono>

#include <zmq.hpp>

#include "Containers/ThreadSafeQueue/ThreadSafeQueue.h"

using namespace std::chrono_literals;

namespace NTC
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    using MessageQueue_t = containers::ThreadSafeQueue<std::string>;
}

#include "Logger/Logger.h"