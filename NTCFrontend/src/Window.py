from PyQt5.QtWidgets import QMainWindow, QPushButton, QTextEdit, QFileDialog, QAction, QMenuBar, QMenu
from PyQt5 import uic

from IPCController import *


class mainWindow(QMainWindow):
    def __init__(self, IPCC: IPCController):
        super(mainWindow, self).__init__()

        self._IPCC: IPCController = IPCC

        uic.loadUi("../Res/Main.ui", self)

        self.actionAdd_torrent.triggered.connect(self._AddTorrent)

        self.show()

    def _AddTorrent(self):
        FileName: str = QFileDialog.getOpenFileName(self, "Select file", "c:\\", "Torrent files (*.torrent)")
        if FileName[0] != "":
            self._IPCC.SendMessage("READ_FILE: " + FileName[0])
            
        for i in range(0, 100):
            self._IPCC.SendMessage("TEST QUEUE: " + str(i))