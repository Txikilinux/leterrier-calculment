#include "abstractexercise.h"

AbstractExercise::AbstractExercise(QWidget *parent):
    AbulEduCommonStatesV1(parent),
    m_localDebug(true),
    m_parent(parent)
{
    /* Création de l'aire de jeu et de sa scène */
    m_AireDeJeu = new QGraphicsView();
    m_sceneAireDeJeu = new QGraphicsScene(this);
    m_AireDeJeu->setScene(m_sceneAireDeJeu);
    m_AireDeJeu->setSceneRect(m_AireDeJeu->rect());
    /* On la place sur l'AbulEduExerciceWidgetAireDeTravailV1 par l'intermédiaire d'un QGraphicsProxyWidget,
       qui va permettre de modifier l'ordre d'affichage des objets. La gv_AireDeJeu sera en dessous des autres objets */
    m_proxyAireDeJeu = getAbeExerciceAireDeTravailV1()->ui->gvPrincipale->scene()->addWidget(m_AireDeJeu);
    m_proxyAireDeJeu->setZValue(-1);
    m_AireDeJeu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_AireDeJeu->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    /* On lui donne un fond transparent et pas de bordure */
    m_AireDeJeu->setStyleSheet("background-color: rgba(0,0,0,0)");
    m_AireDeJeu->setFrameShape(QFrame::NoFrame);
    m_AireDeJeu->setVisible(true);

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

    abeStateMachineSetOnPeutPresenterSequence(true);
    abeStateMachineSetOnPeutPresenterExercice(false);
    abeStateMachineSetOnPeutPresenterBilanExercice(false);

    /* Choix : le niveau de difficulté est le même pour tous les exercices d'une séquence */
//    setAbeLevel( -> faut voir, quoi ...
//    abeStateMachineSetNbExercices( -> il faudra regarder le nombre de questions

    /* Attention : la ligne ci-dessous peut être déplacée à condition de rester avant l'appel à setDimensionsWidgets(); */
    AbulEduCommonStatesV1::slotSequenceEntered();
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

}

void AbstractExercise::slotFinVerificationQuestionEntered()
{

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
    return QObject::eventFilter(obj,event);
}

void AbstractExercise::slotAide()
{

}
