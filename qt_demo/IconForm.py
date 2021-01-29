import sys
from PyQt5 import QtWidgets, QtGui


class IconForm(QtWidgets.QMainWindow):
    def __init__(self):
        super(IconForm, self).__init__()
        self.initUI()

    def initUI(self):
        self.setGeometry(400, 400, 400, 300)
        self.setWindowTitle("设置图标")
        self.setWindowIcon(QtGui.QIcon('./images/icon.png'))


if __name__ == '__main__':
    # 利用UI类来设置图标，Windows下会覆盖app设置的图标
    app = QtWidgets.QApplication(sys.argv)
    app.setWindowIcon(QtGui.QIcon('./images/icon2.png'))
    main = IconForm()
    main.show()

    sys.exit(app.exec_())
