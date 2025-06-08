#include "httpmanager.h"
#include "mainwindow.h"

HttpManager::HttpManager(QObject *parent) : QObject(parent)
{
    //QNetworkReply自动释放
    manager.setAutoDeleteReplies(true);
}

void HttpManager::login(QString username,QString pwd)
{
    QNetworkRequest requestSalt;
    requestSalt.setUrl(QUrl((baseurl+"/getsalt?username=%1").arg(username)));
    requestSalt.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *replySalt = manager.get(requestSalt);
    QEventLoop eventLoopSalt;
    //数据可读
    QJsonDocument rcvJsonDocument;
    connect(replySalt, &QNetworkReply::finished, &eventLoopSalt, &QEventLoop::quit);
    eventLoopSalt.exec();
    QByteArray bytes = replySalt->readAll();
    rcvJsonDocument = QJsonDocument::fromJson(bytes);
    QJsonObject rcvJsonObject;
    rcvJsonObject = rcvJsonDocument.object();
    QString salt = rcvJsonObject["salt"].toString();

    QNetworkRequest requestLogin;
    requestLogin.setUrl(QUrl(baseurl+"/login"));
    requestLogin.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject jsonObj;
    QByteArray byteArray;
    byteArray.append(pwd.toStdString());
    byteArray.append(salt.toStdString());
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);
    QString str256 = hash.toHex();
    jsonObj.insert("username",username);
    jsonObj.insert("password",str256);
    QJsonDocument jsonDoc = QJsonDocument(jsonObj);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    qDebug()<<post_data;
    QNetworkReply *replyLogin = manager.post(requestLogin,post_data);

    QEventLoop eventLoopLogin;
    connect(replyLogin, &QNetworkReply::finished, &eventLoopLogin, &QEventLoop::quit);
    eventLoopLogin.exec();
    bytes = replyLogin->readAll();
    rcvJsonDocument = QJsonDocument::fromJson(bytes);
    rcvJsonObject = rcvJsonDocument.object();
    if(rcvJsonObject["success"] == true)
    {
        qDebug()<<"success";
        QString cookie = replyLogin->rawHeader("Set-Cookie");
        QString sessionId;
        if (!cookie.isEmpty())
        {
            size_t pos = cookie.indexOf("sessionId=");
            if (pos != std::string::npos)
            {
                pos += 10; // 跳过"sessionId="
                size_t end = cookie.indexOf(';', pos);
                if (end != -1)
                {
                    sessionId = cookie.mid(pos, end - pos);
                }
                else
                {
                    sessionId = cookie.mid(pos);
                }
            }
        }
        qDebug()<<sessionId;
        IniSettings::getInstance().setValue("User","sessionId",sessionId);
        MainWindow::permission = rcvJsonObject["permission"].toInt();
        emit loginSuccess();
    }
    else
    {
        emit loginFail();
    }

    qDebug()<<bytes;
}

void HttpManager::logout()
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/logout"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        emit logoutSuccess();
        IniSettings::getInstance().setValue("User","sessionId","");
    }
    else
    {
        emit logoutFail();
    }
}

void HttpManager::userRegister(QString username,QString pwd,QString email)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QJsonObject jsonObj;

    jsonObj.insert("username",username);
    jsonObj.insert("password",pwd);
    jsonObj.insert("email",email);
    QJsonDocument jsonDoc = QJsonDocument(jsonObj);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    qDebug()<<post_data;
    QNetworkReply *replyRegister = manager.post(request,post_data);

    QEventLoop eventLoopRegister;
    connect(replyRegister, &QNetworkReply::finished, &eventLoopRegister, &QEventLoop::quit);
    eventLoopRegister.exec();
    QByteArray bytes = replyRegister->readAll();
    QJsonDocument rcvJsonDocument = QJsonDocument::fromJson(bytes);

    if(rcvJsonDocument["status"] == "success")
    {
        emit registerSuccess();
    }
    else
    {
        emit registerFail();
    }
}

QJsonObject HttpManager::getuserInfo()
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/getuserinfo"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();
    QJsonDocument rcvJsonDocument = QJsonDocument::fromJson(bytes);
    QJsonObject userInfo = rcvJsonDocument.object();
    qDebug()<<rcvJsonDocument;
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<status_code;
    qDebug()<<"[download JSON] finished";
    return userInfo;
}

