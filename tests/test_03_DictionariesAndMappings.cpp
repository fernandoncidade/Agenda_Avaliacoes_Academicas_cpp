#include "TestSupport.hpp"
#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/CURSOS_EN.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/CURSOS_PT.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/dicionario_de_cores.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/dicionario_disciplinas.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/lista_avaliacoes_en_US.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/lista_avaliacoes_pt_BR.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/lista_cursos_en_US.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/lista_cursos_pt_BR.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/lista_turmas_en_US.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/lista_turmas_pt_BR.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/mapeamento_cursos.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/mapeamento_ementas.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/mapeamento_semestres.hpp"

#include <QApplication>
#include <QtTest>

class TestDictionariesAndMappings : public QObject {
    Q_OBJECT

private:
    static void verifyCurriculumStructure(const EstruturaCursos &estrutura, const QString &label) {
        QVERIFY2(!estrutura.isEmpty(), qPrintable(label + " must not be empty"));
        for (const auto &curso : estrutura) {
            QVERIFY2(!curso.nome.trimmed().isEmpty(), qPrintable(label + " contains blank course"));
            QVERIFY2(!curso.ementas.isEmpty(), qPrintable(curso.nome + " has no syllabi"));
            for (const auto &ementa : curso.ementas) {
                QVERIFY2(!ementa.nome.trimmed().isEmpty(), qPrintable(label + " contains blank syllabus"));
                QVERIFY2(!ementa.semestres.isEmpty(), qPrintable(ementa.nome + " has no semesters"));
                for (const auto &semestre : ementa.semestres) {
                    QVERIFY2(!semestre.nome.trimmed().isEmpty(), qPrintable(label + " contains blank semester"));
                    QVERIFY2(!semestre.disciplinas.isEmpty(), qPrintable(semestre.nome + " has no subjects"));
                    for (const QString &disc : semestre.disciplinas) {
                        QVERIFY2(!disc.trimmed().isEmpty(), qPrintable(semestre.nome + " contains empty subject"));
                    }
                }
            }
        }
    }

private slots:
    void testCurriculaStructuralIntegrity() {
        verifyCurriculumStructure(CURSOS_PT(), QStringLiteral("CURSOS_PT"));
        verifyCurriculumStructure(CURSOS_EN(), QStringLiteral("CURSOS_EN"));
        QCOMPARE(CURSOS_PT().size(), CURSOS_EN().size());
    }

    void testStaticListsConsistency() {
        QVERIFY(lista_cursos_pt_BR().contains(QStringLiteral("Engenharia Civil")));
        QVERIFY(lista_cursos_en_US().contains(QStringLiteral("Civil Engineering")));
        QCOMPARE(lista_turmas_pt_BR().size(), lista_turmas_en_US().size());
        QVERIFY(lista_turmas_pt_BR().size() > 5);
        QCOMPARE(lista_avaliacoes_pt_BR().size(), lista_avaliacoes_en_US().size());
        QVERIFY(lista_avaliacoes_pt_BR().contains(QStringLiteral("PROVA")));
        QVERIFY(lista_avaliacoes_en_US().contains(QStringLiteral("EXAM")));
    }

    void testColorDictionariesAndCodeLookup() {
        const auto &cores = dicionario_de_cores();
        QCOMPARE(cores.size(), 24);
        for (auto it = cores.cbegin(); it != cores.cend(); ++it) {
            QVERIFY2(AgendaTestSupport::isValidHexColor(it.value()),
                     qPrintable(QStringLiteral("Invalid hex color for %1: %2").arg(it.key(), it.value())));
        }

        const auto &discs = dicionario_disciplinas();
        QCOMPARE(discs.size(), cores.size());
        for (auto it = discs.cbegin(); it != discs.cend(); ++it) {
            QVERIFY2(cores.contains(it.key()), qPrintable(QStringLiteral("Color key missing: %1").arg(it.key())));
        }

        const QHash<QString, QString> codeMap = construir_mapeamento_codigo_para_cor();
        QVERIFY2(!codeMap.isEmpty(), "Subject code -> color map must not be empty");
        QVERIFY(codeMap.contains(QStringLiteral("CM201")));
        QVERIFY(codeMap.contains(QStringLiteral("TH045")));

        // Regex lookup by trailing code (even when written in English or lowercase!)
        const QString colorByPt = obter_cor_por_nome(QStringLiteral("Cálculo Diferencial e Integral I – CM201"));
        const QString colorByEn = obter_cor_por_nome(QStringLiteral("Differential Calculus I - cm201"));
        QVERIFY(AgendaTestSupport::isValidHexColor(colorByPt));
        QCOMPARE(colorByEn, colorByPt);
        QCOMPARE(obter_cor_por_nome(QStringLiteral("Engenharia Social – TH045")), codeMap.value(QStringLiteral("TH045")));

        // Empty and unknown subjects return empty color
        QCOMPARE(obter_cor_por_nome(QString()), QString());
        QCOMPARE(obter_cor_por_nome(QStringLiteral("   ")), QString());
        QCOMPARE(obter_cor_por_nome(QStringLiteral("Disciplina Completamente Inexistente")), QString());
    }

    void testCurriculumTranslationsPtToEn() {
        QVERIFY(mapeamento_cursos().contains(QStringLiteral("pt_BR")));
        QVERIFY(mapeamento_cursos().contains(QStringLiteral("en_US")));
        QVERIFY(mapeamento_ementas().contains(QStringLiteral("en_US")));
        QVERIFY(mapeamento_semestres().contains(QStringLiteral("en_US")));

        QCOMPARE(traduzir_curso(QStringLiteral("Engenharia Civil"), QStringLiteral("pt_BR")),
                 QStringLiteral("Engenharia Civil"));
        QCOMPARE(traduzir_curso(QStringLiteral("Engenharia Civil"), QStringLiteral("en_US")),
                 QStringLiteral("Civil Engineering"));
        QCOMPARE(traduzir_curso(QStringLiteral("Curso Customizado"), QStringLiteral("en_US")),
                 QStringLiteral("Curso Customizado"));

        QCOMPARE(traduzir_ementa(QStringLiteral("Ementa - 2011"), QStringLiteral("en_US")),
                 QStringLiteral("Syllabus - 2011"));
        QCOMPARE(traduzir_ementa(QStringLiteral("Ementa - 2023"), QStringLiteral("en_US")),
                 QStringLiteral("Syllabus - 2023"));
        QCOMPARE(traduzir_ementa(QStringLiteral("Ementa Custom"), QStringLiteral("en_US")),
                 QStringLiteral("Ementa Custom"));

        QCOMPARE(traduzir_semestre(QStringLiteral("1º Semestre"), QStringLiteral("en_US")),
                 QStringLiteral("1st Semester"));
        QCOMPARE(traduzir_semestre(QStringLiteral("Optativas (mínimo 6)"), QStringLiteral("en_US")),
                 QStringLiteral("Electives (minimum 6)"));
        QCOMPARE(traduzir_semestre(QStringLiteral("Verão"), QStringLiteral("en_US")),
                 QStringLiteral("Verão"));
    }
};

int main(int argc, char *argv[]) {
    AgendaTestSupport::IsolatedLocalAppDataGuard envGuard;
    QApplication app(argc, argv);
    TestDictionariesAndMappings tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_03_DictionariesAndMappings.moc"
