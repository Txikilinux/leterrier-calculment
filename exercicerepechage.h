#ifndef EXERCICEREPECHAGE_H
#define EXERCICEREPECHAGE_H
#include "exercice.h"
#include <QDesktopWidget>
#include <QGraphicsItemAnimation>

class ExerciceRepechage : public exercice
{
public:
    ExerciceRepechage(QStringList listeErreurs, int scoreActuel, int total, QString exo,QWidget *parent=0);
private:
    void on_btnBallon_clicked();
    void on_btnFeu_clicked();
    void ajouteErreur();
};

#endif // EXERCICEREPECHAGE_H