void HttpManager::userInfoChange(QString username,QString email)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/userinfochange"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QJsonObject obj;
    obj.insert("username","username");
    obj.insert("email","email");
    QJsonDocument jsonDoc = QJsonDocument(obj);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    qDebug()<<post_data;
    QNetworkReply *reply = manager.post(request,post_data);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();
    QJsonDocument rcvJsonDocument = QJsonDocument::fromJson(bytes);
    QJsonObject userInfo = rcvJsonDocument.object();
    qDebug()<<rcvJsonDocument;
    if(reply->error() == QNetworkReply::NoError)
    {
        emit userInfoChangeSuccess();
    }
    else
    {
        emit userInfoChangeFail();
    }
}

void HttpManager::sessionAvailable()
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/sessionavailable"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();
    QJsonDocument rcvJsonDocument = QJsonDocument::fromJson(bytes);
    QJsonObject userInfo = rcvJsonDocument.object();
    qDebug()<<rcvJsonDocument;
    if(reply->error() == QNetworkReply::NoError)
    {
        emit sessionIsAvailable();
    }
    else
    {
        emit sessionNotAvailable();
    }
}


void HttpManager::postCloudProject(const QString& userId, const QString& fileName, const QString& path, const QString& fileId)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/postfile?userId="+userId+"&fileName="+fileName+"&fileId="+fileId));
    qDebug()<<QUrl(baseurl+"/postfile?userId="+userId+"&fileName="+fileName+"&fileId="+fileId);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/zip"));
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        qDebug()<<"无法打开";
    }
    qDebug()<<file.fileName();
    QByteArray data = file.readAll();
    int len = data.size();
    qDebug()<<len;
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    request.setHeader(QNetworkRequest::ContentLengthHeader,QString::number(len).toUtf8());
    QNetworkReply *reply = manager.post(request,data);
    file.close();
    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"[upload] finished";
        emit postCloudProjectSuccess();
    }
    else
    {
        qDebug()<<"[upload] failed";
        emit postCloudProjectFail();
    }
}

QJsonArray HttpManager::getCommunityProjectList()
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/getpublicfilelist"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    QJsonDocument rcvJsonDocument;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();

    rcvJsonDocument =  QJsonDocument::fromJson(bytes);
    QJsonArray communityFileList = rcvJsonDocument.array();
    qDebug()<<rcvJsonDocument;
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<status_code;
    qDebug()<<"[download JSON] finished";

    return communityFileList;
}

QJsonObject HttpManager::getCommunityProjectInfo(QString id)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/getpublicfileinfo?projectId="+id));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    QJsonDocument rcvJsonDocument;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();
    
    rcvJsonDocument =  QJsonDocument::fromJson(bytes);
    QJsonObject communityFileInfo = rcvJsonDocument.object();
    qDebug()<<rcvJsonDocument;
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<status_code;
    qDebug()<<"[download JSON] finished";
    return communityFileInfo;
}

void HttpManager::postCommunityProject(QString PID, QString title, QString content)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/filepublic?fileId="+PID));
    qDebug()<<QUrl(baseurl+"/filepublic?fileId="+PID);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QJsonObject jsonObj;

    jsonObj.insert("title",title);
    jsonObj.insert("description",content);
    QJsonDocument jsonDoc = QJsonDocument(jsonObj);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    qDebug()<<post_data;
    QNetworkReply *reply = manager.post(request,post_data);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"[upload] finished";
        emit publicCloudProjectSuccess();
    }
    else
    {
        qDebug()<<reply->error();
        qDebug()<<"[upload] failed";
        emit publicCloudProjectFail();
    }
}

void HttpManager::deleteCommunityProject(QString id)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/delete?type=communityproject&ID="+id));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        emit deleteCommunityProjectSuccess();
    }
    else
    {
        emit deleteCommunityProjectFail();
    }
}


QJsonArray HttpManager::getCloudProjectList()
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/getuserfilelist"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    QJsonDocument rcvJsonDocument;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();
    
    rcvJsonDocument =  QJsonDocument::fromJson(bytes);
    QJsonArray cloudFileList = rcvJsonDocument.array();
    qDebug()<<rcvJsonDocument;
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<status_code;
    qDebug()<<"[download JSON] finished";
    return cloudFileList;
}

