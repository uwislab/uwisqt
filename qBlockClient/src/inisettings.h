#ifndef INISETTINGS_H
#define INISETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QSharedPointer>
#include <QCoreApplication>
//#include <QTextCodec>
#include <QFile>
#include <QFileDialog>
#include<QButtonGroup>

namespace Ui {
class IniSettings;
}

class IniSettings : public QWidget
{
    Q_OBJECT


public:
    void settings_init(); //初始化QSettings对象，并读取ini配置文件,如果不存在配置文件，则创建
    static IniSettings& getInstance();
    void setValue(const QString &section, const QString &key, const QString &value); //写入配置项（section:节点，key:键名，value:键值）
    void removeSection(const QString &section);                           //移除节点(包括其中所有的键值)
    void removeKey(const QString &section, const QString &key);               //移除节点中的某个键值
    QString getValue(const QString &section, const QString &key, const QString &defaultValue);  //读配置项
    QStringList getValueList(const QString &section, const QString &key, QString defaultValue = ""); //批量读取
protected:
    void closeEvent(QCloseEvent *event) override;
public slots:
    void changePythonPath();
    void changeWorkSpacePath();
    void changeColor(QAbstractButton *button);
    void applyChange();
    void cancelChange();

signals:
    void colorChangeSignal(int);
    void updateCheck();
private:

    explicit IniSettings(QWidget *parent = nullptr);
    ~IniSettings();

    Ui::IniSettings *ui;

    QString   defaultStrIniFileName;
    QSharedPointer<QSettings> m_iniFile;
    QButtonGroup *m_pButtonGroup;
};

#endif // INISETTINGS_H
