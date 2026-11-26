#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"
#include "source/utils/ComboBoxUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QLocale>

void InterfaceGerenciadorAtividades::retranslate_ui() {
    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    setWindowTitle(tr("InterfaceGerenciadorAtividades", "Cadastro de Atividades Avaliativas"));
    label_curso->setText(tr("InterfaceGerenciadorAtividades", "Curso:"));
    label_ementa->setText(tr("InterfaceGerenciadorAtividades", "Ementa:"));
    label_semestre->setText(tr("InterfaceGerenciadorAtividades", "Semestre:"));
    label_disciplina->setText(tr("InterfaceGerenciadorAtividades", "Disciplina:"));
    label_codigo->setText(tr("InterfaceGerenciadorAtividades", "Turma da Disciplina:"));
    label_tipo->setText(tr("InterfaceGerenciadorAtividades", "Tipo de Atividade Avaliativa:"));
    label_sequencia->setText(tr("InterfaceGerenciadorAtividades", "Sequência da Atividade:"));
    label_data->setText(tr("InterfaceGerenciadorAtividades", "Defina a Data da Atividade:"));
    label_banco_dados->setText(tr("InterfaceGerenciadorAtividades", "Caixa de Dados:"));
    button_clear_item->setText(tr("InterfaceGerenciadorAtividades", "Excluir Item(ns)"));
    button_clear_ultima->setText(tr("InterfaceGerenciadorAtividades", "Limpar Última Entrada"));
    button_clear->setText(tr("InterfaceGerenciadorAtividades", "Limpar Tudo"));
    button_editar_item->setText(tr("InterfaceGerenciadorAtividades", "Editar Item(ns)"));
    button_submiter->setText(tr("InterfaceGerenciadorAtividades", "Registrar Definições"));
    button_export->setText(tr("InterfaceGerenciadorAtividades", "Exportar para PDF"));
    if (menu_arquivo)
        menu_arquivo->setTitle(tr("InterfaceGerenciadorAtividades", "Arquivo"));
    if (menu_config)
        menu_config->setTitle(tr("InterfaceGerenciadorAtividades", "Configurações"));
    if (menu_idiomas)
        menu_idiomas->setTitle(tr("InterfaceGerenciadorAtividades", "Idiomas"));
    if (menu_excluir_item_listas)
        menu_excluir_item_listas->setTitle(tr("InterfaceGerenciadorAtividades", "Excluir Item"));
    for (auto it = delete_list_menus.begin(); it != delete_list_menus.end(); ++it) {
        it.value()->setTitle(
            tr("InterfaceGerenciadorAtividades", delete_list_action_sources.value(it.key()).toUtf8().constData()));
    }
    if (menu_cores)
        menu_cores->setTitle(tr("InterfaceGerenciadorAtividades", "Cores"));
    if (menu_sobre)
        menu_sobre->setTitle(tr("InterfaceGerenciadorAtividades", "Sobre"));
    if (action_excluir_item)
        action_excluir_item->setText(tr("InterfaceGerenciadorAtividades", "Excluir Item(ns)"));
    if (action_limpar_ultima)
        action_limpar_ultima->setText(tr("InterfaceGerenciadorAtividades", "Limpar Última Entrada"));
    if (action_limpar_tudo)
        action_limpar_tudo->setText(tr("InterfaceGerenciadorAtividades", "Limpar Tudo"));
    if (action_editar_item)
        action_editar_item->setText(tr("InterfaceGerenciadorAtividades", "Editar Item(ns)"));
    if (action_registrar)
        action_registrar->setText(tr("InterfaceGerenciadorAtividades", "Registrar Definições"));
    if (action_export_pdf)
        action_export_pdf->setText(tr("InterfaceGerenciadorAtividades", "Exportar para PDF"));
    if (action_sair)
        action_sair->setText(tr("InterfaceGerenciadorAtividades", "Sair"));
    if (action_sobre)
        action_sobre->setText(tr("InterfaceGerenciadorAtividades", "Sobre"));
    if (color_actions.contains("preto"))
        color_actions.value("preto")->setText(tr("InterfaceGerenciadorAtividades", "Preto"));
    if (color_actions.contains("coloridas"))
        color_actions.value("coloridas")->setText(tr("InterfaceGerenciadorAtividades", "Coloridas"));
    if (gerenciador_traducao && gerenciador_traducao->obter_idioma_atual().startsWith("pt")) {
        calendar->setLocale(QLocale(QLocale::Portuguese));
    } else {
        calendar->setLocale(QLocale(QLocale::English));
    }
    const QString cursoAtual = combo_curso->currentText();
    const QString turmaAtual = entry_codigo->currentText();
    const QString tipoAtual = combo_tipo->currentText();
    const QString sequenciaAtual = combo_sequencia->currentText();
    substituir_itens_combo(combo_curso, obter_cursos(), cursoAtual);
    substituir_itens_combo(entry_codigo, obter_turmas(), turmaAtual);
    substituir_itens_combo(combo_tipo, obter_avaliacoes(), tipoAtual);
    substituir_itens_combo(combo_sequencia, obter_sequencias(), sequenciaAtual);
    update_ementa();
    if (gerenciador_traducao) {
        for (auto it = gerenciador_traducao->idiomas_disponiveis.cbegin();
             it != gerenciador_traducao->idiomas_disponiveis.cend(); ++it) {
            if (lang_actions.contains(it.key()))
                lang_actions.value(it.key())->setText(
                    tr("InterfaceGerenciadorAtividades", it.value().toUtf8().constData()));
        }
    }
}
