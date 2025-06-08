#ifndef WEBHANDLE_H
#define WEBHANDLE_H

#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>

class WebHandle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString content READ getContent WRITE setContent NOTIFY contentChanged)
    //Q_PROPERTY(QJsonObject state READ loadState WRITE saveState NOTIFY saveStateSignal)
    Q_PROPERTY(QJsonObject state)
public:
    explicit WebHandle(QObject *parent = nullptr);
    QString getContent() const { return m_content; }
    void updateProjectName(QString);
    void updateInstanceDir(QString);

public slots:
    void setContent(const QString &value);
    void saveState(const QJsonObject &value);
    QJsonObject loadState(QString fileName);

private:
    QString m_content;
    QJsonObject m_state;
    QString m_projectName;
    QString instanceDir="";

signals:
    void contentChanged(QString);
    void saveStateSignal();
    void loadStateSignal(const QJsonObject &jsonData);
    void loadError();
    void loadSuccess();
};

#endif // WEBHANDLE_H
