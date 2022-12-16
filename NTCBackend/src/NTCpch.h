#pragma once

#include <Windows.h>

#include <iostream>

#include <string>
#include <queue>
#include <list>
#include <unordered_map>

#include <thread>
#include <mutex>

#include <future>
#include <chrono>

#include <zmq.hpp>

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

    using LaunchStatus = DWORD;
}

// Turning on/off logging of recived messages 
//#define IPC_LOGGING

#include "Logger/Logger.h"