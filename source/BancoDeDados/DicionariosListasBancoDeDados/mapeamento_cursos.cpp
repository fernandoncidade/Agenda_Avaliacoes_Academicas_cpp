#include "mapeamento_cursos.hpp"

const QHash<QString, QHash<QString, QString>> &mapeamento_cursos() {
    static const QHash<QString, QHash<QString, QString>> valores = {
        {QString::fromUtf8(u8"pt_BR"),
         QHash<QString, QString>{{QString::fromUtf8(u8"Engenharia Civil"), QString::fromUtf8(u8"Engenharia Civil")}}},
        {QString::fromUtf8(u8"en_US"),
         QHash<QString, QString>{{QString::fromUtf8(u8"Engenharia Civil"), QString::fromUtf8(u8"Civil Engineering")}}}};
    return valores;
}
