#include "TestSupport.hpp"
#include "source/BancoDeDados/Listas_Personalizadas.hpp"
#include "source/GerenciamentoInterfaceGrafica/GerenciadorInterfaceGrafica/ginter_01_SobreDialog.hpp"
#include "source/GerenciamentoInterfaceGrafica/GerenciadorInterfaceGrafica/ginter_12_excluir_item_lista_personalizada.hpp"
#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QPushButton>
#include <QTabWidget>
#include <QTemporaryDir>
#include <QTextBrowser>
#include <QTextDocument>
#include <QtTest>

class TestGuiMainWindowAndWorkflows : public QObject {
    Q_OBJECT

private:
    AgendaTestSupport::IsolatedLocalAppDataGuard m_envGuard;

private slots:
    void initTestCase() {
        QVERIFY(m_envGuard.isValid());
    }

    void testMainWindowConstructionCascadingAndRetranslation() {
        InterfaceGerenciadorAtividades window;
        QVERIFY(window.combo_curso != nullptr);
        QVERIFY(window.entry_ementa != nullptr);
        QVERIFY(window.entry_semestre != nullptr);
        QVERIFY(window.entry_disciplina != nullptr);
        QVERIFY(window.entry_codigo != nullptr);
        QVERIFY(window.combo_tipo != nullptr);
        QVERIFY(window.combo_sequencia != nullptr);
        QVERIFY(window.calendar != nullptr);
        QVERIFY(window.textbox != nullptr);
        QVERIFY(window.button_submiter != nullptr);
        QVERIFY(window.button_export != nullptr);
        QVERIFY(window.menu_arquivo != nullptr);
        QVERIFY(window.menu_config != nullptr);
        QVERIFY(window.menu_idiomas != nullptr);
        QVERIFY(window.menu_excluir_item_listas != nullptr);
        QVERIFY(window.menu_cores != nullptr);
        QVERIFY(window.menu_sobre != nullptr);
        QCOMPARE(window.delete_list_menus.size(), 7);

        // Exercise cascading curriculum selection in pt_BR
        window.combo_curso->setCurrentText(QStringLiteral("Engenharia Civil"));
        QVERIFY(window.entry_ementa->count() > 1);
        window.entry_ementa->setCurrentText(QStringLiteral("Ementa - 2011"));
        QVERIFY(window.entry_semestre->count() > 1);
        window.entry_semestre->setCurrentText(QStringLiteral("7º Semestre"));
        QVERIFY(window.entry_disciplina->count() > 1);
        QVERIFY(window.entry_disciplina->findText(QStringLiteral("Engenharia Social – TH045")) >= 0);

        // Live language switch -> English
        window.change_language(QStringLiteral("en_US"));
        QCOMPARE(window.windowTitle(), QStringLiteral("Assessment Activities Registration"));
        QCOMPARE(window.label_curso->text(), QStringLiteral("Course:"));
        QCOMPARE(window.button_submiter->text(), QStringLiteral("Register Entries"));
        QCOMPARE(window.calendar->locale().language(), QLocale::English);
        QVERIFY(window.lang_actions.value(QStringLiteral("en_US"))->isChecked());

        // Live color mode switch
        window.change_color_mode(QStringLiteral("preto"));
        QVERIFY(window.color_actions.value(QStringLiteral("preto"))->isChecked());
        QCOMPARE(window.gerenciamento_atividades.obter_modo_cores(), QStringLiteral("preto"));
        window.change_color_mode(QStringLiteral("coloridas"));
        QVERIFY(window.color_actions.value(QStringLiteral("coloridas"))->isChecked());

        // Live language switch -> back to Portuguese
        window.change_language(QStringLiteral("pt_BR"));
        QCOMPARE(window.label_curso->text(), QStringLiteral("Curso:"));
        QCOMPARE(window.calendar->locale().language(), QLocale::Portuguese);

        window.center_on_screen();
        window.show();
        QTest::qWait(20);
        window.close();
    }

