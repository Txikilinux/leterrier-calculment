/** Classe de base pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  *
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

#ifndef ABSTRACTEXERCISE_H
#define ABSTRACTEXERCISE_H

#include "abuleducommonstatesv1.h"
#include "baudruche.h"

class AbstractExercise : public AbulEduCommonStatesV1
{
    Q_OBJECT

public:
    AbstractExercise(QWidget *parent);
    virtual ~AbstractExercise();

protected:
    /** Contrôle la possibilité d'affichage des debugs
     * Pour avoir des debugs en sortie, il faut les activer dans le fichier localdebug.pri, faire un qmake et recompiler
     */
    bool m_localDebug;

    /** Parent de l'exercice, écrit dans une variable membre pour être accessible de tous les slots */
    QWidget *m_parent;

    /** L'aire de jeu est une QGraphicsView la plupart du temps, mais vous pouvez la transformer en une autre classe à condition de répercuter partout vos transformations */
    QGraphicsView* m_AireDeJeu;

    /** Pointeur vers la scène de l'attribut gv_AireDeJeu */
    QGraphicsScene *m_sceneAireDeJeu;

    /** Fonction à implémenter pour retirer pour remettre à zéro l'aire de jeu
      * Dans le cas où l'aire de jeu est une QGraphicsView, il s'agit d'enlever les éléments contenus dans sa scène */
    virtual void clearScene();


    /** Dimensionne et place les widgets
      * @param ratio le ratio est par défaut celui calculé dans l'AbulEduApplicationV1 */
    virtual void setDimensionsWidgets(float ratio = abeApp->getAbeApplicationDecorRatio());

    ///
    /// \brief L'objet qui porte l'opération
    ///
    baudruche* m_baudruche;

    ///
    /// \brief Le QPoint* duquel part la baudruche
    ///
    QPoint* m_depart;

    ///
    /// \brief L'image de fond qu'on met à la scène
    ///
    QPixmap* m_imageFond;

    ///
    /// \brief Nombre de réponses correctes
    ///
    int m_score;

    ///
    /// \brief Nombre de questions déjà posées (celle en cours incluse)
    ///
    int m_total;

    ///
    /// \brief Stocke la réponse attendue
    ///
    int m_resultatEnCours;

    ///
    /// \brief Un nombre passé en paramètre. Table de ..., compléments à ...
    ///
    int m_cible;

    ///
    /// \brief L'opération proposée sous forme d'une chaine de caractères
    ///
    QString m_trace;

    ///
    /// \brief QLineEdit ajouté à la télécommande pour écrire les réponses
    ///
    QLineEdit* m_leResultat;

    ///
    /// \brief Opérations dont la réponse est erronée. Leur stockage permet de les reproposer
    ///
    QStringList m_listeEchecs;

    /** QGraphicsProxyWidget de l'aire de jeu
      * Il permet d'intégrer l'aire de jeu dans l'AbulEduExerciceWidgetAireDeTravailV1 */
    QGraphicsProxyWidget *m_proxyAireDeJeu;

protected slots:

    /** Entrée dans l'état "sequence" de l'AbulEduStateMachineV1
      * Passage unique au lancement de la machine à états */
    virtual void slotSequenceEntered();

    /** Entrée dans l'état "realisationExercice" de l'AbulEduStateMachineV1
      * Passage une seule fois au début de chaque exercice */
    virtual void slotRealisationExerciceEntered();

    /** Entrée dans l'état "initQuestion" de l'AbulEduStateMachineV1
      * Passage en boucle à chaque nouvelle question
      * Par contre, on n'y repasse pas si on refait la même question */
    virtual void slotInitQuestionEntered();

    /** Entrée dans l'état "question" de l'AbulEduStateMachineV1
      * Passage une fois au début de chaque question, puis en cas de réponse incorrecte ou clic sur btnRefaire */
    virtual void slotQuestionEntered();

    /** Entrée dans l'état "afficheVerificationQuestion" de l'AbulEduStateMachineV1
      * Passage à chaque clic sur btnVerifier */
    virtual void slotAfficheVerificationQuestionEntered();

    /** Entrée dans l'état "finVerificationQuestion" de l'AbulEduStateMachineV1
      * Passage en sortie de l'état "afficheVerificationQuestion" */
    virtual void slotFinVerificationQuestionEntered();

    /** Entrée dans l'état "afficheCorrectionQuestion" de l'AbulEduStateMachineV1
      * Passage à chaque clic sur btnCorriger */
    virtual void slotAfficheCorrectionQuestionEntered();

    /** Entrée dans l'état "finCorrectionQuestion" de l'AbulEduStateMachineV1
      * Passage en sortie de l'état "afficheCorrectionQuestion" */
    virtual void slotFinCorrectionQuestionEntered();

    //    /** Entrée dans l'état "bilanExercice" de l'AbulEduStateMachineV1
    //      * Passage pour chaque exercice si la variable onPeutPresenterBilanExercice vaut true */
    //    virtual void slotBilanExerciceEntered();

    /** Entrée dans l'état "bilanSequence" de l'AbulEduStateMachineV1
      * @warning Passage unique en fin de séquence */
    virtual void slotBilanSequenceEntered();

    //    /** Connecté au clic sur btnQuitter */
    //    virtual void slotQuitter();

    /** Intercepte les evénénements issus de abeApp
     * Utilisé pour filtrer les appuis sur le clavier (pour l'instant)
     */
    bool eventFilter(QObject *obj, QEvent *event);

    /** Slot destiné à fournir une aide à l'utilisateur lors de la réalisation des exercices */
    virtual void slotAide();

signals:
    /** Émis dans le destructeur pour signaler que l'exercice est terminé. Connecté au slot "abeBackFromExercice" de la LeTerrierMainWindow */
    void signalExerciseExited();
};

#endif // ABSTRACTEXERCISE_H
