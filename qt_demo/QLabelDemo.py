import sys
from PyQt5 import QtGui, QtWidgets, QtCore


class QLabelDemo(QtWidgets.QWidget):
    def __init__(self):
        super(QLabelDemo, self).__init__()
        self.initUI()

    def initUI(self):
        label1 = QtWidgets.QLabel(self)
        label2 = QtWidgets.QLabel(self)
        label3 = QtWidgets.QLabel(self)
        label4 = QtWidgets.QLabel(self)
        QtWidgets.QToolTip.setFont(QtGui.QFont('SansSerif', 10))

        # 富文本显示文本
        label1.setText("<font color=yellow size=5>这是一个文本标签.</font>")
        # 利用调色板设置背景色
        label1.setAutoFillBackground(True)
        palette = QtGui.QPalette()
        palette.setColor(QtGui.QPalette.Window, QtCore.Qt.blue)
        label1.setPalette(palette)
        # 设置对齐方式
        label1.setAlignment(QtCore.Qt.AlignCenter)

        label2.setText("<font size=5><a href='#'><b>欢迎使用Python GUI</b></a></font>")

        label3.setToolTip("这是一张图片")
        label3.setAlignment(QtCore.Qt.AlignCenter)
        label3.setPixmap(QtGui.QPixmap('./images/pic.png'))
        # 设为true则为浏览器打开
        # label4.setOpenExternalLinks(True)
        label4.setText("<font size=4><a href='https://www.bilibili.com/'><b>这是一个B站</b></a></font>")
        label4.setAlignment(QtCore.Qt.AlignRight)
        label4.setToolTip("这是个超链接")

        vbox = QtWidgets.QVBoxLayout()
        vbox.addWidget(label1)
        vbox.addWidget(label2)
        vbox.addWidget(label3)
        vbox.addWidget(label4)

        self.setLayout(vbox)

        label2.linkHovered.connect(lambda: self.linkHovered())

        label4.linkActivated.connect(lambda: self.linkActivated())

    def linkHovered(self):
        print("鼠标滑过了2标签")

    def linkActivated(self):
        print("鼠标点击了4标签")


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)

    widget = QLabelDemo()
    widget.show()

    sys.exit(app.exec_())