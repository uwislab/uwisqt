#ifndef CLOUDPROJECTSWIDGET_H
#define CLOUDPROJECTSWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include "projectcard.h"
#include "QtCore/private/qzipreader_p.h"
#include "httpmanager.h"

namespace Ui {
class CloudProjectsWidget;
}

class CloudProjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CloudProjectsWidget(QWidget *parent = nullptr);
    ~CloudProjectsWidget();

    void populateProjects(const QJsonArray &projects);
signals:
    void projectDownloadClicked(const QString &projectId, const QString& projectName);
    void projectPublicClicked(const QString &projectId, const QString& projectName);
    void projectPublicChangeClicked(const QString &projectId, const QString &projectName);
    void projectDeleteClicked(const QString &projectId, const QString& projectName);

    void refreshCloudProjectWidget();
private:
    void setupUI();
private:
    Ui::CloudProjectsWidget *ui;

    QVBoxLayout *m_layout;
    QWidget *m_container;
};

#endif // CLOUDPROJECTSWIDGET_H
