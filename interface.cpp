#include "interface.h"
#include "ui_interface.h"
#include "exercice.h"
#include "ui_exercice.h"
#include "editeur.h"
#include "boutonspolygone.h"
#include "dialogapropos.h"

#include <QApplication>
 #include <QDesktopWidget>


interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    ui->setupUi(this);
    QRect ecran;
    ecran=QApplication::desktop()->screenGeometry();

    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf");
    if (!fichierConf->exists()) qDebug()<<QString::fromUtf8(tr("Fichier config NON trouvé").toStdString().c_str());
    else qDebug() << QString::fromUtf8(tr("Fichier config trouvé").toStdString().c_str());
    m_hauteurMax = ecran.height() - 50;
    qDebug() << "Taille ecran : " << ecran.width()<< " X "<<ecran.height();
    this->resize(ecran.width(),ecran.height());
    ui->fete->resize(ecran.width(),m_hauteurMax);
    ui->btnInitialise->setGeometry(ecran.width()-180,10,170,30);

    QPixmap imgFond(QCoreApplication::applicationDirPath()+"/data/images/fondecran.jpg");
    QPixmap imgFond2=imgFond.scaled(ecran.width(),m_hauteurMax,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    qDebug() << "Taille imageFond : " << imgFond.width()<< " X "<<imgFond.height();
    qDebug() << "Taille imageFond2 : " << imgFond2.width()<< " X "<<imgFond2.height();
    QBrush* fond = new QBrush(imgFond2);
    ui->fete->setBackgroundBrush(*fond);

    double kw=static_cast<double>(imgFond2.width())/static_cast<double>(imgFond.width());
    double kh=static_cast<double>(imgFond2.height())/static_cast<double>(imgFond.height());
    qDebug()<<"kw vaut "<<kw;
    qDebug()<<"kh vaut "<<kh;

    QGraphicsScene* dessin = new QGraphicsScene(this);
        ui->fete->setScene(dessin);
        dessin->setSceneRect(0, 0, ecran.width(),m_hauteurMax);

    //ui->btnInitialise->setGeometry(ecran.width()*0.86,0,150,30);

    m_editeur = new Editeur();

    //Bouton sur les auto-tamponneuses
    boutonsPolygone* btnPoly1 = new boutonsPolygone("addition");
        btnPoly1->deplace(0*kw,440*kh);
        btnPoly1->retaille(630*kw,235*kh);
        //btnPoly1->tourne(-20);
        btnPoly1->QGraphicsItem::setToolTip("Faire des additions");
        btnPoly1->setTexte("Additions");
        dessin->addItem(btnPoly1);

    //Bouton sur le manège
    boutonsPolygone* btnPoly2 = new boutonsPolygone("multiplication");
        btnPoly2->deplace(635*kw,550*kh);
        btnPoly2->retaille(430*kw,410*kh);
        btnPoly2->QGraphicsItem::setToolTip("Faire des multiplications");
        btnPoly2->setTexte("Multiplications");
        dessin->addItem(btnPoly2);

    //Bouton sur la barbapapa
    boutonsPolygone* btnPoly3 = new boutonsPolygone("editeur");
        btnPoly3->deplace(60*kw,680*kh);
        btnPoly3->retaille(190*kw,285*kh);
        btnPoly3->QGraphicsItem::setToolTip(QString::fromUtf8(tr("Lancer l'éditeur").toStdString().c_str()));
        dessin->addItem(btnPoly3);

    //Bouton sur le panneau sortie
    boutonsPolygone* btnPoly4 = new boutonsPolygone("sortie");
        btnPoly4->deplace(1325*kw,710*kh);
        btnPoly4->retaille(200*kw,70*kh);
        btnPoly4->QGraphicsItem::setToolTip("Quitter");
        connect(btnPoly4, SIGNAL(sortie()), this, SLOT(close()));
        dessin->addItem(btnPoly4);
        
    //Bouton sur la grande roue
    boutonsPolygone* btnPoly5 = new boutonsPolygone("1tableM");
        btnPoly5->deplace(40*kw,15*kh);
        btnPoly5->retaille(340*kw,365*kh);
        btnPoly5->QGraphicsItem::setToolTip("Tables de multiplication");
        btnPoly5->setTexte("Tables de multiplication");
        dessin->addItem(btnPoly5);

    //Bouton sur le train fantôme
     boutonsPolygone* btnPoly6 = new boutonsPolygone("2complementA");
        btnPoly6->deplace(765*kw,275*kh);
        btnPoly6->retaille(395*kw,200*kh);
        btnPoly6->tourne(12);
        btnPoly6->QGraphicsItem::setToolTip(QString::fromUtf8(tr("Compléments additifs").toStdString().c_str()));
        btnPoly6->setTexte("Complements additifs");
        dessin->addItem(btnPoly6);

    //Bouton sur le stand de tir
    boutonsPolygone* btnPoly7 = new boutonsPolygone("3complementM");
        btnPoly7->deplace(1135*kw,510*kh);
        btnPoly7->retaille(180*kw,120*kh);
        btnPoly7->tourne(10);
        btnPoly7->QGraphicsItem::setToolTip("Multiples");
        btnPoly7->setTexte("Multiples");
        dessin->addItem(btnPoly7);

    //Bouton sur la chenille
    boutonsPolygone* btnPoly8 = new boutonsPolygone("soustraction");
        btnPoly8->deplace(515*kw,220*kh);
        btnPoly8->retaille(220*kw,220*kh);
        btnPoly8->QGraphicsItem::setToolTip(QString::fromUtf8(tr("Soustractions").toStdString().c_str()));
        btnPoly8->setTexte("Soustractions");
        dessin->addItem(btnPoly8);

    //Bouton sur le "rooster"
    boutonsPolygone* btnPoly9 = new boutonsPolygone("4tableA");
        btnPoly9->deplace(425*kw,185*kh);
        btnPoly9->retaille(300*kw,50*kh);
        btnPoly9->tourne(-35);
        btnPoly9->QGraphicsItem::setToolTip("Tables d'addition");
        btnPoly9->setTexte("Tables d'addition");
        dessin->addItem(btnPoly9);

    //Bouton sur le palais des glaces
    boutonsPolygone* btnPoly10 = new boutonsPolygone("5approche",100);
        btnPoly10->deplace(1315*kw,560*kh);
        btnPoly10->retaille(280*kw,150*kh);
        dessin->addItem(btnPoly10);

    }

