#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/ComboBoxUtils.hpp"

void GerenciamentoAtividades::update_ementa() {
    const QString selected = combo_curso ? combo_curso->currentText() : QString();
    substituir_itens_combo(entry_ementa, QStringList{QString()} + obter_ementas(selected));
    update_semestre();
}
