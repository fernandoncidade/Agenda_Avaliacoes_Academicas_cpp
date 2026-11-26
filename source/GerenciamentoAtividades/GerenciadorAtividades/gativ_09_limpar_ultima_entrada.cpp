#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

void GerenciamentoAtividades::limpar_ultima_entrada() {
    QSqlQuery query(conexao);
    if (!query.exec("SELECT id FROM atividades ORDER BY id DESC LIMIT 1") || !query.next()) {
        QMessageBox::warning(nullptr, "Erro", "Nenhuma atividade para remover.");
        return;
    }
    const int id = query.value(0).toInt();
    QSqlQuery del(conexao);
    del.prepare("DELETE FROM atividades WHERE id = ?");
    del.addBindValue(id);
    if (!del.exec()) {
        LogManager::critical(QString("Erro fatal ao limpar ultima entrada: %1").arg(del.lastError().text()));
        return;
    }
    listar_atividades();
    update_textbox();
    QMessageBox::information(nullptr, "Sucesso", "Última atividade removida com sucesso!");
}
