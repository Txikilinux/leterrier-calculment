#ifndef EXERCICEREPECHAGEMAISONNOMBRES_H
#define EXERCICEREPECHAGEMAISONNOMBRES_H
#include "exercicemaisonnombres.h"

class ExerciceRepechageMaisonNombres : public ExerciceMaisonNombres
{
public:
    ExerciceRepechageMaisonNombres(QStringList listeErreurs, int tempsVie, int scoreActuel, int total, QString exo, QString nomABE, QString skillABE, int valeur, QWidget *parent=0);
private:
    void on_btnBallon_clicked();
};

#endif // EXERCICEREPECHAGEMAISONNOMBRES_H
