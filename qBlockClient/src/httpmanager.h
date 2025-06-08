#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QCryptographicHash>
#include "inisettings.h"

class HttpManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpManager(QObject *parent = nullptr);

public slots:
    void login(QString username,QString pwd);
    void logout();
    void userRegister(QString username,QString pwd,QString email);
    QJsonObject getuserInfo();
    void userInfoChange(QString username,QString email);
    void sessionAvailable();

    QJsonArray getCloudProjectList();
    void postCloudProject(const QString& userId, const QString& fileName, const QString& path, const QString& fileId);
    void deleteCloudProject(QString pid);
    QJsonArray getCommunityProjectList();
    QJsonObject getCommunityProjectInfo(QString id);
    void postCommunityProject(QString PID, QString title, QString content);
    void deleteCommunityProject(QString id);

    void postImg(QString type,QString id,QString ImgPath);
    QByteArray getFile(const QString& path);

    QJsonArray getCommentList(QString PID);
    void commentPost(const QString &PID,const QString &content,const QDateTime &time);
    void commentDelete(QString commentID);

    void updateCheck();

signals:
    void loginSuccess();
    void loginFail();
    void logoutSuccess();
    void logoutFail();
    void registerSuccess();
    void registerFail();
    void userInfoChangeSuccess();
    void userInfoChangeFail();
    void sessionIsAvailable();
    void sessionNotAvailable();

    void postCloudProjectSuccess();
    void postCloudProjectFail();
    void deleteCloudProjectSuccess();
    void deleteCloudProjectFail();

    void publicCloudProjectSuccess();
    void publicCloudProjectFail();
    void deleteCommunityProjectSuccess();
    void deleteCommunityProjectFail();

    void commentPostSuccess(int id, int uid, QString userName);
    void commentPostFail();
    void commentDeleteSuccess();
    void commentDeleteFail();

    void postImgSuccess();
    void postImgFail();
    void downloadFinish(const QByteArray& data);
    void notfound(const QByteArray& data);

    void versionResult(QString version);

private:
    QNetworkAccessManager manager;
    QString baseurl = "http://"+IniSettings::getInstance().getValue("Server","ip","118.24.161.211")+":"
                      +IniSettings::getInstance().getValue("Server","port","80");
};

#endif // HTTPMANAGER_H
