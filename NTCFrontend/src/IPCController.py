import queue
import threading
from enum import Enum, unique, auto

import zmq


@unique
class LogType(Enum):
    TRACE = 0
    INFO = 1
    WARN = auto()
    ERROR = auto()
    CRITICAL = auto()


class IPCController:
    def __init__(self):
        self._context = zmq.Context()
        self._socket = self._context.socket(zmq.REQ)
        self._socket.connect("ipc://NTC")
        self._socket.set()

        self._MessageQueue = queue.Queue()

        self._SendingThread = threading.Thread(target=self._SendMessageImpl, name="SendingThread")
        self._SendingThread.start()

        self.SendLogMessage("Frontend IPCController initialized successful")

    def SendMessage(self, messageToSend: str):
        self._MessageQueue.put(messageToSend)

    def SendLogMessage(self, messageToSend: str, logType: LogType = LogType.TRACE):
        messageToSend = "LOG:" + logType.name + " " + messageToSend
        self._MessageQueue.put(messageToSend)

    def _SendMessageImpl(self):
        while True:
            if self._MessageQueue.not_empty:
                message = self._MessageQueue.get()
                self._socket.send_string(message)
                self._socket.recv_string()

                if message == "App closes":
                    break
