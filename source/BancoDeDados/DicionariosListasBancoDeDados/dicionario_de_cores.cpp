#include "dicionario_de_cores.hpp"

const QHash<QString, QString> &dicionario_de_cores() {
    static const QHash<QString, QString> valores = {
        {QString::fromUtf8(u8"1ª Coloracao Disciplina"), QString::fromUtf8(u8"#FF0000")},
        {QString::fromUtf8(u8"2ª Coloracao Disciplina"), QString::fromUtf8(u8"#0000FF")},
        {QString::fromUtf8(u8"3ª Coloracao Disciplina"), QString::fromUtf8(u8"#008000")},
        {QString::fromUtf8(u8"4ª Coloracao Disciplina"), QString::fromUtf8(u8"#808000")},
        {QString::fromUtf8(u8"5ª Coloracao Disciplina"), QString::fromUtf8(u8"#800080")},
        {QString::fromUtf8(u8"6ª Coloracao Disciplina"), QString::fromUtf8(u8"#FFA500")},
        {QString::fromUtf8(u8"7ª Coloracao Disciplina"), QString::fromUtf8(u8"#A52A2A")},
        {QString::fromUtf8(u8"8ª Coloracao Disciplina"), QString::fromUtf8(u8"#FF1493")},
        {QString::fromUtf8(u8"9ª Coloracao Disciplina"), QString::fromUtf8(u8"#008080")},
        {QString::fromUtf8(u8"10ª Coloracao Disciplina"), QString::fromUtf8(u8"#FF00FF")},
        {QString::fromUtf8(u8"11ª Coloracao Disciplina"), QString::fromUtf8(u8"#2F4F4F")},
        {QString::fromUtf8(u8"12ª Coloracao Disciplina"), QString::fromUtf8(u8"#00FFFF")},
        {QString::fromUtf8(u8"13ª Coloracao Disciplina"), QString::fromUtf8(u8"#FFD700")},
        {QString::fromUtf8(u8"14ª Coloracao Disciplina"), QString::fromUtf8(u8"#4B0082")},
        {QString::fromUtf8(u8"15ª Coloracao Disciplina"), QString::fromUtf8(u8"#00FF00")},
        {QString::fromUtf8(u8"16ª Coloracao Disciplina"), QString::fromUtf8(u8"#FF4500")},
        {QString::fromUtf8(u8"17ª Coloracao Disciplina"), QString::fromUtf8(u8"#DA70D6")},
        {QString::fromUtf8(u8"18ª Coloracao Disciplina"), QString::fromUtf8(u8"#87CEEB")},
        {QString::fromUtf8(u8"19ª Coloracao Disciplina"), QString::fromUtf8(u8"#4682B4")},
        {QString::fromUtf8(u8"20ª Coloracao Disciplina"), QString::fromUtf8(u8"#D2691E")},
        {QString::fromUtf8(u8"21ª Coloracao Disciplina"), QString::fromUtf8(u8"#B22222")},
        {QString::fromUtf8(u8"22ª Coloracao Disciplina"), QString::fromUtf8(u8"#FF6347")},
        {QString::fromUtf8(u8"23ª Coloracao Disciplina"), QString::fromUtf8(u8"#40E0D0")},
        {QString::fromUtf8(u8"24ª Coloracao Disciplina"), QString::fromUtf8(u8"#9ACD32")}};
    return valores;
}
