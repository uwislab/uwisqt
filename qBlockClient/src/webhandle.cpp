#include "webhandle.h"
#include "inisettings.h"

WebHandle::WebHandle(QObject *parent)
    : QObject{parent}
{}


void WebHandle::setContent(const QString &value) {
    if (m_content != value) {
        m_content = value;
        emit contentChanged(m_content);
    }
}

void WebHandle::saveState(const QJsonObject &value)
{
    if (m_state != value) {
        m_state = value;
    }
    QString path = "";
    qDebug()<<instanceDir;
    if(instanceDir == "")
    {
        path = IniSettings::getInstance().getValue("Path","workspacePath","/");
    }
    else
    {
        path = instanceDir;
    }
    QString projectName = this->m_projectName;
    if( projectName == "" )
    {
        return;
    }

    QString fileName = path + "/"+ projectName+".json";
    if( fileName == "" )
    {
        return;
    }
    qDebug()<<fileName;
    QFile file(fileName);

    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file);

        out << QString(QJsonDocument(m_state).toJson());
        qDebug()<<QString(QJsonDocument(m_state).toJson());

        file.close();
    }
    else
    {
        qDebug()<<("error");
    }
}

QJsonObject WebHandle::loadState(QString fileName)
{
    QJsonObject stateLoadFromFile;
    //QString fileName = "D:/test/newproject.txt";
    //qDebug()<<fileName;
    qDebug()<<fileName;
    if( fileName == "" )
    {
        return stateLoadFromFile;
    }
    QFile file(fileName);
    QByteArray fileData;
    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        fileData = file.readAll();
        file.close();
    }
    else
    {
        emit loadError();
        qDebug()<<("error");
    }
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData);
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< fileData;
        emit loadError();
        return stateLoadFromFile;
    }
    stateLoadFromFile = jsonDocument.object();
    //qDebug()<<stateLoadFromFile;
    emit loadSuccess();
    return stateLoadFromFile;
}

void WebHandle::updateProjectName(QString projectName)
{
    this->m_projectName=projectName;
}

void WebHandle::updateInstanceDir(QString dirName)
{
    this->instanceDir=dirName;
}
