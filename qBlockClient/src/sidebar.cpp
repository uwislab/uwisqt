#include "sidebar.h"
#include "ui_sidebar.h"

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setBotton(ui->editor_Pb,":/icon/editor.png","编辑器");
    setBotton(ui->cloud_Pb,":/icon/cloud.png","云文件");
    setBotton(ui->community_Pb,":/icon/community.png","社区");

    QPixmap source(":/icon/Avatar.png");
    source = source.scaled(60, 60, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap circularImg(60, 60);
    circularImg.fill(Qt::transparent);
    {
        QPainter painter(&circularImg);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(source));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 60, 60);
    }

    ui->user_Pb->setFixedSize(60, 60);
    ui->user_Pb->setIcon(QIcon(circularImg));
    ui->user_Pb->setIconSize(QSize(60, 60));
    ui->user_Pb->setStyleSheet(R"(
    QPushButton {
        border: 2px solid #cccccc;
        background-color: white;
        border-radius: 30px;
    }
    QPushButton:hover {
        border-color: #0078d4;
    }
    QPushButton:pressed {
        border-color: #005a9e;
    }
)");

    connect(ui->user_Pb,&QPushButton::clicked,[this](){
        emit openUserWidget();
    });
    connect(ui->editor_Pb,&QPushButton::clicked,[this](){
        emit openEditorWidget();
    });
    connect(ui->cloud_Pb,&QPushButton::clicked,[this](){
        emit openCloudFileWidget();
    });
    connect(ui->community_Pb,&QPushButton::clicked,[this](){
        emit openCommunityWidget();
    });

}

void SideBar::setBotton(QPushButton *btn,const QString &iconPath,const QString &text)
{
    QLabel *iconLabel = new QLabel(btn);
    iconLabel->setPixmap(QPixmap(iconPath).scaled(32,32,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    QLabel *textLabel = new QLabel(text,btn);

    btn->setStyleSheet(R"(
            QPushButton {
                background-color:#80ccff;
                text-align: left;
                padding: 12px 20px;
                border: none;
                border-radius: 5px;
                font-size: 14px;
                font-family: "Segoe UI", Arial;
            }
            QPushButton:hover {
                border: 2px solid aqua;
                color: #ecf0f1;
            }

            /* 按下状态 */
            QPushButton:pressed {
                background-color: #73b8e6;
            }
            )");

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(btn);
    layout->addWidget(iconLabel, 0, Qt::AlignHCenter); // 图片水平居中
    layout->addWidget(textLabel, 0, Qt::AlignHCenter);
    layout->setSpacing(5); // 调整图文间距
}

SideBar::~SideBar()
{
    delete ui;
}

void SideBar::updateAvatar(QPixmap source)
{
    // QPixmap source;
    // source.loadFromData(array);
    source = source.scaled(60, 60, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap circularImg(60, 60);
    circularImg.fill(Qt::transparent);
    {
        QPainter painter(&circularImg);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(source));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 60, 60);
    }

    ui->user_Pb->setFixedSize(60, 60);
    ui->user_Pb->setIcon(QIcon(circularImg));
    ui->user_Pb->setIconSize(QSize(60, 60));
}
