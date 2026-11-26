#pragma once

#include <QCalendarWidget>
#include <QComboBox>
#include <QHash>
#include <QSqlDatabase>
#include <QString>
#include <QTextBrowser>

#include <optional>

struct Atividade {
    QString data;
    QString tipo;
    QString sequencia;
    QString nome;
    QString turma;
};

QString chave_atividade(const Atividade &atividade);
QString atividade_para_linha(const Atividade &atividade);

class GerenciamentoAtividades {
public:
    GerenciamentoAtividades();
    ~GerenciamentoAtividades();
    void criar_tabela();
    void adicionar_atividade(const Atividade &atividade);
    void remover_atividade(const Atividade &atividade);
    QList<Atividade> listar_atividades();
    std::optional<Atividade> buscar_atividade(const Atividade &atividade);
    bool atualizar_atividade(const Atividade &atividade, const Atividade &novos_dados);
    void submiter();
    void limpar_entradas();
    void limpar_ultima_entrada();
    void exportar_para_pdf();
    void update_textbox();
    void excluir_item();
    void editar_item();
    void editar_detalhes_atividade(const Atividade &atividade);
    void update_ementa();
    void update_semestre();
    void update_disciplinas();
    void carregar_dados();
    bool definir_modo_cores(const QString &modo);
    QString obter_modo_cores() const;
    QString cor_texto_do_sistema() const;
    QHash<QString, Atividade> atividades;
    QComboBox *combo_curso = nullptr;
    QComboBox *entry_ementa = nullptr;
    QComboBox *entry_semestre = nullptr;
    QComboBox *entry_disciplina = nullptr;
    QComboBox *entry_codigo = nullptr;
    QComboBox *combo_tipo = nullptr;
    QComboBox *combo_sequencia = nullptr;
    QCalendarWidget *calendar = nullptr;
    QTextBrowser *textbox = nullptr;
    QString modo_cores = "coloridas";
    QSqlDatabase conexao;
    QString connectionName;
};
