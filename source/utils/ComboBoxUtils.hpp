#pragma once

#include <QComboBox>
#include <QStringList>
QString _normalizar_texto(const QString &texto);
void configurar_combo_texto_livre(QComboBox *combo);
QStringList obter_itens_combo(QComboBox *combo);
bool combo_contem_texto(QComboBox *combo, const QString &texto);
void adicionar_item_combo_se_ausente(QComboBox *combo, const QString &texto);
void substituir_itens_combo(QComboBox *combo, const QStringList &itens, const QString &texto_atual = QString());
