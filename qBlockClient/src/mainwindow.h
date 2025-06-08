#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>
#include <QStackedWidget>
#include <QScrollArea>
#include "output.h"
#include "showcodes.h"
#include "webhandle.h"
#include "inisettings.h"
#include "filetree.h"
#include "sidebar.h"
#include "login.h"
#include "httpmanager.h"
#include "cloudprojectswidget.h"
#include "cloudprojectedit.h"
#include "cloudprojectview.h"
#include "communitywidget.h"
#include "usermanager.h"
#include "zipcompress.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static int permission;
public slots:
    void makeNewProject(QString);
    QString getNowProjectDir();
    void changeColor(int);

private:
    Ui::MainWindow *ui;
    QWebEngineView *webView = nullptr;
    QWebChannel *webChannel = nullptr;
    WebHandle *webobj = nullptr;
    output *output_wd = nullptr;
    showCodes *showcodes_wd = nullptr;
    FileTree *filetree_wd = nullptr;

    Login *login_wd = nullptr;
    UserManager *userinfo_wd = nullptr;

    QStackedWidget *codeStackedWidget = nullptr;
    QStackedWidget *mainStackedWidget = nullptr;

    QWidget *editorWidget = nullptr;
    CommunityWidget *communityWidget = nullptr;
    CloudProjectsWidget *cloudFileWidget = nullptr;
    //QWidget *cloudFileWidget = nullptr;

    CloudProjectView *cloudview = nullptr;
    CloudProjectEdit *cloudedit = nullptr;

    SideBar *sideBarWidget = nullptr;

    QString nowProjectDir = "";

    HttpManager *manager = nullptr;
    ZipCompress *zip = nullptr;

    bool isLogin = false;
};
#endif // MAINWINDOW_H
