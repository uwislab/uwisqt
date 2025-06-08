#include "showcodes.h"
#include "ui_showcodes.h"
#include "inisettings.h"

showCodes::showCodes(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::showCodes)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");

    ui->run_Pb->setStyleSheet("QPushButton {height:30;width:50;background-color:white;border-radius:3px;}");
    ui->save_Pb->setStyleSheet("QPushButton {height:30;width:65;background-color:white;border-radius:3px;}");
    ui->fileTree_Pb->setStyleSheet("QPushButton {height:30;width:30;background-color:white;border-radius:3px;}");
    ui->projectName_Lb->installEventFilter(this);
    ui->projectName_Le->installEventFilter(this);
    ui->fileTree_Pb->setEnabled(false);


    connect(this->ui->run_Pb,&QPushButton::clicked,this,&showCodes::runcode);
    connect(this->ui->save_Pb,&QPushButton::clicked,this,&showCodes::save);
    connect(this->ui->fileTree_Pb,&QPushButton::clicked,[this](){
        emit changetoFileTreeWidget();
    });
}

showCodes::~showCodes()
{
    delete ui;
}

void showCodes::show(QString str)
{
    ui->textEdit->setPlainText(str);
}


void showCodes::save()
{
    QString path = QFileDialog::getExistingDirectory(this,"选择文件夹",IniSettings::getInstance().getValue("Path","workspacePath","/"));
    qDebug()<<path;
    if( path == "" )
    {
        return;
    }

    QString filePath = path+"/"+this->getProjectName()+".py";
    QFile file(filePath);

    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file);

        out << ui->textEdit->toPlainText();
        file.close();
    }
    else
    {
        qDebug()<<("创建失败");
    }
    qDebug()<<filePath;

}


void showCodes::runcode()
{
    QString path = "";
    if(instanceDir == "")
    {
        path = IniSettings::getInstance().getValue("Path","workspacePath","/");
        emit makeNewProject(this->getProjectName());
    }
    path = instanceDir;
    qDebug()<<"2";
    QString projectName = this->getProjectName();
    if( projectName == "" )
    {
        return;
    }
    QString filePath = path+"/"+projectName+".py";
    QFile file(filePath);

    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file);

        out << ui->textEdit->toPlainText();
        file.close();
    }
    else
    {
        qDebug()<<("创建失败");
    }
    qDebug()<<filePath;
    emit readtoRun(filePath);
}

void showCodes::rename(QString str)
{
    ui->projectName_Lb->setText(str);
}

QString showCodes::getProjectName()
{
    return ui->projectName_Lb->text();
}


void showCodes::updateInstanceDir(QString dirName)
{
    this->instanceDir=dirName;
    if(!instanceDir.isEmpty())
    {
        ui->fileTree_Pb->setEnabled(true);
    }
}

bool showCodes::eventFilter(QObject* obj, QEvent* evt)
{
    if (obj == ui->projectName_Lb)
    {
        if (evt->type() == QEvent::MouseButtonDblClick)
        {
            ui->projectName_Le->setText(ui->projectName_Lb->text());
            ui->stackedWidget->setCurrentIndex(1);
            //ui->projectName_Le->setFocus();
        }
    }
    else if (obj == ui->projectName_Le)
    {
        if (evt->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyevt = static_cast<QKeyEvent*>(evt);
            if ((keyevt->key() == Qt::Key_Return) ||
                (keyevt->key() == Qt::Key_Escape) ||
                (keyevt->key() == Qt::Key_Enter))
            {
                QString projectName = ui->projectName_Lb->text();
                if(!(ui->projectName_Le->text().isEmpty()))
                {
                    qDebug()<<ui->projectName_Le->text().isEmpty();
                    ui->projectName_Lb->setText(ui->projectName_Le->text());
                }
                else
                {
                    ui->projectName_Lb->setText(projectName);
                }
                ui->stackedWidget->setCurrentIndex(0);
            }
        }
        else if (evt->type() == QEvent::FocusOut)
        {
            QString projectName = ui->projectName_Lb->text();
            if(!(ui->projectName_Le->text().isEmpty()))
            {
                qDebug()<<ui->projectName_Le->text().isEmpty();
                ui->projectName_Lb->setText(ui->projectName_Le->text());
            }
            else
            {
                ui->projectName_Lb->setText(projectName);
            }
            ui->stackedWidget->setCurrentIndex(0);
        }
    }

    return QWidget::eventFilter(obj, evt);
}

