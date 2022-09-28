#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QPair>
#include <QtCore/QQueue>
#include "fileutility.h"

namespace ns {

bool FileUtility::rmdir(const QString& path)
{
    bool ret = true;
    QDir dir(path);

    if (dir.exists()) {
        const QFileInfoList infos = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System |
            QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

        foreach (const QFileInfo& info, infos) {
            if (info.isDir()) {
                ret = rmdir(info.absoluteFilePath());
            } else {
                ret = QFile::remove(info.absoluteFilePath());
            }
            if (!ret)
                return ret;
        }

        ret = dir.rmdir(dir.absolutePath());
    }

    return ret;
}

bool FileUtility::cpdir(const QString& srcPath, const QString& destPath)
{
    QQueue<QPair<QString, QString> > queue;
    queue.enqueue(qMakePair(srcPath, destPath));

    while (!queue.isEmpty()) {
        QPair<QString, QString> p = queue.dequeue();
        QDir srcDir(p.first);
        QDir destDir(p.second);

        if (!srcDir.exists())
            return false;

        if (!destDir.exists()) {
            if (!destDir.mkpath(p.second))
                return false;
        }

        const QStringList files = srcDir.entryList(QDir::Files);
        foreach (const QString& file, files) {
            const QString srcPath = srcDir.absoluteFilePath(file);
            const QString destPath = destDir.absoluteFilePath(file);
            if (!QFile::copy(srcPath, destPath))
                return false;
        }

        const QStringList dirs = srcDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        foreach (const QString& dir, dirs) {
            const QString srcPath = srcDir.absoluteFilePath(dir);
            const QString destPath = destDir.absoluteFilePath(dir);
            queue.enqueue(qMakePair(srcPath, destPath));
        }
    }

    return true;
}

} // namespace ns
