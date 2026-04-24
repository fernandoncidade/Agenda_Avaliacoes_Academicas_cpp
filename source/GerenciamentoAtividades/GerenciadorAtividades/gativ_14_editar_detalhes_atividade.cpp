#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/CURSOS_EN.hpp"
#include "source/BancoDeDados/DicionariosListasBancoDeDados/CURSOS_PT.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/ComboBoxUtils.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCalendarWidget>
#include <QComboBox>
#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>

namespace {
    bool disciplina_corresponde(const QString &disciplina, const QString &alvo) {
        if (disciplina.trimmed().compare(alvo.trimmed(), Qt::CaseInsensitive) == 0)
            return true;
        const QRegularExpression rx("([A-Z]{1,3}\\d{3,4})$", QRegularExpression::CaseInsensitiveOption);
        const auto a = rx.match(disciplina.trimmed());
        const auto b = rx.match(alvo.trimmed());
        return a.hasMatch() && b.hasMatch() && a.captured(1).compare(b.captured(1), Qt::CaseInsensitive) == 0;
    }

    QStringList buscar_disciplinas_relacionadas(const QString &nome) {
        if (nome.trimmed().isEmpty())
            return {};
        for (const EstruturaCursos *estrutura : {&obter_estrutura_cursos(), &CURSOS_PT(), &CURSOS_EN()}) {
            for (const auto &curso : *estrutura) {
                for (const auto &ementa : curso.ementas) {
                    for (const auto &semestre : ementa.semestres) {
                        for (const QString &disciplina : semestre.disciplinas) {
                            if (disciplina_corresponde(disciplina, nome)) {
                                return semestre.disciplinas;
                            }
                        }
                    }
                }
            }
        }
        return {nome};
    }
} // namespace

void GerenciamentoAtividades::editar_detalhes_atividade(const Atividade &atividade) {
    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    QDialog dialog;
    const QString iconPath = get_icon_path("ReviewsManager.ico");
    if (!iconPath.isEmpty() && QFile::exists(iconPath))
        dialog.setWindowIcon(QIcon(iconPath));
    dialog.setWindowTitle(tr("InterfaceGerenciadorAtividades", "Editar Detalhes da Atividade"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *labelData = new QLabel(tr("InterfaceGerenciadorAtividades", "Data:"), &dialog);
    auto *entryData = new QCalendarWidget(&dialog);
    if (calendar)
        entryData->setLocale(calendar->locale());
    QDate data = QDate::fromString(atividade.data, "dd/MM/yyyy");
    if (!data.isValid())
        data = QDate::currentDate();
    entryData->setSelectedDate(data);
    layout->addWidget(labelData);
    layout->addWidget(entryData);
    auto *labelTipo = new QLabel(tr("InterfaceGerenciadorAtividades", "Tipo:"), &dialog);
    auto *entryTipo = new QComboBox(&dialog);
    configurar_combo_texto_livre(entryTipo);
    entryTipo->addItems(obter_avaliacoes());
    entryTipo->setCurrentText(atividade.tipo);
    layout->addWidget(labelTipo);
    layout->addWidget(entryTipo);
    auto *labelSequencia = new QLabel(tr("InterfaceGerenciadorAtividades", "Sequência:"), &dialog);
    auto *entrySequencia = new QComboBox(&dialog);
    configurar_combo_texto_livre(entrySequencia);
    entrySequencia->addItems(obter_sequencias());
    entrySequencia->setCurrentText(atividade.sequencia);
    layout->addWidget(labelSequencia);
    layout->addWidget(entrySequencia);
    auto *labelNome = new QLabel(tr("InterfaceGerenciadorAtividades", "Nome:"), &dialog);
    auto *entryNome = new QComboBox(&dialog);
    configurar_combo_texto_livre(entryNome);
    entryNome->addItems(QStringList{QString()} + buscar_disciplinas_relacionadas(atividade.nome));
    entryNome->setCurrentText(atividade.nome);
    layout->addWidget(labelNome);
    layout->addWidget(entryNome);
    auto *labelTurma = new QLabel(tr("InterfaceGerenciadorAtividades", "Turma:"), &dialog);
    auto *entryTurma = new QComboBox(&dialog);
    configurar_combo_texto_livre(entryTurma);
    entryTurma->addItems(obter_turmas());
    entryTurma->setCurrentText(atividade.turma);
    layout->addWidget(labelTurma);
    layout->addWidget(entryTurma);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    if (dialog.exec() == QDialog::Accepted) {
        Atividade novos{entryData->selectedDate().toString("dd/MM/yyyy"), entryTipo->currentText(),
                        entrySequencia->currentText(), entryNome->currentText(), entryTurma->currentText()};
        if (!atualizar_atividade(atividade, novos)) {
            QMessageBox::warning(nullptr, tr("InterfaceGerenciadorAtividades", "Erro"),
                                 tr("InterfaceGerenciadorAtividades", "Erro ao atualizar a atividade."));
        }
    }
}
