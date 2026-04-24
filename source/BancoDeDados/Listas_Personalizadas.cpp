#include "Listas_Personalizadas.hpp"

#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QUuid>

namespace {
    bool g_tabelasCriadas = false;

    struct DbHandle {
        QString connectionName;
        QSqlDatabase db;

        ~DbHandle() {
            if (db.isValid()) {
                db.close();
            }
            const QString name = connectionName;
            db = QSqlDatabase();
            if (!name.isEmpty()) {
                QSqlDatabase::removeDatabase(name);
            }
        }
    };

    DbHandle abrir_conexao() {
        DbHandle handle;
        handle.connectionName = "listas_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
        handle.db = QSqlDatabase::addDatabase("QSQLITE", handle.connectionName);
        handle.db.setDatabaseName(obter_caminho_listas_db());
        if (!handle.db.open()) {
            LogManager::error(QString("Erro ao abrir listas.db: %1").arg(handle.db.lastError().text()));
        }
        return handle;
    }

    bool exec_sql(QSqlDatabase &db, const QString &sql) {
        QSqlQuery query(db);
        if (!query.exec(sql)) {
            LogManager::error(QString("Erro SQL: %1 | %2").arg(query.lastError().text(), sql));
            return false;
        }
        return true;
    }

    QStringList listar_simples(const QString &tabela, const QString &idioma) {
        criar_tabelas_listas();
        QStringList valores;
        auto handle = abrir_conexao();
        if (!handle.db.isOpen()) {
            return valores;
        }
        QSqlQuery query(handle.db);
        query.prepare(QString("SELECT valor FROM %1 WHERE idioma = ? ORDER BY valor COLLATE NOCASE").arg(tabela));
        query.addBindValue(_texto(idioma));
        if (!query.exec()) {
            LogManager::error(
                QString("Erro ao listar valores personalizados de %1: %2").arg(tabela, query.lastError().text()));
            return valores;
        }
        while (query.next()) {
            valores << query.value(0).toString();
        }
        return valores;
    }

    bool inserir_simples(const QString &tabela, const QString &idioma, const QString &valorBruto) {
        const QString valor = _texto(valorBruto);
        if (valor.isEmpty()) {
            return false;
        }
        criar_tabelas_listas();
        auto handle = abrir_conexao();
        if (!handle.db.isOpen()) {
            return false;
        }
        QSqlQuery query(handle.db);
        query.prepare(
            QString("INSERT OR IGNORE INTO %1 (idioma, valor, valor_normalizado) VALUES (?, ?, ?)").arg(tabela));
        query.addBindValue(_texto(idioma));
        query.addBindValue(valor);
        query.addBindValue(_normalizar(valor));
        if (!query.exec()) {
            LogManager::error(
                QString("Erro ao inserir valor personalizado em %1: %2").arg(tabela, query.lastError().text()));
            return false;
        }
        return query.numRowsAffected() > 0;
    }

    QString formatar_item_personalizado(const QString &categoria, const QSqlQuery &query) {
        if (categoria == "curso" || categoria == "turma" || categoria == "tipo" || categoria == "sequencia") {
            return query.value(1).toString();
        }
        if (categoria == "ementa") {
            return QString("%1 > %2").arg(query.value(1).toString(), query.value(2).toString());
        }
        if (categoria == "semestre") {
            return QString("%1 > %2 > %3")
                .arg(query.value(1).toString(), query.value(2).toString(), query.value(3).toString());
        }
        if (categoria == "disciplina") {
            return QString("%1 > %2 > %3 > %4")
                .arg(query.value(1).toString(), query.value(2).toString(), query.value(3).toString(),
                     query.value(4).toString());
        }
        return QString();
    }

    struct CategoriaConfig {
        QString tabela;
        QString select;
    };

