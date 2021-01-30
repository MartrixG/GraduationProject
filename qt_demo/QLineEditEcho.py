import sys
from PyQt5 import QtWidgets


class QLineEditEcho(QtWidgets.QWidget):
    def __init__(self):
        super(QLineEditEcho, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('回显模式')

        normalLineEdit = QtWidgets.QLineEdit(self)
        noEchoLineEdit = QtWidgets.QLineEdit(self)
        passwordLineEdit = QtWidgets.QLineEdit(self)
        passwordEchoOnEditLine = QtWidgets.QLineEdit(self)

        normalLineEdit.setEchoMode(QtWidgets.QLineEdit.Normal)
        noEchoLineEdit.setEchoMode(QtWidgets.QLineEdit.NoEcho)
        passwordLineEdit.setEchoMode(QtWidgets.QLineEdit.Password)
        passwordEchoOnEditLine.setEchoMode(QtWidgets.QLineEdit.PasswordEchoOnEdit)

        layout = QtWidgets.QFormLayout()
        layout.addRow('Normal', normalLineEdit)
        layout.addRow('NoEcho', noEchoLineEdit)
        layout.addRow('Password', passwordLineEdit)
        layout.addRow('PasswordEchoOnEdit', passwordEchoOnEditLine)

        normalLineEdit.setPlaceholderText('Normal')
        noEchoLineEdit.setPlaceholderText('NoEcho')
        passwordLineEdit.setPlaceholderText('Password')
        passwordEchoOnEditLine.setPlaceholderText('PasswordEchoOnEdit')

        self.setLayout(layout)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    widget = QLineEditEcho()
    widget.show()
    sys.exit(app.exec_())