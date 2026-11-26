#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"

#include <QFont>

QPushButton *InterfaceGerenciadorAtividades::create_button() {
    auto *button = new QPushButton(this);
    QFont font = button->font();
    font.setPointSize(10);
    button->setFont(font);
    button->setMinimumWidth(130);
    return button;
}
