#include "projectcard.h"
#include "ui_projectcard.h"

void ProjectCard::setupUI(const QJsonObject &project)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 20, 10);

    // 左侧图标区域
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/icon/default.png").scaled(40, 40));
    icon->setStyleSheet("margin-right: 15px;");

    // 中央信息区域
    QWidget *infoWidget = new QWidget;
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);

    QLabel *nameLabel = new QLabel(project["title"].toString());
    nameLabel->setStyleSheet("font-size: 16px; font-weight: 500;");

    QLabel *detailLabel = new QLabel(QString("🕒 %1 | 📦 %2 KB")
                                         .arg(project["updateTime"].toString())
                                         .arg(project["size"].toDouble(), 0, 'f', 2));
    detailLabel->setStyleSheet("color: #666; font-size: 13px;");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(detailLabel);

    m_projectId = QString::number(project["projectId"].toInt());
    m_projectName = project["title"].toString();

    // 右侧操作按钮
    QPushButton *actionBtn = new QPushButton("下载");
    actionBtn->setFixedSize(60, 30);
    connect(actionBtn, &QPushButton::clicked, [this](){
        qDebug()<<"aaa"+m_projectId+"aaa";
        emit projectDownloadClicked(m_projectId, m_projectName);
    });
    QPushButton *publicBtn = new QPushButton(project["isPublic"].toInt()==1?"修改":"公开");
    publicBtn->setFixedSize(60, 30);
    if(!(project["isPublic"].toInt()==1))
    {
        connect(publicBtn, &QPushButton::clicked, [this](){
            emit projectPublicClicked(m_projectId, m_projectName);
            qDebug()<<"发布1";
        });
    }
    else
    {
        connect(publicBtn, &QPushButton::clicked, [this](){
            emit projectPublicChangeClicked(m_projectId, m_projectName);
            qDebug()<<"修改1";
        });
    }
    QPushButton *deleteBtn = new QPushButton("删除");
    deleteBtn->setFixedSize(60, 30);
    connect(deleteBtn, &QPushButton::clicked, [this](){
        emit projectDeleteClicked(m_projectId, m_projectName);
    });

    mainLayout->addWidget(icon);
    mainLayout->addWidget(infoWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(actionBtn);
    mainLayout->addWidget(publicBtn);
    mainLayout->addWidget(deleteBtn);
    setAttribute(Qt::WA_StyledBackground);
    // 卡片样式
    setStyleSheet(R"(
            ProjectCard {
                background: white;
                border: 1px solid #e0e0e0;
                border-radius: 6px;
                margin: 4px 8px;
            }
            ProjectCard:hover {
                border-color: #c0c0c0;
                background: #f8f8f8;
            }
            QPushButton {
                border: 1px solid #0078d4;
                border-radius: 4px;
                color: #0078d4;
                padding: 5px;
            }
            QPushButton:hover {
                background: #e5f1fb;
            }
        )");
}
