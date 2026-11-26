#include "TestSupport.hpp"
#include "source/BancoDeDados/Listas_Personalizadas.hpp"

#include <QApplication>
#include <QFileInfo>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUuid>
#include <QtTest>

class TestCustomListsDatabase : public QObject {
    Q_OBJECT

private:
    AgendaTestSupport::IsolatedLocalAppDataGuard m_envGuard;

private slots:
    void initTestCase() {
        QVERIFY(m_envGuard.isValid());
    }

    void testTextNormalizationHelpersAndSchemaCreation() {
        QCOMPARE(_texto(QStringLiteral("  Matemática  ")), QStringLiteral("Matemática"));
        QCOMPARE(_normalizar(QStringLiteral("  ÁLGEBRA Linear  ")), QStringLiteral("álgebra linear"));

        const QString dbPath = obter_caminho_listas_db();
        QVERIFY(dbPath.endsWith(QStringLiteral("listas.db")));

        criar_tabelas_listas();
        QVERIFY(QFileInfo::exists(dbPath));

        const QString conn = QStringLiteral("verify_schema_") + QUuid::createUuid().toString(QUuid::WithoutBraces);
        QSet<QString> tables;
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), conn);
            db.setDatabaseName(dbPath);
            QVERIFY(db.open());
            QSqlQuery query(db);
            QVERIFY(query.exec(QStringLiteral("SELECT name FROM sqlite_master WHERE type='table'")));
            while (query.next()) {
                tables.insert(query.value(0).toString());
            }
            db.close();
        }
        QSqlDatabase::removeDatabase(conn);

        for (const QString &expected :
             {QStringLiteral("lista_cursos"), QStringLiteral("lista_ementas"), QStringLiteral("lista_semestres"),
              QStringLiteral("lista_disciplinas"), QStringLiteral("lista_turmas"),
              QStringLiteral("lista_tipos_atividade"), QStringLiteral("lista_sequencias_atividade")}) {
            QVERIFY2(tables.contains(expected), qPrintable(QStringLiteral("Missing SQLite table: %1").arg(expected)));
        }
    }

    void testCrudDeduplicationAndLanguageIsolation() {
        // Empty rejections
        QVERIFY(!inserir_curso(QStringLiteral("pt_BR"), QStringLiteral("   ")));
        QVERIFY(!inserir_turma(QStringLiteral("pt_BR"), QString()));
        QVERIFY(!inserir_tipo_atividade(QStringLiteral("pt_BR"), QStringLiteral("")));
        QVERIFY(!inserir_sequencia_atividade(QStringLiteral("pt_BR"), QStringLiteral(" ")));
        QVERIFY(!inserir_ementa(QStringLiteral("pt_BR"), QString(), QStringLiteral("E1")));
        QVERIFY(!inserir_semestre(QStringLiteral("pt_BR"), QStringLiteral("C1"), QString(), QStringLiteral("S1")));
        QVERIFY(!inserir_disciplina(QStringLiteral("pt_BR"), QStringLiteral("C1"), QStringLiteral("E1"), QString(),
                                    QStringLiteral("D1")));

        // Batch registration & case-insensitive dedup
        const auto firstBatch = registrar_listas_personalizadas(
            QStringLiteral("pt_BR"), QStringLiteral("Ciência da Computação"), QStringLiteral("Currículo 2026"),
            QStringLiteral("3º Período"), QStringLiteral("Compiladores – CC301"), QStringLiteral("Z9"),
            QStringLiteral("HACKATHON"), QStringLiteral("ÚNICA"));
        for (auto it = firstBatch.cbegin(); it != firstBatch.cend(); ++it) {
            QVERIFY2(it.value(), qPrintable(QStringLiteral("Expected insert true for %1").arg(it.key())));
        }

        // Duplicate differing only in case & whitespace must be ignored
        QVERIFY(!inserir_curso(QStringLiteral("pt_BR"), QStringLiteral("  ciência da computação ")));
        QVERIFY(!inserir_ementa(QStringLiteral("pt_BR"), QStringLiteral("CIÊNCIA DA COMPUTAÇÃO"),
                                QStringLiteral("currículo 2026")));
        QVERIFY(!inserir_semestre(QStringLiteral("pt_BR"), QStringLiteral("Ciência da Computação"),
                                  QStringLiteral("currículo 2026"), QStringLiteral("3º período")));
        QVERIFY(!inserir_disciplina(QStringLiteral("pt_BR"), QStringLiteral("Ciência da Computação"),
                                    QStringLiteral("Currículo 2026"), QStringLiteral("3º Período"),
                                    QStringLiteral("compiladores – cc301")));

        // Verify pt_BR listings contain our items while en_US stays completely isolated
        QCOMPARE(listar_cursos(QStringLiteral("pt_BR")), QStringList({QStringLiteral("Ciência da Computação")}));
        QVERIFY(listar_cursos(QStringLiteral("en_US")).isEmpty());
        QCOMPARE(listar_ementas(QStringLiteral("pt_BR"), QStringLiteral("ciência da computação")),
                 QStringList({QStringLiteral("Currículo 2026")}));
        QCOMPARE(listar_semestres(QStringLiteral("pt_BR"), QStringLiteral("ciência da computação"),
                                  QStringLiteral("currículo 2026")),
                 QStringList({QStringLiteral("3º Período")}));
        QCOMPARE(listar_disciplinas(QStringLiteral("pt_BR"), QStringLiteral("ciência da computação"),
                                    QStringLiteral("currículo 2026"), QStringLiteral("3º período")),
                 QStringList({QStringLiteral("Compiladores – CC301")}));
        QVERIFY(listar_disciplinas(QStringLiteral("en_US"), QStringLiteral("Ciência da Computação"),
                                   QStringLiteral("Currículo 2026"), QStringLiteral("3º Período"))
                    .isEmpty());
    }

    void testDeletionFormattingAndSqlInjectionResilience() {
        // Adversarial SQL-injection-like string must be persisted literally via parameterized queries
        const QString hostile = QStringLiteral("Curso'; DROP TABLE lista_cursos; -- \"ÁÉÍÓÚ\"");
        QVERIFY(inserir_curso(QStringLiteral("pt_BR"), hostile));
        QVERIFY(listar_cursos(QStringLiteral("pt_BR")).contains(hostile));

        const auto discItems =
            listar_itens_personalizados_para_exclusao(QStringLiteral("disciplina"), QStringLiteral("pt_BR"));
        QCOMPARE(discItems.size(), 1);
        QCOMPARE(discItems.first().texto,
                 QStringLiteral("Ciência da Computação > Currículo 2026 > 3º Período > Compiladores – CC301"));

        QVERIFY(!remover_item_personalizado(QStringLiteral("categoria_invalida"), 1));
        QVERIFY(!remover_item_personalizado(QStringLiteral("disciplina"), 999999));
        QVERIFY(remover_item_personalizado(QStringLiteral("disciplina"), discItems.first().id));
        QVERIFY(listar_itens_personalizados_para_exclusao(QStringLiteral("disciplina"), QStringLiteral("pt_BR")).isEmpty());
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestCustomListsDatabase tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_04_CustomListsDatabase.moc"
