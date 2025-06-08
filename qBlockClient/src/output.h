#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>
#include <QProcess>
#include <QPainterPath>

namespace Ui {
class output;
}

class output : public QWidget
{
    Q_OBJECT

public:
    explicit output(QWidget *parent = nullptr);
    ~output();

public slots:
    void textappend(QString str);
    void startPythonProcess(QString scriptPath);//启动进程
    void sendInputToProcess();//输入参数

private:
    Ui::output *ui;
    QProcess *m_process;
};

#endif // OUTPUT_H
