#pragma once

namespace NTC
{
    class IPCController
    {
    public:
        IPCController(const IPCController&) = delete;
        IPCController(const IPCController&&) = delete;
        IPCController& operator=( const IPCController&&) = delete;
        IPCController& operator=(const IPCController&) = delete;
        
        virtual ~IPCController() = default;

        inline static void Run() { Instance.RunImp(); }
        
    protected:
        IPCController();

        void RunImp();

        static IPCController Instance;
        
        Ref<zmq::context_t> cont;
        Ref<zmq::socket_t> socket;
    };
}