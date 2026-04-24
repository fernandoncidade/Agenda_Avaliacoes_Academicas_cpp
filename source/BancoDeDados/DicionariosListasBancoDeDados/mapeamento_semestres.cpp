#include "mapeamento_semestres.hpp"

const QHash<QString, QHash<QString, QString>> &mapeamento_semestres() {
    static const QHash<QString, QHash<QString, QString>> valores = {
        {QString::fromUtf8(u8"pt_BR"),
         QHash<QString, QString>{
             {QString::fromUtf8(u8"1º Semestre"), QString::fromUtf8(u8"1º Semestre")},
             {QString::fromUtf8(u8"2º Semestre"), QString::fromUtf8(u8"2º Semestre")},
             {QString::fromUtf8(u8"3º Semestre"), QString::fromUtf8(u8"3º Semestre")},
             {QString::fromUtf8(u8"4º Semestre"), QString::fromUtf8(u8"4º Semestre")},
             {QString::fromUtf8(u8"5º Semestre"), QString::fromUtf8(u8"5º Semestre")},
             {QString::fromUtf8(u8"6º Semestre"), QString::fromUtf8(u8"6º Semestre")},
             {QString::fromUtf8(u8"7º Semestre"), QString::fromUtf8(u8"7º Semestre")},
             {QString::fromUtf8(u8"8º Semestre"), QString::fromUtf8(u8"8º Semestre")},
             {QString::fromUtf8(u8"9º Semestre"), QString::fromUtf8(u8"9º Semestre")},
             {QString::fromUtf8(u8"10º Semestre"), QString::fromUtf8(u8"10º Semestre")},
             {QString::fromUtf8(u8"Optativas (mínimo 6)"), QString::fromUtf8(u8"Optativas (mínimo 6)")}}},
        {QString::fromUtf8(u8"en_US"),
         QHash<QString, QString>{
             {QString::fromUtf8(u8"1º Semestre"), QString::fromUtf8(u8"1st Semester")},
             {QString::fromUtf8(u8"2º Semestre"), QString::fromUtf8(u8"2nd Semester")},
             {QString::fromUtf8(u8"3º Semestre"), QString::fromUtf8(u8"3rd Semester")},
             {QString::fromUtf8(u8"4º Semestre"), QString::fromUtf8(u8"4th Semester")},
             {QString::fromUtf8(u8"5º Semestre"), QString::fromUtf8(u8"5th Semester")},
             {QString::fromUtf8(u8"6º Semestre"), QString::fromUtf8(u8"6th Semester")},
             {QString::fromUtf8(u8"7º Semestre"), QString::fromUtf8(u8"7th Semester")},
             {QString::fromUtf8(u8"8º Semestre"), QString::fromUtf8(u8"8th Semester")},
             {QString::fromUtf8(u8"9º Semestre"), QString::fromUtf8(u8"9th Semester")},
             {QString::fromUtf8(u8"10º Semestre"), QString::fromUtf8(u8"10th Semester")},
             {QString::fromUtf8(u8"Optativas (mínimo 6)"), QString::fromUtf8(u8"Electives (minimum 6)")}}}};
    return valores;
}
