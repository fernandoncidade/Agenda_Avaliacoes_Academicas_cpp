#pragma once

#include <QString>
#include <QWidget>
QString get_text(const QString &text);

class TrialManager {
public:
    static QString get_config_path();
    static qint64 get_first_run_timestamp();
    static void set_first_run_timestamp(qint64 timestamp);
    static void delete_first_run_timestamp();
    static qint64 get_trial_first_run();
    static bool is_trial_expired();
    static void enforce_trial(QWidget *parent = nullptr);

private:
    static constexpr const char *CONFIG_FILE = "trial_info.json";
    static constexpr const char *DEFAULT_TRIAL = "minutes";
    static constexpr int DEFAULT_TRIAL_VALUE = 1;
    static constexpr const char *PAID_VERSION_URL = "ms-windows-store://pdp/?productid=9P289X0185C3";
    static constexpr bool LIBERAR_USO_DEFINITIVO = false;
};
