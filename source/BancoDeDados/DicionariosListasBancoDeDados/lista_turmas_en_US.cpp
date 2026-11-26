#include "lista_turmas_en_US.hpp"

const QStringList &lista_turmas_en_US() {
    static const QStringList valores = {
        QString::fromUtf8(u8""),        QString::fromUtf8(u8"Class A"), QString::fromUtf8(u8"Class B"),
        QString::fromUtf8(u8"Class C"), QString::fromUtf8(u8"Class D"), QString::fromUtf8(u8"Class E"),
        QString::fromUtf8(u8"Class F"), QString::fromUtf8(u8"Class G"), QString::fromUtf8(u8"Class H"),
        QString::fromUtf8(u8"Class I"), QString::fromUtf8(u8"Class J"), QString::fromUtf8(u8"Class K"),
        QString::fromUtf8(u8"Class L"), QString::fromUtf8(u8"Class M"), QString::fromUtf8(u8"Class N"),
        QString::fromUtf8(u8"Class O"), QString::fromUtf8(u8"Class P"), QString::fromUtf8(u8"Class Q"),
        QString::fromUtf8(u8"Class R"), QString::fromUtf8(u8"Class S"), QString::fromUtf8(u8"Class T"),
        QString::fromUtf8(u8"Class U"), QString::fromUtf8(u8"Class V"), QString::fromUtf8(u8"Class W"),
        QString::fromUtf8(u8"Class X"), QString::fromUtf8(u8"Class Y"), QString::fromUtf8(u8"Class Z")};
    return valores;
}
