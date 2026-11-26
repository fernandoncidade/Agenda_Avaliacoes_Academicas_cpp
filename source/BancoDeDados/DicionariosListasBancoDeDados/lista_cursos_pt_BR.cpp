#include "lista_cursos_pt_BR.hpp"

const QStringList &lista_cursos_pt_BR() {
    static const QStringList valores = {QString::fromUtf8(u8""), QString::fromUtf8(u8"Engenharia Civil")};
    return valores;
}
