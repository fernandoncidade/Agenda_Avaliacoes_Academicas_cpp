#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/ComboBoxUtils.hpp"

void GerenciamentoAtividades::update_semestre() {
    const QString curso = combo_curso ? combo_curso->currentText() : QString();
    const QString ementa = entry_ementa ? entry_ementa->currentText() : QString();
    substituir_itens_combo(entry_semestre, QStringList{QString()} + obter_semestres(curso, ementa));
    update_disciplinas();
}
