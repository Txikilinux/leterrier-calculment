/** Exercice de repéchage pour l'exercice Maison des nombres du logiciel Calcul mental
  * L'utilisateur se voit proposer le même type d'exercice que celui depuis
  * lequel il a lancé. Seules lui sont proposées les questions dont la
  * réponse était incorrecte.
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef EXERCICEREPECHAGEMAISONNOMBRES_H
#define EXERCICEREPECHAGEMAISONNOMBRES_H
#include "exercicemaisonnombres.h"
#include "ui_exercice.h"

class ExerciceRepechageMaisonNombres : public ExerciceMaisonNombres
{
public:
    ExerciceRepechageMaisonNombres(QStringList listeErreurs, int tempsVie, int scoreActuel, int total, QString exo, QString nomABE, QString skillABE, int valeur, QWidget *parent=0);
private:
    void on_btnBallon_clicked();
    void on_btnFeu_clicked();
};

#endif // EXERCICEREPECHAGEMAISONNOMBRES_H