    QHash<QString, CategoriaConfig> categorias_exclusao() {
        return {
            {"curso",
             {"lista_cursos", "SELECT id, valor FROM lista_cursos WHERE idioma = ? ORDER BY valor COLLATE NOCASE"}},
            {"ementa",
             {"lista_ementas", "SELECT id, curso, valor FROM lista_ementas WHERE idioma = ? ORDER BY curso COLLATE "
                               "NOCASE, valor COLLATE NOCASE"}},
            {"semestre",
             {"lista_semestres", "SELECT id, curso, ementa, valor FROM lista_semestres WHERE idioma = ? ORDER BY curso "
                                 "COLLATE NOCASE, ementa COLLATE NOCASE, valor COLLATE NOCASE"}},
            {"disciplina",
             {"lista_disciplinas",
              "SELECT id, curso, ementa, semestre, valor FROM lista_disciplinas WHERE idioma = ? ORDER BY curso "
              "COLLATE NOCASE, ementa COLLATE NOCASE, semestre COLLATE NOCASE, valor COLLATE NOCASE"}},
            {"turma",
             {"lista_turmas", "SELECT id, valor FROM lista_turmas WHERE idioma = ? ORDER BY valor COLLATE NOCASE"}},
            {"tipo",
             {"lista_tipos_atividade",
              "SELECT id, valor FROM lista_tipos_atividade WHERE idioma = ? ORDER BY valor COLLATE NOCASE"}},
            {"sequencia",
             {"lista_sequencias_atividade",
              "SELECT id, valor FROM lista_sequencias_atividade WHERE idioma = ? ORDER BY valor COLLATE NOCASE"}}};
    }
} // namespace

QString _texto(const QString &valor) {
    return valor.trimmed();
}

QString _normalizar(const QString &valor) {
    return _texto(valor).toCaseFolded();
}

QString obter_caminho_listas_db() {
    QDir dir(obter_caminho_persistente());
    dir.mkpath(".");
    return dir.filePath("listas.db");
}

void criar_tabelas_listas() {
    if (g_tabelasCriadas) {
        return;
    }
    auto handle = abrir_conexao();
    if (!handle.db.isOpen()) {
        return;
    }
    exec_sql(handle.db, "CREATE TABLE IF NOT EXISTS lista_cursos (id INTEGER PRIMARY KEY AUTOINCREMENT, idioma TEXT "
                        "NOT NULL, valor TEXT NOT NULL, valor_normalizado TEXT NOT NULL, criado_em TEXT DEFAULT "
                        "CURRENT_TIMESTAMP, UNIQUE(idioma, valor_normalizado))");
    exec_sql(handle.db,
             "CREATE TABLE IF NOT EXISTS lista_ementas (id INTEGER PRIMARY KEY AUTOINCREMENT, idioma TEXT NOT NULL, "
             "curso TEXT NOT NULL, curso_normalizado TEXT NOT NULL, valor TEXT NOT NULL, valor_normalizado TEXT NOT "
             "NULL, criado_em TEXT DEFAULT CURRENT_TIMESTAMP, UNIQUE(idioma, curso_normalizado, valor_normalizado))");
    exec_sql(handle.db,
             "CREATE TABLE IF NOT EXISTS lista_semestres (id INTEGER PRIMARY KEY AUTOINCREMENT, idioma TEXT NOT NULL, "
             "curso TEXT NOT NULL, curso_normalizado TEXT NOT NULL, ementa TEXT NOT NULL, ementa_normalizada TEXT NOT "
             "NULL, valor TEXT NOT NULL, valor_normalizado TEXT NOT NULL, criado_em TEXT DEFAULT CURRENT_TIMESTAMP, "
             "UNIQUE(idioma, curso_normalizado, ementa_normalizada, valor_normalizado))");
    exec_sql(handle.db,
             "CREATE TABLE IF NOT EXISTS lista_disciplinas (id INTEGER PRIMARY KEY AUTOINCREMENT, idioma TEXT NOT "
             "NULL, curso TEXT NOT NULL, curso_normalizado TEXT NOT NULL, ementa TEXT NOT NULL, ementa_normalizada "
             "TEXT NOT NULL, semestre TEXT NOT NULL, semestre_normalizado TEXT NOT NULL, valor TEXT NOT NULL, "
             "valor_normalizado TEXT NOT NULL, criado_em TEXT DEFAULT CURRENT_TIMESTAMP, UNIQUE(idioma, "
             "curso_normalizado, ementa_normalizada, semestre_normalizado, valor_normalizado))");
    exec_sql(handle.db, "CREATE TABLE IF NOT EXISTS lista_turmas (id INTEGER PRIMARY KEY AUTOINCREMENT, idioma TEXT "
                        "NOT NULL, valor TEXT NOT NULL, valor_normalizado TEXT NOT NULL, criado_em TEXT DEFAULT "
                        "CURRENT_TIMESTAMP, UNIQUE(idioma, valor_normalizado))");
    exec_sql(handle.db, "CREATE TABLE IF NOT EXISTS lista_tipos_atividade (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "idioma TEXT NOT NULL, valor TEXT NOT NULL, valor_normalizado TEXT NOT NULL, criado_em TEXT "
                        "DEFAULT CURRENT_TIMESTAMP, UNIQUE(idioma, valor_normalizado))");
    exec_sql(handle.db, "CREATE TABLE IF NOT EXISTS lista_sequencias_atividade (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "idioma TEXT NOT NULL, valor TEXT NOT NULL, valor_normalizado TEXT NOT NULL, criado_em TEXT "
                        "DEFAULT CURRENT_TIMESTAMP, UNIQUE(idioma, valor_normalizado))");
    g_tabelasCriadas = true;
}

