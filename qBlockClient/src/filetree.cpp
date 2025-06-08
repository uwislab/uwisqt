#include "filetree.h"
#include "ui_filetree.h"
#include "inisettings.h"

FileTree::FileTree(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileTree)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");
    ui->fileTree_Pb->setStyleSheet("QPushButton {height:30;width:30;background-color:white;border-radius:3px;}");
    ui->refresh_Pb->setStyleSheet("QPushButton {height:30;width:30;background-color:white;border-radius:3px;}");

    ui->treeWidget->clear();
    ui->treeWidget->setHeaderHidden(true);
    connect(ui->fileTree_Pb,&QPushButton::clicked,[this](){
        emit changetoShowCodeWidget();
    });
    connect(ui->refresh_Pb,&QPushButton::clicked,[this](){
        ui->treeWidget->clear();
        showTree(dirPath,nullptr);
    });
}

FileTree::~FileTree()
{
    delete ui;
}


void FileTree::showTree(QString sPath, QTreeWidgetItem* pParentItem)
{
    ui->treeWidget->clear();
    dirPath = sPath;
    if(sPath=="")
    {
        //sPath=IniSettings::getInstance().getValue("Path","workspacePath","/");
        return;
    }
    QDir dir(sPath); // sPath待遍历路径
    if(!dir.exists())
    {
        return;
    }

    QTreeWidgetItem* pRoot;

    int pos1 = sPath.lastIndexOf('\\');
    int pos2 = sPath.lastIndexOf('/');
    int pos = pos1>pos2? pos1:pos2;
    assert(pos>=0);
    QString sDirName =sPath.mid(pos+1);

    if(pParentItem == nullptr)
    {
        QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->treeWidget);
        pItem->setText(0,sDirName);
        pItem->setIcon(0,QIcon(":/icon/dir.png"));
        ui->treeWidget->addTopLevelItem(pItem);
        pRoot = pItem;
    }
    else{

        QTreeWidgetItem* pItem = new QTreeWidgetItem;
        pItem->setText(0,sDirName);
        pItem->setIcon(0,QIcon(":/icon/dir.png"));
        pParentItem->addChild(pItem);
        pRoot = pItem;
    }


    // 定义迭代器并设置过滤器
    QDirIterator dir_iterator(sPath,QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    QStringList string_list;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString sName = file_info.fileName();
        if(file_info.isDir()){
            showTree(file_info.absoluteFilePath(),pRoot);
        }
        else
        {
            if(QString::compare(file_info.suffix(),"txt")==0){
                QTreeWidgetItem* pItem = new QTreeWidgetItem;
                pItem->setText(0,sName);
                pItem->setIcon(0,QIcon(":/icon/txt.png"));
                pRoot->addChild(pItem);
            }
            else if(QString::compare(file_info.suffix(),"py")==0)
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem;
                pItem->setText(0,sName);
                pItem->setIcon(0,QIcon(":/icon/Python.png"));
                pRoot->addChild(pItem);
            }
            else if(QString::compare(file_info.suffix(),"json")==0)
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem;
                pItem->setText(0,sName);
                pItem->setIcon(0,QIcon(":/icon/JSON.png"));
                pRoot->addChild(pItem);
            }
            else if(QString::compare(file_info.suffix(),"mp3")==0)
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem;
                pItem->setText(0,sName);
                pItem->setIcon(0,QIcon(":/icon/audio.png"));
                pRoot->addChild(pItem);
            }
            else if(QString::compare(file_info.suffix(),"jpg")==0||QString::compare(file_info.suffix(),"png")==0)
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem;
                pItem->setText(0,sName);
                pItem->setIcon(0,QIcon(":/icon/picture.png"));
                pRoot->addChild(pItem);
            }
            else
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem;
                pItem->setText(0,sName);
                pItem->setIcon(0,QIcon(":/icon/file.png"));
                pRoot->addChild(pItem);
            }
        }
    }
}
