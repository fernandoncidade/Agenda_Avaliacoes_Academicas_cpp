#pragma once

#include <QList>
#include <QPair>
#include <QString>
class InterfaceGerenciadorAtividades;
class QMenu;
extern const QList<QPair<QString, QString>> CATEGORIAS_EXCLUSAO_LISTAS;
void popular_menu_itens_personalizados(InterfaceGerenciadorAtividades *parent, QMenu *menu, const QString &categoria);
