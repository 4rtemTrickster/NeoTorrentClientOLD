#pragma once

#include <Windows.h>

#include <iostream>
#include <fstream>

#include <string>
#include <queue>
#include <list>
#include <unordered_map>
#include <map>

#include <thread>
#include <mutex>

#include <future>
#include <chrono>

#include <algorithm>
#include <ranges>
#include <utility>

#include <filesystem>

#include <zmq.hpp>

#include <boost/optional/optional.hpp>
#include <boost/compute/detail/sha1.hpp>
#include <boost/algorithm/string.hpp>

namespace NTC
{
    template<class T, class U>
    concept Derived = std::is_base_of_v<U, T>;
    
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
    
    template <typename T, typename ... Args>
    constexpr Ref<T> DynamicCast(Args&& ... args)
    {
        return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...);
    }

    using LaunchStatus = DWORD;
}

// Turning on/off logging of recived messages 
//#define IPC_LOGGING

#include "Logger/Logger.h"
#include "Debug/Instrumentor.h"