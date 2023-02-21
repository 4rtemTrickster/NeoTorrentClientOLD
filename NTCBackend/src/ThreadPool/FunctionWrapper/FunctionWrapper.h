#pragma once
#include "NTCpch.h"

namespace NTC
{
    class FunctionWrapper
    {
    public:
        template<typename F>
        FunctionWrapper(F&& f)
            : impl(new impl_type<F>(std::move(f))) {}

        FunctionWrapper(FunctionWrapper&& other) noexcept
            : impl(std::move(other.impl)) {}

        FunctionWrapper& operator=(FunctionWrapper&& other) noexcept
        {
            impl = std::move(other.impl);
            return *this;
        }

        FunctionWrapper(const FunctionWrapper&) = delete;
        FunctionWrapper(FunctionWrapper&) = delete;
        FunctionWrapper() = default;

        FunctionWrapper& operator=(const FunctionWrapper&) = delete;

        void operator()() { impl->call(); }

    protected:
        struct impl_base
        {
            virtual void call() = 0;
            virtual ~impl_base() {}
        };

        template <typename F>
        struct impl_type : impl_base
        {
            F f;
            impl_type(F&& f_) : f(std::move(f_)) {}
            virtual void call() override { f(); }
        };

        Scope<impl_base> impl;
    };
}
