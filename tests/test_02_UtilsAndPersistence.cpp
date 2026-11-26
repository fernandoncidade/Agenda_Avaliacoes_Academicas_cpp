#include "TestSupport.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/ComboBoxUtils.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"
#include "source/utils/TrialManager.hpp"

#include <QComboBox>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QSignalSpy>
#include <QThread>
#include <QtTest>

#include <memory>
#include <vector>

class TestUtilsAndPersistence : public QObject {
    Q_OBJECT

private:
    AgendaTestSupport::IsolatedLocalAppDataGuard m_envGuard;

private slots:
    void initTestCase() {
        QVERIFY(m_envGuard.isValid());
    }

    void testLogManagerRotationAndConcurrency() {
        // Pre-seed 15 dummy old log files BEFORE first LogManager call in this process
        // so configure_logging() -> prune_old_log_files() enforces kMaxLogFiles = 10 cleanly.
        const QDir logDir(m_envGuard.appDataDir() + QStringLiteral("/logs"));
        QVERIFY(logDir.mkpath(QStringLiteral(".")));
        const QDateTime baseTime = QDateTime::currentDateTime().addDays(-30);
        for (int i = 0; i < 15; ++i) {
            const QString dummyName = QStringLiteral("file_agenda_202501%1_010101.log").arg(i + 10, 2, 10, QLatin1Char('0'));
            QFile dummy(logDir.filePath(dummyName));
            QVERIFY(dummy.open(QIODevice::WriteOnly | QIODevice::Text));
            dummy.write("old log\n");
            dummy.close();
            if (dummy.open(QIODevice::ReadWrite)) {
                dummy.setFileTime(baseTime.addSecs(i * 60), QFileDevice::FileModificationTime);
                dummy.close();
            }
        }

        const QString activeLogFile = LogManager::get_log_file();
        LogManager::info(QStringLiteral("INIT_LOG_PROBE"));
        QVERIFY(QFileInfo::exists(activeLogFile));
        QVERIFY(activeLogFile.startsWith(QDir::cleanPath(logDir.absolutePath())) ||
                QFileInfo(activeLogFile).dir() == logDir);

        const QFileInfoList afterInit =
            logDir.entryInfoList(QStringList{QStringLiteral("file_agenda_*.log")}, QDir::Files | QDir::NoSymLinks);
        QVERIFY2(afterInit.size() <= 10,
                 qPrintable(QStringLiteral("Expected <= 10 rotated log files, got %1").arg(afterInit.size())));

        // Concurrent multi-thread writes (8 threads x 25 writes = 200 messages)
        constexpr int kThreads = 8;
        constexpr int kPerThread = 25;
        std::vector<std::unique_ptr<QThread>> workers;
        workers.reserve(kThreads);
        for (int t = 0; t < kThreads; ++t) {
            QThread *worker = QThread::create([t]() {
                for (int i = 0; i < kPerThread; ++i) {
                    const QString payload = QStringLiteral("CONCURRENCY_T%1_M%2_ÁÉÍÓÚ").arg(t).arg(i);
                    if (i % 2 == 0) {
                        LogManager::info(payload);
                    } else {
                        LogManager::debug(payload);
                    }
                }
            });
            workers.emplace_back(worker);
            worker->start();
        }
        for (auto &worker : workers) {
            QVERIFY(worker->wait(10000));
        }

        LogManager::warning(QStringLiteral("WARN_PROBE"));
        LogManager::error(QStringLiteral("ERROR_PROBE"));
        LogManager::critical(QStringLiteral("CRIT_PROBE"));

        const QString logText = AgendaTestSupport::readUtf8File(activeLogFile);
        QVERIFY(logText.contains(QStringLiteral("CONCURRENCY_T0_M0_ÁÉÍÓÚ")));
        QVERIFY(logText.contains(QStringLiteral("CONCURRENCY_T7_M24_ÁÉÍÓÚ")));
        QVERIFY(logText.contains(QStringLiteral("[WARNING]")));
        QVERIFY(logText.contains(QStringLiteral("[ERROR]")));
        QVERIFY(logText.contains(QStringLiteral("[CRITICAL]")));
    }

    void testPersistentPathAndAssetResolution() {
        const QString persistentPath = obter_caminho_persistente();
        QCOMPARE(QDir::cleanPath(persistentPath), QDir::cleanPath(m_envGuard.appDataDir()));
        QVERIFY(QDir(persistentPath).exists());

        const QString basePath = get_app_base_path();
        QVERIFY2(!basePath.isEmpty() && QDir(basePath).exists(), qPrintable(basePath));

        const QString aboutPath = get_text_file_path(QStringLiteral("ABOUT_pt_BR.txt"), QStringLiteral("ABOUT"));
        QVERIFY2(QFileInfo::exists(aboutPath), qPrintable(aboutPath));

        const QString aboutContent = load_text_file(QStringLiteral("ABOUT_pt_BR.txt"), QStringLiteral("ABOUT"));
        QVERIFY2(aboutContent.contains(QStringLiteral("AVALIAÇÕES ACADÊMICAS")), "UTF-8 accents must decode cleanly");

        const QString missingContent =
            load_text_file(QStringLiteral("DEFINITELY_MISSING_ASSET_XYZ.txt"), QStringLiteral("ABOUT"));
        QVERIFY(missingContent.isEmpty());

        const QString iconPath = get_icon_path(QStringLiteral("ReviewsManager.ico"));
        QVERIFY2(QFileInfo::exists(iconPath), qPrintable(iconPath));
    }

