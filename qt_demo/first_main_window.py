import sys
from PyQt5.QtWidgets import QWidget, QApplication, QMainWindow, QDesktopWidget, QPushButton, QHBoxLayout
from PyQt5.QtGui import QIcon


class MainWin(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('first main window')
        self.resize(400, 300)
        self.status = self.statusBar()
        self.status.showMessage('只显示5秒', 5000)
        self.defaultMove()

        self.button1 = QPushButton('退出')
        self.button1.clicked.connect(lambda: self.onClickButton())

        layout = QHBoxLayout()
        layout.addWidget(self.button1)

        main_frame = QWidget()
        main_frame.setLayout(layout)

        self.setCentralWidget(main_frame)

    def defaultMove(self):
        screen_size = QDesktopWidget().screenGeometry()
        size = self.geometry()
        left = (screen_size.width() - size.width()) / 5
        top = (screen_size.height() - size.height()) / 5
        self.move(left, top)

    def onClickButton(self):
        print(self.sender().text() + '按钮被按下')
        app = QApplication.instance()
        app.quit()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon('./images/icon.png'))

    w = MainWin()
    w.show()
    sys.exit(app.exec_())