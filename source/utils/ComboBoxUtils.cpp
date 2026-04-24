#include "ComboBoxUtils.hpp"

#include <QLineEdit>
#include <QSignalBlocker>

QString _normalizar_texto(const QString &texto) {
    return texto.trimmed().toCaseFolded();
}

void configurar_combo_texto_livre(QComboBox *combo) {
    if (!combo) {
        return;
    }
    combo->setEditable(true);
    combo->setInsertPolicy(QComboBox::NoInsert);
    if (QLineEdit *edit = combo->lineEdit()) {
        edit->setClearButtonEnabled(true);
    }
}

QStringList obter_itens_combo(QComboBox *combo) {
    QStringList itens;
    if (!combo) {
        return itens;
    }
    for (int i = 0; i < combo->count(); ++i) {
        itens << combo->itemText(i);
    }
    return itens;
}

bool combo_contem_texto(QComboBox *combo, const QString &texto) {
    const QString normalizado = _normalizar_texto(texto);
    if (normalizado.isEmpty()) {
        return true;
    }
    for (const QString &item : obter_itens_combo(combo)) {
        if (_normalizar_texto(item) == normalizado) {
            return true;
        }
    }
    return false;
}

void adicionar_item_combo_se_ausente(QComboBox *combo, const QString &texto) {
    if (!combo) {
        return;
    }
    const QString limpo = texto.trimmed();
    if (limpo.isEmpty()) {
        return;
    }
    QSignalBlocker blocker(combo);
    if (!combo_contem_texto(combo, limpo)) {
        combo->addItem(limpo);
    }
    combo->setCurrentText(limpo);
}

void substituir_itens_combo(QComboBox *combo, const QStringList &itens, const QString &texto_atual) {
    if (!combo) {
        return;
    }
    QSignalBlocker blocker(combo);
    combo->clear();
    combo->addItems(itens);
    if (!texto_atual.trimmed().isEmpty()) {
        adicionar_item_combo_se_ausente(combo, texto_atual);
    }
}
