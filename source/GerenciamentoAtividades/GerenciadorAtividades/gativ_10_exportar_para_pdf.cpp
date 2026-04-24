#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QTextDocument>

#include <algorithm>

namespace {
    QDate data_atividade(const Atividade &a) {
        return QDate::fromString(a.data, "dd/MM/yyyy");
    }
} // namespace

void GerenciamentoAtividades::exportar_para_pdf() {
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Salvar PDF", QString(), "PDF Files (*.pdf)");
    if (filePath.isEmpty())
        return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";
    auto lista = listar_atividades();
    std::sort(lista.begin(), lista.end(),
              [](const Atividade &a, const Atividade &b) { return data_atividade(a) < data_atividade(b); });
    QStringList linhas;
    for (const Atividade &atividade : lista) {
        if (atividade.tipo.isEmpty() || atividade.sequencia.isEmpty() || atividade.nome.isEmpty() ||
            atividade.turma.isEmpty())
            continue;
        QString cor = modo_cores == "preto" ? cor_texto_do_sistema() : obter_cor_por_nome(atividade.nome);
        if (cor.isEmpty())
            cor = "#000000";
        const QString nome =
            QString("<b><span style='color:%1'>%2</span></b>").arg(cor, atividade.nome.toHtmlEscaped());
        linhas << QString("<p style='font-size:10pt; margin-bottom:18pt;'><u "
                          "style='text-decoration-color:%1'><b>%2</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%3 – "
                          "%4&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%5&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%6</u></p>")
                      .arg(cor, atividade.data.toHtmlEscaped(), atividade.tipo.toHtmlEscaped(),
                           atividade.sequencia.toHtmlEscaped(), nome, atividade.turma.toHtmlEscaped());
    }
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::Letter));
    writer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);
    QTextDocument doc;
    doc.setHtml("<html><body>" + linhas.join(QString()) + "</body></html>");
    doc.print(&writer);
    QMessageBox::information(nullptr, "Sucesso", "Atividades exportadas com sucesso para PDF!");
}
