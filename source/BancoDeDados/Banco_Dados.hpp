#pragma once

#include "DicionariosListasBancoDeDados/EstruturasCursos.hpp"

#include <QHash>
#include <QString>
#include <QStringList>
QString obter_idioma_atual();
QStringList _mesclar_listas(const QList<QStringList> &listas);
QStringList obter_cursos();
QStringList obter_turmas();
QStringList obter_avaliacoes();
QStringList obter_sequencias();
QString traduzir_curso(const QString &curso, const QString &idioma_destino = QString());
QString traduzir_ementa(const QString &ementa, const QString &idioma_destino = QString());
QString traduzir_semestre(const QString &semestre, const QString &idioma_destino = QString());
const EstruturaCursos &obter_estrutura_cursos();
QStringList obter_ementas(const QString &curso);
QStringList obter_semestres(const QString &curso, const QString &ementa);
QStringList obter_disciplinas(const QString &curso, const QString &ementa, const QString &semestre);
QHash<QString, bool> registrar_valores_personalizados(
    const QString &curso = QString(), const QString &ementa = QString(), const QString &semestre = QString(),
    const QString &disciplina = QString(), const QString &turma = QString(), const QString &tipo = QString(),
    const QString &sequencia = QString(), bool registrar_curso = false, bool registrar_ementa = false,
    bool registrar_semestre = false, bool registrar_disciplina = false, bool registrar_turma = false,
    bool registrar_tipo = false, bool registrar_sequencia = false);
void atualizar_listas_exportadas();
QStringList lista_cursos();
QStringList lista_turmas();
QStringList lista_avaliacoes();
QStringList lista_sequencias();
QHash<QString, QString> construir_mapeamento_codigo_para_cor();
QString obter_cor_por_nome(const QString &nome_disciplina);
