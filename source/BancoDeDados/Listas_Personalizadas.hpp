#pragma once

#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>

struct ItemPersonalizadoExclusao {
    int id = 0;
    QString categoria;
    QString texto;
};

QString _texto(const QString &valor);
QString _normalizar(const QString &valor);
QString obter_caminho_listas_db();
void criar_tabelas_listas();
QStringList listar_cursos(const QString &idioma);
QStringList listar_turmas(const QString &idioma);
QStringList listar_tipos_atividade(const QString &idioma);
QStringList listar_sequencias_atividade(const QString &idioma);
bool inserir_curso(const QString &idioma, const QString &curso);
bool inserir_turma(const QString &idioma, const QString &turma);
bool inserir_tipo_atividade(const QString &idioma, const QString &tipo);
bool inserir_sequencia_atividade(const QString &idioma, const QString &sequencia);
QStringList listar_ementas(const QString &idioma, const QString &curso);
QStringList listar_semestres(const QString &idioma, const QString &curso, const QString &ementa);
QStringList listar_disciplinas(const QString &idioma, const QString &curso, const QString &ementa,
                               const QString &semestre);
bool inserir_ementa(const QString &idioma, const QString &curso, const QString &ementa);
bool inserir_semestre(const QString &idioma, const QString &curso, const QString &ementa, const QString &semestre);
bool inserir_disciplina(const QString &idioma, const QString &curso, const QString &ementa, const QString &semestre,
                        const QString &disciplina);
QHash<QString, bool> registrar_listas_personalizadas(const QString &idioma, const QString &curso = QString(),
                                                     const QString &ementa = QString(),
                                                     const QString &semestre = QString(),
                                                     const QString &disciplina = QString(),
                                                     const QString &turma = QString(), const QString &tipo = QString(),
                                                     const QString &sequencia = QString());
QList<ItemPersonalizadoExclusao> listar_itens_personalizados_para_exclusao(const QString &categoria,
                                                                           const QString &idioma);
bool remover_item_personalizado(const QString &categoria, int item_id);
