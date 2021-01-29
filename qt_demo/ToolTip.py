import sys
from PyQt5 import QtGui, QtWidgets


class ToolTipForm(QtWidgets.QMainWindow):
    def __init__(self):
        super(ToolTipForm, self).__init__()
        self.initUI()

    def initUI(self):
        self.resize(400, 300)
        self.setWindowTitle("控件提示信息")

        QtWidgets.QToolTip.setFont(QtGui.QFont('SansSerif', 12))
        self.setToolTip('今天是<b>星期五</b>')

        self.button1 = QtWidgets.QPushButton('这是一个按钮')
        self.button1.setToolTip('这是一个按钮')

        layout = QtWidgets.QHBoxLayout()
        layout.addWidget(self.button1)

        mainFrame = QtWidgets.QWidget()
        mainFrame.setLayout(layout)

        self.setCentralWidget(mainFrame)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)

    widget = ToolTipForm()
    widget.show()

    sys.exit(app.exec_())