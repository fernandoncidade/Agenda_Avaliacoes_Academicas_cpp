#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"

#include <QShowEvent>

void InterfaceGerenciadorAtividades::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    center_on_screen();
}
