#include "TestSupport.hpp"
#include "compileTranslations.hpp"
#include "find_VersionEditor.hpp"

#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QProcess>
#include <QPushButton>
#include <QRegularExpression>
#include <QSet>
#include <QSpinBox>
#include <QTemporaryDir>
#include <QTextEdit>
#include <QTranslator>
#include <QtTest>

class TestMocksAndDeploymentIntegrity : public QObject {
    Q_OBJECT

private:
    static bool copyFileOverwriting(const QString &src, const QString &dst) {
        QFileInfo dstInfo(dst);
        QDir().mkpath(dstInfo.absolutePath());
        if (QFile::exists(dst)) {
            QFile::remove(dst);
        }
        return QFile::copy(src, dst);
    }

    static QPushButton *findButtonByText(QWidget *root, const QString &text) {
        for (QPushButton *btn : root->findChildren<QPushButton *>()) {
            if (btn->text() == text) {
                return btn;
            }
        }
        return nullptr;
    }

private slots:
    void testVersionEditorSandboxCheckAndUpdate() {
        const QDir realRoot(QString::fromUtf8(AGENDA_REAL_SOURCE_DIR));
        QDir sandbox(QString::fromUtf8(AGENDA_VERSION_SANDBOX_DIR));
        if (sandbox.exists()) {
            QVERIFY(sandbox.removeRecursively());
        }

        const QString currentReleaseDoc =
            QStringLiteral("docs/release-v%1.md").arg(QString::fromUtf8(AGENDA_PROJECT_VERSION));

        const QStringList trackedFiles = {
            QStringLiteral("README.md"),
            QStringLiteral("CODE_OF_CONDUCT.md"),
            QStringLiteral("CONTRIBUTING.md"),
            QStringLiteral("OBSERVACAO.md"),
            QStringLiteral("MANUAL.md"),
            QStringLiteral("SECURITY.md"),
            QStringLiteral("docs/architecture.md"),
            QStringLiteral("docs/compliance.md"),
            QStringLiteral("docs/licensing.md"),
            QStringLiteral("docs/privacy-policy.md"),
            currentReleaseDoc,
            QStringLiteral("docs/security-model.md"),
            QStringLiteral("docs/suggested-issues.md"),
            QStringLiteral("docs/technical-audit-and-roadmap.md"),
            QStringLiteral("docs/user-guide.md"),
            QStringLiteral("library/Agenda_Avaliacoes_Academicas.iss"),
            QStringLiteral("library/Agenda_Avaliacoes_Academicas_mingw.iss"),
            QStringLiteral("library/Agenda_Avaliacoes_Academicas_msvc.iss"),
            QStringLiteral("library/Agenda_Avaliacoes_Academicas_cpp.txt"),
            QStringLiteral("source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt"),
            QStringLiteral("source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt"),
            QStringLiteral("source/assets/NOTICES/NOTICE_pt_BR.txt"),
            QStringLiteral("source/assets/NOTICES/NOTICE_en_US.txt"),
            QStringLiteral("source/assets/EULA/EULA_pt_BR.txt"),
            QStringLiteral("source/assets/EULA/EULA_en_US.txt"),
            QStringLiteral("source/assets/COPYRIGHT/AVISO DE COPYRIGHT E MARCA REGISTRA_pt_BR.txt"),
            QStringLiteral("source/assets/COPYRIGHT/COPYRIGHT AND TRADEMARK NOTICE_en_US.txt"),
            QStringLiteral("source/assets/CLC/CLC_pt_BR.txt"),
            QStringLiteral("source/assets/CLC/CLC_en_US.txt"),
            QStringLiteral("source/assets/ABOUT/ABOUT_pt_BR.txt"),
            QStringLiteral("source/assets/ABOUT/ABOUT_en_US.txt"),
            QStringLiteral("source/assets/icones/ReviewsManager.ico"),
        };

        for (const QString &rel : trackedFiles) {
            QVERIFY2(copyFileOverwriting(realRoot.filePath(rel), sandbox.filePath(rel)),
                     qPrintable(QStringLiteral("Failed copying %1 into sandbox").arg(rel)));
        }

        VersionEditor editor;
        auto *statusBox = editor.findChild<QTextEdit *>();
        QVERIFY(statusBox != nullptr);

        const QString checkText = statusBox->toPlainText();
        QVERIFY2(checkText.contains(QDir::cleanPath(sandbox.absolutePath())), qPrintable(checkText));
        QVERIFY2(!checkText.contains(QStringLiteral("arquivo ausente")), qPrintable(checkText));
        QVERIFY2(!checkText.contains(QStringLiteral("ausente:\n")), qPrintable(checkText));

        const QStringList expectedKeys = {
            QStringLiteral("privacy_pt"),
            QStringLiteral("privacy_en"),
            QStringLiteral("notice_pt"),
            QStringLiteral("notice_en"),
            QStringLiteral("eula_pt"),
            QStringLiteral("eula_en"),
            QStringLiteral("copyright_pt"),
            QStringLiteral("copyright_en"),
            QStringLiteral("clc_pt"),
            QStringLiteral("clc_en"),
            QStringLiteral("about_pt"),
            QStringLiteral("about_en"),
            QStringLiteral("readme_pt"),
            QStringLiteral("readme_en"),
            QStringLiteral("iss_main"),
            QStringLiteral("iss_mingw"),
            QStringLiteral("iss_msvc"),
            QStringLiteral("agenda_cpp_txt"),
            QStringLiteral("code_of_conduct_md"),
            QStringLiteral("contributing_md"),
            QStringLiteral("observacao_md"),
            QStringLiteral("manual_md"),
            QStringLiteral("security_md"),
            QStringLiteral("docs_architecture_md"),
            QStringLiteral("docs_compliance_md"),
            QStringLiteral("docs_licensing_md"),
            QStringLiteral("docs_privacy_policy_md"),
            QStringLiteral("docs_release_md"),
            QStringLiteral("docs_security_model_md"),
            QStringLiteral("docs_suggested_issues_md"),
            QStringLiteral("docs_technical_audit_md"),
            QStringLiteral("docs_user_guide_md"),
        };
        QCOMPARE(expectedKeys.size(), 32);

        for (const QString &key : expectedKeys) {
            QVERIFY2(checkText.contains(QStringLiteral("%1: arquivo existe").arg(key)), qPrintable(key));
        }

        // Set synthetic version & date on both PT and EN groups
        QList<QLineEdit *> versionEdits;
        for (QLineEdit *le : editor.findChildren<QLineEdit *>()) {
            if (qobject_cast<QGroupBox *>(le->parentWidget()) != nullptr) {
                versionEdits.append(le);
            }
        }
        QCOMPARE(versionEdits.size(), 2);
        versionEdits.at(0)->setText(QStringLiteral("2099.12.31.0"));
        versionEdits.at(1)->setText(QStringLiteral("2099.12.31.0"));

        const QList<QGroupBox *> groups = editor.findChildren<QGroupBox *>();
        QCOMPARE(groups.size(), 2);
        for (QGroupBox *group : groups) {
            const QList<QSpinBox *> spins = group->findChildren<QSpinBox *>();
            QCOMPARE(spins.size(), 2);
            spins.at(0)->setValue(31);
            spins.at(1)->setValue(2099);

            const QList<QComboBox *> combos = group->findChildren<QComboBox *>();
            QCOMPARE(combos.size(), 1);
            combos.at(0)->setCurrentIndex(11);
        }

        QPushButton *saveBtn = findButtonByText(&editor, QStringLiteral("Salvar alterações"));
        QVERIFY(saveBtn != nullptr);
        saveBtn->click();

        const QString saveResult = statusBox->toPlainText();
        QVERIFY2(!saveResult.contains(QStringLiteral("FALHA")), qPrintable(saveResult));
        for (const QString &key : expectedKeys) {
            QVERIFY2(saveResult.contains(QStringLiteral("%1: OK").arg(key)), qPrintable(saveResult));
        }

        const QString updatedReadme = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("README.md")));
        QVERIFY(updatedReadme.contains(QStringLiteral("Versão: v2099.12.31.0")));
        QVERIFY(updatedReadme.contains(QStringLiteral("Version: v2099.12.31.0")));

        for (const QString &issRel : {QStringLiteral("library/Agenda_Avaliacoes_Academicas.iss"),
                                      QStringLiteral("library/Agenda_Avaliacoes_Academicas_mingw.iss"),
                                      QStringLiteral("library/Agenda_Avaliacoes_Academicas_msvc.iss")}) {
            const QString issText = AgendaTestSupport::readUtf8File(sandbox.filePath(issRel));
            QVERIFY2(issText.contains(QStringLiteral("#define MyAppVersion \"2099.12.31.0\"")), qPrintable(issRel));
        }

        const QString cppTxt =
            AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("library/Agenda_Avaliacoes_Academicas_cpp.txt")));
        QVERIFY(cppTxt.contains(QStringLiteral("docs\\release-v2099.12.31.0.md\";")));
        QVERIFY(cppTxt.contains(QStringLiteral("NOTAS DE LANÇAMENTO: Versão 2099.12.31.0\n31 de dezembro de 2099")));
        QVERIFY(cppTxt.contains(QStringLiteral("RELEASE NOTES: Version 2099.12.31.0\nDecember 31, 2099")));

        const QString cocMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("CODE_OF_CONDUCT.md")));
        QVERIFY(cocMd.contains(QStringLiteral("**Versão:** `2099.12.31.0` | **Data:** `31 de dezembro de 2099`")));
        QVERIFY(cocMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));
        QVERIFY(cocMd.contains(QStringLiteral("versão 2.1")));
        QVERIFY(cocMd.contains(QStringLiteral("version 2.1")));

        const QString contribMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("CONTRIBUTING.md")));
        QVERIFY(contribMd.contains(QStringLiteral("**Versão:** `2099.12.31.0` | **Data:** `31 de dezembro de 2099`")));
        QVERIFY(contribMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));

        const QString obsMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("OBSERVACAO.md")));
        QVERIFY(obsMd.contains(QStringLiteral("Versão documentada: `2099.12.31.0`")));
        QVERIFY(obsMd.contains(QStringLiteral("Data do snapshot técnico: `31 de dezembro de 2099`")));
        QVERIFY(obsMd.contains(QStringLiteral("Documented Version: `2099.12.31.0`")));
        QVERIFY(obsMd.contains(QStringLiteral("Technical Snapshot Date: `December 31, 2099`")));
        QVERIFY(obsMd.contains(QStringLiteral("release-v2099.12.31.0.md")));

        const QString manualMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("MANUAL.md")));
        QVERIFY(manualMd.contains(QStringLiteral("**Versão / Version:** `2099.12.31.0`")));
        QVERIFY(manualMd.contains(QStringLiteral("**Data / Date:** `31 de dezembro de 2099` / `December 31, 2099`")));

        const QString secMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("SECURITY.md")));
        QVERIFY(secMd.contains(QStringLiteral("**Versão:** `2099.12.31.0` | **Data:** `31 de dezembro de 2099`")));
        QVERIFY(secMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));
        QCOMPARE(secMd.count(QStringLiteral("| `v2099.12.x` |")), 2);
        QCOMPARE(secMd.count(QStringLiteral("| `< v2099.12` |")), 2);

        const QString archMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/architecture.md")));
        QVERIFY(archMd.contains(QStringLiteral("**Versão:** `2099.12.31.0`")));
        QVERIFY(archMd.contains(QStringLiteral("**Data:** `31 de dezembro de 2099`")));
        QVERIFY(archMd.contains(QStringLiteral("**Version:** `2099.12.31.0`")));
        QVERIFY(archMd.contains(QStringLiteral("**Date:** `December 31, 2099`")));

        const QString compMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/compliance.md")));
        QVERIFY(compMd.contains(QStringLiteral("**Versão:** `2099.12.31.0`")));
        QVERIFY(compMd.contains(QStringLiteral("**Data:** `31 de dezembro de 2099`")));
        QVERIFY(compMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));

        const QString licMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/licensing.md")));
        QVERIFY(licMd.contains(QStringLiteral("**Versão:** `2099.12.31.0`")));
        QVERIFY(licMd.contains(QStringLiteral("**Data:** `31 de dezembro de 2099`")));
        QVERIFY(licMd.contains(QStringLiteral("**Version:** `2099.12.31.0`")));
        QVERIFY(licMd.contains(QStringLiteral("**Date:** `December 31, 2099`")));

        const QString privMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/privacy-policy.md")));
        QVERIFY(privMd.contains(QStringLiteral("**Última atualização:** `31 de dezembro de 2099`")));
        QVERIFY(privMd.contains(QStringLiteral("**Versão:** `2099.12.31.0`")));
        QVERIFY(privMd.contains(QStringLiteral("**Last updated:** `December 31, 2099`")));
        QVERIFY(privMd.contains(QStringLiteral("**Version:** `2099.12.31.0`")));

        QVERIFY(!QFileInfo::exists(sandbox.filePath(currentReleaseDoc)));
        const QString relMd =
            AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/release-v2099.12.31.0.md")));
        QVERIFY(!relMd.isEmpty());
        QVERIFY(relMd.contains(QStringLiteral("v2099.12.31.0")));
        QVERIFY(relMd.contains(QStringLiteral("Versão 2099.12.31.0")));
        QVERIFY(relMd.contains(QStringLiteral("**Data de Lançamento:** 31 de dezembro de 2099")));
        QVERIFY(relMd.contains(QStringLiteral("Version 2099.12.31.0")));
        QVERIFY(relMd.contains(QStringLiteral("**Release Date:** December 31, 2099")));

        const QString secModMd =
            AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/security-model.md")));
        QVERIFY(secModMd.contains(QStringLiteral("**Versão:** `2099.12.31.0` | **Data:** `31 de dezembro de 2099`")));
        QVERIFY(secModMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));

        const QString sugMd =
            AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/suggested-issues.md")));
        QVERIFY(sugMd.contains(QStringLiteral("**Versão:** `2099.12.31.0` | **Data:** `31 de dezembro de 2099`")));
        QVERIFY(sugMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));

        const QString auditMd =
            AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/technical-audit-and-roadmap.md")));
        QVERIFY(auditMd.contains(QStringLiteral("**Versão / Version:** `2099.12.31.0`")));
        QVERIFY(auditMd.contains(QStringLiteral("**Data / Date:** `31 de dezembro de 2099` / `December 31, 2099`")));

        const QString ugMd = AgendaTestSupport::readUtf8File(sandbox.filePath(QStringLiteral("docs/user-guide.md")));
        QVERIFY(ugMd.contains(QStringLiteral("**Versão:** `2099.12.31.0` | **Data:** `31 de dezembro de 2099`")));
        QVERIFY(ugMd.contains(QStringLiteral("**Version:** `2099.12.31.0` | **Date:** `December 31, 2099`")));
    }

    void testCompileTranslationsWindowInSandbox() {
        const QDir realRoot(QString::fromUtf8(AGENDA_REAL_SOURCE_DIR));
        const QDir sandbox(QString::fromUtf8(AGENDA_TRANSLATIONS_SANDBOX_DIR));

        const QStringList tsFiles = {
            QStringLiteral("manager_compression_en_US.ts"),
            QStringLiteral("manager_compression_pt_BR.ts"),
        };
        for (const QString &ts : tsFiles) {
            const QString src = realRoot.filePath(QStringLiteral("source/language/translations/") + ts);
            const QString dst = sandbox.filePath(QStringLiteral("translations/") + ts);
            QVERIFY2(copyFileOverwriting(src, dst), qPrintable(ts));
            QFile::remove(sandbox.filePath(QStringLiteral("translations/") +
                                         ts.left(ts.size() - 3) + QStringLiteral(".qm")));
        }

        CompileTranslationsWindow win;
        auto *listWidget = win.findChild<QListWidget *>();
        QVERIFY(listWidget != nullptr);
        QCOMPARE(listWidget->count(), 2);

        QPushButton *compileAllBtn = findButtonByText(&win, QStringLiteral("Compilar todos"));
        QVERIFY(compileAllBtn != nullptr);
        compileAllBtn->click();

        const QString enQm = sandbox.filePath(QStringLiteral("translations/manager_compression_en_US.qm"));
        const QString ptQm = sandbox.filePath(QStringLiteral("translations/manager_compression_pt_BR.qm"));
        QVERIFY2(QFileInfo::exists(enQm) && QFileInfo(enQm).size() > 0, qPrintable(enQm));
        QVERIFY2(QFileInfo::exists(ptQm) && QFileInfo(ptQm).size() > 0, qPrintable(ptQm));

        QTranslator tr;
        QVERIFY(tr.load(enQm));
        QCOMPARE(tr.translate("InterfaceGerenciadorAtividades", "Curso:"), QStringLiteral("Course:"));
    }

    void testWorkflowPresetsAndVsCodeSuiteIntegration() {
        const QDir repoRoot(QString::fromUtf8(AGENDA_REAL_SOURCE_DIR));

        // 1. Validate README matches .github/workflows/update-readme-version.yml regexes identically
        const QString readme = AgendaTestSupport::readUtf8File(repoRoot.filePath(QStringLiteral("README.md")));
        const QRegularExpression ptRx(QStringLiteral(R"(^(Versão:\s*)(v?\d+\.\d+\.\d+\.\d+)(\s*(?:<br\s*/?>)?\s*)$)"),
                                      QRegularExpression::MultilineOption);
        const QRegularExpression enRx(QStringLiteral(R"(^(Version:\s*)(v?\d+\.\d+\.\d+\.\d+)(\s*(?:<br\s*/?>)?\s*)$)"),
                                      QRegularExpression::MultilineOption);
        const auto ptMatch = ptRx.match(readme);
        const auto enMatch = enRx.match(readme);
        QVERIFY2(ptMatch.hasMatch(), "README.md missing canonical pt_BR version line");
        QVERIFY2(enMatch.hasMatch(), "README.md missing canonical en_US version line");
        QCOMPARE(ptMatch.captured(2), enMatch.captured(2));
        QCOMPARE(ptMatch.captured(2), QStringLiteral("v") + QString::fromUtf8(AGENDA_PROJECT_VERSION));

        // 2. Verify CMakePresets.json integrates suite-msvc & suite-mingw to exact required dirs
        const QByteArray presetsBytes =
            AgendaTestSupport::readUtf8File(repoRoot.filePath(QStringLiteral("CMakePresets.json"))).toUtf8();
        const QJsonObject presetsObj = QJsonDocument::fromJson(presetsBytes).object();
        QVERIFY(!presetsObj.isEmpty());

        bool foundSuiteMsvc = false;
        bool foundSuiteMingw = false;
        for (const QJsonValue &val : presetsObj.value(QStringLiteral("configurePresets")).toArray()) {
            const QJsonObject obj = val.toObject();
            const QString name = obj.value(QStringLiteral("name")).toString();
            const QString binDir = obj.value(QStringLiteral("binaryDir")).toString();
            if (name == QStringLiteral("suite-msvc")) {
                foundSuiteMsvc = true;
                QCOMPARE(binDir, QStringLiteral("${sourceDir}/builds/build_suite_msvc"));
                QCOMPARE(obj.value(QStringLiteral("cacheVariables"))
                             .toObject()
                             .value(QStringLiteral("AGENDA_BUILD_TESTS"))
                             .toString(),
                         QStringLiteral("ON"));
            } else if (name == QStringLiteral("suite-mingw")) {
                foundSuiteMingw = true;
                QCOMPARE(binDir, QStringLiteral("${sourceDir}/builds/build_suite_mingw"));
                QCOMPARE(obj.value(QStringLiteral("cacheVariables"))
                             .toObject()
                             .value(QStringLiteral("AGENDA_BUILD_TESTS"))
                             .toString(),
                         QStringLiteral("ON"));
            }
        }
        QVERIFY2(foundSuiteMsvc, "CMakePresets.json missing suite-msvc preset");
        QVERIFY2(foundSuiteMingw, "CMakePresets.json missing suite-mingw preset");

        // 3. Verify .vscode/tasks.json structure, hidden subtasks, absence of "detail", and CMake task runner
        QVERIFY2(QFileInfo::exists(repoRoot.filePath(QStringLiteral("cmake/AgendaTaskRunner.cmake"))),
                 "Missing cmake/AgendaTaskRunner.cmake");
        QVERIFY2(QFileInfo::exists(repoRoot.filePath(QStringLiteral("cmake/AgendaClearBuilds.cmake"))),
                 "Missing cmake/AgendaClearBuilds.cmake");

        const QByteArray tasksBytes =
            AgendaTestSupport::readUtf8File(repoRoot.filePath(QStringLiteral(".vscode/tasks.json"))).toUtf8();
        const QJsonObject tasksObj = QJsonDocument::fromJson(tasksBytes).object();
        QVERIFY(!tasksObj.isEmpty());
        const QJsonArray tasksArray = tasksObj.value(QStringLiteral("tasks")).toArray();
        QCOMPARE(tasksArray.size(), 10);

        QSet<QString> visibleLabels;
        QSet<QString> hiddenLabels;
        bool foundTaskMsvc = false;
        bool foundTaskMingw = false;
        bool foundMocksMsvc = false;
        bool foundMocksMingw = false;
        for (const QJsonValue &val : tasksArray) {
            const QJsonObject taskObj = val.toObject();
            const QString label = taskObj.value(QStringLiteral("label")).toString();
            QVERIFY2(!taskObj.contains(QStringLiteral("detail")),
                     qPrintable(QStringLiteral("Forbidden 'detail' field in task: %1").arg(label)));

            if (taskObj.value(QStringLiteral("hide")).toBool(false)) {
                hiddenLabels.insert(label);
            } else {
                visibleLabels.insert(label);
            }

            const QJsonArray args = taskObj.value(QStringLiteral("args")).toArray();
            QStringList argStrings;
            for (const QJsonValue &argVal : args) {
                argStrings.append(argVal.toString());
            }
            const QString joinedArgs = argStrings.join(QChar::Space);
            QVERIFY2(!joinedArgs.contains(QStringLiteral("qt_task.ps1")),
                     qPrintable(QStringLiteral("Obsolete qt_task.ps1 reference in task: %1").arg(label)));

            if (label == QStringLiteral("Qt6: Compilar e Executar SUITE (MSVC)")) {
                foundTaskMsvc = true;
                QCOMPARE(taskObj.value(QStringLiteral("command")).toString(), QStringLiteral("cmake"));
                QVERIFY2(joinedArgs.contains(QStringLiteral("cmake/AgendaTaskRunner.cmake")), qPrintable(joinedArgs));
            } else if (label == QStringLiteral("Qt6: Compilar e Executar SUITE (MinGW)")) {
                foundTaskMingw = true;
                QCOMPARE(taskObj.value(QStringLiteral("command")).toString(), QStringLiteral("cmake"));
                QVERIFY2(joinedArgs.contains(QStringLiteral("cmake/AgendaTaskRunner.cmake")), qPrintable(joinedArgs));
            } else if (label == QStringLiteral("Qt6: Compilar e Executar MOCKS (MSVC)")) {
                foundMocksMsvc = true;
                QCOMPARE(taskObj.value(QStringLiteral("command")).toString(), QStringLiteral("cmake"));
                QVERIFY2(joinedArgs.contains(QStringLiteral("-DACTION=RunMocks")), qPrintable(joinedArgs));
                QVERIFY2(joinedArgs.contains(QStringLiteral("cmake/AgendaTaskRunner.cmake")), qPrintable(joinedArgs));
            } else if (label == QStringLiteral("Qt6: Compilar e Executar MOCKS (MinGW)")) {
                foundMocksMingw = true;
                QCOMPARE(taskObj.value(QStringLiteral("command")).toString(), QStringLiteral("cmake"));
                QVERIFY2(joinedArgs.contains(QStringLiteral("-DACTION=RunMocks")), qPrintable(joinedArgs));
                QVERIFY2(joinedArgs.contains(QStringLiteral("cmake/AgendaTaskRunner.cmake")), qPrintable(joinedArgs));
            }
        }
        const QSet<QString> expectedVisible = {
            QStringLiteral("Qt6: Compilar & Executar AGENDA (MSVC)"),
            QStringLiteral("Qt6: Compilar & Executar AGENDA (MinGW)"),
            QStringLiteral("Qt6: Compilar e Executar MOCKS (MSVC)"),
            QStringLiteral("Qt6: Compilar e Executar MOCKS (MinGW)"),
            QStringLiteral("Qt6: Compilar e Executar SUITE (MinGW)"),
            QStringLiteral("Qt6: Compilar e Executar SUITE (MSVC)"),
        };
        const QSet<QString> expectedHidden = {
            QStringLiteral("Qt6: Compilar AGENDA (MSVC)"),
            QStringLiteral("Qt6: Executar AGENDA (MSVC)"),
            QStringLiteral("Qt6: Compilar AGENDA (MinGW)"),
            QStringLiteral("Qt6: Executar AGENDA (MinGW)"),
        };
        QCOMPARE(visibleLabels, expectedVisible);
        QCOMPARE(hiddenLabels, expectedHidden);
        QVERIFY2(foundTaskMsvc, "Missing VS Code task: Qt6: Compilar e Executar SUITE (MSVC)");
        QVERIFY2(foundTaskMingw, "Missing VS Code task: Qt6: Compilar e Executar SUITE (MinGW)");
        QVERIFY2(foundMocksMsvc, "Missing VS Code task: Qt6: Compilar e Executar MOCKS (MSVC)");
        QVERIFY2(foundMocksMingw, "Missing VS Code task: Qt6: Compilar e Executar MOCKS (MinGW)");

        // 4. Verify configured build does NOT create external distribution inside test build tree
        const QDir testBinaryDir(QCoreApplication::applicationDirPath());
        QVERIFY2(!testBinaryDir.absolutePath().contains(QStringLiteral("MISCELANEAS"), Qt::CaseInsensitive),
                 "Test binaries must NOT reside in external distribution directory");
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestMocksAndDeploymentIntegrity tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_09_MocksAndDeploymentIntegrity.moc"
