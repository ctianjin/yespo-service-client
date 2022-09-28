#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <QtCore/QString>
#include "utilityglobal.h"

namespace ns {

class BUILDSYS_UTILITY_DECL FileUtility
{
public:
    static bool rmdir(const QString& path);
    static bool cpdir(const QString& srcPath, const QString& destPath);

private:
    Q_DISABLE_COPY(FileUtility)
};

} // namespace ns

#endif // FILEUTILITY_H
