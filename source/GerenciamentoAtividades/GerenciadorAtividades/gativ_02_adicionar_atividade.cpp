#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QSqlError>
#include <QSqlQuery>

void GerenciamentoAtividades::adicionar_atividade(const Atividade &atividade) {
    atividades.insert(chave_atividade(atividade), atividade);
    QSqlQuery query(conexao);
    query.prepare("INSERT INTO atividades (data, tipo, sequencia, nome, turma) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(atividade.data);
    query.addBindValue(atividade.tipo);
    query.addBindValue(atividade.sequencia);
    query.addBindValue(atividade.nome);
    query.addBindValue(atividade.turma);
    if (!query.exec()) {
        LogManager::error(QString("Erro ao adicionar atividade: %1").arg(query.lastError().text()));
    }
}
