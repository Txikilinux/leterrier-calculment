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
        
    //for (int i=2;i<10;i++) {
        QPoint origine(870,110);
        int R=90;
        int nb=8;
    for (int i=0;i<nb;i++) {
        boutonsPolygone* btnPoly5 = new boutonsPolygone("tableM",i+2);
        //btnPoly5->deplace(700+30*i,170 -(((10*i)%3))*80);
        btnPoly5->deplace(origine.x()+R*cos((2*3.14/nb)*i), origine.y()+R*sin((2*3.14/nb)*i));
        btnPoly5->retaille(30,20);
        btnPoly5->QGraphicsItem::setToolTip("table x"+QString::number(i+2));
        dessin->addItem(btnPoly5);
    }

//    boutonsPolygone* btnPoly5b = new boutonsPolygone("tableM",3);
//        btnPoly5b->deplace(855,25);
//        btnPoly5b->retaille(30,20);
//        btnPoly5b->QGraphicsItem::setToolTip("table x3");
//        dessin->addItem(btnPoly5b);
//
//    boutonsPolygone* btnPoly5c = new boutonsPolygone("tableM",4);
//        btnPoly5c->deplace(950,30);
//        btnPoly5c->retaille(30,20);
//        btnPoly5c->QGraphicsItem::setToolTip("table x4");
//        dessin->addItem(btnPoly5c);
//
//    boutonsPolygone* btnPoly5c = new boutonsPolygone("tableM",4);
//        btnPoly5c->deplace(950,30);
//        btnPoly5c->retaille(30,20);
//        btnPoly5c->QGraphicsItem::setToolTip("table x4");
//        dessin->addItem(btnPoly5c);
//
//    boutonsPolygone* btnPoly5c = new boutonsPolygone("tableM",4);
//        btnPoly5c->deplace(950,30);
//        btnPoly5c->retaille(30,20);
//        btnPoly5c->QGraphicsItem::setToolTip("table x4");
//        dessin->addItem(btnPoly5c);
//
//    boutonsPolygone* btnPoly5c = new boutonsPolygone("tableM",4);
//        btnPoly5c->deplace(950,30);
//        btnPoly5c->retaille(30,20);
//        btnPoly5c->QGraphicsItem::setToolTip("table x4");
//        dessin->addItem(btnPoly5c);


    }

interface::~interface()
{
    delete ui;
}

void interface::paintEvent(QPaintEvent* e )
{
    QMainWindow::paintEvent(e);
}
