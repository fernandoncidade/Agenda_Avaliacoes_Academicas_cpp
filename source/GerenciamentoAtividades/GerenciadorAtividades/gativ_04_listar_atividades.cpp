#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QSqlError>
#include <QSqlQuery>

QList<Atividade> GerenciamentoAtividades::listar_atividades() {
    QList<Atividade> lista;
    atividades.clear();
    QSqlQuery query(conexao);
    if (!query.exec("SELECT data, tipo, sequencia, nome, turma FROM atividades")) {
        LogManager::error(QString("Erro ao listar atividades: %1").arg(query.lastError().text()));
        return lista;
    }
    while (query.next()) {
        Atividade atividade{query.value(0).toString(), query.value(1).toString(), query.value(2).toString(),
                            query.value(3).toString(), query.value(4).toString()};
        atividades.insert(chave_atividade(atividade), atividade);
        lista << atividade;
    }
    return lista;
}
