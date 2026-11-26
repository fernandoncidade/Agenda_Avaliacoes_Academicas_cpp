#include "lista_avaliacoes_en_US.hpp"

const QStringList &lista_avaliacoes_en_US() {
    static const QStringList valores = {QString::fromUtf8(u8""),
                                        QString::fromUtf8(u8"EVALUATION"),
                                        QString::fromUtf8(u8"EXAM"),
                                        QString::fromUtf8(u8"TASK"),
                                        QString::fromUtf8(u8"TEST"),
                                        QString::fromUtf8(u8"MAKE-UP EXAM"),
                                        QString::fromUtf8(u8"EXAM"),
                                        QString::fromUtf8(u8"ASSIGNMENT"),
                                        QString::fromUtf8(u8"REPORT"),
                                        QString::fromUtf8(u8"EXERCISE LIST"),
                                        QString::fromUtf8(u8"EXERCISE"),
                                        QString::fromUtf8(u8"PRESENTATION"),
                                        QString::fromUtf8(u8"ORAL PRESENTATION"),
                                        QString::fromUtf8(u8"SEMINAR"),
                                        QString::fromUtf8(u8"KAHOOT"),
                                        QString::fromUtf8(u8"ROCKBOWL"),
                                        QString::fromUtf8(u8"PROBLEM BASED LEARNING (PBL)"),
                                        QString::fromUtf8(u8"PROJECT BASED LEARNING (PBL)"),
                                        QString::fromUtf8(u8"INQUIRY BASED LEARNING (IBL)")};
    return valores;
}
