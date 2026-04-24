#pragma once

#include <QString>
QString get_app_base_path();
QString get_text_file_path(const QString &filename, const QString &folder = QString());
QString load_text_file(const QString &filename, const QString &folder = QString(), const char *encoding = "UTF-8");
