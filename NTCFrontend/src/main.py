from PyQt5 import QtWidgets
import sys

from IPCController import *
from Window import *


class FrontedApp:
    def __init__(self):
        self._IPCC = IPCController()
        self.app = QtWidgets.QApplication(sys.argv)
        self.app.aboutToQuit.connect(self._ATQ)

        self._Window = mainWindow(self._IPCC)

        self._IPCC.SendLogMessage("Frontend app initialized successful")

    def _ATQ(self):
        self._IPCC.SendMessage("App closes")

    def exec(self):
        self.app.exec_()


if __name__ == "__main__":
    App = FrontedApp()
    sys.exit(App.exec())

