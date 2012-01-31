#include "exercicerepechagemaisonnombres.h"
#include <QDesktopWidget>

ExerciceRepechageMaisonNombres::ExerciceRepechageMaisonNombres(QStringList listeErreurs, int tempsVie, int scoreActuel, int total, QString exo, QString nomABE, QString skillABE, int valeur, QWidget *parent)
    :ExerciceMaisonNombres(exo,parent)
{
    qDebug()<<"ExerciceRepechageMaisonNombres::constructeur(1)";
    m_score = scoreActuel;
    m_total = total;
    m_operation = exo;
    m_cible = valeur;
    *m_listeEchecs = listeErreurs;
    m_temps = tempsVie;
//    m_ui->btn2chance->setEnabled(false);
//    m_ui->lblPoints->setText(QString::number(m_score));
//    m_ui->lblTotal->setText(QString::number(m_total));
    this->setAbeExerciceName(nomABE);
    this->setAbeSkill(skillABE);
    this->setWindowTitle(getAbeExerciceName());
    qDebug()<<"ExerciceRepechage::constructeur(2) "<<getAbeExerciceName();
}

void ExerciceRepechageMaisonNombres::on_btnBallon_clicked()
{
    qDebug()<<"ExerciceRepechageMaisonNombres::on_btnBallon_clicked(1)";
//    if (!m_ui->lblMsgReponse->isHidden()) {
//        m_ui->lblMsgReponse->hide();
//        m_ui->lblMsgReponse->setText(trUtf8("Réponse exacte : "));
//    }
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    if (!m_listeEchecs->isEmpty())
    {
    QString operationRatee = m_listeEchecs->takeFirst();
    qDebug()<<"Premier élément de m_listeEchecs : "<<operationRatee;
    float operG = operationRatee.split(";")[0].toFloat();
    QString signe = operationRatee.split(";")[1];
    float operD = operationRatee.split(";")[2].toFloat();
    m_resultatEnCours = operationRatee.split(";")[3].toFloat();
//    QString imageBallon = operationRatee.split(";")[4];
    qDebug()<<operG<<" - "<<signe<<" - "<<operD;
    m_depart = new QPoint(m_imgFond->width()/2-80*factY,500*factY);
    m_baudruche = new baudruche(operG,operD,m_temps,"addition",*m_depart,m_scene);
//    qDebug()<<"Ballon créé avec comme résultat "<<m_resultatEnCours<<" et temps de vie "<<m_temps;
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    m_scene->addItem(m_baudruche);
    m_baudruche->m_timer->start();
    }
}
