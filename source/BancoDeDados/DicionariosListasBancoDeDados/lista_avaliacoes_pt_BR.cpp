#include "lista_avaliacoes_pt_BR.hpp"

const QStringList &lista_avaliacoes_pt_BR() {
    static const QStringList valores = {QString::fromUtf8(u8""),
                                        QString::fromUtf8(u8"AVALIAÇÃO"),
                                        QString::fromUtf8(u8"PROVA"),
                                        QString::fromUtf8(u8"TAREFA"),
                                        QString::fromUtf8(u8"TESTE"),
                                        QString::fromUtf8(u8"2ª CHAMADA"),
                                        QString::fromUtf8(u8"EXAME"),
                                        QString::fromUtf8(u8"TRABALHO"),
                                        QString::fromUtf8(u8"RELATÓRIO"),
                                        QString::fromUtf8(u8"LISTA DE EXERCÍCIOS"),
                                        QString::fromUtf8(u8"EXERCÍCIO"),
                                        QString::fromUtf8(u8"APRESENTAÇÃO"),
                                        QString::fromUtf8(u8"APRESENTAÇÃO ORAL"),
                                        QString::fromUtf8(u8"SEMINÁRIO"),
                                        QString::fromUtf8(u8"KAHOOT"),
                                        QString::fromUtf8(u8"ROCKBOWL"),
                                        QString::fromUtf8(u8"APRENDIZADO BASEADO EM PROBLEMAS (PBL)"),
                                        QString::fromUtf8(u8"APRENDIZADO BASEADO EM PROJETOS (ABP)"),
                                        QString::fromUtf8(u8"APRENDIZADO BASEADO EM INQUÉRITO (IBL)")};
    return valores;
}
