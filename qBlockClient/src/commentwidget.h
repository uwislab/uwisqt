#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>

class CommentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommentWidget(const QString& id,const QString& userID, const QString &userName, const QString &PID,const QString &content,
                           const QDateTime &time,
                           QWidget *parent = nullptr);
private:
    void setupUI();
    void loadAvatar();
    QLabel* m_avatarLabel;
    QLabel* m_usernameLabel;
    QLabel* m_contentLabel;
    QLabel* m_timeLabel;
    //QString m_avatarUrl;

    QString id;
    QString userID;
    QString userName;
    QString PID;
    QString content;
    QDateTime time;
};

#endif // COMMENTWIDGET_H
