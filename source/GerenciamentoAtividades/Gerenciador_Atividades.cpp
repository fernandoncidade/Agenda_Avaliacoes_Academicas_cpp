#include "Gerenciador_Atividades.hpp"

#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlError>
#include <QUuid>

QString chave_atividade(const Atividade &atividade) {
    return QStringList{atividade.data, atividade.tipo, atividade.sequencia, atividade.nome, atividade.turma}.join(
        QChar(0x1f));
}

QString atividade_para_linha(const Atividade &atividade) {
    return QString("%1 %2 – %3 %4 %5")
        .arg(atividade.data, atividade.tipo, atividade.sequencia, atividade.nome, atividade.turma);
}

GerenciamentoAtividades::GerenciamentoAtividades() {
    try {
        QString persistDir = obter_caminho_persistente();
        QDir dir(persistDir);
        dir.mkpath(".");
        QFile config(dir.filePath("config.json"));
        if (config.open(QIODevice::ReadOnly)) {
            const QString modo = QJsonDocument::fromJson(config.readAll()).object().value("modo_cores").toString();
            if (modo == "preto" || modo == "coloridas") {
                modo_cores = modo;
            }
        }
        connectionName = "atividades_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
        conexao = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        conexao.setDatabaseName(dir.filePath("atividades.db"));
        if (!conexao.open()) {
            LogManager::error(QString("Erro ao abrir atividades.db: %1").arg(conexao.lastError().text()));
        }
        criar_tabela();
    } catch (const std::exception &e) {
        LogManager::error(QString("Erro ao inicializar GerenciamentoAtividades: %1").arg(e.what()));
    } catch (...) {
        LogManager::error("Erro ao inicializar GerenciamentoAtividades");
    }
}

GerenciamentoAtividades::~GerenciamentoAtividades() {
    const QString name = connectionName;
    if (conexao.isValid()) {
        conexao.close();
    }
    conexao = QSqlDatabase();
    if (!name.isEmpty()) {
        QSqlDatabase::removeDatabase(name);
    }
}