QStringList listar_cursos(const QString &idioma) {
    return listar_simples("lista_cursos", idioma);
}

QStringList listar_turmas(const QString &idioma) {
    return listar_simples("lista_turmas", idioma);
}

QStringList listar_tipos_atividade(const QString &idioma) {
    return listar_simples("lista_tipos_atividade", idioma);
}

QStringList listar_sequencias_atividade(const QString &idioma) {
    return listar_simples("lista_sequencias_atividade", idioma);
}

bool inserir_curso(const QString &idioma, const QString &curso) {
    return inserir_simples("lista_cursos", idioma, curso);
}

bool inserir_turma(const QString &idioma, const QString &turma) {
    return inserir_simples("lista_turmas", idioma, turma);
}

bool inserir_tipo_atividade(const QString &idioma, const QString &tipo) {
    return inserir_simples("lista_tipos_atividade", idioma, tipo);
}

bool inserir_sequencia_atividade(const QString &idioma, const QString &sequencia) {
    return inserir_simples("lista_sequencias_atividade", idioma, sequencia);
}

QStringList listar_ementas(const QString &idioma, const QString &curso) {
    criar_tabelas_listas();
    QStringList valores;
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare(
        "SELECT valor FROM lista_ementas WHERE idioma = ? AND curso_normalizado = ? ORDER BY valor COLLATE NOCASE");
    query.addBindValue(_texto(idioma));
    query.addBindValue(_normalizar(curso));
    if (query.exec()) {
        while (query.next())
            valores << query.value(0).toString();
    }
    return valores;
}

QStringList listar_semestres(const QString &idioma, const QString &curso, const QString &ementa) {
    criar_tabelas_listas();
    QStringList valores;
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare("SELECT valor FROM lista_semestres WHERE idioma = ? AND curso_normalizado = ? AND ementa_normalizada "
                  "= ? ORDER BY valor COLLATE NOCASE");
    query.addBindValue(_texto(idioma));
    query.addBindValue(_normalizar(curso));
    query.addBindValue(_normalizar(ementa));
    if (query.exec()) {
        while (query.next())
            valores << query.value(0).toString();
    }
    return valores;
}

QStringList listar_disciplinas(const QString &idioma, const QString &curso, const QString &ementa,
                               const QString &semestre) {
    criar_tabelas_listas();
    QStringList valores;
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare("SELECT valor FROM lista_disciplinas WHERE idioma = ? AND curso_normalizado = ? AND "
                  "ementa_normalizada = ? AND semestre_normalizado = ? ORDER BY valor COLLATE NOCASE");
    query.addBindValue(_texto(idioma));
    query.addBindValue(_normalizar(curso));
    query.addBindValue(_normalizar(ementa));
    query.addBindValue(_normalizar(semestre));
    if (query.exec()) {
        while (query.next())
            valores << query.value(0).toString();
    }
    return valores;
}

