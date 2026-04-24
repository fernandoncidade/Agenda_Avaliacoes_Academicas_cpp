#include "ginter_12_excluir_item_lista_personalizada.hpp"

#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/BancoDeDados/Listas_Personalizadas.hpp"
#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QAction>
#include <QCoreApplication>
#include <QFile>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
const QList<QPair<QString, QString>> CATEGORIAS_EXCLUSAO_LISTAS = {{"curso", "Curso:"},
                                                                   {"ementa", "Ementa:"},
                                                                   {"semestre", "Semestre:"},
                                                                   {"disciplina", "Disciplina:"},
                                                                   {"turma", "Turma da Disciplina:"},
                                                                   {"tipo", "Tipo de Atividade Avaliativa:"},
                                                                   {"sequencia", "Sequência da Atividade:"}};

namespace {
    void aplicar_icone(QMessageBox &dialog) {
        const QString iconPath = get_icon_path("ReviewsManager.ico");
        if (!iconPath.isEmpty() && QFile::exists(iconPath))
            dialog.setWindowIcon(QIcon(iconPath));
    }

    bool confirmar_exclusao(QWidget *parent, const QString &texto_item) {
        auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
        QMessageBox mensagem(parent);
        aplicar_icone(mensagem);
        mensagem.setIcon(QMessageBox::Question);
        mensagem.setWindowTitle(tr("InterfaceGerenciadorAtividades", "Confirmar exclusão"));
        mensagem.setText(tr("InterfaceGerenciadorAtividades", "Deseja remover o item selecionado?"));
        mensagem.setInformativeText(texto_item);
        mensagem.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mensagem.setDefaultButton(QMessageBox::No);
        if (auto *botao = mensagem.button(QMessageBox::Yes))
            botao->setText(tr("InterfaceGerenciadorAtividades", "Sim"));
        if (auto *botao = mensagem.button(QMessageBox::No))
            botao->setText(tr("InterfaceGerenciadorAtividades", "Não"));
        return mensagem.exec() == QMessageBox::Yes;
    }
} // namespace

void popular_menu_itens_personalizados(InterfaceGerenciadorAtividades *parent, QMenu *menu, const QString &categoria) {
    if (!menu)
        return;
    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    menu->clear();
    const auto itens = listar_itens_personalizados_para_exclusao(categoria, obter_idioma_atual());
    if (itens.isEmpty()) {
        QAction *action =
            menu->addAction(tr("InterfaceGerenciadorAtividades", "Nenhum item personalizado encontrado."));
        action->setEnabled(false);
        return;
    }
    for (const auto &item : itens) {
        QAction *action = menu->addAction(item.texto);
        QObject::connect(action, &QAction::triggered, parent, [parent, menu, item]() {
            auto tr = [](const char *context, const char *source) {
                return QCoreApplication::translate(context, source);
            };
            if (!confirmar_exclusao(parent, item.texto))
                return;
            if (remover_item_personalizado(item.categoria, item.id)) {
                popular_menu_itens_personalizados(parent, menu, item.categoria);
                parent->retranslate_ui();
                QMessageBox::information(parent, tr("InterfaceGerenciadorAtividades", "Item excluído"),
                                         tr("InterfaceGerenciadorAtividades", "O item foi excluído."));
            } else {
                QMessageBox::warning(parent, tr("InterfaceGerenciadorAtividades", "Erro"),
                                     tr("InterfaceGerenciadorAtividades", "Não foi possível excluir o item."));
            }
        });
    }
}
