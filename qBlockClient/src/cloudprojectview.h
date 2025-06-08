#ifndef CLOUDPROJECTVIEW_H
#define CLOUDPROJECTVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "httpmanager.h"
#include "commentwidget.h"

class CloudProjectView : public QWidget
{
    Q_OBJECT
public:
    explicit CloudProjectView(QWidget *parent = nullptr);

    void setProjectData(const QString &title,
                        const QString &description,
                        const QString &PID,
                        const QString &projectName);
public slots:
    void populateComment(const QJsonArray &Comments);
    void btnsetVisible(bool premission);
signals:
    void deleteCommunityProject(QString);
private slots:
    void onDownloadClicked();
    void onSubmitComment();


private:
    void setupUI();
    void createHeaderSection(QVBoxLayout *mainLayout);
    void createImageSection(QVBoxLayout *mainLayout);
    void createDescriptionSection(QVBoxLayout *mainLayout);
    void createDownloadSection(QVBoxLayout *mainLayout);
    void createCommentSection(QVBoxLayout *mainLayout);


    QLabel *m_titleLabel;
    QLabel *m_imageLabel;
    QTextBrowser *m_descriptionBrowser;
    QPushButton *m_downloadButton;
    QPushButton *m_deleteButton;
    QListWidget *m_commentList;
    QLineEdit *m_commentInput;

    QString PID="";
    QString projectName;
};

#endif // CLOUDPROJECTVIEW_H
