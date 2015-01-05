#include "exercicemaisonnombres.h"
#include "ui_exercice.h"
#include "exercicerepechagemaisonnombres.h"
#include "pixmapmaison.h"
#include <QDesktopWidget>

ExerciceMaisonNombres::ExerciceMaisonNombres(QString exo,QWidget *parent,int val, QString niveau)
    :ExerciceOperation(exo, parent,val,niveau)
{
    m_AireDeJeu->setInteractive(true);
    m_operationName = exo;
    m_valeurBase = val;
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    m_depart = new QPoint(m_imageFond->width()/2-80*factY,500*factY);
    qDebug() <<"Opération : "<<m_operationName<<", valeur passée : "<<m_cible<<" et niveau : "<<getAbeLevel();
    chargerParametres();
    qDebug()<<"Apres chargement des parametres, m_temps vaut "<<m_temps;
    int ordonneMaison = 0;
    int nombreMaisons = 5;
    for (int i=1;i<=10;i++)
    {
        QPixmap dessinBouton (":/calculment/elements/maison"+QString::number(i+m_valeurBase));
        QPixmap dessinBouton2 = dessinBouton.scaled(dessinBouton.width()*factX, dessinBouton.height()*factY, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        PixmapMaison* maison = new PixmapMaison(dessinBouton2);
        maison->setToolTip("Maison du "+QString::number(i+m_valeurBase));
        maison->setProperty("Valeur",i+m_valeurBase);
        ordonneMaison = qFloor((i-1)/2)*(dessinBouton2.height()-1 + ((m_imageFond->height()-nombreMaisons*dessinBouton2.height())/nombreMaisons-1));
        maison->setPos(((1+qPow(-1,i))/2)*(m_imageFond->width() - dessinBouton2.width()),ordonneMaison);
        m_sceneAireDeJeu->addItem(maison);
    }
}

void ExerciceMaisonNombres::on_btn2chance_clicked()
{
    qDebug()<<"ExerciceMaisonNombres::on_btn2chance_clicked()";
    ExerciceRepechageMaisonNombres* essaieEncore = new ExerciceRepechageMaisonNombres(m_listeEchecs, m_temps, m_score, m_total, m_operationName,getAbeExerciceName(), getAbeSkill(),m_cible);
    this->deleteLater();
}

void ExerciceMaisonNombres::affichePosBaudruche(QPoint point)
{
    qDebug()<<"ExerciceMaisonNombres::affichePosBaudruche("<<point<<")";

    if (m_sceneAireDeJeu->itemAt(point,QTransform())!=0)
    {
        m_leResultat->setText(QString::number(m_valeurSurvolee));
        /** Je dois remplacer l'appel de on_btnFeu_clicked();
         *  pour pouvoir appeler click(), il faut que je rende le bouton Enabled le temps du clic, puis je le remets comme il était */
        bool isVerifierEnabled = getAbeExerciceTelecommandeV1()->ui->btnVerifier->isEnabled();
        getAbeExerciceTelecommandeV1()->ui->btnVerifier->setEnabled(true);
        getAbeExerciceTelecommandeV1()->ui->btnVerifier->click();
        getAbeExerciceTelecommandeV1()->ui->btnVerifier->setEnabled(isVerifierEnabled);
        //    qDebug()<<"ExerciceMaisonNombres::affichePosBaudruche : "<<point<<" , Valeur recue : "<<m_scene->itemAt(point)->toolTip()<<" Valeur affichee "<<m_ui->leResultat->text();
    }
}

/* sans doute scorie d'un ancien essai, à voir si on peut supprimer */
void ExerciceMaisonNombres::selectionChanged() {
    qDebug() << "debug selectionChanged()";
    // Affiche la position de chaque élément de la sélection
    foreach(QGraphicsItem * item, m_sceneAireDeJeu->selectedItems()) {
        qDebug() << item->scenePos();
    }
    qDebug() << "fin selectionChanged()";
}

void ExerciceMaisonNombres::mousePressEvent(QMouseEvent *)
{
    /** @todo ce mousePressEvent() n'est jamais appelé... l'événement est-il capté ailleurs ?! Mais est-ce utile qu'il soit appelé ? */
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
    }
    if (getAbeExerciceTelecommandeV1()->ui->btnSuivant->isEnabled()){
        sequenceMachine->postEvent(new StringEvent("finAfficheVerification"));
    }
}

void ExerciceMaisonNombres::ajouteErreur(QString msg)
{
    ExerciceOperation::ajouteErreur(msg);
}

