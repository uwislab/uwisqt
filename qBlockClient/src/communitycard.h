#ifndef COMMUNITYCARD_H
#define COMMUNITYCARD_H

#include <QObject>
#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

class CommunityCard: public QWidget
{
    Q_OBJECT
public:
    explicit CommunityCard(const QJsonObject &project, QWidget *parent = nullptr)
        : QWidget(parent) {
        setupUI(project);
    }
signals:
    void projectClicked(const QString &projectId);
private:
    void setupUI(const QJsonObject &project);
private:
    QString projectId;
};

#endif // COMMUNITYCARD_H
