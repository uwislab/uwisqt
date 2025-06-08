#include "mainwindow.h"

#include <QApplication>
#include <QWebEngineView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString appDir = QCoreApplication::applicationDirPath();
    qDebug() << "程序路径：" << appDir;
    return a.exec();
}
