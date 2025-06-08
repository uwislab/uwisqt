#include "login.h"
#include "ui_login.h"
#include <QRegularExpression>
#include <QMouseEvent>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    QPixmap source(":/icon/LOGO.png");
    source = source.scaled(ui->label->height()-50, ui->label->height()-50, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    ui->label->setPixmap(source);
    ui->label_3->setPixmap(source.scaled(ui->label_3->height()-10, ui->label_3->height()-10, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    ui->gotoLogin_lb->installEventFilter(this);
    ui->gotoRegister_lb->installEventFilter(this);
    this->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(R"(
        QPushButton#close_pb
        {
            border:none;
            image:url(":/icon/close.png");
        }
        QPushButton#close_pb:hover
        {
            border:none;
            background-color:red;
        }
        QLineEdit#name_le
        {
            border-radius:8px;
            font-size:20px;
        }
        QLineEdit#pwd_le
        {
            border-radius:8px;
            font-size:20px;
        }
        QPushButton#login_pb
        {
            border:none;
            border-radius:8px;
            color:white;
            background-color:rgb(20,114,208);
        }
        QPushButton#login_pb:pressed
        {
            background-color:rgb(49,60,102);
        }
        QPushButton#close_pb_2
        {
            border:none;
            image:url(":/icon/close.png");
        }
        QPushButton#close_pb_2:hover
        {
            border:none;
            background-color:red;
        }
        QLineEdit#name_le_2
        {
            border-radius:8px;
            font-size:20px;
        }
        QLineEdit#pwd_le_2
        {
            border-radius:8px;
            font-size:20px;
        }
        QLineEdit#pwd_le_3
        {
            border-radius:8px;
            font-size:20px;
        }
        QLineEdit#email_le
        {
            border-radius:8px;
            font-size:20px;
        }
        QPushButton#register_pb
        {
            border:none;
            border-radius:8px;
            color:white;
            background-color:rgb(20,114,208);
        }
        QPushButton#register_pb:pressed
        {
            background-color:rgb(49,60,102);
        })");
    connect(ui->close_pb,&QPushButton::clicked,[&](){
        this->close();
    });
    connect(ui->close_pb_2,&QPushButton::clicked,[&](){
        this->close();
    });

    connect(ui->login_pb,&QPushButton::clicked,[&](){
        emit login(ui->name_le->text(),ui->pwd_le->text());
    });
    connect(ui->register_pb,&QPushButton::clicked,[this](){
        userRegister();
    });
}

bool Login::eventFilter(QObject* obj, QEvent* evt)
{
    if (obj == ui->gotoRegister_lb)
    {
        if (evt->type() == QEvent::MouseButtonPress)
        {
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
    else if (obj == ui->gotoLogin_lb)
    {
        if (evt->type() == QEvent::MouseButtonPress)
        {
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
    else if(obj == this)
    {
        QMouseEvent* mouseEv = dynamic_cast<QMouseEvent*>(evt);
        if(mouseEv)
        {
            static QPoint offset;
            if(mouseEv->type()==QEvent::Type::MouseButtonPress)
            {
                offset=mouseEv->globalPosition().toPoint() - this->pos();
            }
            else if(mouseEv->type()==QEvent::Type::MouseMove)
            {
                this->move(mouseEv->globalPosition().toPoint() - offset);
            }
        }
    }
    return QWidget::eventFilter(obj, evt);
}

Login::~Login()
{
    qDebug()<<"close";
    delete ui;
}


bool Login::userRegister()
{
    QString username = ui->name_le_2->text();
    QString pwd = ui->pwd_le_2->text();
    QString email = ui->email_le->text();
    if(username.length()< 3)
    {
        QMessageBox::warning(this, "错误", "用户名不能小于三个字符", QMessageBox::Yes);
        return false;
    }
    if(pwd.length()< 5)
    {
        QMessageBox::warning(this, "错误", "密码不能小于五个字符", QMessageBox::Yes);
        return false;
    }
    if(pwd != ui->pwd_le_3->text())
    {
        QMessageBox::warning(this, "错误", "两次密码不一致", QMessageBox::Yes);
        return false;
    }
    static QRegularExpression emailRegex(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"
        );
    if (!emailRegex.match(ui->email_le->text()).hasMatch()) {
        QMessageBox::warning(this, "错误", "邮箱格式不正确", QMessageBox::Yes);
        return false;
    }

    emit Register(username, pwd, email);
    return true;
}
