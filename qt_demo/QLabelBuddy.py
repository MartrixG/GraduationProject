import sys
from PyQt5 import QtWidgets, QtGui


class QLabelBuddy(QtWidgets.QDialog):
    def __init__(self):
        super(QLabelBuddy, self).__init__()
        self.defaultFont = 'Arial'
        self.initUI()

    def initUI(self):
        #Monaco
        nameLabel = QtWidgets.QLabel('&Name', self)
        nameLabel.setFont(QtGui.QFont(self.defaultFont, 10))
        nameLineEdit = QtWidgets.QLineEdit(self)
        nameLineEdit.setFont(QtGui.QFont(self.defaultFont, 10))
        nameLabel.setBuddy(nameLineEdit)

        passwordLabel = QtWidgets.QLabel('&Name', self)
        passwordLabel.setFont(QtGui.QFont(self.defaultFont, 10))
        passwordLineEdit = QtWidgets.QLineEdit(self)
        passwordLineEdit.setFont(QtGui.QFont(self.defaultFont, 10))
        passwordLabel.setBuddy(passwordLineEdit)

        btnOK = QtWidgets.QPushButton("&OK")
        btnOK.setFont(QtGui.QFont(self.defaultFont, 10))
        btnCancel = QtWidgets.QPushButton("&Cancel")
        btnCancel.setFont(QtGui.QFont(self.defaultFont, 10))

        layout = QtWidgets.QGridLayout(self)
        layout.addWidget(nameLabel, 0, 0)
        layout.addWidget(nameLineEdit, 0, 1, 1, 2)

        layout.addWidget(passwordLabel, 1, 0)
        layout.addWidget(passwordLineEdit, 1, 1, 1, 2)

        layout.addWidget(btnOK, 2, 1)
        layout.addWidget(btnCancel, 2, 2)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)

    widget = QLabelBuddy()
    widget.show()

    sys.exit(app.exec_())