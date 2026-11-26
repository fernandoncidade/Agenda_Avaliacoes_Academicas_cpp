#include "ginter_03_exibir_sobre.hpp"
#include "ginter_12_excluir_item_lista_personalizada.hpp"
#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"
#include "source/utils/LogManager.hpp"

#include <QAction>
#include <QActionGroup>
#include <QCoreApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

void InterfaceGerenciadorAtividades::create_menu() {
    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    QMenuBar *bar = menuBar();
    menu_arquivo = bar->addMenu(tr("InterfaceGerenciadorAtividades", "Arquivo"));
    action_excluir_item = new QAction(tr("InterfaceGerenciadorAtividades", "Excluir Item(ns)"), this);
    connect(action_excluir_item, &QAction::triggered, this, &InterfaceGerenciadorAtividades::excluir_item);
    menu_arquivo->addAction(action_excluir_item);
    action_limpar_ultima = new QAction(tr("InterfaceGerenciadorAtividades", "Limpar Última Entrada"), this);
    connect(action_limpar_ultima, &QAction::triggered, this, &InterfaceGerenciadorAtividades::limpar_ultima_entrada);
    menu_arquivo->addAction(action_limpar_ultima);
    action_limpar_tudo = new QAction(tr("InterfaceGerenciadorAtividades", "Limpar Tudo"), this);
    connect(action_limpar_tudo, &QAction::triggered, this, &InterfaceGerenciadorAtividades::limpar_entradas);
    menu_arquivo->addAction(action_limpar_tudo);
    action_editar_item = new QAction(tr("InterfaceGerenciadorAtividades", "Editar Item(ns)"), this);
    connect(action_editar_item, &QAction::triggered, this, &InterfaceGerenciadorAtividades::editar_item);
    menu_arquivo->addAction(action_editar_item);
    action_registrar = new QAction(tr("InterfaceGerenciadorAtividades", "Registrar Definições"), this);
    connect(action_registrar, &QAction::triggered, this, &InterfaceGerenciadorAtividades::submiter);
    menu_arquivo->addAction(action_registrar);
    action_export_pdf = new QAction(tr("InterfaceGerenciadorAtividades", "Exportar para PDF"), this);
    connect(action_export_pdf, &QAction::triggered, this, &InterfaceGerenciadorAtividades::exportar_para_pdf);
    menu_arquivo->addAction(action_export_pdf);
    menu_arquivo->addSeparator();
    action_sair = new QAction(tr("InterfaceGerenciadorAtividades", "Sair"), this);
    connect(action_sair, &QAction::triggered, qApp, &QCoreApplication::quit);
    menu_arquivo->addAction(action_sair);
    menu_config = bar->addMenu(tr("InterfaceGerenciadorAtividades", "Configurações"));
    menu_idiomas = menu_config->addMenu(tr("InterfaceGerenciadorAtividades", "Idiomas"));
    lang_action_group = new QActionGroup(this);
    lang_action_group->setExclusive(true);
    for (auto it = gerenciador_traducao->idiomas_disponiveis.cbegin();
         it != gerenciador_traducao->idiomas_disponiveis.cend(); ++it) {
        QAction *action = new QAction(tr("InterfaceGerenciadorAtividades", it.value().toUtf8().constData()), this);
        action->setCheckable(true);
        action->setData(it.key());
        connect(action, &QAction::triggered, this, [this, codigo = it.key()] { change_language(codigo); });
        menu_idiomas->addAction(action);
        lang_action_group->addAction(action);
        lang_actions.insert(it.key(), action);
    }
    if (lang_actions.contains(gerenciador_traducao->obter_idioma_atual()))
        lang_actions.value(gerenciador_traducao->obter_idioma_atual())->setChecked(true);
    menu_excluir_item_listas = menu_config->addMenu(tr("InterfaceGerenciadorAtividades", "Excluir Item"));
    for (const auto &categoria : CATEGORIAS_EXCLUSAO_LISTAS) {
        QMenu *submenu = new QMenu(tr("InterfaceGerenciadorAtividades", categoria.second.toUtf8().constData()),
                                   menu_excluir_item_listas);
        menu_excluir_item_listas->addMenu(submenu);
        connect(submenu, &QMenu::aboutToShow, this, [this, submenu, codigo = categoria.first] {
            popular_menu_itens_personalizados(this, submenu, codigo);
        });
        delete_list_menus.insert(categoria.first, submenu);
        delete_list_action_sources.insert(categoria.first, categoria.second);
    }
    menu_cores = menu_config->addMenu(tr("InterfaceGerenciadorAtividades", "Cores"));
    color_action_group = new QActionGroup(this);
    color_action_group->setExclusive(true);
    for (const auto &par : QList<QPair<QString, QString>>{{"preto", "Preto"}, {"coloridas", "Coloridas"}}) {
        QAction *action = new QAction(tr("InterfaceGerenciadorAtividades", par.second.toUtf8().constData()), this);
        action->setCheckable(true);
        action->setData(par.first);
        connect(action, &QAction::triggered, this, [this, codigo = par.first] { change_color_mode(codigo); });
        menu_cores->addAction(action);
        color_action_group->addAction(action);
        color_actions.insert(par.first, action);
    }
    if (color_actions.contains(gerenciamento_atividades.obter_modo_cores()))
        color_actions.value(gerenciamento_atividades.obter_modo_cores())->setChecked(true);
    menu_sobre = bar->addMenu(tr("InterfaceGerenciadorAtividades", "Sobre"));
    action_sobre = new QAction(tr("InterfaceGerenciadorAtividades", "Sobre"), this);
    connect(action_sobre, &QAction::triggered, this, [this] { exibir_sobre(this); });
    menu_sobre->addAction(action_sobre);
}
