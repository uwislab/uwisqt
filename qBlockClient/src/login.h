#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    bool userRegister();

    bool eventFilter(QObject*, QEvent*);
signals:
    void login(QString username,QString pwd);
    void Register(QString username,QString pwd,QString email);

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