    void testComboBoxUtilsBehaviorAndSignals() {
        QCOMPARE(_normalizar_texto(QStringLiteral("  Engenharia   CIVIL ")), QStringLiteral("engenharia   civil"));
        QCOMPARE(_normalizar_texto(QStringLiteral("  Ágil  ")), QStringLiteral("ágil"));

        QComboBox combo;
        configurar_combo_texto_livre(&combo);
        QVERIFY(combo.isEditable());
        QCOMPARE(combo.insertPolicy(), QComboBox::NoInsert);
        QVERIFY(combo.lineEdit() != nullptr);
        QVERIFY(combo.lineEdit()->isClearButtonEnabled());

        // Null pointer guards should never crash
        configurar_combo_texto_livre(nullptr);
        QVERIFY(obter_itens_combo(nullptr).isEmpty());
        adicionar_item_combo_se_ausente(nullptr, QStringLiteral("X"));
        substituir_itens_combo(nullptr, {QStringLiteral("A")});

        QSignalSpy textSpy(&combo, &QComboBox::currentTextChanged);

        adicionar_item_combo_se_ausente(&combo, QStringLiteral("   "));
        QCOMPARE(combo.count(), 0);
        QCOMPARE(textSpy.count(), 0);

        adicionar_item_combo_se_ausente(&combo, QStringLiteral("  Cálculo I  "));
        QCOMPARE(combo.count(), 1);
        QCOMPARE(combo.currentText(), QStringLiteral("Cálculo I"));
        QVERIFY2(textSpy.count() == 0, "QSignalBlocker inside adicionar_item_combo_se_ausente must silence signals");

        QVERIFY(combo_contem_texto(&combo, QStringLiteral("cálculo i")));
        QVERIFY(combo_contem_texto(&combo, QStringLiteral("")));
        QVERIFY(!combo_contem_texto(&combo, QStringLiteral("Física III")));

        adicionar_item_combo_se_ausente(&combo, QStringLiteral("cálculo i"));
        QCOMPARE(combo.count(), 1);

        substituir_itens_combo(&combo, {QStringLiteral("A"), QStringLiteral("B")}, QStringLiteral("Custom C"));
        QCOMPARE(obter_itens_combo(&combo), QStringList({QStringLiteral("A"), QStringLiteral("B"), QStringLiteral("Custom C")}));
        QCOMPARE(combo.currentText(), QStringLiteral("Custom C"));
        QCOMPARE(textSpy.count(), 0);
    }

    void testTrialManagerWithRegistryGuard() {
        AgendaTestSupport::RegistryTimestampGuard regGuard;

        TrialManager::delete_first_run_timestamp();
        QCOMPARE(TrialManager::get_first_run_timestamp(), qint64(0));

        const qint64 now = QDateTime::currentSecsSinceEpoch();
        TrialManager::set_first_run_timestamp(now);
        QCOMPARE(TrialManager::get_first_run_timestamp(), now);
        QVERIFY(!TrialManager::is_trial_expired());

        // Safe non-expired enforce_trial path returns immediately without std::exit(0)
        TrialManager::enforce_trial(nullptr);

        // Verify expired calculation (without calling enforce_trial)
        TrialManager::set_first_run_timestamp(now - 3600);
        QVERIFY(TrialManager::is_trial_expired());

        // Verify get_trial_first_run initializes trial_info.json when registry cleared
        TrialManager::delete_first_run_timestamp();
        QVERIFY(!QFileInfo::exists(TrialManager::get_config_path()));
        const qint64 initializedTs = TrialManager::get_trial_first_run();
        QVERIFY(initializedTs > 0);
        QVERIFY(QFileInfo::exists(TrialManager::get_config_path()));

        QFile jsonFile(TrialManager::get_config_path());
        QVERIFY(jsonFile.open(QIODevice::ReadOnly));
        const QJsonObject obj = QJsonDocument::fromJson(jsonFile.readAll()).object();
        QCOMPARE(obj.value(QStringLiteral("first_run")).toVariant().toLongLong(), initializedTs);

        QVERIFY(!get_text(QStringLiteral("Curso:")).isEmpty());
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestUtilsAndPersistence tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_02_UtilsAndPersistence.moc"
