#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <algorithm>

namespace {
    QDate data_atividade_excluir(const Atividade &a) {
        return QDate::fromString(a.data, "dd/MM/yyyy");
    }
} // namespace

void GerenciamentoAtividades::excluir_item() {
    QDialog dialog;
    dialog.setWindowTitle("Excluir Item(ns)");
    auto *layout = new QVBoxLayout(&dialog);
    QList<QPair<QCheckBox *, Atividade>> checkboxes;
    auto lista = listar_atividades();
    std::sort(lista.begin(), lista.end(), [](const Atividade &a, const Atividade &b) {
        return data_atividade_excluir(a) < data_atividade_excluir(b);
    });
    for (const Atividade &atividade : lista) {
        if (atividade.tipo.isEmpty() || atividade.sequencia.isEmpty() || atividade.nome.isEmpty() ||
            atividade.turma.isEmpty())
            continue;
        auto *checkbox = new QCheckBox(atividade_para_linha(atividade), &dialog);
        checkboxes.append({checkbox, atividade});
        layout->addWidget(checkbox);
    }
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    if (dialog.exec() == QDialog::Accepted) {
        for (const auto &pair : checkboxes) {
            if (pair.first->isChecked()) {
                remover_atividade(pair.second);
            }
        }
        update_textbox();
    }
}
