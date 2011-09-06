#ifndef EXERCICEMAISONNOMBRES_H
#define EXERCICEMAISONNOMBRES_H
#include "exercice.h"

class ExerciceMaisonNombres : public exercice
{
    Q_OBJECT
public:
    ExerciceMaisonNombres(QString exo,QWidget *parent = 0,int val = 0, QString niveau = "");
signals:
    /** Signal destiné à prévenir qu'une baudruche est lancée
      * Créé pour avoir un comportement d'un autre objet SI une baudruche est lancée
      */
    void baudrucheLancee();
    void baudrucheDetruite();
protected slots:
    void affichePosBaudruche(QPoint point);
    void selectionChanged();
private:
    void on_btnBallon_clicked();
};

#endif // EXERCICEMAISONNOMBRES_H
