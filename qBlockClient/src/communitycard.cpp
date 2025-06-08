#include "communitycard.h"

void CommunityCard::setupUI(const QJsonObject &project) {

    setObjectName("CommunityCard");

    // 启用背景绘制
    setAttribute(Qt::WA_StyledBackground);
    setAutoFillBackground(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    int id = project["projectId"].toInt();
    projectId = QString::number(id);
    qDebug()<<"aaa"+projectId;
    // 图片展示区域
    QLabel *imageLabel = new QLabel;
    QPixmap pixmap(":/icon/default.png");
    imageLabel->setPixmap(pixmap.scaled(180, 150, Qt::KeepAspectRatioByExpanding));
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background: #F0F0F0;border: 1px solid #B2EBF2; border-radius: 4px;");

    // 项目名称
    QLabel *nameLabel = new QLabel(project["title"].toString());
    nameLabel->setStyleSheet(R"(
            font-size: 16px;
            font-weight: 600;
            color: #333;
            margin-top: 8px;
        )");

    // 详细信息区域
    QWidget *infoWidget = new QWidget;
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 5, 0, 5);

    // 修改时间
    QLabel *timeLabel = new QLabel(QString("🕒 %1").arg(project["updateTime"].toString()));
    timeLabel->setStyleSheet("color: #666; font-size: 12px;");

    // // 附加信息
    // QLabel *sizeLabel = new QLabel(QString("📦 %1 KB").arg(project["size"].toDouble(), 0, 'f', 2));
    // sizeLabel->setStyleSheet("color: #666; font-size: 12px;");

    // 点击区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *clickButton = new QPushButton("查看详情");
    buttonLayout->addStretch();
    buttonLayout->addWidget(clickButton);
    buttonLayout->addStretch();
    clickButton->setCursor(Qt::PointingHandCursor);
    clickButton->setStyleSheet("width:160px;background-color:white;border:1px solid black;border-radius: 5px;");
    connect(clickButton, &QPushButton::clicked, [this](){
        emit projectClicked(projectId);
    });

    // 组装布局
    infoLayout->addWidget(timeLabel);
    //infoLayout->addWidget(sizeLabel);
    infoLayout->addLayout(buttonLayout);

    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(infoWidget);

    // 卡片样式
    setStyleSheet(R"(
            #CommunityCard {
                background: #FFFFFF;
                border-radius: 10px;
                padding: 15px;
                margin: 10px;
                border: 2px solid #B2EBF2;
                font-family: 'Segoe UI';
            }
            #CommunityCard:hover {
                border: 2px solid #81D4FA;
                background-color: #E3F2FD;
            }
            QLabel#nameLabel {
                color: #00ACC1;        // 深水蓝色标题
                font-size: 16px;
                font-weight: 600;
            }
            QLabel#timeLabel {
                color: #546E7A;       // 中灰色正文
                font-size: 14px;
            }
        )");
}
