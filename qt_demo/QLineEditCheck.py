import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QIntValidator, QDoubleValidator, QRegExpValidator
from PyQt5.QtCore import QRegExp


class QLineEditValidator(QWidget):
    def __init__(self):
        super(QLineEditValidator, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('校验器')

        intLineEdit = QLineEdit(self)
        doubleLineEdit = QLineEdit(self)
        validatorLineEdit = QLineEdit(self)

        layout = QFormLayout()
        layout.addRow("整数", intLineEdit)
        layout.addRow("浮点数", doubleLineEdit)
        layout.addRow("正则表达式", validatorLineEdit)

        intValidator = QIntValidator(self)
        intValidator.setRange(1, 99)

        doubleValidator = QDoubleValidator(self)

        doubleValidator.setRange(0.0, 360.0, 2)
        # doubleValidator.setNotation(QDoubleValidator.StandardNotation)
        # doubleValidator.setDecimals(2)

        reg = QRegExp('[a-zA-Z0-9]+$')
        validator = QRegExpValidator(self)
        validator.setRegExp(reg)

        intLineEdit.setValidator(intValidator)
        doubleLineEdit.setValidator(doubleValidator)
        validatorLineEdit.setValidator(validator)

        self.setLayout(layout)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = QLineEditValidator()
    widget.show()
    sys.exit(app.exec_())