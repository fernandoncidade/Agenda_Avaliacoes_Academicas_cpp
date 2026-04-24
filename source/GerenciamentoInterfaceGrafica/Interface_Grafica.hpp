#pragma once

#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"

#include <QAction>
#include <QActionGroup>
#include <QCalendarWidget>
#include <QComboBox>
#include <QHash>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QTextBrowser>

#include <memory>

class InterfaceGerenciadorAtividades : public QMainWindow {
    Q_OBJECT
public:
    explicit InterfaceGerenciadorAtividades(GerenciadorTraducao *gerenciador_traducao = nullptr,
                                            QWidget *parent = nullptr);
    void center_on_screen();
    void create_widgets();
    QPushButton *create_button();
    void create_menu();
    void change_language(const QString &codigo_idioma);
    void change_color_mode(const QString &codigo_modo);
    void retranslate_ui();

protected:
    void showEvent(QShowEvent *event) override;
private slots:
    void submiter();
    void limpar_entradas();
    void limpar_ultima_entrada();
    void excluir_item();
    void editar_item();
    void exportar_para_pdf();
    void update_textbox();
    void update_ementa();
    void update_semestre();
    void update_disciplinas();

public:
    GerenciamentoAtividades gerenciamento_atividades;
    std::unique_ptr<GerenciadorTraducao> gerenciador_traducao_proprio;
    GerenciadorTraducao *gerenciador_traducao = nullptr;
    QLabel *label_curso = nullptr;
    QLabel *label_ementa = nullptr;
    QLabel *label_semestre = nullptr;
    QLabel *label_disciplina = nullptr;
    QLabel *label_codigo = nullptr;
    QLabel *label_tipo = nullptr;
    QLabel *label_sequencia = nullptr;
    QLabel *label_data = nullptr;
    QLabel *label_banco_dados = nullptr;
    QComboBox *combo_curso = nullptr;
    QComboBox *entry_ementa = nullptr;
    QComboBox *entry_semestre = nullptr;
    QComboBox *entry_disciplina = nullptr;
    QComboBox *entry_codigo = nullptr;
    QComboBox *combo_tipo = nullptr;
    QComboBox *combo_sequencia = nullptr;
    QCalendarWidget *calendar = nullptr;
    QTextBrowser *textbox = nullptr;
    QPushButton *button_clear_item = nullptr;
    QPushButton *button_clear_ultima = nullptr;
    QPushButton *button_clear = nullptr;
    QPushButton *button_editar_item = nullptr;
    QPushButton *button_submiter = nullptr;
    QPushButton *button_export = nullptr;
    QMenu *menu_arquivo = nullptr;
    QMenu *menu_config = nullptr;
    QMenu *menu_idiomas = nullptr;
    QMenu *menu_excluir_item_listas = nullptr;
    QMenu *menu_cores = nullptr;
    QMenu *menu_sobre = nullptr;
    QAction *action_excluir_item = nullptr;
    QAction *action_limpar_ultima = nullptr;
    QAction *action_limpar_tudo = nullptr;
    QAction *action_editar_item = nullptr;
    QAction *action_registrar = nullptr;
    QAction *action_export_pdf = nullptr;
    QAction *action_sair = nullptr;
    QAction *action_sobre = nullptr;
    QActionGroup *lang_action_group = nullptr;
    QActionGroup *color_action_group = nullptr;
    QHash<QString, QAction *> lang_actions;
    QHash<QString, QAction *> color_actions;
    QHash<QString, QMenu *> delete_list_menus;
    QHash<QString, QString> delete_list_action_sources;
    bool _atualizando_ementa = false;
    bool _atualizando_semestre = false;
    bool _atualizando_disciplinas = false;
};
