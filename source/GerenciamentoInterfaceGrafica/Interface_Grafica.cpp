#include "Interface_Grafica.hpp"

#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QFile>
#include <QIcon>
#include <QVBoxLayout>
#include <QWidget>

InterfaceGerenciadorAtividades::InterfaceGerenciadorAtividades(GerenciadorTraducao *traducao, QWidget *parent)
    : QMainWindow(parent) {
    if (traducao) {
        gerenciador_traducao = traducao;
    } else {
        gerenciador_traducao_proprio = std::make_unique<GerenciadorTraducao>();
        gerenciador_traducao = gerenciador_traducao_proprio.get();
    }
    gerenciador_traducao->aplicar_traducao();
    connect(gerenciador_traducao, &GerenciadorTraducao::idioma_alterado, this,
            [this](const QString &) { retranslate_ui(); });
    QString iconPath = get_icon_path("ReviewsManager.ico");
    if (iconPath.isEmpty() || !QFile::exists(iconPath)) {
        iconPath = get_icon_path("Reviews-Manager_PySide6-pyppeteer.1.0.ico");
    }
    if (!iconPath.isEmpty() && QFile::exists(iconPath)) {
        setWindowIcon(QIcon(iconPath));
    }
    setWindowTitle(QString());
    auto *central = new QWidget(this);
    setCentralWidget(central);
    auto *layout = new QVBoxLayout(central);
    Q_UNUSED(layout);
    create_widgets();
    create_menu();
    retranslate_ui();
    update_textbox();
}

void InterfaceGerenciadorAtividades::submiter() {
    gerenciamento_atividades.submiter();
}

void InterfaceGerenciadorAtividades::limpar_entradas() {
    gerenciamento_atividades.limpar_entradas();
}

void InterfaceGerenciadorAtividades::limpar_ultima_entrada() {
    gerenciamento_atividades.limpar_ultima_entrada();
}

void InterfaceGerenciadorAtividades::excluir_item() {
    gerenciamento_atividades.excluir_item();
}

void InterfaceGerenciadorAtividades::editar_item() {
    gerenciamento_atividades.editar_item();
}

void InterfaceGerenciadorAtividades::exportar_para_pdf() {
    gerenciamento_atividades.exportar_para_pdf();
}

void InterfaceGerenciadorAtividades::update_textbox() {
    gerenciamento_atividades.update_textbox();
}

void InterfaceGerenciadorAtividades::update_ementa() {
    if (_atualizando_ementa)
        return;
    _atualizando_ementa = true;
    gerenciamento_atividades.update_ementa();
    _atualizando_ementa = false;
}

void InterfaceGerenciadorAtividades::update_semestre() {
    if (_atualizando_semestre)
        return;
    _atualizando_semestre = true;
    gerenciamento_atividades.update_semestre();
    _atualizando_semestre = false;
}

void InterfaceGerenciadorAtividades::update_disciplinas() {
    if (_atualizando_disciplinas)
        return;
    _atualizando_disciplinas = true;
    gerenciamento_atividades.update_disciplinas();
    _atualizando_disciplinas = false;
}
