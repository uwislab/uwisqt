#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPainter>

namespace Ui {
class SideBar;
}

class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar();
public slots:
    void updateAvatar(QPixmap source);
signals:
    void openUserWidget();
    void openEditorWidget();
    void openCloudFileWidget();
    void openCommunityWidget();
private:
    void setBotton(QPushButton *btn,const QString &iconPath,const QString &text);
    Ui::SideBar *ui;
};

#endif // SIDEBAR_H
