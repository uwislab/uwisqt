#ifndef SHOWCODES_H
#define SHOWCODES_H

#include <QWidget>
#include <QDir>
#include <qevent.h>
#include <QFileDialog>

namespace Ui {
class showCodes;
}

class showCodes : public QWidget
{
    Q_OBJECT

public:
    explicit showCodes(QWidget *parent = nullptr);
    ~showCodes();

    bool eventFilter(QObject*, QEvent*);

    QString getProjectName();
    void updateInstanceDir(QString);

public slots:
    void show(QString str);//显示代码
    void runcode();
    void rename(QString str);
    void save();
signals:
    void readtoRun(QString path);
    void makeNewProject(QString);
    void changetoFileTreeWidget();
private:
    Ui::showCodes *ui;

    QString instanceDir="";
};

#endif // SHOWCODES_H
