#include "inisettings.h"
#include "ui_inisettings.h"

IniSettings::IniSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IniSettings)
{
    ui->setupUi(this);
    m_pButtonGroup = new QButtonGroup(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setStyleSheet("QWidget{background-color:gray;border-top-left-radius:15px;border-top-right-radius:5px;}");

    ui->apply_PB->setStyleSheet("QPushButton {height:30;width:60;background-color:white;border-radius:5px;}");
    ui->confirm_PB->setStyleSheet("QPushButton {height:30;width:60;background-color:white;border-radius:5px;}");
    ui->cancel_PB->setStyleSheet("QPushButton {height:30;width:60;background-color:white;border-radius:5px;}");
    ui->pythonPath_PB->setStyleSheet("QPushButton {height:20;width:75;background-color:white;border-radius:3px;}");
    ui->workSpace_PB->setStyleSheet("QPushButton {height:20;width:75;background-color:white;border-radius:3px;}");
    ui->update_PB->setStyleSheet("QPushButton {height:20;width:75;background-color:white;border-radius:3px;}");

    connect(this->ui->pythonPath_PB,&QPushButton::clicked,this,&IniSettings::changePythonPath);
    connect(this->ui->workSpace_PB,&QPushButton::clicked,this,&IniSettings::changeWorkSpacePath);
    connect(this->m_pButtonGroup,&QButtonGroup::buttonClicked,this,&IniSettings::changeColor);

    connect(this->ui->apply_PB,&QPushButton::clicked,this,&IniSettings::applyChange);
    connect(this->ui->confirm_PB,&QPushButton::clicked,[this](){
        IniSettings::getInstance().applyChange();
        this->close();
    });
    connect(this->ui->cancel_PB,&QPushButton::clicked,this,&IniSettings::cancelChange);
    connect(this->ui->update_PB,&QPushButton::clicked,[this](){
        emit updateCheck();
    });
}

IniSettings::~IniSettings()
{
    delete ui;
}
IniSettings& IniSettings::getInstance()
{
    static IniSettings setting;
    return setting;
}
void IniSettings::settings_init()
{
    defaultStrIniFileName = QCoreApplication::applicationDirPath() + "/" + "config.ini";
    m_iniFile = QSharedPointer<QSettings>(new QSettings(defaultStrIniFileName, QSettings::IniFormat));

    ui->pythonPath_LE->setText(IniSettings::getInstance().getValue("Path","pythonPath","/"));
    ui->workSpace_LE->setText(IniSettings::getInstance().getValue("Path","workspacePath","/"));
    ui->version_LB->setText("当前版本：V"+IniSettings::getInstance().getValue("Version","v","1.0.0"));

    m_pButtonGroup->addButton(ui->color1_RB);
    m_pButtonGroup->addButton(ui->color2_RB);
    m_pButtonGroup->addButton(ui->color3_RB);
    m_pButtonGroup->addButton(ui->color4_RB);
    m_pButtonGroup->addButton(ui->color5_RB);

    switch(IniSettings::getInstance().getValue("User","color","1").toInt())
    {
    case 1:
        ui->color1_RB->setChecked(true);
        break;
    case 2:
        ui->color2_RB->setChecked(true);
        break;
    case 3:
        ui->color3_RB->setChecked(true);
        break;
    case 4:
        ui->color4_RB->setChecked(true);
        break;
    case 5:
        ui->color5_RB->setChecked(true);
        break;
    default:
        ui->color1_RB->setChecked(true);
        break;
    }
}

void IniSettings::setValue(const QString &section, const QString &key, const QString &value)
{
    m_iniFile->beginGroup(section);     // 设置节点名
    m_iniFile->setValue(key, value);    //设置键名和键值
    m_iniFile->endGroup();              // 结束当前节的操作
}

void IniSettings::removeSection(const QString &section)
{
    m_iniFile->remove(section);
}

void IniSettings::removeKey(const QString &section, const QString &key)
{
    m_iniFile->beginGroup(section);
    m_iniFile->remove(key);
    m_iniFile->endGroup();
}

QString IniSettings::getValue(const QString &section, const QString &key, const QString &defaultValue)
{
    QString path = QString("%1/%2").arg(section).arg(key);
    QString result = m_iniFile->value(path, defaultValue).toString();
    return result;
}

QStringList IniSettings::getValueList(const QString & section, const QString & key, QString defaultValue)
{
    QVariant variant = m_iniFile->value(QString("%1/").arg(section) + key);
    QStringList list = variant.value<QStringList>();
    return list;
}

void IniSettings::changePythonPath()
{
    QString pythonPath = QFileDialog::getOpenFileName(this,"选择文件",QFileInfo(IniSettings::getInstance().getValue("Path","pythonPath","/")).absoluteDir().absolutePath());

    if(pythonPath!="")
    {
        this->ui->pythonPath_LE->setText(pythonPath);
    }
}

void IniSettings::changeWorkSpacePath()
{
    QString workSpaceDir = QFileDialog::getExistingDirectory(this,"选择文件夹",IniSettings::getInstance().getValue("Path","workspacePath","/"));

    if(workSpaceDir!="")
    {
        this->ui->workSpace_LE->setText(workSpaceDir);
    }
}

void IniSettings::changeColor(QAbstractButton *button)
{
    QList<QAbstractButton*> list = this->m_pButtonGroup->buttons();
    for(int i =0; i<list.size(); i++)
    {
        QAbstractButton *pButton = list[i];
        QString strStatus = pButton->isChecked() ? "Checked" : "Unchecked";
        qDebug() << QString("Button : %1 is %2").arg(pButton->text()).arg(strStatus);

        if(strStatus == "Checked")
        {
            emit colorChangeSignal(i + 1);
        }
    }
}

void IniSettings::applyChange()
{
    IniSettings::getInstance().setValue("Path","pythonPath",ui->pythonPath_LE->text());
    IniSettings::getInstance().setValue("Path","workspacePath",ui->workSpace_LE->text());

    QList<QAbstractButton*> list = this->m_pButtonGroup->buttons();
    for(int i =0; i<list.size(); i++)
    {
        QAbstractButton *pButton = list[i];
        QString strStatus = pButton->isChecked() ? "Checked" : "Unchecked";
        if(strStatus == "Checked")
        {
            IniSettings::getInstance().setValue("User","color",QString::number(i + 1));
        }
    }
}

void IniSettings::cancelChange()
{
    // ui->pythonPath_LE->setText(IniSettings::getInstance().getValue("Path","pythonPath","/"));
    // ui->workSpace_LE->setText(IniSettings::getInstance().getValue("Path","workspacePath","/"));

    // int color = IniSettings::getInstance().getValue("User","color","1").toInt();
    // switch(color)
    // {
    // case 1:
    //     ui->color1_RB->setChecked(true);
    //     break;
    // case 2:
    //     ui->color2_RB->setChecked(true);
    //     break;
    // case 3:
    //     ui->color3_RB->setChecked(true);
    //     break;
    // case 4:
    //     ui->color4_RB->setChecked(true);
    //     break;
    // case 5:
    //     ui->color5_RB->setChecked(true);
    //     break;
    // default:
    //     ui->color1_RB->setChecked(true);
    //     break;
    // }

    // emit colorChangeSignal(color);

    this->close();
}

void IniSettings::closeEvent(QCloseEvent *event)
{
    ui->pythonPath_LE->setText(IniSettings::getInstance().getValue("Path","pythonPath","/"));
    ui->workSpace_LE->setText(IniSettings::getInstance().getValue("Path","workspacePath","/"));

    int color = IniSettings::getInstance().getValue("User","color","1").toInt();
    switch(color)
    {
    case 1:
        ui->color1_RB->setChecked(true);
        break;
    case 2:
        ui->color2_RB->setChecked(true);
        break;
    case 3:
        ui->color3_RB->setChecked(true);
        break;
    case 4:
        ui->color4_RB->setChecked(true);
        break;
    case 5:
        ui->color5_RB->setChecked(true);
        break;
    default:
        ui->color1_RB->setChecked(true);
        break;
    }

    emit colorChangeSignal(color);
}
