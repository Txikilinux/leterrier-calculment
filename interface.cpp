#include "interface.h"
#include "ui_interface.h"
#include "exercice.h"
#include "ui_exercice.h"
#include "boutonspolygone.h"

interface::interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfaceClass)
{
    ui->setupUi(this);

    QPixmap* imgFond = new QPixmap("./images/oui.png");
    QBrush* fond = new QBrush(*imgFond);
    ui->fete->setBackgroundBrush(*fond);
    QGraphicsScene* dessin = new QGraphicsScene(this);
    ui->fete->setScene(dessin);
    dessin->setSceneRect(0, 50, 700, 500);

    QPointF* ptBase1= new QPointF(50,100);
    boutonsPolygone* btnPoly1 = new boutonsPolygone(ptBase1,ui->fete);
        connect(btnPoly1, SIGNAL(clicked()), ui->btnAddition, SLOT(click()));
    QPointF* ptBase2= new QPointF(300,100);
        boutonsPolygone* btnPoly2 = new boutonsPolygone(ptBase2,ui->fete);
    connect(btnPoly2, SIGNAL(clicked()), ui->btnMultiplication, SLOT(click()));


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

//    QVector<QPointF> ensPoints(0);
//            QPointF pt1(10,30);
//            ensPoints.append(pt1);
//            QPointF pt2(30,10);
//            ensPoints.append(pt2);
//            QPointF pt4(50,30);
//            ensPoints.append(pt4);
//            QPointF pt3(30,50);
//            ensPoints.append(pt3);
//    boutonsPolygone* btnPoly1 = new boutonsPolygone(&ensPoints);
//        btnPoly1->setToolTip("toto");
//        btnPoly1->setEnabled(1);
//        connect(btnPoly1, SIGNAL(clicked()), ui->btnAddition, SLOT(click()));
//    peintre.drawPolygon(*btnPoly1);
}
