#include "mapeamento_ementas.hpp"

const QHash<QString, QHash<QString, QString>> &mapeamento_ementas() {
    static const QHash<QString, QHash<QString, QString>> valores = {
        {QString::fromUtf8(u8"pt_BR"),
         QHash<QString, QString>{{QString::fromUtf8(u8"Ementa - 2011"), QString::fromUtf8(u8"Ementa - 2011")},
                                 {QString::fromUtf8(u8"Ementa - 2023"), QString::fromUtf8(u8"Ementa - 2023")}}},
        {QString::fromUtf8(u8"en_US"),
         QHash<QString, QString>{{QString::fromUtf8(u8"Ementa - 2011"), QString::fromUtf8(u8"Syllabus - 2011")},
                                 {QString::fromUtf8(u8"Ementa - 2023"), QString::fromUtf8(u8"Syllabus - 2023")}}}};
    return valores;
}
