#include "LogManager.hpp"

#include <algorithm>

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QTextStream>

namespace {
    QString g_logFile;
    QMutex g_mutex;

    QString localAppDataRoot() {
        const QString env = qEnvironmentVariable("LOCALAPPDATA");
        if (!env.trimmed().isEmpty()) {
            return env;
        }
        return QDir::homePath() + "/AppData/Local";
    }
} // namespace

QString LogManager::get_log_file() {
    configure_logging();
    return g_logFile;
}

QString LogManager::ensure_unicode(const QString &message) {
    return message;
}

void LogManager::configure_logging() {
    QMutexLocker locker(&g_mutex);
    if (!g_logFile.isEmpty()) {
        return;
    }
    QDir logDir(localAppDataRoot() + "/AgendaAvaliacoesAcademicas/logs");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }
    const QString stamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    g_logFile = logDir.filePath(QString("file_agenda_%1.log").arg(stamp));
    prune_old_log_files();
}

void LogManager::prune_old_log_files() {
    QDir logDir(localAppDataRoot() + "/AgendaAvaliacoesAcademicas/logs");
    QFileInfoList logFiles = logDir.entryInfoList(
        QStringList() << QStringLiteral("file_agenda_*.log"), QDir::Files | QDir::NoSymLinks);

    std::sort(logFiles.begin(), logFiles.end(), [](const QFileInfo &left, const QFileInfo &right) {
        if (left.lastModified() != right.lastModified()) {
            return left.lastModified() > right.lastModified();
        }
        return left.fileName() > right.fileName();
    });

    for (int index = kMaxLogFiles; index < logFiles.size(); ++index) {
        QFile::remove(logFiles.at(index).absoluteFilePath());
    }
}

void LogManager::write(const QString &level, const QString &message) {
    configure_logging();
    const QString line =
        QString("%1 [%2] FileCompression: %3")
            .arg(QDateTime::currentDateTime().toString(Qt::ISODateWithMs), level, ensure_unicode(message));
    {
        QMutexLocker locker(&g_mutex);
        QFile file(g_logFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setEncoding(QStringConverter::Utf8);
            stream << line << '\n';
        }
        prune_old_log_files();
    }
    if (level == "ERROR" || level == "CRITICAL") {
        qCritical().noquote() << line;
    } else if (level == "WARNING") {
        qWarning().noquote() << line;
    } else if (level == "INFO") {
        qInfo().noquote() << line;
    } else {
        qDebug().noquote() << line;
    }
}

void LogManager::debug(const QString &message) {
    write("DEBUG", message);
}

void LogManager::info(const QString &message) {
    write("INFO", message);
}

void LogManager::warning(const QString &message) {
    write("WARNING", message);
}

void LogManager::error(const QString &message, bool) {
    write("ERROR", message);
}

void LogManager::critical(const QString &message, bool) {
    write("CRITICAL", message);
}