    void testSubmitValidationModalAndFullWorkflows() {
        InterfaceGerenciadorAtividades window;
        window.gerenciamento_atividades.limpar_entradas();

        // 1. Validation warning when required fields are empty
        window.combo_tipo->setCurrentText(QString());
        window.combo_sequencia->setCurrentText(QString());
        window.entry_disciplina->setCurrentText(QString());
        window.entry_codigo->setCurrentText(QString());
        bool warningHandled = false;
        AgendaTestSupport::scheduleModalDialogAction([&](QWidget *modal) {
            if (auto *box = qobject_cast<QMessageBox *>(modal)) {
                warningHandled = true;
                box->accept();
                return true;
            }
            return false;
        });
        window.button_submiter->click();
        QTRY_VERIFY_WITH_TIMEOUT(warningHandled, 3000);
        QVERIFY(window.gerenciamento_atividades.listar_atividades().isEmpty());

        // 2. Full custom registration -> persists to both atividades.db and listas.db
        window.combo_curso->setCurrentText(QStringLiteral("Engenharia de Software"));
        window.entry_ementa->setCurrentText(QStringLiteral("Grade 2026"));
        window.entry_semestre->setCurrentText(QStringLiteral("5º Período"));
        window.entry_disciplina->setCurrentText(QStringLiteral("Qualidade de Software – QS501"));
        window.entry_codigo->setCurrentText(QStringLiteral("ES-A"));
        window.combo_tipo->setCurrentText(QStringLiteral("PROJETO"));
        window.combo_sequencia->setCurrentText(QStringLiteral("1"));
        window.calendar->setSelectedDate(QDate(2026, 9, 10));
        window.button_submiter->click();

        // Register a second activity on an earlier date to verify chronological sorting
        window.calendar->setSelectedDate(QDate(2026, 8, 5));
        window.combo_sequencia->setCurrentText(QStringLiteral("2"));
        window.button_submiter->click();

        const auto list = window.gerenciamento_atividades.listar_atividades();
        QCOMPARE(list.size(), 2);
        QVERIFY(window.textbox->toPlainText().indexOf(QStringLiteral("05/08/2026")) <
                window.textbox->toPlainText().indexOf(QStringLiteral("10/09/2026")));
        QVERIFY(listar_cursos(QStringLiteral("pt_BR")).contains(QStringLiteral("Engenharia de Software")));

        // 3. Test custom item deletion menu (Settings > Delete Item -> Curso)
        QMenu *cursoMenu = window.delete_list_menus.value(QStringLiteral("curso"));
        QVERIFY(cursoMenu != nullptr);
        popular_menu_itens_personalizados(&window, cursoMenu, QStringLiteral("curso"));
        QAction *customCursoAction = nullptr;
        for (QAction *act : cursoMenu->actions()) {
            if (act->text() == QStringLiteral("Engenharia de Software")) {
                customCursoAction = act;
                break;
            }
        }
        QVERIFY(customCursoAction != nullptr);

        bool confirmHandled = false;
        bool infoHandled = false;
        AgendaTestSupport::scheduleModalDialogAction([&](QWidget *modal) {
            if (auto *box = qobject_cast<QMessageBox *>(modal)) {
                if (!confirmHandled && box->button(QMessageBox::Yes)) {
                    confirmHandled = true;
                    box->button(QMessageBox::Yes)->click();
                    return false; // keep watching for the following information QMessageBox
                }
                if (confirmHandled && box->button(QMessageBox::Ok)) {
                    infoHandled = true;
                    box->button(QMessageBox::Ok)->click();
                    return true;
                }
            }
            return false;
        });
        customCursoAction->trigger();
        QTRY_VERIFY_WITH_TIMEOUT(confirmHandled && infoHandled, 4000);
        QVERIFY(!listar_cursos(QStringLiteral("pt_BR")).contains(QStringLiteral("Engenharia de Software")));

        // 4. Test Editar Item(ns) -> nested Editar Detalhes da Atividade modal dialogs
        bool editListHandled = false;
        bool editDetailsHandled = false;
        AgendaTestSupport::scheduleModalDialogAction([&](QWidget *modal) {
            if (auto *dlg = qobject_cast<QDialog *>(modal)) {
                if (!editListHandled && dlg->windowTitle().contains(QStringLiteral("Editar Item"))) {
                    auto checks = dlg->findChildren<QCheckBox *>();
                    if (!checks.isEmpty()) {
                        checks.first()->setChecked(true);
                    }
                    auto *btns = dlg->findChild<QDialogButtonBox *>();
                    if (btns && btns->button(QDialogButtonBox::Ok)) {
                        editListHandled = true;
                        btns->button(QDialogButtonBox::Ok)->click();
                    }
                    return false;
                }
                if (editListHandled && !editDetailsHandled &&
                    dlg->windowTitle().contains(QStringLiteral("Editar Detalhes"))) {
                    auto combos = dlg->findChildren<QComboBox *>();
                    // 4th QComboBox is entryTurma in editar_detalhes_atividade
                    if (combos.size() >= 4) {
                        combos.at(3)->setCurrentText(QStringLiteral("EDITADA-Z"));
                    }
                    auto *btns = dlg->findChild<QDialogButtonBox *>();
                    if (btns && btns->button(QDialogButtonBox::Ok)) {
                        editDetailsHandled = true;
                        btns->button(QDialogButtonBox::Ok)->click();
                        return true;
                    }
                }
            }
            return false;
        });
        window.button_editar_item->click();
        QTRY_VERIFY_WITH_TIMEOUT(editListHandled && editDetailsHandled, 4000);

        bool foundEditedClass = false;
        for (const Atividade &item : window.gerenciamento_atividades.listar_atividades()) {
            if (item.turma == QStringLiteral("EDITADA-Z")) {
                foundEditedClass = true;
                break;
            }
        }
        QVERIFY2(foundEditedClass, "Edited class EDITADA-Z must persist in SQLite");

        // 5. Test Excluir Item(ns) modal dialog
        bool deleteDialogHandled = false;
        AgendaTestSupport::scheduleModalDialogAction([&](QWidget *modal) {
            if (auto *dlg = qobject_cast<QDialog *>(modal)) {
                if (dlg->windowTitle().contains(QStringLiteral("Excluir Item"))) {
                    auto checks = dlg->findChildren<QCheckBox *>();
                    if (!checks.isEmpty()) {
                        checks.first()->setChecked(true);
                    }
                    auto *btns = dlg->findChild<QDialogButtonBox *>();
                    if (btns && btns->button(QDialogButtonBox::Ok)) {
                        deleteDialogHandled = true;
                        btns->button(QDialogButtonBox::Ok)->click();
                        return true;
                    }
                }
            }
            return false;
        });
        window.button_clear_item->click();
        QTRY_VERIFY_WITH_TIMEOUT(deleteDialogHandled, 4000);
        QCOMPARE(window.gerenciamento_atividades.listar_atividades().size(), 1);

        // 6. Test Limpar Última Entrada
        bool clearLastHandled = false;
        AgendaTestSupport::scheduleModalDialogAction([&](QWidget *modal) {
            if (auto *box = qobject_cast<QMessageBox *>(modal)) {
                clearLastHandled = true;
                box->accept();
                return true;
            }
            return false;
        });
        window.button_clear_ultima->click();
        QTRY_VERIFY_WITH_TIMEOUT(clearLastHandled, 3000);
        QVERIFY(window.gerenciamento_atividades.listar_atividades().isEmpty());
    }

