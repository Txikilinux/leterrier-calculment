#include "exercicerepechage.h"
#include "ui_exercice.h"

const int NBCHIFFRE = 3;
const float SEUIL_NON_ACQUIS=0.4;
const float SEUIL_ACQUIS=0.8;

ExerciceRepechage::ExerciceRepechage(QStringList listeErreurs, int scoreActuel, int total, QString exo,QWidget *parent)
    :exercice(exo, parent)
{
    qDebug()<<"ExerciceRepechage::constructeur(1)";
    m_score = scoreActuel;
    m_total = total;
    m_operation = exo;
    *m_listeEchecs = listeErreurs;
    m_ui->btn2chance->setEnabled(false);
    m_ui->lblPoints->setText(QString::number(m_score));
    m_ui->lblTotal->setText(QString::number(m_total));
}

void ExerciceRepechage::on_btnBallon_clicked()
{
    qDebug()<<"ExerciceRepechage::on_btnBallon_clicked(1)";
    if (!m_ui->lblMsgReponse->isHidden()) m_ui->lblMsgReponse->hide();
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
    QString imageBallon = operationRatee.split(";")[4];
    qDebug()<<operG<<" - "<<signe<<" - "<<operD;
    if (m_operation=="addition") m_depart = new QPoint(0*factX,m_imgFond->height()*0.3*factX);
    else if(m_operation.left(6)=="tableA"|| m_operation.left(6)=="tableM") m_depart = new QPoint(m_imgFond->width()/2-80*factY,0*factY);
    else m_depart = new QPoint(m_imgFond->width()/2-80*factY,500*factY);
    m_baudruche = new baudruche(operG,operD,10,m_operation,*m_depart,this,imageBallon);


    m_scene->addItem(m_baudruche);

    animeBaudruche();

    //accessibilité des boutons
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    connect(m_baudruche->m_timer, SIGNAL(finished()), this, SLOT(ajouteErreur()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
    m_ui->btnFeu->setEnabled(true);
    if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
    m_ui->leResultat->setEnabled(true);
    m_ui->leResultat->setFocus(Qt::OtherFocusReason);
    QRegExp expressionReguliere("[0-9][0-9]{0,"+QString::number(NBCHIFFRE)+"}");
    m_ui->leResultat->setValidator(new QRegExpValidator(expressionReguliere, this));

    }
}

void ExerciceRepechage::on_btnFeu_clicked()
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

void ExerciceRepechage::ajouteErreur()
{
    qDebug()<<"ExerciceRepechage::ajouteErreur(1)";
    if (m_listeEchecs->isEmpty()) m_ui->btnBallon->setEnabled(false);
}
