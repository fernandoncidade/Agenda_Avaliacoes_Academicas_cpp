#include "TestSupport.hpp"
#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"

#include <QApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextBrowser>
#include <QtTest>

class TestActivityManagerCRUD : public QObject {
    Q_OBJECT

private:
    AgendaTestSupport::IsolatedLocalAppDataGuard m_envGuard;

private slots:
    void initTestCase() {
        QVERIFY(m_envGuard.isValid());
    }

    void testKeyAndLineFormatting() {
        const Atividade a{QStringLiteral("20/07/2026"), QStringLiteral("PROVA"), QStringLiteral("1"),
                          QStringLiteral("Cálculo Diferencial e Integral I – CM201"), QStringLiteral("A")};
        QCOMPARE(atividade_para_linha(a),
                 QStringLiteral("20/07/2026 PROVA – 1 Cálculo Diferencial e Integral I – CM201 A"));
        QVERIFY(chave_atividade(a).contains(QChar(0x1f)));
    }

    void testCrudLifecycleAndPersistenceAcrossInstances() {
        const Atividade first{QStringLiteral("15/08/2026"), QStringLiteral("PROVA"), QStringLiteral("1"),
                              QStringLiteral("Cálculo Diferencial e Integral I – CM201"), QStringLiteral("T1")};
        const Atividade updated{QStringLiteral("22/08/2026"), QStringLiteral("TRABALHO"), QStringLiteral("2"),
                                QStringLiteral("Cálculo Diferencial e Integral I – CM201"), QStringLiteral("T2")};
        const Atividade missing{QStringLiteral("01/01/2099"), QStringLiteral("EXAME"), QStringLiteral("1"),
                                QStringLiteral("Nada"), QStringLiteral("Z")};

        {
            GerenciamentoAtividades mgr;
            mgr.limpar_entradas();
            QVERIFY(mgr.listar_atividades().isEmpty());

            mgr.adicionar_atividade(first);
            const auto found = mgr.buscar_atividade(first);
            QVERIFY(found.has_value());
            QCOMPARE(found->turma, QStringLiteral("T1"));
            QVERIFY(!mgr.buscar_atividade(missing).has_value());

            QVERIFY(!mgr.atualizar_atividade(missing, updated));
            QVERIFY(mgr.atualizar_atividade(first, updated));
            QVERIFY(!mgr.buscar_atividade(first).has_value());
            QVERIFY(mgr.buscar_atividade(updated).has_value());
        }

        // Re-open in fresh instance -> verifies real SQLite durability in atividades.db
        {
            GerenciamentoAtividades reloaded;
            const QList<Atividade> list = reloaded.listar_atividades();
            QCOMPARE(list.size(), 1);
            QCOMPARE(list.first().data, QStringLiteral("22/08/2026"));
            QCOMPARE(list.first().tipo, QStringLiteral("TRABALHO"));
            QCOMPARE(list.first().turma, QStringLiteral("T2"));

            reloaded.remover_atividade(updated);
            QVERIFY(reloaded.listar_atividades().isEmpty());
        }
    }