void HttpManager::deleteCloudProject(QString pid)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/delete?type=cloudproject&ID="+pid));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        emit deleteCloudProjectSuccess();
    }
    else
    {
        qDebug()<<reply->error();
        emit deleteCloudProjectFail();
    }
}


QJsonArray HttpManager::getCommentList(QString PID)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/getcommentlist?fileId="+PID));
    qDebug()<<QUrl(baseurl+"/getcommentlist?fileId="+PID);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    QJsonDocument rcvJsonDocument;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QByteArray bytes = reply->readAll();
    
    rcvJsonDocument =  QJsonDocument::fromJson(bytes);
    QJsonArray commentList = rcvJsonDocument.array();
    qDebug()<<rcvJsonDocument;
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<status_code;
    qDebug()<<"[download JSON] finished";
    return commentList;
}

void HttpManager::commentPost(const QString &PID,const QString &content,const QDateTime &time)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/postcomment"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QJsonObject jsonObj;

    jsonObj.insert("fileId",PID);
    jsonObj.insert("content",content);
    jsonObj.insert("time",time.toString("yyyy-MM-dd HH:mm:ss"));

    QJsonDocument jsonDoc = QJsonDocument(jsonObj);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    qDebug()<<post_data;
    QNetworkReply *reply = manager.post(request,post_data);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonDocument rcvJsonDocument = QJsonDocument::fromJson(bytes);
        QJsonObject rcvJsonObject = rcvJsonDocument.object();
        qDebug()<<rcvJsonObject;
        qDebug()<<"成功";
        emit commentPostSuccess(rcvJsonObject["commentId"].toInt(),rcvJsonObject["userId"].toInt(), rcvJsonObject["username"].toString());
    }
    else
    {
        qDebug()<<reply->error();
        qDebug()<<"失败";
        emit commentPostFail();
    }
}



void HttpManager::commentDelete(QString commentID)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/delete?type=comment&ID="+commentID));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        emit commentDeleteSuccess();
    }
    else
    {
        emit commentDeleteFail();
    }
}
void HttpManager::updateCheck()
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/latestversion"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonDocument rcvJsonDocument = QJsonDocument::fromJson(bytes);
        QJsonObject rcvJsonObject = rcvJsonDocument.object();

        emit versionResult(rcvJsonObject["version"].toString());
    }
}

QByteArray HttpManager::getFile(const QString& path)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl + path));
    //request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/zip"));
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    QNetworkReply *reply = manager.get(request);

    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QByteArray bytes = reply->readAll();
    qDebug()<<bytes.length();
    if (reply->error() == QNetworkReply::NoError) {
        emit downloadFinish(bytes);
        qDebug()<<"downloadFinish";
    }
    else if(reply->error() == QNetworkReply::ContentNotFoundError)
    {
        emit notfound(QByteArray());
    }
    return bytes;
}

void HttpManager::postImg(QString type,QString id, QString ImgPath)
{
    if(ImgPath.isEmpty())
    {
        emit postImgSuccess();
        return;
    }
    QNetworkRequest request;
    request.setUrl(QUrl(baseurl+"/postpicture?type="+type+"&id="+id));
    qDebug()<<QUrl(baseurl+"/postpicture?type="+type+"&id="+id);
    QFile file(ImgPath);
    if (!file.open(QFile::ReadOnly))
    {
        qDebug()<<"无法打开";
    }
    QFileInfo fileInfo = QFileInfo(ImgPath);
    QString imgtype = fileInfo.suffix();
    qDebug()<<file.fileName();
    if(imgtype=="jpg"||imgtype=="jpeg")
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("image/jpeg"));
    }
    else
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("image/png"));
    }
    QByteArray data = file.readAll();
    int len = data.size();
    qDebug()<<len;
    QNetworkCookie sessionId("sessionId",IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit());
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(sessionId));
    request.setHeader(QNetworkRequest::ContentLengthHeader,QString::number(len).toUtf8());
    QNetworkReply *reply = manager.post(request,data);
    file.close();
    //同步等待
    QEventLoop eventLoop;
    //数据可读
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        emit postImgSuccess();
        qDebug()<<"[upload] finished";
    }
    else
    {
        emit postImgFail();
        qDebug()<<"[upload] failed";
    }
}


