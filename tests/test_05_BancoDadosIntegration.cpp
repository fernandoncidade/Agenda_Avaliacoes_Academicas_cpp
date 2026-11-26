#include "TestSupport.hpp"
#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/BancoDeDados/Listas_Personalizadas.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtTest>

class TestBancoDadosIntegration : public QObject {
    Q_OBJECT

private:
    AgendaTestSupport::IsolatedLocalAppDataGuard m_envGuard;

    static void writeLanguageJson(const QString &idioma) {
        QFile file(QDir(obter_caminho_persistente()).filePath(QStringLiteral("language.json")));
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
        QJsonObject obj;
        obj.insert(QStringLiteral("idioma"), idioma);
        file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }

private slots:
    void testMesclarListsAndDefaultLanguage() {
        QCOMPARE(obter_idioma_atual(), QStringLiteral("pt_BR"));

        const QStringList merged = _mesclar_listas(
            {{QStringLiteral(""), QStringLiteral("A"), QStringLiteral("b")},
             {QStringLiteral(" a "), QStringLiteral("B"), QStringLiteral("C")}});
        QCOMPARE(merged, QStringList({QStringLiteral(""), QStringLiteral("A"), QStringLiteral("b"), QStringLiteral("C")}));
    }

    void testSelectiveCustomRegistrationAndCascadeMerging() {
        writeLanguageJson(QStringLiteral("pt_BR"));

        // With registrar_curso = false, custom course must NOT be inserted into DB
        const auto partial = registrar_valores_personalizados(
            QStringLiteral("Engenharia Civil"), QStringLiteral("Ementa - 2023"), QStringLiteral("1º Semestre"),
            QStringLiteral("Disciplina Nova – NX900"), QStringLiteral("TURMA-ESP"), QStringLiteral("PROVA"),
            QStringLiteral("1"), false, false, false, true, true, false, false);
        QCOMPARE(partial.size(), 2);
        QVERIFY(partial.value(QStringLiteral("disciplina")));
        QVERIFY(partial.value(QStringLiteral("turma")));
        QVERIFY(!listar_cursos(QStringLiteral("pt_BR")).contains(QStringLiteral("Engenharia Civil")));

        const QStringList ementas = obter_ementas(QStringLiteral("Engenharia Civil"));
        QVERIFY(ementas.contains(QStringLiteral("Ementa - 2023")));

        const QStringList semestres = obter_semestres(QStringLiteral("Engenharia Civil"), QStringLiteral("Ementa - 2023"));
        QVERIFY(semestres.contains(QStringLiteral("1º Semestre")));

        const QStringList disciplinas =
            obter_disciplinas(QStringLiteral("Engenharia Civil"), QStringLiteral("Ementa - 2023"),
                              QStringLiteral("1º Semestre"));
        QVERIFY(disciplinas.contains(QStringLiteral("Disciplina Nova – NX900")));
        QVERIFY(obter_turmas().contains(QStringLiteral("TURMA-ESP")));
        QVERIFY(obter_avaliacoes().contains(QStringLiteral("PROVA")));
        QVERIFY(obter_sequencias().contains(QStringLiteral("10")));

        // Custom-only hierarchy resolution
        registrar_valores_personalizados(
            QStringLiteral("Arquitetura"), QStringLiteral("Matriz 2027"), QStringLiteral("2º Período"),
            QStringLiteral("Projeto Urbano I"), QStringLiteral("U1"), QStringLiteral("PORTFÓLIO"), QStringLiteral("S1"),
            true, true, true, true, true, true, true);
        QVERIFY(obter_cursos().contains(QStringLiteral("Arquitetura")));
        QCOMPARE(obter_ementas(QStringLiteral("Arquitetura")), QStringList({QStringLiteral("Matriz 2027")}));
        QCOMPARE(obter_semestres(QStringLiteral("Arquitetura"), QStringLiteral("Matriz 2027")),
                 QStringList({QStringLiteral("2º Período")}));
        QCOMPARE(obter_disciplinas(QStringLiteral("Arquitetura"), QStringLiteral("Matriz 2027"),
                                   QStringLiteral("2º Período")),
                 QStringList({QStringLiteral("Projeto Urbano I")}));
        QVERIFY(obter_sequencias().contains(QStringLiteral("S1")));

        atualizar_listas_exportadas();
        QVERIFY(lista_cursos().contains(QStringLiteral("Arquitetura")));
        QVERIFY(lista_turmas().contains(QStringLiteral("U1")));
        QVERIFY(lista_avaliacoes().contains(QStringLiteral("PORTFÓLIO")));
        QVERIFY(lista_sequencias().contains(QStringLiteral("S1")));
    }

    void testEnglishLanguageResolution() {
        writeLanguageJson(QStringLiteral("en_US"));
        QCOMPARE(obter_idioma_atual(), QStringLiteral("en_US"));
        QVERIFY(obter_cursos().contains(QStringLiteral("Civil Engineering")));
        const QStringList enSyllabi = obter_ementas(QStringLiteral("Civil Engineering"));
        QVERIFY(!enSyllabi.isEmpty());
        const QStringList enSemesters = obter_semestres(QStringLiteral("Civil Engineering"), enSyllabi.first());
        QVERIFY(!enSemesters.isEmpty());
        QVERIFY(!obter_disciplinas(QStringLiteral("Civil Engineering"), enSyllabi.first(), enSemesters.first()).isEmpty());
        QVERIFY(obter_avaliacoes().contains(QStringLiteral("EXAM")));

        atualizar_listas_exportadas();
        QVERIFY(lista_cursos().contains(QStringLiteral("Civil Engineering")));
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard earlyGuard;
    QApplication app(argc, argv);
    TestBancoDadosIntegration tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_05_BancoDadosIntegration.moc"
