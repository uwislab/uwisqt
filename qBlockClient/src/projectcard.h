#ifndef PROJECTCARD_H
#define PROJECTCARD_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

namespace Ui {
class ProjectCard;
}

class ProjectCard : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectCard(const QJsonObject &project, QWidget *parent = nullptr): QWidget(parent), m_projectId(project["projectId"].toString())
    {
        setupUI(project);
        setFixedHeight(80);
    }
    //~ProjectCard();
signals:
    void projectDownloadClicked(const QString &projectId, const QString &projectName);
    void projectPublicClicked(const QString &projectId, const QString &projectName);
    void projectPublicChangeClicked(const QString &projectId, const QString &projectName);
    void projectDeleteClicked(const QString &projectId, const QString &projectName);

private:
    void setupUI(const QJsonObject &project);
private:
    Ui::ProjectCard *ui;

    QString m_projectId;
    QString m_projectName;
};

#endif // PROJECTCARD_H