void ExerciceMaisonNombres::trouveMaisonSurvolee(QString bulleAide)
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    zeroMaisonSurvolee();
    foreach(QGraphicsItem * item, m_sceneAireDeJeu->items())
    {
        PixmapMaison* itemMaison = static_cast<PixmapMaison*>(item);
        if (itemMaison->toolTip() == bulleAide)
        {
            QPixmap dessinBouton (":/calculment/elements/maison"+QString::number(itemMaison->property("Valeur").toInt())+"b");
            QPixmap dessinBouton2 = dessinBouton.scaled(dessinBouton.width()*factX, dessinBouton.height()*factY, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            itemMaison->setPixmap(dessinBouton2);
            m_valeurSurvolee = itemMaison->property("Valeur").toInt();
        }
    }
}

void ExerciceMaisonNombres::zeroMaisonSurvolee()
{
    qDebug()<<"ExerciceMaisonNombres::zeroMaisonSurvolee(1)";
    m_valeurSurvolee = 0;
    foreach(QGraphicsItem * item, m_sceneAireDeJeu->items())
    {
        if (item->toolTip().left(6) == "Maison")
        {
        PixmapMaison* itemMaison = static_cast<PixmapMaison*>(item);
        itemMaison->setPixmap(itemMaison->getMPixmapInitial());
        }
    }
}

void ExerciceMaisonNombres::slotInitQuestionEntered()
{
    if(m_localDebug){
        ABULEDU_LOG_DEBUG()  << __PRETTY_FUNCTION__;
        ABULEDU_LOG_DEBUG() << sequenceMachine->configuration().toList();
        ABULEDU_LOG_DEBUG() <<m_total<<getAbeNbTotalQuestions()<<getAbeNumQuestion()<<m_score;
    }
    AbstractExercise::slotInitQuestionEntered();
    bool inferieurA11 = false;
    while (!inferieurA11) {
        m_baudruche = new baudruche(0,9,0,9,m_temps,"addition",*m_depart,m_sceneAireDeJeu,"fantome");
        m_valeurSurvolee = 0;
        this->m_resultatEnCours=m_baudruche->getMResultat();
//        qDebug()<<" Ballon créé avec comme résultat "<<m_resultatEnCours<<" et comme parent "<<m_scene<<" euh "<<m_baudruche.data()->parent();
        if (m_resultatEnCours > (10+m_valeurBase) || m_resultatEnCours < (1+m_valeurBase)) {
            m_baudruche->deleteLater();
        }
        else inferieurA11 = true;
    }
    emit baudrucheLancee();
    m_baudruche->setFlag(QGraphicsItem::ItemIsMovable);
//    qDebug()<<"Creation de baudruche avec temps "<<m_temps;
//    qDebug()<<" attribut résultat de l'exercice "<<m_resultatEnCours;
    /** @todo machine à états ... */
//    if (m_total<m_nbTotalQuestions - 1) {
//        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
//        connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
//    }
//    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_leResultat, SLOT(clear()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_leResultat, SLOT(setDisabled(bool)));
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(ajouteErreur(QString)));
    ///connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
    ///connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(afficheResultat(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(pousseLogs(QString)));
    connect(m_baudruche, SIGNAL(lacheIci(QPoint)), this, SLOT(affichePosBaudruche(QPoint)));

    connect(m_baudruche, SIGNAL(baudrucheSurvole(QString)), this, SLOT(trouveMaisonSurvolee(QString)));
    connect(m_baudruche, SIGNAL(baudrucheSurvoleRien()), this, SLOT(zeroMaisonSurvolee()));

    m_baudruche->emetRes();
    m_sceneAireDeJeu->addItem(m_baudruche);

    m_trace = m_baudruche->getMAffichage();
    qDebug()<<"Calcul propose : "<<m_trace;

    //affichage du nombre de ballons déjà instanciés
    ///m_total = m_ui->lblTotal->text().toInt();
    m_total++;
    /** QString monTotal = "";
    monTotal = monTotal.setNum(m_total);
    m_ui->lblTotal->setText(monTotal);
    m_ui->lblMsg->setText("");
    QPixmap* rien = new QPixmap("");
    m_ui->lblImgMsg->setPixmap(*rien); */

    m_baudruche->m_timer->start();
    ///if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
}

void ExerciceMaisonNombres::slotSequenceEntered()
{
    ExerciceOperation::slotSequenceEntered();
    question->assignProperty(getAbeExerciceTelecommandeV1()->ui->btnVerifier, "enabled",false);
    question->assignProperty(m_leResultat, "enabled", false);
}
