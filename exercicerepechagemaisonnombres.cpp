#include "exercicerepechagemaisonnombres.h"
#include <QDesktopWidget>

ExerciceRepechageMaisonNombres::ExerciceRepechageMaisonNombres(QStringList listeErreurs, int tempsVie, int scoreActuel, int total, QString exo, QString nomABE, QString skillABE, int valeur, QWidget *parent)
    :ExerciceMaisonNombres(exo,parent)
{
    qDebug()<<"ExerciceRepechageMaisonNombres::constructeur(1)";
    qDebug()<<"Parent récupéré : "<<parent;
    m_score = scoreActuel;
    m_total = total;
    m_operation = exo;
    m_cible = valeur;
    *m_listeEchecs = listeErreurs;
    m_temps = tempsVie;
    m_ui->btn2chance->setEnabled(false);
    m_ui->lblPoints->setText(QString::number(m_score));
    m_ui->lblTotal->setText(QString::number(m_total));
    this->setAbeExerciceName(nomABE);
    this->setAbeSkill(skillABE);
    this->setWindowTitle(getAbeExerciceName());
    qDebug()<<"ExerciceRepechage::constructeur(2) "<<getAbeExerciceName();
}

void ExerciceRepechageMaisonNombres::on_btnBallon_clicked()
{
    qDebug()<<"ExerciceRepechageMaisonNombres::on_btnBallon_clicked(1)";
    if (!m_ui->lblMsgReponse->isHidden()) {
        m_ui->lblMsgReponse->hide();
        m_ui->lblMsgReponse->setText(trUtf8("Réponse exacte : "));
    }
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
    qDebug()<<"Ballon créé avec comme résultat "<<m_resultatEnCours<<" et temps de vie "<<m_temps;
    qDebug()<<"Le parent du ballon est "<<m_baudruche.data()->parent();
    m_baudruche->setFlag(QGraphicsItem::ItemIsMovable);
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
//    connect(m_baudruche->m_timer, SIGNAL(finished()), this, SLOT(ajouteErreur()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->leResultat, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(lacheIci(QPoint)), this, SLOT(affichePosBaudruche(QPoint)));
    m_scene->addItem(m_baudruche);
    if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
    m_baudruche->m_timer->start();
    }
    qDebug()<<"ExerciceRepechageMaisonNombres::on_btnBallon_clicked(2)";
}

void ExerciceRepechageMaisonNombres::on_btnFeu_clicked()
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    m_ui->leResultat->setDisabled(true);
    QString evaluation="";
    float proposition = m_ui->leResultat->text().toFloat();

    if (proposition==m_resultatEnCours) {
        m_score++;
        m_ui->lblMsg->setText(tr("GAGNE"));
        QPixmap* imgO = new QPixmap("./data/images/will-win.png");
        imgO->scaledToHeight(imgO->height()*factY);
        m_ui->lblImgMsg->setPixmap(*imgO);
        evaluation="a";
        }
    else {
        m_ui->lblMsg->setText(tr("PERDU"));
        QPixmap* imgN = new QPixmap("./data/images/will-lose.png");
        imgN->scaledToHeight(imgN->height()*factY);
        m_ui->lblImgMsg->setPixmap(*imgN);
        m_ui->lblMsgReponse->show();
        m_ui->lblMsgReponse->setText(m_ui->lblMsgReponse->text()+QString::number(m_resultatEnCours));
        evaluation="d";
        }
    m_ui->lblPoints->setText(QString::number(m_score));

    QString reponseAttendueEnString;
        reponseAttendueEnString.setNum(m_resultatEnCours);
    QString propositionEnString;
        propositionEnString.setNum(proposition);
        setAbeLineLog(m_baudruche->getMLigne(),m_ui->leResultat->text(),m_score, m_total,evaluation,reponseAttendueEnString);
        qDebug()<<getPluginLogs();

    if (m_baudruche!=NULL) m_baudruche->detruire();

    m_ui->btnFeu->setDisabled(true);

    if (m_listeEchecs->isEmpty()) {
        //erics 20110209 sinon coredump en cas de "aucune erreur" ou "que des erreurs"
        //if (m_baudruche) delete m_baudruche;
        afficheResultat("peutImporteCeQuiEstEcritIci");
    }
}
