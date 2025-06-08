#ifndef ZIPCOMPRESS_H
#define ZIPCOMPRESS_H

#include <QObject>
#include <QtCore/private/qzipreader_p.h>
#include <QtCore/private/qzipwriter_p.h>


class ZipCompress : public QObject
{
    Q_OBJECT
public:
    ZipCompress(QObject *parent = nullptr);


    /**
     * @brief 压缩文件
     * @param fileNames 需要压缩的文件名
     * @param saveName  压缩后的文件路径和名字
     */
    bool zipCompressFile(const QStringList &fileNames, const QString &saveName);



    /**
     * @brief 压缩文件夹
     * @param dirName   需要压缩的文件夹名
     * @param saveName  压缩后的文件路径和名字
     */
    bool zipCompressDirector(const QString &dirName, const QString &saveName);



    /**
     * @brief 解压缩
     * @param zipFile   压缩包
     * @param saveDir   解压路径
     * @return
     */
    bool zipUnCompressFile(const QString &zipFile, const QString &saveDir);



private:
    /**
     * @brief 递归压缩
     * @param zipWriterHandle       QZipWriter指针
     * @param zipParentDirName      父路径，相对路径
     * @param srcDir                当前需要处理的文件夹路径，绝对路径
     */
    void zipCompressDirector(void *zipWriterHandle, const QString &zipParentDirName, const QString &srcDir);


private:
    QZipReader *mZipReader;
    QZipWriter *mZipWriter;
};

#endif // ZIPCOMPRESS_H
