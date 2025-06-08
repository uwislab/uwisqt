#include "cloudprojectview.h"
#include <QFileInfo>
#include <QDateTime>
#include <QNetworkReply>
#include <QScrollBar>
#include <QMessageBox>
#include "zipcompress.h"
#include "mainwindow.h"


CloudProjectView::CloudProjectView(QWidget *parent) : QWidget(parent) {
    setupUI();
    setMinimumSize(700, 1000);
    setStyleSheet(R"(
        QWidget {
            font-family: 'Segoe UI', Arial;
            background: #f8f9fa;
        }
        QLabel#title {
            font-size: 28px;
            color: #1a1a1a;
            font-weight: 600;
            margin-bottom: 15px;
        }
        QTextBrowser {
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            padding: 15px;
            background: white;
            font-size: 15px;
            line-height: 1.6;
        }
        QPushButton#deleteBtn {
            background-color: #0078d4;
            color: white;
            border-radius: 6px;
            padding: 12px 24px;
            font-size: 16px;
            min-width: 120px;
        }
        QPushButton#deleteBtn:hover {
            background-color: #006cbd;
        }
        QPushButton#downloadBtn {
            background-color: #0078d4;
            color: white;
            border-radius: 6px;
            padding: 12px 24px;
            font-size: 16px;
            min-width: 120px;
        }
        QPushButton#downloadBtn:hover {
            background-color: #006cbd;
        }
        QListWidget {
            background: white;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            padding: 10px;
        }
        QLineEdit {
            border: 1px solid #e0e0e0;
            border-radius: 20px;
            padding: 12px 20px;
            font-size: 14px;
        }
    )");
}

void CloudProjectView::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(25);

    createHeaderSection(mainLayout);
    createImageSection(mainLayout);
    createDescriptionSection(mainLayout);
    createDownloadSection(mainLayout);
    createCommentSection(mainLayout);
}

void CloudProjectView::createHeaderSection(QVBoxLayout *mainLayout)
{
    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("title");
    m_titleLabel->setWordWrap(true);
    mainLayout->addWidget(m_titleLabel);
}

void CloudProjectView::createImageSection(QVBoxLayout *mainLayout)
{
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumHeight(300);
    m_imageLabel->setStyleSheet(R"(
        QLabel {
            border: 1px solid #e0e0e0;
            border-radius: 12px;
            background: white;
        }
    )");
    mainLayout->addWidget(m_imageLabel);
}

void CloudProjectView::createDescriptionSection(QVBoxLayout *mainLayout)
{
    m_descriptionBrowser = new QTextBrowser(this);
    m_descriptionBrowser->setOpenExternalLinks(true);
    mainLayout->addWidget(m_descriptionBrowser);
}

void CloudProjectView::createDownloadSection(QVBoxLayout *mainLayout)
{
    QHBoxLayout *downloadLayout = new QHBoxLayout;

    m_downloadButton = new QPushButton("下载项目", this);
    m_downloadButton->setObjectName("downloadBtn");
    connect(m_downloadButton, &QPushButton::clicked,
            this, &CloudProjectView::onDownloadClicked);
    m_deleteButton = new QPushButton("删除项目", this);
    m_deleteButton->setObjectName("deleteBtn");
    connect(m_deleteButton, &QPushButton::clicked,[this](){
        emit deleteCommunityProject(PID);
    });
    downloadLayout->addWidget(m_deleteButton);
    downloadLayout->addStretch();
    downloadLayout->addWidget(m_downloadButton);
    mainLayout->addLayout(downloadLayout);
    m_deleteButton->setVisible(false);
}

void CloudProjectView::createCommentSection(QVBoxLayout *mainLayout)
{
    QLabel *commentTitle = new QLabel("评论", this);
    commentTitle->setStyleSheet("font-size: 20px; color: #333;");

    m_commentList = new QListWidget(this);
    m_commentList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_commentList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_commentList->verticalScrollBar()->setSingleStep(10);

    QHBoxLayout *commentInputLayout = new QHBoxLayout;
    m_commentInput = new QLineEdit(this);
    m_commentInput->setPlaceholderText("输入您的评论...");
    QPushButton *submitButton = new QPushButton("发布", this);
    submitButton->setStyleSheet(R"(
        QPushButton {
            background: #28a745;
            color: white;
            border-radius: 15px;
            padding: 8px 20px;
            margin-left: 10px;
        }
        QPushButton:hover {
            background: #218838;
        }
    )");
    connect(submitButton, &QPushButton::clicked,
            this, &CloudProjectView::onSubmitComment);

    commentInputLayout->addWidget(m_commentInput);
    commentInputLayout->addWidget(submitButton);

    QVBoxLayout *commentLayout = new QVBoxLayout;
    commentLayout->addWidget(commentTitle);
    commentLayout->addWidget(m_commentList);
    commentLayout->addLayout(commentInputLayout);

    mainLayout->addLayout(commentLayout);
}

