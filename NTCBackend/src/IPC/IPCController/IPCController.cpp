#include "NTCpch.h"
#include "IPCController.h"

#include "IPC/MessageQueue/MessageQueue.h"

namespace NTC
{
    IPCController IPCController::Instance;

    static const std::string ReceivedStr = "Received";

    IPCController::IPCController()
    {
        cont = CreateRef<zmq::context_t>();
        socket = CreateRef<zmq::socket_t>(*cont, zmq::socket_type::rep);

        socket->bind("ipc://NTC");
    }

    void IPCController::RunImp()
    {
        NTC_PROFILE_FUNCTION();
        while (true)
        {
            zmq::message_t request;

            zmq::recv_result_t res = socket->recv(request, zmq::recv_flags::none);
            if (res)
            {
                LOG_MESSAGE("Recived from frontend: " + request.to_string());
                MessageQueue::AddMessage(request.to_string());
                socket->send(zmq::buffer(ReceivedStr), zmq::send_flags::none);
            }
            else
                NTC_WARN("Recived frontend: NONE");

            if (request.to_string() == "App closes") break;
        }

        NTC_TRACE("Message input is ended");
    }
}