import sys
from PyQt5.QtWidgets import QWidget, QApplication, QPushButton

app = QApplication(sys.argv)


def onClickButton():
    # 窗口和工作区的差别只在xy值的不同，widget的xy是窗口的位置,widget.geometry的xy是工作区的左上角坐标
    # 窗口和工作区的宽度和高度相同,都是工作区的宽度和高度
    # 框架即整个窗口的最外围,因此xy值和窗口的xy相同.宽度是工作区宽度外加两个像素的边框,高度是工作区的高度加37像素的标题栏加一像素的边框
    print("带标题栏的窗口")
    print("widget.x() = {:}".format(w.x()))
    print("widget.y() = {:}".format(w.y()))
    print("widget.width() = {:}".format(w.width()))
    print("widget.height() = {:}".format(w.height()))
    print("工作区")
    print("widget.geometry().x() = {:}".format(w.geometry().x()))
    print("widget.geometry().y() = {:}".format(w.geometry().y()))
    print("widget.geometry().width() = {:}".format(w.geometry().width()))
    print("widget.geometry().height() = {:}".format(w.geometry().height()))
    #
    print("框架")
    print("widget.frameGeometry().x() = {:}".format(w.frameGeometry().x()))
    print("widget.frameGeometry().y() = {:}".format(w.frameGeometry().y()))
    print("widget.frameGeometry().width() = {:}".format(w.frameGeometry().width()))
    print("widget.frameGeometry().height() = {:}".format(w.frameGeometry().height()))

w = QWidget()
btn = QPushButton(w)
btn.setText("111")
btn.move(30, 30)
btn.clicked.connect(lambda: onClickButton())
w.resize(400, 300)
w.move(200, 250)
w.setWindowTitle('1111')
w.show()

sys.exit(app.exec_())
