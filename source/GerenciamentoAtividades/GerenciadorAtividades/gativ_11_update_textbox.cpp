#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/LogManager.hpp"

#include <algorithm>

namespace {
    QDate data_atividade(const Atividade &a) {
        return QDate::fromString(a.data, "dd/MM/yyyy");
    }
} // namespace

void GerenciamentoAtividades::update_textbox() {
    if (!textbox)
        return;
    auto lista = listar_atividades();
    std::sort(lista.begin(), lista.end(),
              [](const Atividade &a, const Atividade &b) { return data_atividade(a) < data_atividade(b); });
    QStringList linhas;
    for (const Atividade &atividade : lista) {
        if (atividade.tipo.isEmpty() || atividade.sequencia.isEmpty() || atividade.nome.isEmpty() ||
            atividade.turma.isEmpty())
            continue;
        QString nome = atividade.nome.toHtmlEscaped();
        QString underlineStyle;
        if (modo_cores != "preto") {
            const QString cor = obter_cor_por_nome(atividade.nome);
            if (!cor.isEmpty()) {
                nome = QString("<span style='color: %1;'>%2</span>").arg(cor, nome);
                underlineStyle = QString("style='text-decoration-color: %1;'").arg(cor);
            }
        }
        linhas << QString("<u %1><b>%2</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%3 – "
                          "%4&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%5&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%6</u>")
                      .arg(underlineStyle, atividade.data.toHtmlEscaped(), atividade.tipo.toHtmlEscaped(),
                           atividade.sequencia.toHtmlEscaped(), nome, atividade.turma.toHtmlEscaped());
    }
    textbox->setHtml(linhas.join("<br><br>"));
}
