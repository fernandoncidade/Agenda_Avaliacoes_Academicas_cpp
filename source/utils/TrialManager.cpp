#include "TrialManager.hpp"

#include "CaminhoPersistenteUtils.hpp"
#include "IconUtils.hpp"
#include "LogManager.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

QString get_text(const QString &text) {
    return QCoreApplication::translate("InterfaceGerenciadorAtividades", text.toUtf8().constData());
}

QString TrialManager::get_config_path() {
    return QDir(obter_caminho_persistente()).filePath(CONFIG_FILE);
}

qint64 TrialManager::get_first_run_timestamp() {
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\EisenhowerOrganizer", QSettings::NativeFormat);
    return settings.value("FirstRunTimestamp", 0).toLongLong();
}

void TrialManager::set_first_run_timestamp(qint64 timestamp) {
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\EisenhowerOrganizer", QSettings::NativeFormat);
    settings.setValue("FirstRunTimestamp", timestamp);
}

void TrialManager::delete_first_run_timestamp() {
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\EisenhowerOrganizer", QSettings::NativeFormat);
    settings.remove("FirstRunTimestamp");
    QFile::remove(get_config_path());
}

qint64 TrialManager::get_trial_first_run() {
    const QString path = get_config_path();
    qint64 firstRun = get_first_run_timestamp();
    if (firstRun > 0) {
        return firstRun;
    }
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        const QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
        firstRun = obj.value("first_run").toVariant().toLongLong();
    }
    if (firstRun <= 0) {
        firstRun = QDateTime::currentSecsSinceEpoch();
    }
    set_first_run_timestamp(firstRun);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonObject obj;
        obj.insert("first_run", firstRun);
        file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }
    return firstRun;
}

bool TrialManager::is_trial_expired() {
    const qint64 firstRun = get_trial_first_run();
    const qint64 now = QDateTime::currentSecsSinceEpoch();
    const qint64 seconds =
        QString(DEFAULT_TRIAL) == "days" ? DEFAULT_TRIAL_VALUE * 24 * 3600 : DEFAULT_TRIAL_VALUE * 60;
    return (now - firstRun) > seconds;
}

void TrialManager::enforce_trial(QWidget *parent) {
    if (LIBERAR_USO_DEFINITIVO || !is_trial_expired()) {
        return;
    }
    QMessageBox msg(parent);
    const QString iconFile = get_icon_path("ReviewsManager.ico");
    if (!iconFile.isEmpty()) {
        msg.setWindowIcon(QIcon(iconFile));
    }
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle(get_text("trial_expired_title"));
    msg.setTextFormat(Qt::RichText);
    msg.setText(get_text("trial_expired_message") + "<br>" + get_text("trial_buy_message") + "<br><br>" +
                get_text("trial_uninstall_message") + "<br><br>" +
                QString("<a href=\"%1\">%2</a>").arg(PAID_VERSION_URL, get_text("trial_paid_link")));
    msg.exec();
    QDesktopServices::openUrl(QUrl(PAID_VERSION_URL));
    std::exit(0);
}
