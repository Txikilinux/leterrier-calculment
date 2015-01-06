/** Type particulier d'exercice pour le logiciel Calcul Mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2015 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#ifndef EXERCICEMAISONNOMBRES_H
#define EXERCICEMAISONNOMBRES_H
#include "exerciceoperation.h"

class ExerciceMaisonNombres : public ExerciceOperation
{
    Q_OBJECT
public:
    ExerciceMaisonNombres(QString exo,QWidget *parent = 0,int val = 0, QString niveau = "");
protected:
    int m_valeurSurvolee;
    int m_valeurBase;
    void on_btn2chance_clicked();
    virtual void mousePressEvent(QMouseEvent *);
    void ajouteErreur(QString msg);
    void dessinePixmapMaisons();

signals:
    /** Signal destiné à prévenir qu'une baudruche est lancée
      * Créé pour avoir un comportement d'un autre objet SI une baudruche est lancée
      */
    void baudrucheLancee();

protected slots:
    void affichePosBaudruche(QPoint point);
    void selectionChanged();

    /** Affecte l'image en couleurs inversées pour une PixmapMaison survolée
      * Connecté au signal baudrucheSurvole(QString)
      * @param bulleAide est le toolTip() de la PixmapMaison envoyé par le signal.
      * L'envoi du toolTip plutôt que la QProperty("Valeur") évite un cast dans le mouseMoveEvent de la baudruche
      */
    void trouveMaisonSurvolee(QString bulleAide);

    /** Réinitialise les dessins de PixmapMaisons contenus dans la scène
      * en leur donnant comme image l'image initiale
      * Connecté au signal baudrucheSurvoleRien()
      */
    void zeroMaisonSurvolee();

    /** Réimplémentation destinée à corriger l'accessibilité aux objets de la télécommande */
    virtual void slotSequenceEntered();

    /** Réimplémentation de l'entrée dans l'état "realisationExercice" de l'AbulEduStateMachineV1
      * Il faut pouvoir attendre d'avoir la valeur (déterminée dans l'état presentationExercices) pour dessiner les maisons */
    virtual void slotRealisationExerciceEntered();

    /** Entrée dans l'état "initQuestion" de l'AbulEduStateMachineV1
      * Passage en boucle à chaque nouvelle question
      * Par contre, on n'y repasse pas si on refait la même question */
    virtual void slotInitQuestionEntered();

    /** Appelé par un bouton placé sur l'AbulEduExerciceWidgetMessageV1, ce slot permet d'instancier les objets d'exercices à variantes */
    void slotSetPeculiarity();
};

#endif // EXERCICEMAISONNOMBRES_H
