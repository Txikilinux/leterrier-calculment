#include "exercicemaisonnombres.h"
#include "ui_exercice.h"
#include "exercicerepechagemaisonnombres.h"
#include "pixmapmaison.h"
#include <QDesktopWidget>

ExerciceMaisonNombres::ExerciceMaisonNombres(QString exo,QWidget *parent,int val, QString niveau)
    :exercice(exo, parent,val,niveau)
{
    m_ui->vue->setInteractive(true);
    m_operation = exo;
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    m_depart = new QPoint(m_imgFond->width()/2-80*factY,500*factY);
    qDebug() <<"Opération : "<<m_operation<<", valeur passée : "<<m_cible<<" et niveau : "<<m_level;
    chargerParametres();
    qDebug()<<"Apres chargement des parametres, m_temps vaut "<<m_temps;
    int ordonneMaison = 0;
    int nombreMaisons = 5;
    for (int i=1;i<=10;i++)
    {

        QPixmap dessinBouton ("./data/images/maison"+QString::number(i)+".png");
        QPixmap dessinBouton2 = dessinBouton.scaled(dessinBouton.width()*factX, dessinBouton.height()*factY, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        PixmapMaison* maison = new PixmapMaison(dessinBouton2);
        maison->setToolTip("Maison du "+QString::number(i));
        ordonneMaison = qFloor((i-1)/2)*(dessinBouton2.height()-1 + ((m_imgFond->height()-nombreMaisons*dessinBouton2.height())/nombreMaisons-1));
        maison->setPos(((1+qPow(-1,i))/2)*(m_imgFond->width() - dessinBouton2.width()),ordonneMaison);
//        connect(this,SIGNAL(baudrucheLancee()), maison, SLOT(peutChangerImage()));
//        connect(this,SIGNAL(baudrucheDetruite()),maison, SLOT(nePeutPasChangerImage()));
        m_scene->addItem(maison);
    }
}

void ExerciceMaisonNombres::on_btnBallon_clicked()
{
    qDebug()<<"ExerciceMaisonNombres::on_btnBallon_clicked(1)";
    m_ui->btnBallon->setText(trUtf8("&Suivant"));
    bool inferieurA11 = false;
    while (!inferieurA11) {
        m_baudruche = new baudruche(0,9,0,9,m_temps,"addition",*m_depart,m_scene);
        this->m_resultatEnCours=m_baudruche->getMResultat();
//        qDebug()<<" Ballon créé avec comme résultat "<<m_resultatEnCours<<" et comme parent "<<m_scene<<" euh "<<m_baudruche.data()->parent();
        if (m_resultatEnCours > 10 || m_resultatEnCours < 1) {
            m_baudruche->deleteLater();
        }
        else inferieurA11 = true;
    }
    emit baudrucheLancee();
    m_baudruche->setFlag(QGraphicsItem::ItemIsMovable);
//    qDebug()<<"Creation de baudruche avec temps "<<m_temps;
//    qDebug()<<" attribut résultat de l'exercice "<<m_resultatEnCours;
    if (m_total<m_nbTotalQuestions - 1) {
        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
        connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
    }
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->leResultat, SLOT(setDisabled(bool)));
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(ajouteErreur(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(afficheResultat(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(pousseLogs(QString)));
    connect(m_baudruche, SIGNAL(lacheIci(QPoint)), this, SLOT(affichePosBaudruche(QPoint)));

    connect(m_baudruche.data(), SIGNAL(baudrucheSurvole(QString)), this, SLOT(trouveMaisonSurvolee(QString)));
    connect(m_baudruche.data(), SIGNAL(baudrucheSurvoleRien()), this, SLOT(zeroMaisonSurvolee()));

    m_baudruche->emetRes();
    m_scene->addItem(m_baudruche);

    *m_trace=m_baudruche->getMAffichage();
    qDebug()<<"Calcul propose : "<<*m_trace;

    //affichage du nombre de ballons déjà instanciés
    m_total = m_ui->lblTotal->text().toInt();
    m_total++;
    QString monTotal = "";
    monTotal = monTotal.setNum(m_total);
    m_ui->lblTotal->setText(monTotal);
    m_ui->lblMsg->setText("");
    QPixmap* rien = new QPixmap("");
    m_ui->lblImgMsg->setPixmap(*rien);


    m_baudruche->m_timer->start();
    if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
}

void ExerciceMaisonNombres::on_btn2chance_clicked()
{
    qDebug()<<"ExerciceMaisonNombres::on_btn2chance_clicked()";
    ExerciceRepechageMaisonNombres* essaieEncore = new ExerciceRepechageMaisonNombres(*m_listeEchecs, m_temps, m_score, m_total, m_operation,getAbeExerciceName(), getAbeSkill(),m_cible);
    essaieEncore->show();
    this->deleteLater();
}

void ExerciceMaisonNombres::affichePosBaudruche(QPoint point)
{
    qDebug()<<"ExerciceMaisonNombres::affichePosBaudruche("<<point<<")";

    if (m_scene->itemAt(point)!=0)
    {
        qDebug()<<"L107 Je suis sur l'objet "<<m_scene->itemAt(point);
        if (m_baudruche.data()->getMDropValeur().right(1) == "0")
            m_ui->leResultat->setText(m_baudruche.data()->getMDropValeur().right(2));
        else m_ui->leResultat->setText(m_baudruche.data()->getMDropValeur().right(1));
        on_btnFeu_clicked();
        //    m_baudruche->detruire();
            emit baudrucheDetruite();

        //    qDebug()<<"ExerciceMaisonNombres::affichePosBaudruche : "<<point<<" , Valeur recue : "<<m_scene->itemAt(point)->toolTip()<<" Valeur affichee "<<m_ui->leResultat->text();
    }
}

/* sans doute scorie d'un ancien essai, à voir si on peut supprimer */
void ExerciceMaisonNombres::selectionChanged() {
    qDebug() << "debug selectionChanged()";
    // Affiche la position de chaque élément de la sélection
    foreach(QGraphicsItem * item, m_scene->selectedItems()) {
        qDebug() << item->scenePos();
    }
    qDebug() << "fin selectionChanged()";
}

void ExerciceMaisonNombres::mousePressEvent(QMouseEvent *)
{
    if (m_ui->btnBallon->isEnabled())
    {
        on_btnBallon_clicked();
    }
}

void ExerciceMaisonNombres::ajouteErreur(QString msg)
{
    exercice::ajouteErreur(msg);
    emit baudrucheDetruite();
}

void ExerciceMaisonNombres::trouveMaisonSurvolee(QString bulleAide)
{
    zeroMaisonSurvolee();
    foreach(QGraphicsItem * item, m_scene->items())
    {
        PixmapMaison* itemMaison = static_cast<PixmapMaison*>(item);
        if (itemMaison->toolTip() == bulleAide) itemMaison->setPixmap(QPixmap("./data/images/maison"+itemMaison->toolTip().right(1)+"b.png"));
    }

}

void ExerciceMaisonNombres::zeroMaisonSurvolee()
{
    qDebug()<<"ExerciceMaisonNombres::zeroMaisonSurvolee(1)";
    foreach(QGraphicsItem * item, m_scene->items())
    {
        if (item->toolTip().left(6) == "Maison")
        {
        PixmapMaison* itemMaison = static_cast<PixmapMaison*>(item);
        itemMaison->setPixmap(itemMaison->getMPixmapInitial());
        }
    }
}