interface::~interface()
{
    delete ui;
}

void interface::paintEvent(QPaintEvent* e )
{
    QMainWindow::paintEvent(e);
}
void interface::on_actionQuitter_triggered()
{
    this->close();
}

void interface::on_actionAdditions_triggered()
{
    m_exercice = new exercice("addition",0,"",this);
    m_exercice->show();
}

void interface::on_actionMultiplications_triggered()
{
    m_exercice = new exercice("multiplication",0,"",this);
    m_exercice->show();
}

void interface::on_actionp2_triggered()
{
    m_exercice = new exercice("tableA",2,"",this);
    m_exercice->show();
}

void interface::on_actionp3_triggered()
{
    m_exercice = new exercice("tableA",3,"",this);
    m_exercice->show();
}

void interface::on_actionp4_triggered()
{
    m_exercice = new exercice("tableA",4,"",this);
    m_exercice->show();
}

void interface::on_actionp5_triggered()
{
    m_exercice = new exercice("tableA",5,"",this);
    m_exercice->show();
}

void interface::on_actionp6_triggered()
{
    m_exercice = new exercice("tableA",6,"",this);
    m_exercice->show();
}

void interface::on_actionp7_triggered()
{
    m_exercice = new exercice("tableA",7,"",this);
    m_exercice->show();
}

void interface::on_actionp8_triggered()
{
    m_exercice = new exercice("tableA",8,"",this);
    m_exercice->show();
}

void interface::on_actionp9_triggered()
{
    m_exercice = new exercice("tableA",9,"",this);
    m_exercice->show();
}

void interface::on_actionX_2_triggered()
{
    m_exercice = new exercice("tableM",2,"",this);
    m_exercice->show();
}

void interface::on_actionX_3_triggered()
{
    m_exercice = new exercice("tableM",3,"",this);
    m_exercice->show();
}

void interface::on_actionX_4_triggered()
{
    m_exercice = new exercice("tableM",4,"",this);
    m_exercice->show();
}

void interface::on_actionX_5_triggered()
{
    m_exercice = new exercice("tableM",5,"",this);
    m_exercice->show();
}

void interface::on_actionX_6_triggered()
{
    m_exercice = new exercice("tableM",6,"",this);
    m_exercice->show();
}

void interface::on_actionX_7_triggered()
{
    m_exercice = new exercice("tableM",7,"",this);
    m_exercice->show();
}

void interface::on_actionX_8_triggered()
{
    m_exercice = new exercice("tableM",8,"",this);
    m_exercice->show();
}

void interface::on_actionX_9_triggered()
{
    m_exercice = new exercice("tableM",9,"",this);
    m_exercice->show();
}

void interface::on_actionAfficher_l_diteur_triggered()
{
    m_editeur->show();
}

void interface::on_btnInitialise_clicked()
{
    QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf");
    fichierConf->remove();
    m_editeur->initialiser();
}

void interface::on_actionA_propos_triggered()
{
    DialogAPropos* afficheAPropos = new DialogAPropos(this);
    afficheAPropos->setWindowTitle("A propos de Calcul Mental...");
    afficheAPropos->show();
}

void interface::on_actionDe_5_triggered()
{
    m_exercice = new exercice("complementM5", 5,"",this);
    m_exercice->show();
}

void interface::on_actionDe_10_triggered()
{
    m_exercice = new exercice("complementM10", 10,"",this);
    m_exercice->show();
}
void interface::on_actionDe_15_triggered()
{
    m_exercice = new exercice("complementM15", 15,"",this);
    m_exercice->show();
}
void interface::on_actionDe_20_triggered()
{
    m_exercice = new exercice("complementM20", 20,"",this);
    m_exercice->show();
}
void interface::on_actionDe_25_triggered()
{
    m_exercice = new exercice("complementM25", 25,"",this);
    m_exercice->show();
}
void interface::on_actionDe_50_triggered()
{
    m_exercice = new exercice("complementM50", 50,"",this);
    m_exercice->show();
}

void interface::on_actionSoustractions_triggered()
{
    m_exercice = new exercice("soustraction",0,"",this);
    m_exercice->show();
}

void interface::on_action_10_triggered()
{
    m_exercice = new exercice("complementA10", 10,"",this);
    m_exercice->show();
}

void interface::on_action_20_triggered()
{
    m_exercice = new exercice("complementA20", 20,"",this);
    m_exercice->show();
}

void interface::on_action_100_triggered()
{
    m_exercice = new exercice("complementA100", 100,"",this);
    m_exercice->show();
}

void interface::on_action_un_nombre_al_atoire_triggered()
{

}
