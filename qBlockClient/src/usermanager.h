#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDateTime>
#include <QFormLayout>
#include <QStandardPaths>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QRegularExpression>
#include "httpmanager.h"

namespace Ui {
class UserManager;
}

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = nullptr);
    ~UserManager();

    void setUserData(QString userId,QString userName,QString userEmail,QString userLastLogin) {
        m_currentAvatarPath = "";
        m_userId = userId;
        m_userName = userName;
        m_userEmail = userEmail;
        m_userLastLogin = QDateTime::fromString(userLastLogin,"yyyy-MM-dd hh:mm:ss");
        m_userIdLabel ->setText(m_userId);
        HttpManager manager;
        connect(&manager,&HttpManager::downloadFinish,[this](const QByteArray &array){
            qDebug()<<"size"<<array.length();
            QPixmap pixmap;
            pixmap.loadFromData(array);
            emit avatarLoad(pixmap);
            updateAvatar(pixmap);
        });
        connect(&manager,&HttpManager::notfound,[this](const QByteArray &array){
            qDebug()<<"size"<<array.length();
            QPixmap defaultpixmap(":/icon/Avatar.png");
            emit avatarLoad(defaultpixmap);
            updateAvatar(defaultpixmap);
        });
        manager.getFile("/getpicture?type=user&id="+m_userId);
        userNameEdit->setText(m_userName);
        userEmailEdit->setText(m_userEmail);
        lastLoginLabel->setText(m_userLastLogin.toString());
    }
    QString getuserId(){return m_userId;};
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void avatarChanged(QString path, QString userId);
    void avatarLoad(QPixmap img);
    void profileUpdated(QString ID, QString name, QString email, QString AvatarPath);
    void logout();
private slots:
    void onSaveClicked() {
        if(validateInput()) {
            this->m_userName = userNameEdit->text();
            this->m_userEmail = userEmailEdit->text();
            this->m_currentAvatar = m_currentAvatar;
            emit profileUpdated(m_userId,m_userName,m_userEmail,m_currentAvatarPath);
        }
    }
private:
    void setupUI();
    void createAvatarSection(QVBoxLayout *layout);
    void createInfoForm(QVBoxLayout *layout) ;
    void createActionButtons(QVBoxLayout *layout) ;
    void changeAvatar() ;
    void updateAvatar(const QPixmap &pixmap) ;
    QPixmap createCircularPixmap(const QPixmap &source);
    bool validateInput();
    void showValidationError(const QString &msg, QWidget *focusWidget);
private:
    Ui::UserManager *ui;

    QPushButton *m_avatarButton;
    QLabel *m_userIdLabel;
    QLineEdit *userNameEdit;
    QLineEdit *userEmailEdit;
    QLabel *lastLoginLabel;
    QPixmap m_currentAvatar;
    QString m_currentAvatarPath;
    QString m_userId;
    QString m_userName;
    QString m_userEmail;
    QDateTime m_userLastLogin;
};

#endif // USERMANAGER_H
