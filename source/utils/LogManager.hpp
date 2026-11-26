#pragma once

#include <QString>

class LogManager {
public:
    static QString get_log_file();
    static QString ensure_unicode(const QString &message);
    static void debug(const QString &message);
    static void info(const QString &message);
    static void warning(const QString &message);
    static void error(const QString &message, bool exc_info = false);
    static void critical(const QString &message, bool exc_info = true);

private:
    static constexpr int kMaxLogFiles = 10;

    static void configure_logging();
    static void prune_old_log_files();
    static void write(const QString &level, const QString &message);
};
