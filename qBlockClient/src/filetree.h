#ifndef FILETREE_H
#define FILETREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QDirIterator>

namespace Ui {
class FileTree;
}

class FileTree : public QWidget
{
    Q_OBJECT

public:
    explicit FileTree(QWidget *parent = nullptr);
    ~FileTree();

    void showTree(QString sPath,QTreeWidgetItem* pParentItem);
signals:
    void changetoShowCodeWidget();
private:
    Ui::FileTree *ui;

    QString dirPath = "";
};

#endif // FILETREE_H
