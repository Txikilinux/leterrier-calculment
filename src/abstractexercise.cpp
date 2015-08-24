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

#include "abstractexercise.h"

AbstractExercise::AbstractExercise(QWidget *parent):
    AbulEduCommonStatesV1(parent),
    m_localDebug(true),
    m_parent(parent),
    m_AireDeJeu(new QGraphicsView()),
    m_sceneAireDeJeu(new QGraphicsScene(this)),
    m_baudruche(0),
    m_depart(0),
    m_imageFond(0),
    m_score(0),
    m_total(0),
    m_resultatEnCours(-1),
    m_cible(-1),
    m_trace(QString()),
    m_leResultat(0)
{
    /* Création de l'aire de jeu et de sa scène */

    m_AireDeJeu->setScene(m_sceneAireDeJeu);
    m_AireDeJeu->setSceneRect(m_AireDeJeu->rect());
    /* On la place sur l'AbulEduExerciceWidgetAireDeTravailV1 par l'intermédiaire d'un QGraphicsProxyWidget,
       qui va permettre de modifier l'ordre d'affichage des objets. La gv_AireDeJeu sera en dessous des autres objets */
    m_proxyAireDeJeu = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->scene()->addWidget(m_AireDeJeu);
    m_proxyAireDeJeu->setZValue(-1);
    m_AireDeJeu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_AireDeJeu->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    /* On lui donne un fond transparent et pas de bordure */
    m_AireDeJeu->setFrameShape(QFrame::NoFrame);
    m_AireDeJeu->setStyleSheet("background-color: rgba(0,0,0,0)");
    m_AireDeJeu->setVisible(true);
    getAbeExerciceAireDeTravailV1()->setStyleSheet("border:8px solid orange;border-radius:18px;");

    connect(getAbeExerciceTelecommandeV1(), SIGNAL(btnTelecommandeAideClicked()), this, SLOT(slotAide()), Qt::UniqueConnection);
    sequenceMachine->start();
}

AbstractExercise::~AbstractExercise()
{

}

void AbstractExercise::clearScene()
{
    m_AireDeJeu->scene()->clear();
}

void AbstractExercise::setDimensionsWidgets(float ratio)
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    /* Placement de AbulEduExerciceWidgetAireDeTravailV1 et AbulEduExerciceWidgetTelecommandeV1.
   Leur taille est recalculée grâce à leur connect avec le signal dimensionsChangees de l'AbulEduPageAccueilV1 */
    getAbeExerciceAireDeTravailV1()->move(0, 0);
    getAbeExerciceTelecommandeV1()->move(1550*ratio, 0);

    /* Dimensionnement et placement de l'aire de jeu
   Les paramètres en dur (250 et 100) sont liés aux contraintes de marges des images de fond */
    int haut  = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->height() - boiteTetes->geometry().height() - 60 * ratio;
    int large = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->width();
    if (m_localDebug){
        ABULEDU_LOG_DEBUG()<<"largeur"<<large<<"hauteur"<<haut;
    }
    m_AireDeJeu->setFixedSize(QSize(large-250 * ratio, haut - 100 * ratio));
    m_AireDeJeu->setSceneRect(m_AireDeJeu->rect());
    m_AireDeJeu->move((250 * ratio) / 2, 100 * ratio);
    m_AireDeJeu->scene()->setSceneRect(m_AireDeJeu->rect());

    /* Placement des têtes */
    boiteTetes->setPos((getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->width() - boiteTetes->geometry().width())/2,
                       getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->height() - boiteTetes->geometry().height() -60*ratio);
}

void AbstractExercise::slotSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }

    getAbeExerciceMessageV1()->setParent(0);
    getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->scene()->addWidget(getAbeExerciceMessageV1());

    abeStateMachineSetOnPeutPresenterSequence(false);
    abeStateMachineSetOnPeutPresenterExercice(false);
    abeStateMachineSetOnPeutPresenterBilanExercice(false);

    /* Choix : le niveau de difficulté est le même pour tous les exercices d'une séquence */
//    setAbeLevel( -> faut voir, quoi ...
//    abeStateMachineSetNbExercices( -> il faudra regarder le nombre de questions

    /* Attention : la ligne ci-dessous peut être déplacée à condition de rester avant l'appel à setDimensionsWidgets(); */
    AbulEduCommonStatesV1::slotSequenceEntered();

    /* Corrections par rapport au fonctionnement général de la machine à états */
    question->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnAide, "enabled",false);
    question->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnCorriger, "enabled",false);
    question->assignProperty(m_leResultat, "enabled", true);
    question->assignProperty(m_leResultat, "focus", true);
    afficheVerificationQuestion->assignProperty(m_leResultat, "enabled", false);
    finVerificationQuestion->addTransition(getAbeExerciceTelecommandeV1()->ui->btnSuivant,SIGNAL(clicked()),initQuestion);

    setDimensionsWidgets();
}

void AbstractExercise::slotRealisationExerciceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    getAbeExerciceMessageV1()->setVisible(false);
    AbulEduCommonStatesV1::slotRealisationExerciceEntered();
}

void AbstractExercise::slotInitQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbulEduCommonStatesV1::slotInitQuestionEntered();
    setAbeExerciceEvaluation(abe::evalY);
    boiteTetes->setEtatTete(m_numExercice, abe::evalY,false,getAbeNbTotalQuestions()-getAbeNumQuestion()+1);
}

void AbstractExercise::slotQuestionEntered()
{
    AbulEduCommonStatesV1::slotQuestionEntered();
}

void AbstractExercise::slotAfficheVerificationQuestionEntered()
{
    AbulEduCommonStatesV1::slotAfficheVerificationQuestionEntered();
    m_leResultat->clearFocus();
}

void AbstractExercise::slotFinVerificationQuestionEntered()
{
//    if(getAbeNumQuestion() == getAbeNbTotalQuestions()) {
//        sequenceMachine->postDelayedEvent(new StringEvent("Questionsdone"),7000);
//    }
//    else {
//        sequenceMachine->postDelayedEvent(new StringEvent("Questionsloop"),7000);
//    }
}

void AbstractExercise::slotAfficheCorrectionQuestionEntered()
{

}

void AbstractExercise::slotFinCorrectionQuestionEntered()
{

}

void AbstractExercise::slotBilanSequenceEntered()
{

}

bool AbstractExercise::eventFilter(QObject *obj, QEvent *event)
{
    return AbulEduCommonStatesV1::eventFilter(obj,event);
}

void AbstractExercise::slotAide()
{

}