bool inserir_ementa(const QString &idioma, const QString &cursoBruto, const QString &ementaBruta) {
    const QString curso = _texto(cursoBruto);
    const QString ementa = _texto(ementaBruta);
    if (curso.isEmpty() || ementa.isEmpty())
        return false;
    criar_tabelas_listas();
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare("INSERT OR IGNORE INTO lista_ementas (idioma, curso, curso_normalizado, valor, valor_normalizado) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(_texto(idioma));
    query.addBindValue(curso);
    query.addBindValue(_normalizar(curso));
    query.addBindValue(ementa);
    query.addBindValue(_normalizar(ementa));
    return query.exec() && query.numRowsAffected() > 0;
}

bool inserir_semestre(const QString &idioma, const QString &cursoBruto, const QString &ementaBruta,
                      const QString &semestreBruto) {
    const QString curso = _texto(cursoBruto), ementa = _texto(ementaBruta), semestre = _texto(semestreBruto);
    if (curso.isEmpty() || ementa.isEmpty() || semestre.isEmpty())
        return false;
    criar_tabelas_listas();
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare("INSERT OR IGNORE INTO lista_semestres (idioma, curso, curso_normalizado, ementa, "
                  "ementa_normalizada, valor, valor_normalizado) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(_texto(idioma));
    query.addBindValue(curso);
    query.addBindValue(_normalizar(curso));
    query.addBindValue(ementa);
    query.addBindValue(_normalizar(ementa));
    query.addBindValue(semestre);
    query.addBindValue(_normalizar(semestre));
    return query.exec() && query.numRowsAffected() > 0;
}

bool inserir_disciplina(const QString &idioma, const QString &cursoBruto, const QString &ementaBruta,
                        const QString &semestreBruto, const QString &disciplinaBruta) {
    const QString curso = _texto(cursoBruto), ementa = _texto(ementaBruta), semestre = _texto(semestreBruto),
                  disciplina = _texto(disciplinaBruta);
    if (curso.isEmpty() || ementa.isEmpty() || semestre.isEmpty() || disciplina.isEmpty())
        return false;
    criar_tabelas_listas();
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare(
        "INSERT OR IGNORE INTO lista_disciplinas (idioma, curso, curso_normalizado, ementa, ementa_normalizada, "
        "semestre, semestre_normalizado, valor, valor_normalizado) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(_texto(idioma));
    query.addBindValue(curso);
    query.addBindValue(_normalizar(curso));
    query.addBindValue(ementa);
    query.addBindValue(_normalizar(ementa));
    query.addBindValue(semestre);
    query.addBindValue(_normalizar(semestre));
    query.addBindValue(disciplina);
    query.addBindValue(_normalizar(disciplina));
    return query.exec() && query.numRowsAffected() > 0;
}

QHash<QString, bool> registrar_listas_personalizadas(const QString &idioma, const QString &curso, const QString &ementa,
                                                     const QString &semestre, const QString &disciplina,
                                                     const QString &turma, const QString &tipo,
                                                     const QString &sequencia) {
    return {{"curso", inserir_curso(idioma, curso)},
            {"ementa", inserir_ementa(idioma, curso, ementa)},
            {"semestre", inserir_semestre(idioma, curso, ementa, semestre)},
            {"disciplina", inserir_disciplina(idioma, curso, ementa, semestre, disciplina)},
            {"turma", inserir_turma(idioma, turma)},
            {"tipo", inserir_tipo_atividade(idioma, tipo)},
            {"sequencia", inserir_sequencia_atividade(idioma, sequencia)}};
}

QList<ItemPersonalizadoExclusao> listar_itens_personalizados_para_exclusao(const QString &categoria,
                                                                           const QString &idioma) {
    criar_tabelas_listas();
    QList<ItemPersonalizadoExclusao> itens;
    const auto configs = categorias_exclusao();
    if (!configs.contains(categoria))
        return itens;
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare(configs.value(categoria).select);
    query.addBindValue(_texto(idioma));
    if (!query.exec()) {
        LogManager::error(
            QString("Erro ao listar itens personalizados para exclusao: %1").arg(query.lastError().text()));
        return itens;
    }
    while (query.next()) {
        itens.append({query.value(0).toInt(), categoria, formatar_item_personalizado(categoria, query)});
    }
    return itens;
}

bool remover_item_personalizado(const QString &categoria, int item_id) {
    criar_tabelas_listas();
    const auto configs = categorias_exclusao();
    if (!configs.contains(categoria))
        return false;
    auto handle = abrir_conexao();
    QSqlQuery query(handle.db);
    query.prepare(QString("DELETE FROM %1 WHERE id = ?").arg(configs.value(categoria).tabela));
    query.addBindValue(item_id);
    return query.exec() && query.numRowsAffected() > 0;
}
