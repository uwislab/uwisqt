#ifndef COMMUNITYWIDGET_H
#define COMMUNITYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "communitycard.h"


class CommunityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommunityWidget(QWidget *parent = nullptr);

    void populateProjects(const QJsonArray &projects);
signals:
    void projectClicked(const QString &projectId);
    void refreshCommunityWidget();
private:
    void setupUI();
    void onProjectClicked(const QString &projectId);
private:
    QGridLayout *m_gridLayout;
    QVBoxLayout *m_layout;
    QWidget *m_container;
};

#endif // COMMUNITYWIDGET_H
