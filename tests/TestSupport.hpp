#pragma once

#include <QApplication>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QRegularExpression>
#include <QSettings>
#include <QString>
#include <QTemporaryDir>
#include <QTextStream>
#include <QTimer>
#include <QVariant>
#include <QWidget>

#include <functional>
#include <memory>
#include <utility>

namespace AgendaTestSupport {

class IsolatedLocalAppDataGuard {
public:
    IsolatedLocalAppDataGuard() {
        m_hadOriginal = qEnvironmentVariableIsSet("LOCALAPPDATA");
        if (m_hadOriginal) {
            m_originalValue = qgetenv("LOCALAPPDATA");
        }
        if (m_tempDir.isValid()) {
            qputenv("LOCALAPPDATA", QDir::toNativeSeparators(m_tempDir.path()).toUtf8());
        }
    }

    ~IsolatedLocalAppDataGuard() {
        if (m_hadOriginal) {
            qputenv("LOCALAPPDATA", m_originalValue);
        } else {
            qunsetenv("LOCALAPPDATA");
        }
    }

    IsolatedLocalAppDataGuard(const IsolatedLocalAppDataGuard &) = delete;
    IsolatedLocalAppDataGuard &operator=(const IsolatedLocalAppDataGuard &) = delete;

    bool isValid() const { return m_tempDir.isValid(); }
    QString rootPath() const { return m_tempDir.path(); }
    QString appDataDir() const { return QDir(m_tempDir.path()).filePath(QStringLiteral("AgendaAvaliacoesAcademicas")); }

private:
    QTemporaryDir m_tempDir;
    bool m_hadOriginal = false;
    QByteArray m_originalValue;
};

class RegistryTimestampGuard {
public:
    RegistryTimestampGuard()
        : m_settings(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\EisenhowerOrganizer"), QSettings::NativeFormat) {
        m_hadValue = m_settings.contains(QStringLiteral("FirstRunTimestamp"));
        if (m_hadValue) {
            m_originalValue = m_settings.value(QStringLiteral("FirstRunTimestamp"));
        }
    }

    ~RegistryTimestampGuard() {
        if (m_hadValue) {
            m_settings.setValue(QStringLiteral("FirstRunTimestamp"), m_originalValue);
        } else {
            m_settings.remove(QStringLiteral("FirstRunTimestamp"));
        }
        m_settings.sync();
    }

    RegistryTimestampGuard(const RegistryTimestampGuard &) = delete;
    RegistryTimestampGuard &operator=(const RegistryTimestampGuard &) = delete;

private:
    QSettings m_settings;
    bool m_hadValue = false;
    QVariant m_originalValue;
};

inline void scheduleModalDialogAction(std::function<bool(QWidget *)> handler, int timeoutMs = 4000) {
    auto timer = std::make_shared<QTimer>();
    auto elapsed = std::make_shared<QElapsedTimer>();
    elapsed->start();
    QObject::connect(timer.get(), &QTimer::timeout, qApp, [timer, elapsed, handler = std::move(handler), timeoutMs]() {
        if (QWidget *modal = QApplication::activeModalWidget()) {
            if (handler(modal)) {
                timer->stop();
                return;
            }
        }
        if (elapsed->elapsed() >= timeoutMs) {
            timer->stop();
        }
    });
    timer->start(20);
}

inline bool isValidHexColor(const QString &value) {
    static const QRegularExpression rx(QStringLiteral("^#[0-9A-Fa-f]{6}$"));
    return rx.match(value).hasMatch();
}

inline QString readUtf8File(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    return stream.readAll();
}

} // namespace AgendaTestSupport
