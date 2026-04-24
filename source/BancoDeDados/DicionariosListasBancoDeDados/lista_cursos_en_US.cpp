#include "lista_cursos_en_US.hpp"

const QStringList &lista_cursos_en_US() {
    static const QStringList valores = {QString::fromUtf8(u8""), QString::fromUtf8(u8"Civil Engineering")};
    return valores;
}
