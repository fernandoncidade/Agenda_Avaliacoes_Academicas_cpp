#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/ComboBoxUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QMessageBox>

namespace {
    QString texto_combo(QComboBox *combo) {
        return combo ? combo->currentText().trimmed() : QString();
    }
} // namespace

void GerenciamentoAtividades::submiter() {
    const QString data =
        calendar ? calendar->selectedDate().toString("dd/MM/yyyy") : QDate::currentDate().toString("dd/MM/yyyy");
    const QString curso = texto_combo(combo_curso);
    const QString ementa = texto_combo(entry_ementa);
    const QString semestre = texto_combo(entry_semestre);
    const QString tipo = texto_combo(combo_tipo);
    const QString sequencia = texto_combo(combo_sequencia);
    const QString nome = texto_combo(entry_disciplina);
    const QString turma = texto_combo(entry_codigo);
    if (tipo.isEmpty() || sequencia.isEmpty() || nome.isEmpty() || turma.isEmpty()) {
        QMessageBox::warning(nullptr, "Erro",
                             "Por favor, preencha todas as informações antes de adicionar a atividade.");
        return;
    }
    registrar_valores_personalizados(curso, ementa, semestre, nome, turma, tipo, sequencia,
                                     !curso.isEmpty() && !combo_contem_texto(combo_curso, curso),
                                     !curso.isEmpty() && !ementa.isEmpty() && !combo_contem_texto(entry_ementa, ementa),
                                     !curso.isEmpty() && !ementa.isEmpty() && !semestre.isEmpty() &&
                                         !combo_contem_texto(entry_semestre, semestre),
                                     !curso.isEmpty() && !ementa.isEmpty() && !semestre.isEmpty() && !nome.isEmpty() &&
                                         !combo_contem_texto(entry_disciplina, nome),
                                     !turma.isEmpty() && !combo_contem_texto(entry_codigo, turma),
                                     !tipo.isEmpty() && !combo_contem_texto(combo_tipo, tipo),
                                     !sequencia.isEmpty() && !combo_contem_texto(combo_sequencia, sequencia));
    adicionar_item_combo_se_ausente(combo_curso, curso);
    adicionar_item_combo_se_ausente(entry_ementa, ementa);
    adicionar_item_combo_se_ausente(entry_semestre, semestre);
    adicionar_item_combo_se_ausente(entry_disciplina, nome);
    adicionar_item_combo_se_ausente(entry_codigo, turma);
    adicionar_item_combo_se_ausente(combo_tipo, tipo);
    adicionar_item_combo_se_ausente(combo_sequencia, sequencia);
    adicionar_atividade({data, tipo, sequencia, nome, turma});
    update_textbox();
}
