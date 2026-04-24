#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QSqlError>
#include <QSqlQuery>

void GerenciamentoAtividades::limpar_entradas() {
    if (combo_curso)
        combo_curso->setCurrentIndex(0);
    if (entry_ementa)
        entry_ementa->clear();
    if (entry_semestre)
        entry_semestre->clear();
    if (entry_disciplina)
        entry_disciplina->clear();
    if (entry_codigo)
        entry_codigo->setCurrentIndex(0);
    if (combo_tipo)
        combo_tipo->setCurrentIndex(0);
    if (combo_sequencia)
        combo_sequencia->setCurrentIndex(0);
    if (calendar)
        calendar->setSelectedDate(QDate::currentDate());
    if (textbox)
        textbox->clear();
    QSqlQuery query(conexao);
    if (!query.exec("DELETE FROM atividades")) {
        LogManager::critical(QString("Erro fatal ao limpar entradas: %1").arg(query.lastError().text()));
    }
    atividades.clear();
}
