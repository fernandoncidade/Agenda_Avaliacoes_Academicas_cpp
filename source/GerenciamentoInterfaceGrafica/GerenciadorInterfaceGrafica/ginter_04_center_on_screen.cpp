#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"

#include <QApplication>
#include <QScreen>

void InterfaceGerenciadorAtividades::center_on_screen() {
    if (QScreen *screen = QApplication::primaryScreen()) {
        const QRect geometry = screen->geometry();
        move(geometry.center() - rect().center());
    }
}
