#pragma once

#include <QList>
#include <QString>
#include <QStringList>

struct SemestreCurricular {
    QString nome;
    QStringList disciplinas;
};

struct EmentaCurricular {
    QString nome;
    QList<SemestreCurricular> semestres;
};

struct CursoCurricular {
    QString nome;
    QList<EmentaCurricular> ementas;
};

using EstruturaCursos = QList<CursoCurricular>;
