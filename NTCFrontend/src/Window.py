from PyQt5.QtWidgets import QMainWindow, QPushButton, QTextEdit, QFileDialog
from PyQt5 import uic

from IPCController import *


class MainWindow(QMainWindow):
    def __init__(self, IPCC: IPCController):
        super(MainWindow, self).__init__()

        self._IPCC: IPCController = IPCC

        uic.loadUi("../Res/Main.ui", self)

        self.SelectPathPushButton = self.findChild(QPushButton, "SelectPathPushButton")

        self.SelectPathPushButton2 = self.findChild(QPushButton, "SelectPathPushButton_2")

        self.SepentPathTextEdit = self.findChild(QTextEdit, "SepentPathTextEdit")
        self.NeitPathTextEdit = self.findChild(QTextEdit, "NeitPathTextEdit")

        self.SelectPathPushButton.clicked.connect(lambda: self._ChDir(self.SepentPathTextEdit))
        self.SelectPathPushButton2.clicked.connect(lambda: self._ChDir(self.NeitPathTextEdit))

        self.show()

    def _ChDir(self, TextEdit):
        FileName: str = QFileDialog.getOpenFileName(self, "Select file", "c:\\")

        if FileName[0] != "":
            TextEdit.setPlainText(FileName[0])
            self._IPCC.SendMessage(FileName[0])
