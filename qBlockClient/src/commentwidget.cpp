#include "commentwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include "httpmanager.h"
CommentWidget::CommentWidget(const QString& id,const QString& userID, const QString &userName, const QString &PID,const QString &content,
                             const QDateTime &time,
                             QWidget *parent):QWidget(parent)
{
    this->id = id;
    this->userID = userID;
    this->userName = userName;
    this->PID = PID;
    this->content = content;
    this->time = time;
    setupUI();
}

void CommentWidget::setupUI()
{
    // 初始化控件
    m_avatarLabel = new QLabel(this);
    m_avatarLabel->setFixedSize(40, 40);

    m_usernameLabel = new QLabel(userName, this);
    m_usernameLabel->setStyleSheet("font-weight: bold; color: #333;");

    m_contentLabel = new QLabel(content, this);
    m_contentLabel->setWordWrap(true);
    m_contentLabel->setStyleSheet("color: #666;");

    m_timeLabel = new QLabel(time.toString("yyyy-MM-dd hh:mm"), this);
    m_timeLabel->setStyleSheet("color: #999; font-size: 12px;");

    // 布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* rightLayout = new QVBoxLayout;

    rightLayout->addWidget(m_usernameLabel);
    rightLayout->addWidget(m_contentLabel);
    rightLayout->addWidget(m_timeLabel);
    rightLayout->setSpacing(4);

    mainLayout->addWidget(m_avatarLabel);
    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(12);

   // 开始加载头像
   loadAvatar();
}

void CommentWidget::loadAvatar()
{
    // if (m_avatarUrl.isEmpty()) {
    //     m_avatarLabel->setPixmap(QPixmap(":/icon/Avatar.png").scaled(40, 40));
    //     return;
    // }

    HttpManager manager;
    connect(&manager,&HttpManager::downloadFinish,[this](const QByteArray &array){
        QPixmap pixmap;
        pixmap.loadFromData(array);
        QPixmap circlePixmap(40, 40);
        circlePixmap.fill(Qt::transparent);
        QPainter painter(&circlePixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 40, 40);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, 40, 40, pixmap);
        m_avatarLabel->setPixmap(circlePixmap);
    });
    connect(&manager,&HttpManager::notfound,[this](const QByteArray& array){
        QPixmap pixmap(":/icon/Avatar.png");
        QPixmap circlePixmap(40, 40);
        circlePixmap.fill(Qt::transparent);
        QPainter painter(&circlePixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 40, 40);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, 40, 40, pixmap);
        m_avatarLabel->setPixmap(circlePixmap);
    });

    manager.getFile("/getpicture?type=user&id="+userID);
}
