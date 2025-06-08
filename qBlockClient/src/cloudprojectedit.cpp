#include "cloudprojectedit.h"
#include <QFileDialog>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QStackedLayout>
#include <QStandardPaths>

CloudProjectEdit::CloudProjectEdit(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setMinimumSize(600, 700);
    setStyleSheet(R"(
        QWidget {
            background: #ffffff;
            font-family: 'Segoe UI';
        }
        QLineEdit, QTextEdit {
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
        }
        QLineEdit {
            font-size: 18px;
            font-weight: 500;
        }
        QTextEdit {
            min-height: 200px;
        }
        QPushButton#publishBtn {
            background-color: #0078d4;
            color: white;
            border-radius: 25px;
            padding: 14px 28px;
            font-size: 16px;
            font-weight: 500;
            min-width: 120px;
        }
        QPushButton#publishBtn:hover {
            background-color: #006cbd;
        }
        QPushButton#imageBtn {
            border: 2px dashed #cccccc;
            border-radius: 8px;
            background: #f8f9fa;
        }
        QPushButton#imageBtn:hover {
            border-color: #0078d4;
            background: #f0f6fc;
        }
    )");
}

void CloudProjectEdit::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);

    // 图片上传区域
    createImageSection(mainLayout);

    // 表单区域
    createFormSection(mainLayout);
}

void CloudProjectEdit::createImageSection(QVBoxLayout *layout)
{
    // 创建容器Widget作为图片区域
    QWidget *imageContainer = new QWidget;
    imageContainer->setFixedSize(320, 180);

    // 使用叠加布局
    QStackedLayout *stackLayout = new QStackedLayout(imageContainer);
    stackLayout->setStackingMode(QStackedLayout::StackAll);

    // 创建背景按钮
    QPushButton *imageBtn = new QPushButton("+ 添加配图");
    imageBtn->setObjectName("imageBtn");
    imageBtn->setFixedSize(320, 180);
    connect(imageBtn, &QPushButton::clicked, this, &CloudProjectEdit::onImageClicked);

    // 创建图片标签（作为按钮的子控件）
    m_imageLabel = new QLabel(imageBtn);
    m_imageLabel->setFixedSize(imageBtn->size());
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setStyleSheet("border-radius: 8px;");
    m_imageLabel->setStyleSheet("background:transparent");
    m_imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // 穿透鼠标事件

    stackLayout->addWidget(imageBtn);

    QHBoxLayout *containerLayout = new QHBoxLayout;
    containerLayout->addStretch();
    containerLayout->addWidget(imageContainer);
    containerLayout->addStretch();

    layout->addLayout(containerLayout);
}

void CloudProjectEdit::createFormSection(QVBoxLayout *layout)
{
    // 标题输入
    m_titleEdit = new QLineEdit;
    m_titleEdit->setPlaceholderText("输入文件标题...");
    layout->addWidget(m_titleEdit);

    // 描述编辑
    m_descEdit = new QTextEdit;
    m_descEdit->setPlaceholderText("详细描述你的文件内容...");
    layout->addWidget(m_descEdit);

    // 发布按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_publishBtn = new QPushButton("发布到社区");
    m_publishBtn->setObjectName("publishBtn");
    connect(m_publishBtn, &QPushButton::clicked, this, &CloudProjectEdit::onPublishClicked);

    btnLayout->addStretch();
    btnLayout->addWidget(m_publishBtn);
    layout->addLayout(btnLayout);
}

void CloudProjectEdit::setInitialData(const QString& title, const QString& description,const QString& projectId)
{
    m_titleEdit->setText(title);
    m_descEdit->setText(description);
    QPixmap pixmap;
    m_currentImage = pixmap;
    QPixmap displayPix = createRectPixmap(pixmap);
    m_imageLabel->setPixmap(displayPix);
    m_imageLabel->lower();
    if(!(projectId.isEmpty()))
    {
        HttpManager manager;
        connect(&manager,&HttpManager::downloadFinish,[this,&pixmap](const QByteArray &array){
            qDebug()<<array.length();
            pixmap.loadFromData(array);
            if(!pixmap.isNull()) {
                m_currentImage = pixmap;
                QPixmap displayPix = createRectPixmap(pixmap);
                m_imageLabel->setPixmap(displayPix);
                m_imageLabel->raise(); // 确保图片显示在顶层
            }
        });
        manager.getFile("/getpicture?type=file&id="+projectId);
    }
    this->projectId = projectId;
    m_currentImagePath = "";
}

QPixmap CloudProjectEdit::createRectPixmap(const QPixmap& source)
{
    // 定义目标尺寸（16:9比例）
    const QSize targetSize(320, 180);
    QPixmap scaled = source.scaled(targetSize,
                                   Qt::KeepAspectRatioByExpanding,
                                   Qt::SmoothTransformation);

    // 创建带圆角的矩形
    QPixmap output(targetSize);
    output.fill(Qt::transparent);

    QPainter painter(&output);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // 绘制圆角矩形路径
    QPainterPath path;
    path.addRoundedRect(0, 0, targetSize.width(), targetSize.height(), 8, 8);
    painter.setClipPath(path);

    // 居中绘制图片
    painter.drawPixmap((targetSize.width() - scaled.width())/2,
                       (targetSize.height() - scaled.height())/2,
                       scaled);

    return output;
}

void CloudProjectEdit::onImageClicked()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        "选择配图",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "图片文件 (*.png *.jpg *.jpeg)"
        );
    if (!file.isEmpty()) {
        QPixmap pixmap(file);
        if (!pixmap.isNull()) {
            m_currentImage = pixmap;
            m_currentImagePath = file;
            QPixmap displayPix = createRectPixmap(pixmap);
            m_imageLabel->setPixmap(displayPix);
            m_imageLabel->raise();
        }
    }
}

void CloudProjectEdit::onPublishClicked()
{
    QString title = m_titleEdit->text().trimmed();
    QString desc = m_descEdit->toPlainText().trimmed();

    if(title.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "标题不能为空");
        m_titleEdit->setFocus();
        return;
    }

    if(desc.length() < 10) {
        QMessageBox::warning(this, "输入错误", "描述需要至少10个字符");
        m_descEdit->setFocus();
        return;
    }

    emit filePublished(this->projectId,title, desc, m_currentImagePath);
}
