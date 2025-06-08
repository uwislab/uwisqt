#include "usermanager.h"
#include "ui_usermanager.h"

UserManager::UserManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserManager)
{
    ui->setupUi(this);
    setupUI();
    //setUserData();
    setMinimumSize(320, 480);
}

UserManager::~UserManager()
{
    delete ui;
}

void UserManager::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 头像区域
    createAvatarSection(mainLayout);

    // 信息表单
    createInfoForm(mainLayout);

    // 操作按钮
    createActionButtons(mainLayout);

    setStyleSheet(R"(
            QLineEdit {
                border: 1px solid #e0e0e0;
                border-radius: 4px;
                padding: 8px;
            }
            QLabel#title {
                font-size: 18px;
                color: #333;
                font-weight: 500;
            }
        )");
}

void UserManager::createAvatarSection(QVBoxLayout *layout) {
    QHBoxLayout *avatarLayout = new QHBoxLayout;

    // 头像按钮
    m_avatarButton = new QPushButton;
    m_avatarButton->setFixedSize(100, 100);
    m_avatarButton->setCursor(Qt::PointingHandCursor);
    m_avatarButton->setStyleSheet(R"(
            QPushButton {
                border: 2px solid #e0e0e0;
                border-radius: 50px;
                padding: 0px;
            }
            QPushButton:hover {
                border-color: #0078d4;
            }
        )");
    connect(m_avatarButton, &QPushButton::clicked,
            this, &UserManager::changeAvatar);

    // 头像操作按钮
    QVBoxLayout *avatarActionLayout = new QVBoxLayout;
    QPushButton *btnUpdate = new QPushButton("更换头像");
    btnUpdate->setStyleSheet(R"(
            QPushButton {
                color: #0078d4;
                border: none;
                text-align: left;
                padding: 4px 0;
            }
            QPushButton:hover {
                color: #005a9e;
            }
        )");
    connect(btnUpdate, &QPushButton::clicked,
            this, &UserManager::changeAvatar);
    avatarActionLayout->addStretch(2);
    avatarActionLayout->addWidget(btnUpdate);
    avatarActionLayout->addStretch(1);

    avatarLayout->addWidget(m_avatarButton);
    avatarLayout->addLayout(avatarActionLayout);
    layout->addLayout(avatarLayout);
}

void UserManager::createInfoForm(QVBoxLayout *layout) {
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // 用户ID
    QHBoxLayout *idlayout = new QHBoxLayout;
    QLabel *idLabel = new QLabel("用户ID:");
    m_userIdLabel = new QLabel;
    idlayout->addWidget(idLabel);
    idlayout->addWidget(m_userIdLabel);
    idlayout->addStretch();
    formLayout->addRow(idlayout);

    // 用户名
    QLabel *nameLabel = new QLabel("用户名:");
    userNameEdit = new QLineEdit;
    formLayout->addRow(nameLabel, userNameEdit);

    // 电子邮箱
    QLabel *emailLabel = new QLabel("电子邮箱:");
    userEmailEdit = new QLineEdit;
    userEmailEdit->setPlaceholderText("example@domain.com");
    formLayout->addRow(emailLabel, userEmailEdit);

    // 最后登录
    QLabel *loginLabel = new QLabel("最后登录:");
    lastLoginLabel = new QLabel;
    formLayout->addRow(loginLabel, lastLoginLabel);

    layout->addLayout(formLayout);
}

void UserManager::createActionButtons(QVBoxLayout *layout) {
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *logoutBtn = new QPushButton();
    QPixmap logoutImg(":/icon/logout.png");
    logoutBtn->setIcon(QIcon(logoutImg));
    logoutBtn->setStyleSheet(R"(
            QPushButton {
                background-color: white;
                color: blue;
                min-width: 30px;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #ff0000;
            }
            QPushButton:pressed {
                background-color: #d10000;
            }
        )");
    // ui->user_Pb->setFixedSize(60, 60);
    // ui->user_Pb->setIcon(QIcon(circularImg));
    // ui->user_Pb->setIconSize(QSize(60, 60));
    QPushButton *saveBtn = new QPushButton("保存修改");
    saveBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #0078d4;
                color: white;
                min-width: 80px;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #006cbd;
            }
            QPushButton:pressed {
                background-color: #005a9e;
            }
        )");
    connect(saveBtn, &QPushButton::clicked,
            this, &UserManager::onSaveClicked);
    connect(logoutBtn, &QPushButton::clicked,
            [this](){
                emit logout();
            });

    btnLayout->addWidget(logoutBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    layout->addLayout(btnLayout);
}

void UserManager::changeAvatar() {
    m_currentAvatarPath = "";
    QString path = QFileDialog::getOpenFileName(
        this,
        "选择头像文件",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "图片文件 (*.png *.jpg *.jpeg)"
        );
    qDebug()<<path;
    if (!path.isEmpty()) {
        QPixmap pixmap(path);
        if (!pixmap.isNull()) {
            updateAvatar(pixmap);
            //emit avatarLoad(pixmap);
        }
        m_currentAvatarPath = path;
    }
}

void UserManager::updateAvatar(const QPixmap &pixmap) {
    m_currentAvatar = createCircularPixmap(pixmap);
    m_avatarButton->setIcon(QIcon(m_currentAvatar));
    m_avatarButton->setIconSize(QSize(100, 100));
}

QPixmap UserManager::createCircularPixmap(const QPixmap &source) {
    QPixmap output(100, 100);
    output.fill(Qt::transparent);

    QPainter painter(&output);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, 100, 100);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 100, 100, source);

    return output;
}

bool UserManager::validateInput() {
    // 用户名非空验证
    if (userNameEdit->text().trimmed().isEmpty()) {
        showValidationError("用户名不能为空", userNameEdit);
        return false;
    }

    // 邮箱格式验证
    static QRegularExpression emailRegex(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"
        );
    if (!emailRegex.match(userEmailEdit->text()).hasMatch()) {
        showValidationError("邮箱格式不正确", userEmailEdit);
        return false;
    }

    return true;
}

void UserManager::showValidationError(const QString &msg, QWidget *focusWidget) {
    QMessageBox::warning(this, "输入错误", msg);
    if (focusWidget) focusWidget->setFocus();
}

void UserManager::closeEvent(QCloseEvent *event)
{
    setUserData(m_userId,m_userName,m_userEmail,m_userLastLogin.toString());
}
