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
    QPixmap* imgFond = new QPixmap("./images/fondecran.png");
    QBrush* fond = new QBrush(*imgFond);
    ui->fete->setBackgroundBrush(*fond);
    QGraphicsScene* dessin = new QGraphicsScene(this);
    ui->fete->setScene(dessin);
    dessin->setSceneRect(0, 0, 1024, 768);

    boutonsPolygone* btnPoly1 = new boutonsPolygone("addition");
    btnPoly1->deplace(-75,340);
    btnPoly1->retaille(60,90);
    btnPoly1->tourne(-20);
    dessin->addItem(btnPoly1);
        boutonsPolygone* btnPoly2 = new boutonsPolygone("multiplication");
        btnPoly2->deplace(250,310);
        btnPoly2->retaille(70,80);
        dessin->addItem(btnPoly2);

        boutonsPolygone* btnPoly3 = new boutonsPolygone("editeur");
        btnPoly3->deplace(930,640);
        btnPoly3->retaille(50,100);
        dessin->addItem(btnPoly3);

        boutonsPolygone* btnPoly4 = new boutonsPolygone("sortie");
        btnPoly4->deplace(160,590);
        btnPoly4->retaille(60,30);
        connect(btnPoly4, SIGNAL(sortie()), this, SLOT(close()));
        dessin->addItem(btnPoly4);
    }

interface::~interface()
{
    delete ui;
}

void interface::on_btnAddition_clicked()
{
    exercice* e=new exercice("addition", this);
    e->show();
}

void interface::on_btnMultiplication_clicked()
{
    exercice* e=new exercice("multiplication", this);
    e->show();
}

void interface::paintEvent(QPaintEvent* e )
{
    QMainWindow::paintEvent(e);
}