    void testColorModesSortingAndHtmlXssSafety() {
        GerenciamentoAtividades mgr;
        mgr.limpar_entradas();

        QCOMPARE(mgr.obter_modo_cores(), QStringLiteral("coloridas"));
        QVERIFY(!mgr.definir_modo_cores(QStringLiteral("azul_invalido")));
        QCOMPARE(mgr.obter_modo_cores(), QStringLiteral("coloridas"));

        QVERIFY(mgr.definir_modo_cores(QStringLiteral("preto")));
        QCOMPARE(mgr.obter_modo_cores(), QStringLiteral("preto"));
        QVERIFY(AgendaTestSupport::isValidHexColor(mgr.cor_texto_do_sistema()));

        // Verify config.json saved and reloaded cleanly by another instance
        {
            GerenciamentoAtividades checkConfig;
            QCOMPARE(checkConfig.obter_modo_cores(), QStringLiteral("preto"));
            QVERIFY(checkConfig.definir_modo_cores(QStringLiteral("coloridas")));
        }

        QTextBrowser browser;
        mgr.textbox = &browser;
        QVERIFY(mgr.definir_modo_cores(QStringLiteral("coloridas")));

        // Insert out-of-order dates plus XSS/HTML metacharacters
        mgr.adicionar_atividade({QStringLiteral("05/12/2026"), QStringLiteral("EXAME"), QStringLiteral("1"),
                                 QStringLiteral("Engenharia Social – TH045"), QStringLiteral("B")});
        mgr.adicionar_atividade({QStringLiteral("10/03/2026"), QStringLiteral("PROVA"), QStringLiteral("2"),
                                 QStringLiteral("Sub <script>alert('xss')</script> & <b>bold</b>"),
                                 QStringLiteral("A\"&T")});
        mgr.adicionar_atividade({QStringLiteral("01/03/2026"), QStringLiteral("TESTE"), QStringLiteral("1"),
                                 QStringLiteral("Cálculo Diferencial e Integral I – CM201"), QStringLiteral("A")});

        mgr.carregar_dados();

        const QString plain = browser.toPlainText();
        const QString html = browser.toHtml();

        // Chronological order: 01/03/2026 < 10/03/2026 < 05/12/2026
        const qsizetype p1 = plain.indexOf(QStringLiteral("01/03/2026"));
        const qsizetype p2 = plain.indexOf(QStringLiteral("10/03/2026"));
        const qsizetype p3 = plain.indexOf(QStringLiteral("05/12/2026"));
        QVERIFY(p1 >= 0 && p2 > p1 && p3 > p2);

        // XSS payload must be HTML-escaped in raw HTML and readable verbatim in plain text
        QVERIFY(!html.contains(QStringLiteral("<script>alert")));
        QVERIFY(html.contains(QStringLiteral("&lt;script&gt;")));
        QVERIFY(plain.contains(QStringLiteral("<script>alert('xss')</script>")));

        // Colorful mode applies CM201 mapped color; monochrome removes mapped color spans
        const QString cm201Color = obter_cor_por_nome(QStringLiteral("CM201"));
        QVERIFY(html.contains(cm201Color, Qt::CaseInsensitive));

        QVERIFY(mgr.definir_modo_cores(QStringLiteral("preto")));
        mgr.update_textbox();
        QVERIFY(!browser.toHtml().contains(cm201Color, Qt::CaseInsensitive));

        mgr.limpar_entradas();
        QVERIFY(browser.toPlainText().isEmpty());
    }

    void testBulkScaleAndSortBenchmark() {
        GerenciamentoAtividades mgr;
        QTextBrowser browser;
        mgr.textbox = &browser;
        mgr.limpar_entradas();

        constexpr int kCount = 250;
        QElapsedTimer timer;
        timer.start();

        for (int i = 0; i < kCount; ++i) {
            const int day = (i % 28) + 1;
            const int month = (i % 12) + 1;
            const QString dateStr = QStringLiteral("%1/%2/2026").arg(day, 2, 10, QLatin1Char('0')).arg(month, 2, 10, QLatin1Char('0'));
            mgr.adicionar_atividade({dateStr, QStringLiteral("PROVA"), QString::number((i % 10) + 1),
                                     QStringLiteral("Cálculo Diferencial e Integral I – CM201 #%1").arg(i),
                                     QStringLiteral("T1")});
        }

        const QList<Atividade> listed = mgr.listar_atividades();
        QCOMPARE(listed.size(), kCount);
        mgr.update_textbox();
        QVERIFY(!browser.toPlainText().isEmpty());

        const qint64 elapsedMs = timer.elapsed();
        QVERIFY2(elapsedMs < 10000, qPrintable(QStringLiteral("Bulk 250-item operation took %1 ms").arg(elapsedMs)));

        mgr.limpar_entradas();
        QVERIFY(mgr.listar_atividades().isEmpty());
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestActivityManagerCRUD tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_06_ActivityManagerCRUD.moc"
