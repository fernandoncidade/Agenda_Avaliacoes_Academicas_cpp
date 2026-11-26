#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCheckBox>
#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QIcon>
#include <QVBoxLayout>

#include <algorithm>

namespace {
    QDate data_atividade_editar(const Atividade &a) {
        return QDate::fromString(a.data, "dd/MM/yyyy");
    }
} // namespace

void GerenciamentoAtividades::editar_item() {
    QDialog dialog;
    const QString iconPath = get_icon_path("ReviewsManager.ico");
    if (!iconPath.isEmpty() && QFile::exists(iconPath))
        dialog.setWindowIcon(QIcon(iconPath));
    dialog.setWindowTitle(QCoreApplication::translate("InterfaceGerenciadorAtividades", "Editar Item(ns)"));
    auto *layout = new QVBoxLayout(&dialog);
    QList<QPair<QCheckBox *, Atividade>> checkboxes;
    auto lista = listar_atividades();
    std::sort(lista.begin(), lista.end(), [](const Atividade &a, const Atividade &b) {
        return data_atividade_editar(a) < data_atividade_editar(b);
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
                editar_detalhes_atividade(pair.second);
            }
        }
        update_textbox();
    }
}
