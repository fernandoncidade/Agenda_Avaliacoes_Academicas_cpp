#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QSqlError>
#include <QSqlQuery>

void GerenciamentoAtividades::criar_tabela() {
    QSqlQuery query(conexao);
    if (!query.exec("CREATE TABLE IF NOT EXISTS atividades (id INTEGER PRIMARY KEY AUTOINCREMENT, data TEXT, tipo "
                    "TEXT, sequencia TEXT, nome TEXT, turma TEXT)")) {
        LogManager::error(QString("Erro ao criar tabela de atividades: %1").arg(query.lastError().text()));
    }
}
