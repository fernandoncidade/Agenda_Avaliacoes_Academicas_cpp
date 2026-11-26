#include "TestSupport.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/IconUtils.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QImage>
#include <QImageReader>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStyleFactory>
#include <QTemporaryDir>
#include <QTextDocument>
#include <QTranslator>
#include <QtTest>

class TestFrameworkAndPlugins : public QObject {
    Q_OBJECT

private slots:
    void testCpp23AndQt612Runtime() {
#if defined(_MSC_VER)
        QVERIFY2(_MSVC_LANG >= 202302L, "MSVC build must target C++23 (_MSVC_LANG >= 202302L)");
#else
        QVERIFY2(__cplusplus >= 202302L, "MinGW build must target C++23 (__cplusplus >= 202302L)");
#endif
        QVERIFY2(QT_VERSION_MAJOR == 6 && QT_VERSION_MINOR >= 12, "Expected compile-time Qt >= 6.12");
        const QString runtimeVersion = QString::fromLatin1(qVersion());
        QVERIFY2(runtimeVersion.startsWith(QStringLiteral("6.12.")), qPrintable(runtimeVersion));
    }

    void testRequiredQtPluginsPhysicalAndActive() {
        const QString pluginsRoot = QDir::cleanPath(QString::fromUtf8(AGENDA_QT_PLUGINS_DIR));
        QVERIFY2(QDir(pluginsRoot).exists(), qPrintable(pluginsRoot));

        const QStringList requiredPlugins = {
            QStringLiteral("platforms/qwindows.dll"),
            QStringLiteral("platforms/qoffscreen.dll"),
            QStringLiteral("styles/qmodernwindowsstyle.dll"),
            QStringLiteral("sqldrivers/qsqlite.dll"),
            QStringLiteral("imageformats/qico.dll"),
        };
        for (const QString &rel : requiredPlugins) {
            const QString fullPath = QDir(pluginsRoot).filePath(rel);
            QVERIFY2(QFileInfo::exists(fullPath), qPrintable(QStringLiteral("Missing Qt plugin: %1").arg(fullPath)));
        }

        QVERIFY2(!QGuiApplication::platformName().isEmpty(), "Platform plugin failed to initialize");
        QVERIFY2(!QStyleFactory::keys().isEmpty(), "Expected at least one available Qt style");
    }

    void testSqliteDriverLiveOperations() {
        QVERIFY2(QSqlDatabase::drivers().contains(QStringLiteral("QSQLITE")), "QSQLITE driver unavailable");

        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());
        const QString dbPath = tempDir.filePath(QStringLiteral("framework_probe.db"));
        const QString connName = QStringLiteral("framework_probe_conn");

        {
            QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connName);
            db.setDatabaseName(dbPath);
            QVERIFY2(db.open(), qPrintable(db.lastError().text()));

            QSqlQuery pragma(db);
            QVERIFY(pragma.exec(QStringLiteral("SELECT sqlite_version()")));
            QVERIFY(pragma.next());
            QVERIFY(!pragma.value(0).toString().isEmpty());

            QVERIFY(pragma.exec(QStringLiteral("CREATE TABLE probe (id INTEGER PRIMARY KEY, label TEXT NOT NULL)")));

            QVERIFY(db.transaction());
            QSqlQuery insert(db);
            insert.prepare(QStringLiteral("INSERT INTO probe (label) VALUES (?)"));
            insert.addBindValue(QStringLiteral("Alpha"));
            QVERIFY(insert.exec());
            QVERIFY(db.commit());

            QVERIFY(db.transaction());
            QSqlQuery rolledBack(db);
            rolledBack.prepare(QStringLiteral("INSERT INTO probe (label) VALUES (?)"));
            rolledBack.addBindValue(QStringLiteral("RollbackMe"));
            QVERIFY(rolledBack.exec());
            QVERIFY(db.rollback());

            QSqlQuery count(db);
            QVERIFY(count.exec(QStringLiteral("SELECT COUNT(*) FROM probe")));
            QVERIFY(count.next());
            QCOMPARE(count.value(0).toInt(), 1);
            db.close();
        }
        QSqlDatabase::removeDatabase(connName);
    }

    void testImageFormatPluginsAndAppIcon() {
        const QList<QByteArray> formats = QImageReader::supportedImageFormats();
        QVERIFY2(formats.contains("ico"), "ICO imageformat plugin unavailable");
        QVERIFY2(formats.contains("png"), "PNG imageformat support unavailable");

        const QString iconPath = get_icon_path(QStringLiteral("ReviewsManager.ico"));
        QVERIFY2(QFileInfo::exists(iconPath), qPrintable(iconPath));

        const QImage img(iconPath);
        QVERIFY2(!img.isNull(), "Failed to decode ReviewsManager.ico via qico plugin");
        QVERIFY(img.width() > 0 && img.height() > 0);

        const QIcon icon(iconPath);
        QVERIFY(!icon.isNull());
        QVERIFY(!icon.pixmap(32, 32).isNull());
    }

    void testPdfWriterPipelineAndQTranslatorLoading() {
        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());
        const QString pdfPath = tempDir.filePath(QStringLiteral("framework_output.pdf"));

        {
            QPdfWriter writer(pdfPath);
            writer.setPageSize(QPageSize(QPageSize::Letter));
            writer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);
            QTextDocument doc;
            doc.setHtml(QStringLiteral("<html><body><h2>Agenda Probe</h2><p>20/07/2026 PROVA – 1</p></body></html>"));
            doc.print(&writer);
        }

        QFile pdfFile(pdfPath);
        QVERIFY2(pdfFile.exists(), "PDF output was not created");
        QVERIFY(pdfFile.open(QIODevice::ReadOnly));
        const QByteArray bytes = pdfFile.readAll();
        QVERIFY2(bytes.startsWith("%PDF-"), "Generated file is missing %PDF- header");
        QVERIFY2(bytes.contains("%%EOF"), "Generated PDF is missing %%EOF marker");
        QVERIFY(bytes.size() > 1000);

        const QString transDir = obter_diretorio_traducoes();
        QVERIFY2(QDir(transDir).exists(), qPrintable(transDir));

        QTranslator enTranslator;
        QVERIFY2(enTranslator.load(QDir(transDir).filePath(QStringLiteral("manager_compression_en_US.qm"))),
                 "Failed to load manager_compression_en_US.qm");
        QVERIFY(!enTranslator.translate("InterfaceGerenciadorAtividades", "Curso:").isEmpty());

        QTranslator ptTranslator;
        QVERIFY2(ptTranslator.load(QDir(transDir).filePath(QStringLiteral("manager_compression_pt_BR.qm"))),
                 "Failed to load manager_compression_pt_BR.qm");
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard envGuard;
    QApplication app(argc, argv);
    TestFrameworkAndPlugins tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_01_FrameworkAndPlugins.moc"
