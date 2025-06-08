#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QSharedPointer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QGridLayout *mainLayout = new QGridLayout(centralWidget);

    QWidget *block_widget = new QWidget(this);
    QHBoxLayout *box = new QHBoxLayout();
    block_widget->setLayout(box);
    webView = new QWebEngineView(parent);
    // 获取当前可执行文件所在目录
    QString appDir = QCoreApplication::applicationDirPath();
    // 拼接web文件夹的绝对路径
    QString webPath = QDir(appDir).filePath("web/qBlockly/qBlockly.html");

    //webView->load(QUrl::fromLocalFile("D:/test/qBlockly/qBlockly.html"));
    webView->load(webPath);
    webChannel = new QWebChannel;
    webobj = new WebHandle(webView);
    webChannel->registerObject("webobj", webobj);
    webView->page()->setWebChannel(webChannel);
    box->addWidget(webView);
    block_widget->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");

    output_wd = new output(this);
    showcodes_wd = new showCodes(this);
    filetree_wd = new FileTree(this);

    codeStackedWidget = new QStackedWidget(this);
    codeStackedWidget->addWidget(showcodes_wd);
    codeStackedWidget->addWidget(filetree_wd);


    editorWidget = new QWidget(this);
    QGridLayout *editorLayout = new QGridLayout(editorWidget);
    // 添加控件时指定行列位置和拉伸比例
    editorLayout->addWidget(block_widget, 0, 0, 2, 2); // 第0行第0列
    editorLayout->addWidget(codeStackedWidget, 0, 2); // 第0行第1列
    editorLayout->addWidget(output_wd, 1, 2); // 跨2列

    // 列比例设置（左区域:右区域 = 2:1）
    editorLayout->setColumnStretch(0, 1);  // 第0列权重
    editorLayout->setColumnStretch(1, 1);  // 第1列权重（与前两列共同组成左侧区域）
    editorLayout->setColumnStretch(2, 1);  // 第2列权重（右侧单独区域）

    // 行比例设置（上下等分）
    editorLayout->setRowStretch(0, 1);     // 第0行权重
    editorLayout->setRowStretch(1, 1);     // 第1行权重


    communityWidget = new CommunityWidget(this);
    cloudFileWidget = new CloudProjectsWidget(this);
    //cloudFileWidget = new QWidget(this);
    sideBarWidget = new SideBar(this);

    cloudedit = new CloudProjectEdit();
    cloudview = new CloudProjectView();
    QScrollArea *cloudviewScrollArea = new QScrollArea;
    cloudviewScrollArea->setWidgetResizable(true); // 关键设置
    cloudviewScrollArea->setFrameShape(QFrame::NoFrame);
    cloudviewScrollArea->setWidget(cloudview);
    cloudviewScrollArea->setAlignment(Qt::AlignHCenter); // 水平居中
    QScrollArea *cloudeditScrollArea = new QScrollArea;
    cloudeditScrollArea->setWidgetResizable(true);
    cloudeditScrollArea->setFrameShape(QFrame::NoFrame);
    cloudeditScrollArea->setWidget(cloudedit);
    cloudeditScrollArea->setAlignment(Qt::AlignHCenter); // 水平居中

    mainStackedWidget = new QStackedWidget(this);
    mainStackedWidget->addWidget(editorWidget);
    mainStackedWidget->addWidget(cloudFileWidget);
    mainStackedWidget->addWidget(communityWidget);
    mainStackedWidget->addWidget(cloudviewScrollArea);
    mainStackedWidget->addWidget(cloudeditScrollArea);

    // 添加控件时指定行列位置和拉伸比例
    mainLayout->addWidget(sideBarWidget, 0, 0); // 第0行第0列
    mainLayout->addWidget(mainStackedWidget, 0, 1); // 第0行第1列
    mainLayout->setColumnStretch(0, 1);  // 第0列权重
    mainLayout->setColumnStretch(1, 14);  // 第1列权重

    login_wd = new Login();
    userinfo_wd = new UserManager();



    IniSettings::getInstance().settings_init();
    changeColor(IniSettings::getInstance().getValue("User","color","1").toInt());

    connect(webobj,&WebHandle::contentChanged,showcodes_wd,&showCodes::show);
    connect(showcodes_wd,&showCodes::readtoRun,output_wd,&output::startPythonProcess);
    connect(showcodes_wd,&showCodes::makeNewProject,[this](QString name){
        makeNewProject(name);
        showcodes_wd->updateInstanceDir(this->nowProjectDir);
        webobj->updateInstanceDir(this->nowProjectDir);
        filetree_wd->showTree(this->nowProjectDir,nullptr);
    });

    connect(showcodes_wd,&showCodes::changetoFileTreeWidget,[this](){
        codeStackedWidget->setCurrentIndex(1);
    });
    connect(filetree_wd,&FileTree::changetoShowCodeWidget,[this](){
        codeStackedWidget->setCurrentIndex(0);
    });

    connect(webobj,&WebHandle::loadError,[this](){
        QMessageBox::information(this,"错误","导入失败，请检查格式是否正确");
    });

    connect(&IniSettings::getInstance(),&IniSettings::colorChangeSignal,this,&MainWindow::changeColor);

    connect(ui->save_a,&QAction::triggered,[this](){
        webobj->updateProjectName(showcodes_wd->getProjectName());
        qDebug()<<this->nowProjectDir;
        if( this->nowProjectDir == "" )
        {
            this->nowProjectDir = IniSettings::getInstance().getValue("Path","workspacePath","/") + "/" + showcodes_wd->getProjectName();
            QDir nowDir(this->nowProjectDir);
            if(!nowDir.exists())
            {
                nowDir.mkdir(this->nowProjectDir);
                qDebug()<<"创建成功";
            }
        }
        showcodes_wd->updateInstanceDir(this->nowProjectDir);
        webobj->updateInstanceDir(this->nowProjectDir);
        filetree_wd->showTree(this->nowProjectDir,nullptr);
        emit webobj->saveStateSignal();
    });
    connect(ui->open_a,&QAction::triggered,[this](){
        QString fileName = QFileDialog::getOpenFileName(this,"选择文件",IniSettings::getInstance().getValue("Path","workspacePath","/"));
        if(fileName.isEmpty())
        {
            return;
        }
        connect(webobj,&WebHandle::loadSuccess,[this,&fileName](){
            qDebug()<<fileName;
            QFileInfo info(fileName);
            this->nowProjectDir = info.absoluteDir().absolutePath();
            qDebug()<<this->nowProjectDir;
            showcodes_wd->updateInstanceDir(this->nowProjectDir);
            showcodes_wd->rename(info.baseName());
            webobj->updateInstanceDir(this->nowProjectDir);
            if( fileName == "" )
            {
                return;
            }
            //qDebug()<<this->nowProjectDir<<2;
            filetree_wd->showTree(this->nowProjectDir,nullptr);
        });
        connect(webobj,&WebHandle::loadError,[this,&fileName](){
            qDebug()<<fileName;
            QFileInfo info(fileName);
            makeNewProject("NewProject");
            //this->nowProjectDir = "";
            qDebug()<<this->nowProjectDir;
            showcodes_wd->updateInstanceDir(this->nowProjectDir);
            //showcodes_wd->rename(info.baseName());
            webobj->updateInstanceDir(this->nowProjectDir);
            if( fileName == "" )
            {
                return;
            }
            //qDebug()<<this->nowProjectDir<<2;
            filetree_wd->showTree(this->nowProjectDir,nullptr);
        });
        emit webobj->loadStateSignal(webobj->loadState(fileName));
    });
    connect(ui->saveas_a,&QAction::triggered,[this](){
        webobj->updateProjectName(showcodes_wd->getProjectName());
        this->nowProjectDir = QFileDialog::getExistingDirectory(this,"选择文件夹",IniSettings::getInstance().getValue("Path","workspacePath","/"));
        qDebug()<<this->nowProjectDir;
        if( this->nowProjectDir == "" )
        {
            this->nowProjectDir = IniSettings::getInstance().getValue("Path","workspacePath","/") + showcodes_wd->getProjectName();
            QDir nowDir(this->nowProjectDir);
            if(!nowDir.exists())
            {
                nowDir.mkdir(this->nowProjectDir);
                qDebug()<<"创建成功";
            }
            return;
        }
        showcodes_wd->updateInstanceDir(this->nowProjectDir);
        webobj->updateInstanceDir(this->nowProjectDir);
        filetree_wd->showTree(this->nowProjectDir,nullptr);
        emit webobj->saveStateSignal();
    });
    connect(ui->new_a,&QAction::triggered,[this](){
        makeNewProject("NewProject");
        QJsonObject emptyjson;
        //this->nowProjectDir = "";
        showcodes_wd->updateInstanceDir(this->nowProjectDir);
        webobj->updateInstanceDir(this->nowProjectDir);
        filetree_wd->showTree(this->nowProjectDir,nullptr);
        emit webobj->loadStateSignal(emptyjson);
    });
    connect(ui->write_a,&QAction::triggered,[this](){

        Qt::WindowFlags flags = IniSettings::getInstance().windowFlags();
        IniSettings::getInstance().setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

        IniSettings::getInstance().setWindowModality(Qt::ApplicationModal);

        IniSettings::getInstance().show();
    });
    zip = new ZipCompress(this);
    manager = new HttpManager(this);
    connect(ui->cloud_a,&QAction::triggered,[this](){
        if(!isLogin)
        {
            QMessageBox::critical(this,"错误", "您尚未登录");
            return;
        }
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,                           // 父窗口
            tr("操作确认"),                 // 标题
            tr("确定将项目上传至云端吗？"),       // 提示内容
            QMessageBox::Yes | QMessageBox::No, // 按钮组合
            QMessageBox::No                 // 默认选中按钮
            );

        // 处理用户选择
        if (reply == QMessageBox::Yes) {
            if(this->nowProjectDir.isEmpty())
            {
                QMessageBox::critical(this,"错误", "错误！文件不存在");
            }
            else
            {
                QString PID = "";
                QFile file(this->nowProjectDir+"/project.ini");
                if(file.open(QIODevice::ReadOnly))
                {
                    QByteArray allArray = file.readAll();
                    QString allStr = QString(allArray);
                    QJsonDocument doc = QJsonDocument::fromJson(allStr.toLocal8Bit().data());
                    QJsonObject jsonObject = doc.object();
                    PID = jsonObject["projectId"].toString();
                    file.close();
                }
                else
                {
                    qDebug()<<("ini文件不存在");
                }
                qDebug()<<PID;
                zip->zipCompressDirector(this->nowProjectDir,IniSettings::getInstance().getValue("Path","workspacePath","/")+"/"+showcodes_wd->getProjectName()+".zip");
                connect(manager,&HttpManager::postCloudProjectSuccess,[this]()
                {
                    QMessageBox::information(this,"上传结果", "上传成功");
                    QFile::remove(IniSettings::getInstance().getValue("Path","workspacePath","/")+"/"+showcodes_wd->getProjectName()+".zip");
                });
                connect(manager,&HttpManager::postCloudProjectFail,[this]()
                        {
                    QMessageBox::information(this,"上传结果", "上传失败");
                });
                manager->postCloudProject(userinfo_wd->getuserId(),showcodes_wd->getProjectName(),IniSettings::getInstance().getValue("Path","workspacePath","/")+"/"+showcodes_wd->getProjectName()+".zip",PID);
            }
        }
    });
    connect(sideBarWidget,&SideBar::openUserWidget,[this](){
        if(isLogin)
        {
            userinfo_wd->show();
        }
        else
        {
            //login_wd -> show();
            Qt::WindowFlags flags = login_wd->windowFlags();
            login_wd->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

            login_wd->setWindowModality(Qt::ApplicationModal);

            login_wd->show();
        }
    });
    connect(sideBarWidget,&SideBar::openEditorWidget,[this](){
        mainStackedWidget->setCurrentIndex(0);
    });
    connect(sideBarWidget,&SideBar::openCloudFileWidget,[this](){
        mainStackedWidget->setCurrentIndex(1);
    });
    connect(sideBarWidget,&SideBar::openCommunityWidget,[this](){
        mainStackedWidget->setCurrentIndex(2);
    });

    //manager = new HttpManager(this);
    connect(login_wd,&Login::login,manager,&HttpManager::login);
    connect(manager,&HttpManager::loginSuccess,[this](){
        login_wd->close();
        isLogin = true;
        qDebug()<<"permission"<<permission;
        cloudview->btnsetVisible(permission);
        QMessageBox::information(NULL,"登录提示","登录成功",QMessageBox::Yes);
        cloudFileWidget->populateProjects(manager->getCloudProjectList());
        communityWidget->populateProjects(manager->getCommunityProjectList());
        QJsonObject userinfo = manager->getuserInfo();
        userinfo_wd->setUserData(QString::number(userinfo["user_id"].toInt()),userinfo["username"].toString(),userinfo["email"].toString(),userinfo["last_login"].toString());
    });
    connect(userinfo_wd,&UserManager::avatarLoad,sideBarWidget,&SideBar::updateAvatar);

    connect(login_wd,&Login::Register,manager,&HttpManager::userRegister);
    connect(manager,&HttpManager::registerSuccess,[this](){
        QMessageBox::information(NULL,"注册提示","注册成功",QMessageBox::Yes);
    });
    connect(manager,&HttpManager::registerFail,[this](){
        QMessageBox::warning(NULL,"注册提示","注册失败",QMessageBox::Yes);
    });
    connect(userinfo_wd,&UserManager::logout,manager,&HttpManager::logout);
    connect(manager,&HttpManager::logoutSuccess,[this](){
        userinfo_wd->close();
        sideBarWidget->updateAvatar(QPixmap(":/icon/Avatar.png"));
        isLogin = false;
        mainStackedWidget->setCurrentIndex(0);
        cloudFileWidget->populateProjects(QJsonArray());
        communityWidget->populateProjects(QJsonArray());
    });
    connect(manager,&HttpManager::logoutFail,[this](){
        QMessageBox::warning(NULL,"登出提示","登出失败",QMessageBox::Yes);
    });
    connect(manager,&HttpManager::sessionIsAvailable,[this](){
        isLogin = true;
        //QMessageBox::information(NULL,"登录提示","登录成功",QMessageBox::Yes);
        cloudFileWidget->populateProjects(manager->getCloudProjectList());
        communityWidget->populateProjects(manager->getCommunityProjectList());
        QJsonObject userinfo = manager->getuserInfo();
        userinfo_wd->setUserData(QString::number(userinfo["user_id"].toInt()),userinfo["username"].toString(),userinfo["email"].toString(),userinfo["last_login"].toString());
    });
    connect(manager,&HttpManager::sessionNotAvailable,[this](){
        isLogin = false;
        //QMessageBox::information(NULL,"登录提示","登录成功",QMessageBox::Yes);
        IniSettings::getInstance().setValue("User","sessionId","");
    });

    connect(userinfo_wd,&UserManager::profileUpdated,[this](QString ID, QString name, QString email, QString img){
        qDebug()<<ID;
        manager->userInfoChange(name, email);
        manager->postImg("user",ID, img);
    });

    connect(communityWidget,&CommunityWidget::projectClicked,[this](QString projectId){
        mainStackedWidget->setCurrentIndex(3);
        QJsonObject info = manager->getCommunityProjectInfo(projectId);
        cloudview->setProjectData(info["title"].toString(),info["description"].toString(),projectId, info["projectName"].toString());
        QJsonArray arr = manager->getCommentList(projectId);
        cloudview->populateComment(arr);
    });
    connect(communityWidget,&CommunityWidget::refreshCommunityWidget,[this](){
        communityWidget->populateProjects(manager->getCommunityProjectList());
    });

    connect(cloudFileWidget,&CloudProjectsWidget::projectDownloadClicked,[this](const QString& projectId, const QString& projectName){
        qDebug()<<projectId;
        qDebug()<<projectName;
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
            zip->zipUnCompressFile(filePath,DirPath);
            QFile::remove(filePath);
            QFile textfile(DirPath+"/project.ini");
            if(textfile.open(QIODevice::WriteOnly|QIODevice::Text))
            {
                QTextStream out(&textfile);
                QJsonObject jsonobj;
                jsonobj.insert("projectId",projectId);
                QJsonDocument doc(jsonobj);
                out << QString(doc.toJson());
                textfile.close();
            }
        });
        filemanager.getFile("/getfile?fileId="+projectId);
    });
    connect(cloudFileWidget,&CloudProjectsWidget::projectPublicClicked,[this](const QString& projectId, const QString& projectName){
        mainStackedWidget->setCurrentIndex(4);
        cloudedit->setInitialData("","",projectId);
        qDebug()<<"发布3";
        qDebug()<<projectId<<projectName;
    });
    connect(cloudedit,&CloudProjectEdit::filePublished,[this](const QString& projectId, const QString& title, const QString& description, const QString& image){
        HttpManager projectmanager;
        bool info = false;
        bool img = false;
        connect(&projectmanager,&HttpManager::publicCloudProjectSuccess,[&info, &img](){
            info = true;
            if(info && img)
            {
                QMessageBox::information(NULL,"提示","发布成功",QMessageBox::Yes);
            }
        });
        connect(&projectmanager,&HttpManager::postImgSuccess,[&info, &img](){
            img = true;
            if(info && img)
            {
                QMessageBox::information(NULL,"提示","发布成功",QMessageBox::Yes);
            }
        });
        qDebug()<<"publish"<<projectId;
        projectmanager.postCommunityProject(projectId,title,description);
        projectmanager.postImg("file",projectId,image);
    });
    connect(cloudFileWidget,&CloudProjectsWidget::projectPublicChangeClicked,[this](const QString& projectId, const QString& projectName){
        mainStackedWidget->setCurrentIndex(4);
        QJsonObject obj = manager->getCommunityProjectInfo(projectId);
        qDebug()<<"修改3";
        qDebug()<<projectId<<projectName;
        cloudedit->setInitialData(obj["title"].toString(),obj["description"].toString(),projectId);
    });

    connect(cloudFileWidget,&CloudProjectsWidget::refreshCloudProjectWidget,[this](){
        cloudFileWidget->populateProjects(manager->getCloudProjectList());
    });

    connect(cloudview,&CloudProjectView::deleteCommunityProject,[this](QString id){
        manager->deleteCommunityProject(id);
    });
    connect(manager,&HttpManager::deleteCommunityProjectSuccess,[this](){
        QMessageBox::information(nullptr, "提示", "删除成功");
        mainStackedWidget->setCurrentIndex(2);
        communityWidget->populateProjects(manager->getCommunityProjectList());
        cloudFileWidget->populateProjects(manager->getCloudProjectList());
    });
    connect(manager,&HttpManager::deleteCommunityProjectFail,[this](){
        QMessageBox::critical(nullptr, "错误", "删除失败");
    });
    connect(cloudFileWidget,&CloudProjectsWidget::projectDeleteClicked,[this](QString PID,QString projectName){
        manager->deleteCloudProject(PID);
    });
    connect(manager,&HttpManager::deleteCloudProjectSuccess,[this](){
        QMessageBox::information(nullptr, "提示", "删除成功");
        cloudFileWidget->populateProjects(manager->getCloudProjectList());
    });
    connect(manager,&HttpManager::deleteCloudProjectFail,[this](){
        QMessageBox::critical(nullptr, "错误", "删除失败");
    });

    if(!IniSettings::getInstance().getValue("User","sessionId","").toLocal8Bit().isEmpty())
    {
        manager->sessionAvailable();
        qDebug()<<"验证";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeNewProject(QString name)
{
    //emit webobj->loadStateSingal();
    int i = 1;
    QString newDirName = IniSettings::getInstance().getValue("Path","workspacePath","/")+"/"+showcodes_wd->getProjectName();
    QString newDirNamecopy = newDirName;
    QDir nowDir(newDirName);
    if(!nowDir.exists())
    {
        nowDir.mkdir(newDirName);
        qDebug()<<"创建成功";
    }
    else
    {
        newDirName = newDirName+"("+QString::number(i)+")";
        nowDir.setPath(newDirName);
        while(nowDir.exists())
        {
            ++i;
            newDirName = newDirNamecopy+"("+QString::number(i)+")";
            nowDir.setPath(newDirName);
        }
        nowDir.mkdir(newDirName);
    }
    showcodes_wd->rename(nowDir.dirName());
    this->nowProjectDir = newDirName;
}

QString MainWindow::getNowProjectDir()
{
    return this->nowProjectDir;
}

void MainWindow::changeColor(int colorNum)
{
    switch(colorNum)
    {
    case 1:
        ui->menubar->setStyleSheet("QMenuBar{background-color:#80ccff;}");
        break;
    case 2:
        ui->menubar->setStyleSheet("QMenuBar{background-color:#4caf50;}");
        break;
    case 3:
        ui->menubar->setStyleSheet("QMenuBar{background-color:#cf5834;}");
        break;
    case 4:
        ui->menubar->setStyleSheet("QMenuBar{background-color:#c4c106;}");
        break;
    case 5:
        ui->menubar->setStyleSheet("QMenuBar{background-color:#ffffff;}");
        break;
    default:
        ui->menubar->setStyleSheet("QMenuBar{background-color:#80ccff;}");
        break;
    }
}

int MainWindow::permission = 0;
