#include "exerciceoperation.h"

ExerciceOperation::ExerciceOperation(QString exerciseName,QWidget *parent) :
    AbstractExercise(parent),
    m_operationName(exerciseName)
{
    m_localDebug = true;
    qDebug()<<" *********      "<<__FUNCTION__<<exerciseName;
//    m_Fond = gsPrincipale->addPixmap(m_imageFond);
    QPixmap imageFond;
    imageFond.load(":/calculment/backgrounds/multiplication");
    QGraphicsPixmapItem* fond = new QGraphicsPixmapItem();
    fond->setPixmap(imageFond);
    m_sceneAireDeJeu->addItem(fond);
    fond->setZValue(-10);

}

ExerciceOperation::~ExerciceOperation()
{
    //    emit signalExerciseExited();
}

int ExerciceOperation::verifieReponse()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    return 0;
}

int ExerciceOperation::donneReponse()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    return -1;
}

void ExerciceOperation::setDimensionsWidgets(float ratio)
{
    abeApp->abeCenterWindow(m_AireDeJeu,getAbeExerciceAireDeTravailV1());
    AbstractExercise::setDimensionsWidgets(ratio);
}

void ExerciceOperation::slotSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotSequenceEntered();
}

void ExerciceOperation::slotRealisationExerciceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotRealisationExerciceEntered();
}

void ExerciceOperation::slotInitQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotInitQuestionEntered();
}

void ExerciceOperation::slotQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    qDebug()<<" *********      "<<__FUNCTION__;
    AbstractExercise::slotQuestionEntered();
}

void ExerciceOperation::slotAfficheVerificationQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotAfficheVerificationQuestionEntered();
}

void ExerciceOperation::slotFinVerificationQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotFinVerificationQuestionEntered();
}

void ExerciceOperation::slotAfficheCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotAfficheCorrectionQuestionEntered();
}

void ExerciceOperation::slotFinCorrectionQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotFinCorrectionQuestionEntered();
}

void ExerciceOperation::slotBilanSequenceEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotBilanSequenceEntered();
}

bool ExerciceOperation::eventFilter(QObject *obj, QEvent *event)
{
    return AbstractExercise::eventFilter(obj,event);
}

void ExerciceOperation::slotAide()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    AbstractExercise::slotAide();
}
