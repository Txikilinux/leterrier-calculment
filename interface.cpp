#include "interface.h"
#include "ui_interface.h"
#include "exercice.h"
#include "ui_exercice.h"
#include "editeur.h"
#include "boutonspolygone.h"

interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    ui->setupUi(this);

    QFile* fichierConf = new QFile("./maConfig.ini");
    if (!fichierConf->exists()) qDebug()<<QString::fromUtf8("Fichier config NON trouvé");
    else qDebug() << QString::fromUtf8("Fichier config trouvé");

    m_editeur = new Editeur();

    QPixmap* imgFond = new QPixmap("./images/fondecran.png");
    QBrush* fond = new QBrush(*imgFond);
    ui->fete->setBackgroundBrush(*fond);

    QGraphicsScene* dessin = new QGraphicsScene(this);
        ui->fete->setScene(dessin);
        dessin->setSceneRect(0, 0, 1024, 768);

    boutonsPolygone* btnPoly1 = new boutonsPolygone("addition",0);
        btnPoly1->deplace(-75,340);
        btnPoly1->retaille(60,90);
        btnPoly1->tourne(-20);
        dessin->addItem(btnPoly1);

    boutonsPolygone* btnPoly2 = new boutonsPolygone("multiplication",0);
        btnPoly2->deplace(250,310);
        btnPoly2->retaille(70,80);
        dessin->addItem(btnPoly2);

    boutonsPolygone* btnPoly3 = new boutonsPolygone("editeur",0);
        btnPoly3->deplace(930,640);
        btnPoly3->retaille(50,100);
        dessin->addItem(btnPoly3);

    boutonsPolygone* btnPoly4 = new boutonsPolygone("sortie",0);
        btnPoly4->deplace(160,590);
        btnPoly4->retaille(60,30);
        connect(btnPoly4, SIGNAL(sortie()), this, SLOT(close()));
        dessin->addItem(btnPoly4);
        
        QPoint origine(870,70);
        int R=90;
        int nb=8;
        float deformation=0.8;
    for (int i=0;i<nb;i++) {
        boutonsPolygone* btnPoly5 = new boutonsPolygone("tableM",i+2);
        //btnPoly5->deplace(700+30*i,170 -(((10*i)%3))*80);
        btnPoly5->deplace(origine.x()+R*cos((2*3.14/nb)*i), origine.y()+deformation*R*sin((2*3.14/nb)*i));
        btnPoly5->retaille(40,34);
        btnPoly5->QGraphicsItem::setToolTip("table x"+QString::number(i+2));
        QPixmap im("./images/pomme.png");
        btnPoly5->setImage(im);
        btnPoly5->setMTransformable(2);
        dessin->addItem(btnPoly5);

       // dessin->update(btnPoly5->getMBase().x(), btnPoly5->getMBase().y(),btnPoly5->getMTaille().width(), btnPoly5->getMTaille().height());
        }

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
    m_exercice = new exercice("addition",0,this);
    m_exercice->show();
}

void interface::on_actionMultiplications_triggered()
{
    m_exercice = new exercice("multiplication",0,this);
    m_exercice->show();
}

void interface::on_actionp2_triggered()
{
    m_exercice = new exercice("tableA",2,this);
    m_exercice->show();
}

void interface::on_actionp3_triggered()
{
    m_exercice = new exercice("tableA",3,this);
    m_exercice->show();
}

void interface::on_actionp4_triggered()
{
    m_exercice = new exercice("tableA",4,this);
    m_exercice->show();
}

void interface::on_actionp5_triggered()
{
    m_exercice = new exercice("tableA",5,this);
    m_exercice->show();
}

void interface::on_actionp6_triggered()
{
    m_exercice = new exercice("tableA",6,this);
    m_exercice->show();
}

void interface::on_actionp7_triggered()
{
    m_exercice = new exercice("tableA",7,this);
    m_exercice->show();
}

void interface::on_actionp8_triggered()
{
    m_exercice = new exercice("tableA",8,this);
    m_exercice->show();
}

void interface::on_actionp9_triggered()
{
    m_exercice = new exercice("tableA",9,this);
    m_exercice->show();
}

void interface::on_actionX_2_triggered()
{
    m_exercice = new exercice("tableM",2,this);
    m_exercice->show();
}

void interface::on_actionX_3_triggered()
{
    m_exercice = new exercice("tableM",3,this);
    m_exercice->show();
}

void interface::on_actionX_4_triggered()
{
    m_exercice = new exercice("tableM",4,this);
    m_exercice->show();
}

void interface::on_actionX_5_triggered()
{
    m_exercice = new exercice("tableM",5,this);
    m_exercice->show();
}

void interface::on_actionX_6_triggered()
{
    m_exercice = new exercice("tableM",6,this);
    m_exercice->show();
}

void interface::on_actionX_7_triggered()
{
    m_exercice = new exercice("tableM",7,this);
    m_exercice->show();
}

void interface::on_actionX_8_triggered()
{
    m_exercice = new exercice("tableM",8,this);
    m_exercice->show();
}

void interface::on_actionX_9_triggered()
{
    m_exercice = new exercice("tableM",9,this);
    m_exercice->show();
}

void interface::on_actionAfficher_l_diteur_triggered()
{
    m_editeur->show();
}
