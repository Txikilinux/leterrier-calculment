#include "exercicemaisonnombres.h"
#include "ui_exercice.h"
#include "pixmapmaison.h"
#include <QDesktopWidget>

ExerciceMaisonNombres::ExerciceMaisonNombres(QString exo,QWidget *parent,int val, QString niveau)
    :exercice(exo, parent,val,niveau)
{
    m_ui->vue->setInteractive(true);
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    m_depart = new QPoint(m_imgFond->width()/2-80*factY,500*factY);
    m_temps = 10;//provisoirement
//    QGraphicsPixmapItem* maison = new QGraphicsPixmapItem(QPixmap("./data/images/maison2.png"),0,m_scene);
    PixmapMaison* maison = new PixmapMaison(QPixmap("./data/images/maison2.png"));
    maison->setToolTip("Maison du 2");
    maison->setPos(10,10);
    connect(this, SIGNAL(baudrucheLancee()),maison, SLOT(rendSelectionnable()));
    connect(this, SIGNAL(baudrucheDetruite()),maison, SLOT(rendNonSelectionnable()));

    m_scene->addItem(maison);
    connect(m_scene, SIGNAL(selectionChanged()), SLOT(selectionChanged()));
}

void ExerciceMaisonNombres::on_btnBallon_clicked()
{
    qDebug()<<"ExerciceMaisonNombres::on_btnBallon_clicked(1)";
    bool inferieurA11 = false;
    while (!inferieurA11) {
        m_baudruche = new baudruche(0,9,0,9,m_temps,"addition",*m_depart,0);
        this->m_resultatEnCours=m_baudruche->getMResultat();
        qDebug()<<"Ballon créé avec comme résultat "<<m_resultatEnCours;
        if (m_resultatEnCours > 10) {
            qDebug()<<"Je détruis";
            m_baudruche->deleteLater();
        }
        else inferieurA11 = true;
    }
    emit baudrucheLancee();
    m_baudruche->setFlag(QGraphicsItem::ItemIsMovable);
    qDebug()<<"Creation de baudruche avec temps "<<m_temps;
    qDebug()<<" attribut résultat de l'exercice "<<m_resultatEnCours;
    if (m_total<m_nbTotalQuestions - 1) {
        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
        connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
    }
    connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
    connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
    connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(ajouteErreur(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(afficheResultat(QString)));
    connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(pousseLogs(QString)));
    connect(m_baudruche, SIGNAL(lacheIci(QPoint)), this, SLOT(affichePosBaudruche(QPoint)));
//    connect(m_baudruche, SIGNAL(lache()), m_baudruche, SLOT(detruire()));
    m_baudruche->emetRes();
    m_scene->addItem(m_baudruche);

    m_baudruche->m_timer->start();
}

void ExerciceMaisonNombres::affichePosBaudruche(QPoint point)
{
    m_baudruche->detruire();
    emit baudrucheDetruite();
    qDebug()<<"------->>>>>> Ballon laché au point "<<point;
    QGraphicsPixmapItem item(m_scene->itemAt(point));
    qDebug()<<m_scene->items();
    qDebug()<<item.boundingRect();
}

void ExerciceMaisonNombres::selectionChanged() {
  qDebug() << "debug selectionChanged()";
  // Affiche la position de chaque élément de la sélection
  foreach(QGraphicsItem * item, m_scene->selectedItems()) {
    qDebug() << item->scenePos();
  }
  qDebug() << "fin selectionChanged()";
}
