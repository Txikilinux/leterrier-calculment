#include "interface.h"
#include "ui_interface.h"
#include "exercice.h"
#include "ui_exercice.h"
#include "boutonspolygone.h"

interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    ui->setupUi(this);

    QPixmap* imgFond = new QPixmap("./images/fondecran.png");
    QBrush* fond = new QBrush(*imgFond);
    ui->fete->setBackgroundBrush(*fond);
    QGraphicsScene* dessin = new QGraphicsScene(this);
    ui->fete->setScene(dessin);
    dessin->setSceneRect(0, 0, 1024, 768);

 //   QPoint* ptBase1= new QPoint(50,100);
//    boutonsPolygone* btnPoly1 = new boutonsPolygone(ptBase1,ui->fete);
    boutonsPolygone* btnPoly1 = new boutonsPolygone(ui->fete);
    btnPoly1->retaille(100,100);
    btnPoly1->deplace(50,350);
            connect(btnPoly1, SIGNAL(clicked()), ui->btnAddition, SLOT(click()));
   // QPoint* ptBase2= new QPoint(300,100);
//    boutonsPolygone* btnPoly2 = new boutonsPolygone(ptBase2,ui->fete);
        boutonsPolygone* btnPoly2 = new boutonsPolygone(ui->fete);
    connect(btnPoly2, SIGNAL(clicked()), ui->btnMultiplication, SLOT(click()));

//    if (ui->fete->itemAt(0,0)->isWidget()) qDebug() << "yes";
//    else qDebug() << "no";




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
