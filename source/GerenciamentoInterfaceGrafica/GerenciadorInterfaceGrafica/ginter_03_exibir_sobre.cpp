#include "ginter_03_exibir_sobre.hpp"

#include "ginter_01_SobreDialog.hpp"
#include "ginter_02_OpcoesSobre.hpp"
#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QMessageBox>

namespace {
    QString get_text_sobre(const char *text) {
        return QCoreApplication::translate("InterfaceGrafica", text);
    }
} // namespace

void exibir_sobre(InterfaceGerenciadorAtividades *app) {
    if (!app || !app->gerenciador_traducao) {
        QMessageBox::information(app, "Sobre", "Informação não disponível");
        return;
    }
    auto tr = [](const char *context, const char *source) { return QCoreApplication::translate(context, source); };
    const QString idioma = app->gerenciador_traducao->obter_idioma_atual();
    const bool pt = idioma == "pt_BR";
    const QString descricao =
        pt ? QString("A Agenda de Avaliações Acadêmicas é um aplicativo leve para Windows que ajuda estudantes e "
                     "professores a organizar e visualizar todas as avaliações do semestre (provas, testes, "
                     "trabalhos, apresentações etc.) em um único lugar. Permite cadastrar datas e detalhes das "
                     "atividades, ver as tarefas em ordem cronológica, destacar itens por cor para identificação "
                     "rápida, filtrar e editar entradas, e exportar listas em PDF para impressão ou compartilhamento. "
                     "A interface é simples, rápida de consultar e pode ser usada em Português e "
                     "Inglês.<br><br><b>%1</b><ul><li><a href='https://apps.microsoft.com/detail/9NN8Z5Z700TM'>Linceu "
                     "Lighthouse</a></li><li><a href='https://apps.microsoft.com/detail/9PJMT90R953K'>Compression "
                     "Manager</a></li><li><a href='https://apps.microsoft.com/detail/9P289X0185C3'>Eisenhower "
                     "Organizer</a></li></ul><p><i>Versão Gratuíta, permitida compartilhação!</i></p>")
                 .arg(get_text_sobre("Adquira também os meus seguintes aplicativos:"))
           : QString("The Academic Evaluation Schedule is a lightweight Windows application that helps students and "
                     "teachers organize and view all semester assessments (exams, quizzes, assignments, "
                     "presentations, etc.) in one place. You can register dates and details for activities, see them "
                     "in chronological order, highlight items with colors for quick recognition, filter and edit "
                     "entries, and export lists to PDF for printing or sharing. The interface is simple, fast to "
                     "consult and available in Portuguese and English.<br><br><b>%1</b><ul><li><a "
                     "href='https://apps.microsoft.com/detail/9NN8Z5Z700TM'>Linceu Lighthouse</a></li><li><a "
                     "href='https://apps.microsoft.com/detail/9PJMT90R953K'>Compression Manager</a></li><li><a "
                     "href='https://apps.microsoft.com/detail/9P289X0185C3'>Eisenhower "
                     "Organizer</a></li></ul><p><i>Free version, sharing allowed!</i></p>")
                 .arg(get_text_sobre("Adquira também os meus seguintes aplicativos:"));
    SobreDialogTextos textos;
    textos.titulo = QString("%1 - %2").arg(get_text_sobre("Sobre"),
                                           tr("InterfaceGerenciadorAtividades", "AGENDA DE AVALIAÇÕES ACADÊMICAS"));
    textos.cabecalho_fixo =
        QString(
            "<h3>%1</h3><p><b>%2:</b> 2026.4.24.0</p><p><b>%3:</b> Fernando Nillsson Cidade</p><p><b>%4:</b> %5</p>")
            .arg(tr("InterfaceGerenciadorAtividades", "AGENDA DE AVALIAÇÕES ACADÊMICAS"), get_text_sobre("Versão"),
                 get_text_sobre("Autores"), get_text_sobre("Descrição"), descricao);
    textos.texto_history = pt ? History_APP_pt_BR() : History_APP_en_US();
    textos.detalhes = pt ? ABOUT_TEXT_PT_BR() : ABOUT_TEXT_EN_US();
    textos.licencas = pt ? LICENSE_TEXT_PT_BR() : LICENSE_TEXT_EN_US();
    textos.sites_licencas = SITE_LICENSES();
    textos.avisos = pt ? NOTICE_TEXT_PT_BR() : NOTICE_TEXT_EN_US();
    textos.privacy_policy = pt ? Privacy_Policy_pt_BR() : Privacy_Policy_en_US();
    textos.release_notes = pt ? RELEASE_NOTES_pt_BR() : RELEASE_NOTES_en_US();
    textos.show_history_text = get_text_sobre("Histórico");
    textos.hide_history_text = get_text_sobre("Ocultar histórico");
    textos.show_details_text = get_text_sobre("Detalhes");
    textos.hide_details_text = get_text_sobre("Ocultar detalhes");
    textos.show_licenses_text = get_text_sobre("Licenças");
    textos.hide_licenses_text = get_text_sobre("Ocultar licenças");
    textos.show_notices_text = get_text_sobre("Avisos");
    textos.hide_notices_text = get_text_sobre("Ocultar avisos");
    textos.show_privacy_policy_text = get_text_sobre("Política de Privacidade");
    textos.hide_privacy_policy_text = get_text_sobre("Ocultar política de privacidade");
    textos.show_release_notes_text = get_text_sobre("Notas de versão");
    textos.hide_release_notes_text = get_text_sobre("Ocultar notas de versão");
    textos.ok_text = tr("Dialog", "OK");
    textos.site_oficial_text = get_text_sobre("Site oficial");
    textos.info_not_available_text = get_text_sobre("Informação não disponível");
    SobreDialog dialog(textos, app);
    dialog.exec();
}
