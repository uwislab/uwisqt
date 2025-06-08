#include "zipcompress.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

ZipCompress::ZipCompress(QObject *parent)
    : QObject(parent)
{
}

bool ZipCompress::zipCompressFile(const QStringList &fileNames, const QString &saveName)
{
    QZipWriter *zipWriter = new QZipWriter(saveName);
    if (!zipWriter) {
        qDebug() << "压缩失败！";
        return false;
    }

    QFile file;
    QFileInfo fileInfo;
    foreach (const QString &fileName, fileNames) {
        if (fileName.isEmpty() || "" == fileName) {
            continue;
        }

        // 判断文件是否存在
        fileInfo.setFile(fileName);
        if (!fileInfo.exists()) {
            qDebug() << fileName << " : 文件不存在！";
            continue;
        }

        if (fileInfo.isFile()) {
            // 打开文件
            file.setFileName(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug() << fileName << " : 文件打开失败！";
                continue;
            }

            // 添加到压缩包中
            zipWriter->addFile(fileInfo.fileName(), file.readAll());
            file.close();

        } else if (fileInfo.isDir()) {
            // 添加到压缩包中
            zipWriter->addDirectory(fileName);
        }
    }

    zipWriter->close();
    if (zipWriter) {
        delete zipWriter;
        zipWriter = nullptr;
    }

    return true;
}

bool ZipCompress::zipCompressDirector(const QString &dirName, const QString &saveName)
{
    QFile file(saveName);
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "压缩失败！";
        return false;
    }

    std::shared_ptr<QZipWriter> zipWriter(new QZipWriter(&file));
    if (!zipWriter) {
        qDebug() << "压缩失败！";
        return false;
    }


    zipCompressDirector(zipWriter.get(), "", dirName);
    zipWriter->close();
    file.close();

    return true;
}

bool ZipCompress::zipUnCompressFile(const QString &zipFile, const QString &saveDir)
{
    QFile file(zipFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << zipFile << " : 文件打开失败！";
        return false;
    }

    QDir dir(saveDir);
    if (!dir.exists()) {
        if (!dir.mkpath(saveDir)) { // 创建文件夹
            qDebug() << saveDir << " : 文件夹路径不存在，且创建失败！";
            return false;
        }
    }

    std::shared_ptr<QZipReader> zipReader(new QZipReader(&file));
    return zipReader->extractAll(saveDir);  // 全部解压出来
}

void ZipCompress::zipCompressDirector(void *zipWriterHandle, const QString &zipParentDirName, const QString &srcDir)
{
    QZipWriter *zipWriter = (QZipWriter *)zipWriterHandle;
    QDir directory(srcDir);
    QFileInfoList fileList = directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &fileInfo : fileList) {
        QString relatePath = fileInfo.fileName();       // 获取当前文件名 | 文件夹名

        if (!zipParentDirName.isEmpty()) {
            relatePath.prepend(zipParentDirName + "/"); // 头部拼接上父路径，相对路径
        }

        QString filePath = fileInfo.absoluteFilePath(); // 获取当前文件的绝对路径
        if (fileInfo.isDir()) {
            // 添加文件夹
            zipWriter->addDirectory(relatePath);
            // 参数二：文件夹相对路径      参数三：文件夹绝对路径
            zipCompressDirector(zipWriter, relatePath, filePath);

        } else {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug() << filePath << " : 文件打开失败！";
                continue;
            }

            zipWriter->addFile(relatePath, file.readAll());
            file.close();
        }
    }
}
