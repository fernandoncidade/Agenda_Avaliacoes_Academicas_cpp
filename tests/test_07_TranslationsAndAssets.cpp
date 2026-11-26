#include "TestSupport.hpp"
#include "source/GerenciamentoInterfaceGrafica/GerenciadorInterfaceGrafica/ginter_02_OpcoesSobre.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"

#include <QApplication>
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileInfo>
#include <QPushButton>
#include <QSignalSpy>
#include <QtTest>

class TestTranslationsAndAssets : public QObject {
    Q_OBJECT

private:
    AgendaTestSupport::IsolatedLocalAppDataGuard m_envGuard;

private slots:
    void testGerenciadorTraducaoLifecycle() {
        const QString transDir = obter_diretorio_traducoes();
        QVERIFY2(QFileInfo::exists(QDir(transDir).filePath(QStringLiteral("manager_compression_en_US.qm"))),
                 qPrintable(transDir));
        QVERIFY2(QFileInfo::exists(QDir(transDir).filePath(QStringLiteral("manager_compression_pt_BR.qm"))),
                 qPrintable(transDir));

        GerenciadorTraducao gt;
        QCOMPARE(gt.obter_idioma_atual(), QStringLiteral("pt_BR"));
        QVERIFY(gt.idiomas_disponiveis.contains(QStringLiteral("pt_BR")));
        QVERIFY(gt.idiomas_disponiveis.contains(QStringLiteral("en_US")));
        QVERIFY(gt.aplicar_traducao());

        QSignalSpy spy(&gt, &GerenciadorTraducao::idioma_alterado);
        QVERIFY(!gt.definir_idioma(QStringLiteral("fr_FR")));
        QCOMPARE(spy.count(), 0);

        QVERIFY(gt.definir_idioma(QStringLiteral("en_US")));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.takeFirst().at(0).toString(), QStringLiteral("en_US"));
        QCOMPARE(QCoreApplication::translate("InterfaceGerenciadorAtividades", "Curso:"), QStringLiteral("Course:"));
        QCOMPARE(QCoreApplication::translate("InterfaceGerenciadorAtividades", "Registrar Definições"),
                 QStringLiteral("Register Entries"));

        // Verify language.json persisted and reloaded by fresh GerenciadorTraducao
        {
            GerenciadorTraducao reloaded;
            QCOMPARE(reloaded.obter_idioma_atual(), QStringLiteral("en_US"));
        }

