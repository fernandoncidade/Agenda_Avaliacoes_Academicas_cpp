#include "IconUtils.hpp"

#include "ApplicationPathUtils.hpp"
#include "LogManager.hpp"

#include <QDir>
#include <QFile>

QString get_icon_app_base_path() {
    return get_app_base_path();
}

QString get_icon_path(const QString &icon_name) {
    const QString basePath = get_icon_app_base_path();
    const QStringList possiblePaths = {basePath + "/assets/icones/" + icon_name, basePath + "/icones/" + icon_name,
                                       basePath + "/_internal/icones/" + icon_name,
                                       QDir(basePath).absoluteFilePath("../assets/icones/" + icon_name),
                                       QDir(basePath).absoluteFilePath("../icones/" + icon_name)};
    for (const QString &path : possiblePaths) {
        const QString abs = QDir::cleanPath(path);
        if (QFile::exists(abs)) {
            return abs;
        }
    }
    return possiblePaths.isEmpty() ? QString() : QDir::cleanPath(possiblePaths.first());
}
