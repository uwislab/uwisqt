#include "cloudprojectswidget.h"
#include "ui_cloudprojectswidget.h"

CloudProjectsWidget::CloudProjectsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CloudProjectsWidget)
{
    //ui->setupUi(this);
    setupUI();
}

CloudProjectsWidget::~CloudProjectsWidget()
{
    delete ui;
}

void CloudProjectsWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *titleLayout = new QHBoxLayout(this);

    // 标题
    QPushButton *deleteBtn = new QPushButton();
    deleteBtn->setIcon(QPixmap(":/icon/refresh.png"));
    deleteBtn->setFixedSize(50, 50);
    deleteBtn->setStyleSheet(R"(
            QPushButton {
                background-color:white;
                border: none;
                border-radius: 5px;
            }
            QPushButton:hover {
                border: 2px solid #99d6ff;
                color: #ecf0f1;
            }

            /* 按下状态 */
            QPushButton:pressed {
                background-color: #73b8e6;
            }
            )");
    QLabel *title = new QLabel("我的云项目");
    title->setStyleSheet(R"(
            font-size: 20px;
            font-weight: bold;
            padding: 15px 20px;
            border-bottom: 1px solid #e0e0e0;
            border-radius: 8px;
            background: white;
        )");
    titleLayout->addWidget(title);
    //titleLayout->addStretch();
    titleLayout->addWidget(deleteBtn);
    mainLayout->addLayout(titleLayout);
    // 滚动区域
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background: transparent;
        }
        QScrollArea > QWidgetViewport {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical {
            width: 10px;
            background: transparent;
            margin: 2px;
        }
        /* 滚动条滑块 */
        QScrollBar::handle:vertical {
            background: #ced4da;
            border-radius: 4px;
            min-height: 30px;
        }

        /* 滚动条滑块悬停状态 */
        QScrollBar::handle:vertical:hover {
            background: #adb5bd;
        }

        /* 滚动条背景 */
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: transparent;
        }

        /* 滚动条上下箭头 */
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
            background: none;
        }
    )");

    m_container = new QWidget;
    m_container->setObjectName("m_container");
    m_container->setAttribute(Qt::WA_StyledBackground);
    m_container->setStyleSheet(R"(
        QWidget#m_container {
            /* 基础样式 */
            background-color: #f8f9fa;
            border-radius: 8px;
            border: 1px solid #dee2e6;
            padding: 12px;

            /* 内部间距控制 */
            margin: 4px;

            /* 阴影效果 */
            box-shadow: 0 2px 4px rgba(0,0,0,0.08);
        }
    )");
    m_layout = new QVBoxLayout(m_container);
    m_layout->setSpacing(8);
    m_layout->setContentsMargins(8, 8, 8, 8);

    scrollArea->setWidget(m_container);
    mainLayout->addWidget(scrollArea);

    connect(deleteBtn,&QPushButton::clicked,[this](){
        emit refreshCloudProjectWidget();
    });
}

void CloudProjectsWidget::populateProjects(const QJsonArray &projects)
{
    // 清空原有内容
    QLayoutItem* item;
    while ((item = m_layout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    // 添加新卡片
    foreach (const QJsonValue &value, projects) {
        ProjectCard *card = new ProjectCard(value.toObject());
        connect(card, &ProjectCard::projectDownloadClicked, [this](const QString& PID, const QString& projectName){
            emit projectDownloadClicked(PID, projectName);
        });
        connect(card, &ProjectCard::projectPublicClicked, [this](const QString&  PID, const QString& projectName){
            emit projectPublicClicked(PID, projectName);
            qDebug()<<"发布2";
        });
        connect(card, &ProjectCard::projectPublicChangeClicked, [this](const QString&  PID, const QString& projectName){
            emit projectPublicChangeClicked(PID, projectName);
            qDebug()<<"修改2";
        });
        connect(card, &ProjectCard::projectDeleteClicked, [this](const QString&  PID, const QString& projectName){
            emit projectDeleteClicked(PID, projectName);
        });

        m_layout->addWidget(card);
    }

    // 添加底部弹簧
    m_layout->addStretch();
}
