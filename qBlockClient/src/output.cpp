#include "output.h"
#include "ui_output.h"
#include "inisettings.h"

output::output(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::output)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");

    ui->send_Pb->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->lineEdit->setReadOnly(true);

    // 发送输入到进程
    connect(ui->send_Pb, &QPushButton::clicked, this, &output::sendInputToProcess);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &output::sendInputToProcess);
    connect(ui->refresh_Pb, &QPushButton::clicked, [this](){
        ui->textEdit->clear();
    });
}

output::~output()
{
    delete ui;
}

void output::textappend(QString str)
{
    ui->textEdit->append(str);
}

void output::startPythonProcess(QString scriptPath) {
    ui->textEdit->clear();
    m_process = new QProcess(this);
    QString pythonPath = IniSettings::getInstance().getValue("Path","pythonPath","python");

    // 启动进程
    m_process->start(pythonPath, {scriptPath});
    textappend(">>程序开始运行");
    ui->lineEdit->setReadOnly(false);
    ui->send_Pb->setEnabled(true);
    ui->lineEdit->setFocus();

    // 监听输出信号
    connect(m_process, &QProcess::readyReadStandardOutput, [this]() {
        QString output = ">>" + QString::fromLocal8Bit(m_process->readAllStandardOutput());
        textappend(output);
    });

    // 错误处理
    connect(m_process, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
        QString errorStr;
        switch (error) {
        case QProcess::FailedToStart: errorStr = "进程启动失败"; break;
        case QProcess::Crashed:       errorStr = "进程崩溃";     break;
        case QProcess::Timedout:      errorStr = "进程超时";     break;
        case QProcess::WriteError:    errorStr = "写入错误";     break;
        case QProcess::ReadError:     errorStr = "读取错误";     break;
        default:                      errorStr = "未知错误";     break;
        }

        textappend("进程错误：" + errorStr);
    });

    connect(m_process,&QProcess::finished,[this](){
        ui->lineEdit->setReadOnly(true);
        ui->send_Pb->setEnabled(false);
        textappend(">>程序运行结束\n");
    });
}

void output::sendInputToProcess() {
    QString input =  ui->lineEdit->text();
    if (input.isEmpty()) return;

    // 发送输入到进程的标准输入
    ui->textEdit->insertPlainText(input);
    m_process->write(input.toUtf8() + "\n");
    ui->lineEdit->clear();
    //ui->send_Pb->setEnabled(false);
}
