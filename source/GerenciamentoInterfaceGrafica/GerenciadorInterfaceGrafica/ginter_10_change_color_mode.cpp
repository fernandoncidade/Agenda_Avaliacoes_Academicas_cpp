#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"

void InterfaceGerenciadorAtividades::change_color_mode(const QString &codigo_modo) {
    const bool sucesso = gerenciamento_atividades.definir_modo_cores(codigo_modo);
    if (sucesso) {
        update_textbox();
        for (auto it = color_actions.begin(); it != color_actions.end(); ++it) {
            it.value()->setChecked(it.key() == codigo_modo);
        }
        return;
    }
    const QString atual = gerenciamento_atividades.obter_modo_cores();
    if (color_actions.contains(atual))
        color_actions.value(atual)->setChecked(true);
}
