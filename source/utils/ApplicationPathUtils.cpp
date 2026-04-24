#include "ApplicationPathUtils.hpp"

#include "LogManager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString get_app_base_path() {
    try {
        const QString appDir = QCoreApplication::applicationDirPath();
        const QStringList candidates = {appDir + "/source", appDir, QDir::currentPath() + "/source",
                                        QDir::currentPath()};
        for (const QString &candidate : candidates) {
            if (QDir(candidate + "/assets").exists() || QDir(candidate).exists()) {
                return QDir(candidate).absolutePath();
            }
        }
        return QDir(appDir).absolutePath();
    } catch (...) {
        LogManager::error("Erro ao obter caminho base do app");
        return QDir::currentPath();
    }
}

QString get_text_file_path(const QString &filename, const QString &folder) {
    const QString basePath = get_app_base_path();
    QStringList possiblePaths;
    if (!folder.isEmpty()) {
        possiblePaths << basePath + "/assets/" + folder + "/" + filename << basePath + "/" + folder + "/" + filename
                      << basePath + "/_internal/assets/" + folder + "/" + filename
                      << basePath + "/_internal/" + folder + "/" + filename
                      << QDir(basePath).absoluteFilePath("../assets/" + folder + "/" + filename)
                      << QDir(basePath).absoluteFilePath("../" + folder + "/" + filename)
                      << basePath + "/main.dist/assets/" + folder + "/" + filename
                      << basePath + "/dist/main.dist/assets/" + folder + "/" + filename;
    } else {
        possiblePaths << basePath + "/assets/" + filename << basePath + "/" + filename
                      << basePath + "/_internal/assets/" + filename << basePath + "/_internal/" + filename
                      << QDir(basePath).absoluteFilePath("../assets/" + filename)
                      << QDir(basePath).absoluteFilePath("../" + filename) << basePath + "/main.dist/assets/" + filename
                      << basePath + "/dist/main.dist/assets/" + filename;
    }
    for (const QString &path : possiblePaths) {
        const QString abs = QDir::cleanPath(path);
        if (QFile::exists(abs)) {
            return abs;
        }
    }
    return possiblePaths.isEmpty() ? QString() : QDir::cleanPath(possiblePaths.first());
}

QString load_text_file(const QString &filename, const QString &folder, const char *) {
    const QString filePath = get_text_file_path(filename, folder);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LogManager::error(QString("Erro ao carregar arquivo de texto '%1'").arg(filename));
        return QString();
    }
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    return stream.readAll();
}