    void testSobreDialogAndPdfPipeline() {
        SobreDialogTextos texts{
            QStringLiteral("Sobre Teste"),     QStringLiteral("Cabeçalho"),    QStringLiteral("Linha 1\n- Item"),
            QStringLiteral("Detalhes texto"),  QStringLiteral("Licença AGPL"), QStringLiteral("https://example.org"),
            QStringLiteral("Avisos texto"),    QStringLiteral("Privacidade"),  QStringLiteral("Release 2026.7.20.0"),
            QStringLiteral("Histórico"),       QStringLiteral("Ocultar hist"), QStringLiteral("Detalhes"),
            QStringLiteral("Ocultar det"),     QStringLiteral("Licenças"),     QStringLiteral("Ocultar lic"),
            QStringLiteral("Avisos"),          QStringLiteral("Ocultar av"),   QStringLiteral("Privacidade"),
            QStringLiteral("Ocultar priv"),    QStringLiteral("Notas"),        QStringLiteral("Ocultar notas"),
            QStringLiteral("OK"),              QStringLiteral("Site oficial"), QStringLiteral("Indisponível")};
        SobreDialog dlg(texts);
        auto *tabs = dlg.findChild<QTabWidget *>();
        QVERIFY(tabs != nullptr);
        QCOMPARE(tabs->count(), 6);
        QCOMPARE(tabs->tabText(0), QStringLiteral("Ocultar hist"));
        tabs->setCurrentIndex(2);
        QCOMPARE(tabs->tabText(0), QStringLiteral("Histórico"));
        QCOMPARE(tabs->tabText(2), QStringLiteral("Ocultar lic"));
        dlg.retranslate_ui(texts);

        // Exercise action_sobre -> exibir_sobre() modally
        InterfaceGerenciadorAtividades window;
        bool aboutHandled = false;
        AgendaTestSupport::scheduleModalDialogAction([&](QWidget *modal) {
            if (auto *sobre = qobject_cast<SobreDialog *>(modal)) {
                if (auto *tabWidget = sobre->findChild<QTabWidget *>()) {
                    tabWidget->setCurrentIndex(4);
                }
                aboutHandled = true;
                sobre->accept();
                return true;
            }
            return false;
        });
        window.action_sobre->trigger();
        QTRY_VERIFY_WITH_TIMEOUT(aboutHandled, 4000);

        // Exercise QPdfWriter + QTextDocument pipeline matching gativ_10_exportar_para_pdf.cpp
        window.gerenciamento_atividades.adicionar_atividade(
            {QStringLiteral("20/07/2026"), QStringLiteral("PROVA"), QStringLiteral("1"),
             QStringLiteral("Cálculo Diferencial e Integral I – CM201"), QStringLiteral("A")});
        window.gerenciamento_atividades.update_textbox();

        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());
        const QString pdfPath = tempDir.filePath(QStringLiteral("agenda_export_test.pdf"));
        {
            QPdfWriter writer(pdfPath);
            writer.setPageSize(QPageSize(QPageSize::Letter));
            writer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);
            QTextDocument doc;
            doc.setHtml(window.textbox->toHtml());
            doc.print(&writer);
        }
        QFile pdfFile(pdfPath);
        QVERIFY(pdfFile.exists());
        QVERIFY(pdfFile.open(QIODevice::ReadOnly));
        const QByteArray rawPdf = pdfFile.readAll();
        QVERIFY(rawPdf.startsWith("%PDF-"));
        QVERIFY(rawPdf.contains("%%EOF"));
        QVERIFY(rawPdf.size() > 1200);

        window.gerenciamento_atividades.limpar_entradas();
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestGuiMainWindowAndWorkflows tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_08_GuiMainWindowAndWorkflows.moc"
