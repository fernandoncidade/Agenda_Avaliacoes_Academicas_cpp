#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"

#include <QApplication>
#include <QPalette>

QString GerenciamentoAtividades::cor_texto_do_sistema() const {
    if (textbox) {
        return textbox->palette().color(QPalette::Text).name();
    }
    return QApplication::palette().color(QPalette::Text).name();
}
