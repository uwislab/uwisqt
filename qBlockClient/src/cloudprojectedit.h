#ifndef CLOUDPROJECTEDIT_H
#define CLOUDPROJECTEDIT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "httpmanager.h"

class CloudProjectEdit: public QWidget
{
    Q_OBJECT
public:
    explicit CloudProjectEdit(QWidget *parent = nullptr);
    void setInitialData(const QString& title, const QString& description, const QString& projectId);

signals:
    void filePublished(const QString& projectId, const QString& title, const QString& description, const QString& image);

private slots:
    void onImageClicked();
    void onPublishClicked();

private:
    void setupUI();
    void createImageSection(QVBoxLayout *layout);
    void createFormSection(QVBoxLayout *layout);
    QPixmap createRectPixmap(const QPixmap& source);

    QLabel *m_imageLabel;
    QLineEdit *m_titleEdit;
    QTextEdit *m_descEdit;
    QPushButton *m_publishBtn;
    QPixmap m_currentImage;
    QString m_currentImagePath = "";
    QString projectId = "";
};

#endif // CLOUDPROJECTEDIT_H