void CloudProjectView::setProjectData(const QString &title,
                                   const QString &description,
                                   const QString &PID,
                                   const QString &projectName)
{
    // 设置标题
    m_titleLabel->setText(title);
    this->PID = PID;
    this->projectName = projectName;

    // 加载图片
    HttpManager manager;
    connect(&manager,&HttpManager::downloadFinish,[this](const QByteArray &array){
        QPixmap pixmap;
        pixmap.loadFromData(array);
        if(array.length() == 0){
            pixmap = QPixmap(":/icon/default.png");
            qDebug()<<"array.length()";
        }
        pixmap = pixmap.scaled(m_imageLabel->width() - 20, // 保留边距
                               m_imageLabel->height(),
                               Qt::KeepAspectRatio,
                               Qt::SmoothTransformation);
        m_imageLabel->setPixmap(pixmap);
    });
    connect(&manager,&HttpManager::notfound,[this](const QByteArray &array){
        QPixmap pixmap(":/icon/default.png");
        pixmap = pixmap.scaled(m_imageLabel->width() - 20, // 保留边距
                               m_imageLabel->height(),
                               Qt::KeepAspectRatio,
                               Qt::SmoothTransformation);
        m_imageLabel->setPixmap(pixmap);
    });
    manager.getFile("/getpicture?type=file&id="+PID);
    qDebug()<<PID;

    // 设置描述
    QString formattedDesc = QString("<p style='line-height:1.6'>%1</p>")
                                .arg(description);
    m_descriptionBrowser->setHtml(formattedDesc);

    m_downloadButton->setText(QString("下载项目"));
}

void CloudProjectView::onDownloadClicked()
{
    QMessageBox::information(this, "下载", "开始下载项目文件...");
    HttpManager filemanager;
    connect(&filemanager,&HttpManager::downloadFinish,[&](QByteArray array){
        QString filePath = IniSettings::getInstance().getValue("Path","workspacePath","/")+"/"+projectName+".zip";
        QFile file(filePath);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(array);
            file.close();
        }
        else
        {
            qDebug()<<("创建失败");
        }
        qDebug()<<filePath;
        qDebug()<<file.size();
        QString DirPath = IniSettings::getInstance().getValue("Path","workspacePath","/")+"/"+projectName;
        QDir nowDir(DirPath);
        if(!nowDir.exists())
        {
            nowDir.mkdir(DirPath);
        }
        ZipCompress zip;
        zip.zipUnCompressFile(filePath,DirPath);
        QFile::remove(filePath);
        QFile::remove(DirPath+"/project.ini");
        QMessageBox::information(this, "下载", "下载完成");
    });
    filemanager.getFile("/getfile?fileId="+PID);

}

void CloudProjectView::onSubmitComment()
{
    QString comment = m_commentInput->text().trimmed();
    if(comment.isEmpty()) return;
    HttpManager manager;
    connect(&manager,&HttpManager::commentPostSuccess,[this,&comment](int commentId, int uid, QString username){
        CommentWidget* widget = new CommentWidget(QString::number(commentId), QString::number(uid), username, PID, comment, QDateTime::currentDateTime());
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(widget->sizeHint());


        m_commentList->addItem(item);
        m_commentList->setItemWidget(item, widget);
        m_commentInput->clear();

        // 滚动到底部
        m_commentList->scrollToBottom();
    });
    connect(&manager,&HttpManager::commentPostFail,[this](){
        QMessageBox::critical(this,"错误","评论上传失败");
    });
    manager.commentPost(this->PID,comment,QDateTime::currentDateTime());
}


void CloudProjectView::populateComment(const QJsonArray &Comments)
{
    m_commentList->clear();
    // 添加新评论
    for (const auto& comment : Comments)
    {
        QJsonObject obj = comment.toObject();
        qDebug()<<obj;
        CommentWidget* widget = new CommentWidget(
            QString::number(obj["commentId"].toInt()),
            QString::number(obj["userId"].toInt()),
            obj["userName"].toString(),
            QString::number(obj["projectId"].toInt()),
            obj["content"].toString(),
            QDateTime::fromString(obj["time"].toString(), Qt::ISODate)
        );
        // 创建list item
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(widget->sizeHint());

        // 添加到列表
        m_commentList->addItem(item);
        m_commentList->setItemWidget(item, widget);
    }

}

void CloudProjectView::btnsetVisible(bool premission)
{
    this->m_deleteButton->setVisible(premission);
}