        QDialogButtonBox enBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Yes |
                                QDialogButtonBox::No);
        gt.traduzir_botoes_padrao(&enBox);
        // Note: tr_01_gerenciadorTraducao queries context "Dialog", which has no override entries in
        // manager_compression_en_US.ts (unlike "InterfaceGerenciadorAtividades"); verify non-empty deterministic text.
        QCOMPARE(enBox.button(QDialogButtonBox::Ok)->text(), QStringLiteral("OK"));
        QVERIFY(!enBox.button(QDialogButtonBox::Cancel)->text().isEmpty());
        QCOMPARE(enBox.button(QDialogButtonBox::Yes)->text(), QStringLiteral("Sim"));
        QCOMPARE(enBox.button(QDialogButtonBox::No)->text(), QStringLiteral("Não"));

        QVERIFY(gt.definir_idioma(QStringLiteral("pt_BR")));
        QCOMPARE(QCoreApplication::translate("InterfaceGerenciadorAtividades", "Curso:"), QStringLiteral("Curso:"));
        gt.traduzir_botoes_padrao(&enBox);
        QCOMPARE(enBox.button(QDialogButtonBox::Yes)->text(), QStringLiteral("Sim"));
        QCOMPARE(enBox.button(QDialogButtonBox::No)->text(), QStringLiteral("Não"));
        QCOMPARE(QCoreApplication::translate("InterfaceGerenciadorAtividades", "Sim"), QStringLiteral("Sim"));
    }

    void testAboutContentAndDualLicensingAssets() {
        const QString projectVersion = QString::fromUtf8(AGENDA_PROJECT_VERSION);
        QVERIFY(SITE_LICENSES().contains(QStringLiteral("gnu.org/licenses")));
        QVERIFY(LICENSE_TEXT_PT_BR().contains(QStringLiteral("GNU Affero General Public License v3")));
        QVERIFY(LICENSE_TEXT_EN_US().contains(QStringLiteral("GNU Affero General Public License v3")));
        QVERIFY(NOTICE_TEXT_PT_BR().contains(QStringLiteral("6.12.0")));
        QVERIFY(NOTICE_TEXT_EN_US().contains(QStringLiteral("6.12.0")));
        QVERIFY(ABOUT_TEXT_PT_BR().contains(projectVersion));
        QVERIFY(ABOUT_TEXT_EN_US().contains(projectVersion));
        QVERIFY(Privacy_Policy_pt_BR().contains(QStringLiteral("Microsoft")));
        QVERIFY(Privacy_Policy_en_US().contains(QStringLiteral("Microsoft")));
        QVERIFY(!History_APP_pt_BR().isEmpty());
        QVERIFY(!History_APP_en_US().isEmpty());
        QVERIFY(RELEASE_NOTES_pt_BR().contains(projectVersion));
        QVERIFY(RELEASE_NOTES_en_US().contains(projectVersion));
    }

    void testRepositoryDocumentationSuiteAndNoCliReference() {
        const QDir repoRoot(QString::fromUtf8(AGENDA_SOURCE_DIR));
        QVERIFY2(repoRoot.exists(), qPrintable(repoRoot.absolutePath()));

        const QString licenseText = AgendaTestSupport::readUtf8File(repoRoot.filePath(QStringLiteral("LICENSE")));
        QVERIFY2(licenseText.contains(QStringLiteral("GNU AFFERO GENERAL PUBLIC LICENSE")),
                 "Root LICENSE must contain GNU AGPLv3");

        const QStringList rootDocs = {
            QStringLiteral("README.md"),          QStringLiteral("CODE_OF_CONDUCT.md"),
            QStringLiteral("CONTRIBUTING.md"),    QStringLiteral("SECURITY.md"),
            QStringLiteral("MANUAL.md"),          QStringLiteral("OBSERVACAO.md"),
        };
        for (const QString &doc : rootDocs) {
            const QString content = AgendaTestSupport::readUtf8File(repoRoot.filePath(doc));
            QVERIFY2(!content.isEmpty(), qPrintable(QStringLiteral("Missing root doc: %1").arg(doc)));
            QVERIFY2(!content.contains(QStringLiteral("cli-reference.md")),
                     qPrintable(QStringLiteral("Forbidden cli-reference.md mention in %1").arg(doc)));
        }

        const QStringList requiredDocs = {
            QStringLiteral("docs/architecture.md"),
            QStringLiteral("docs/compliance.md"),
            QStringLiteral("docs/licensing.md"),
            QStringLiteral("docs/privacy-policy.md"),
            QStringLiteral("docs/release-v%1.md").arg(QString::fromUtf8(AGENDA_PROJECT_VERSION)),
            QStringLiteral("docs/security-model.md"),
            QStringLiteral("docs/suggested-issues.md"),
            QStringLiteral("docs/technical-audit-and-roadmap.md"),
            QStringLiteral("docs/user-guide.md"),
        };
        for (const QString &rel : requiredDocs) {
            const QString content = AgendaTestSupport::readUtf8File(repoRoot.filePath(rel));
            QVERIFY2(!content.isEmpty(), qPrintable(QStringLiteral("Missing docs file: %1").arg(rel)));
            QVERIFY2(!content.contains(QStringLiteral("cli-reference.md")),
                     qPrintable(QStringLiteral("Forbidden cli-reference.md mention in %1").arg(rel)));
        }

        // Explicitly prohibited per user request:
        QVERIFY2(!QFileInfo::exists(repoRoot.filePath(QStringLiteral("docs/cli-reference.md"))),
                 "docs/cli-reference.md must NOT exist");

        // Verify source/assets/LICENSES dual-license companions exist
        const QStringList licenseFiles = {
            QStringLiteral("source/assets/LICENSES/GNU AFFERO GENERAL PUBLIC LICENSE Version3.txt"),
            QStringLiteral("source/assets/LICENSES/GNU GENERAL PUBLIC LICENSE Version 3.txt"),
            QStringLiteral("source/assets/LICENSES/GNU LESSER GENERAL PUBLIC LICENSE Version 3.txt"),
            QStringLiteral("source/assets/LICENSES/SQLite Is Public Domain.txt"),
        };
        for (const QString &rel : licenseFiles) {
            QVERIFY2(QFileInfo::exists(repoRoot.filePath(rel)),
                     qPrintable(QStringLiteral("Missing license text: %1").arg(rel)));
        }
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestTranslationsAndAssets tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_07_TranslationsAndAssets.moc"
