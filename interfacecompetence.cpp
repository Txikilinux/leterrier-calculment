#include "interfacecompetence.h"
#include "ui_interfacecompetence.h"
#include "boutonspolygone.h"
#include "interface.h"

#include <QDebug>
#include <QApplication>
 #include <QDesktopWidget>

InterfaceCompetence::InterfaceCompetence(QString competence,QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::InterfaceCompetence)
{
    m_ui->setupUi(this);
    QString initialeCompetence = competence;
        initialeCompetence.truncate(1);
    competence.remove(0,1);
    int choix = initialeCompetence.toInt();
   qDebug() << "choix vaut " << choix << " et compétence : "<<competence;
    switch (choix) {
        case 1 : {
            m_decor = new QPixmap("./images/1gdRoue.png");
            this->setGeometry(0,0, m_decor->width()+26,m_decor->height()+60);
            this->setWindowTitle("Tables de multiplication");
            adapte(*m_decor);
            QPoint origine(m_decor->width()/2 - 60,m_decor->height()/2 - 100);
            int R=m_decor->width()/2.3;
            int nb=8;
            float deformation=1;
            for (int i=0;i<nb;i++) {
                boutonsPolygone* btn = new boutonsPolygone("tableM",i+2);
                btn->deplace(origine.x()+R*cos((2*3.14/nb)*i), origine.y()+deformation*R*sin((2*3.14/nb)*i));
                btn->retaille(100,100);
                btn->QGraphicsItem::setToolTip("Table x"+QString::number(i+2));
//                QPixmap im("./images/nacellePourrie.png");
//                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            }
            break;

        case 2 : {
            m_decor = new QPixmap("./images/2trainFantome.png");
            this->setWindowTitle(QString::fromUtf8("Trouve le complément à..."));
            adapte(*m_decor);
            this->move(400,0);
            //QPoint origine(m_decor->width()/4 ,m_decor->height()/4);
            QPoint origine(m_decor->width()/4,m_decor->height()/4);
            QPixmap im("./images/fantome.png");
            boutonsPolygone* btn1 = new boutonsPolygone("complementA",10);
                btn1->QGraphicsItem::setToolTip(QString::fromUtf8("Compléments à 10"));
                btn1->deplace(origine.x(), origine.y());
                btn1->retaille(50,42);
                btn1->setImage(im);
                btn1->setMTransformable(2);
                m_scene->addItem(btn1);
            boutonsPolygone* btn2 = new boutonsPolygone("complementA",20);
                btn2->QGraphicsItem::setToolTip(QString::fromUtf8("Compléments à 20"));
                btn2->deplace(origine.x()+ m_decor->width()/2, origine.y());
                btn2->retaille(50,42);
                btn2->setImage(im);
                btn2->setMTransformable(2);
                m_scene->addItem(btn2);
            boutonsPolygone* btn3 = new boutonsPolygone("complementA",100);
                btn3->QGraphicsItem::setToolTip(QString::fromUtf8("Compléments à 100"));
                btn3->deplace(origine.x(), origine.y()+ m_decor->height()/2);
                btn3->retaille(50,42);
                btn3->setImage(im);
                btn3->setMTransformable(2);
                m_scene->addItem(btn3);
            boutonsPolygone* btn4 = new boutonsPolygone("complementA",0);
   /*
     c'est pas bon il faut charger dans le fichier config un nombre aléatoire ré-généré et écrit dans le fichier config à la fin d'exécution d'InterfaceCompetence
     parce qu'avec cette façon de faire on a 2 problèmes : il faut se rappeler que pour avoir "complément libre" il faut lui passer 0 ET la valeur max du nombre
     et écrite en dur dans le constructeur pour compléments de baudruche
   */
                btn4->QGraphicsItem::setToolTip(QString::fromUtf8("Compléments libres"));
                btn4->deplace(origine.x()+ m_decor->width()/2, origine.y()+ m_decor->height()/2);
                btn4->retaille(50,42);
                btn4->setImage(im);
                btn4->setMTransformable(2);
                m_scene->addItem(btn4);
            }
            break;

        case 3 : {
            m_decor = new QPixmap("./images/3standTir.png");
            this->setWindowTitle("Les multiples");
            adapte(*m_decor);
            this->move(800,300);
            QPoint origine(m_decor->width()/8,m_decor->height()/5.5);
            QPoint origine2(m_decor->width()/20, m_decor->height()/2.1);
            for (int i=0;i<4;i++) {
                boutonsPolygone* btn = new boutonsPolygone("complementM",i+2);
                btn->deplace(origine.x()+m_decor->width()/6*i, origine.y()+m_decor->height()/16*i);
                btn->retaille(80,44);
                btn->QGraphicsItem::setToolTip(QString::fromUtf8("Multiples de ")+QString::number(i+2));
                QPixmap im("./images/carabine.png");
                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            for (int i=0;i<4;i++) {
                boutonsPolygone* btn = new boutonsPolygone("complementM",i+6);
                btn->deplace(origine2.x()+m_decor->width()/6*i, origine2.y()+m_decor->height()/15*i);
                btn->retaille(80,44);
                btn->QGraphicsItem::setToolTip(QString::fromUtf8("Multiples de ")+QString::number(i+6));
                QPixmap im("./images/carabine.png");
                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }

            }
            break;
            
        case 4 : {
            m_decor = new QPixmap("./images/4booster.png");
            this->setWindowTitle("Tables d'addition");
            adapte(*m_decor);
            move(200,0);
            QPoint origine(m_decor->width()/2 - 35,m_decor->height()/2 - 80);
            int R=m_decor->width()/2.7;
            int nb=8;
            float deformation=0.5;
            for (int i=0;i<nb;i++) {
                boutonsPolygone* btn = new boutonsPolygone("tableA",i+2);
                btn->deplace(origine.x()+R*cos((2*3.14/nb)*i), origine.y()+deformation*R*sin((2*3.14/nb)*i));
                btn->retaille(50,42);
                btn->QGraphicsItem::setToolTip(QString::fromUtf8("Table +")+QString::number(i+2));
                QPixmap im("./images/vomito.png");
                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            }
            break;
        }
}

void InterfaceCompetence::adapte(QPixmap cheminImage)
{
    //QPixmap* imgFond = new QPixmap(cheminImage);
            QBrush* fond = new QBrush(cheminImage);
            m_ui->grvFond->setBackgroundBrush(*fond);
            m_scene = new QGraphicsScene(this);
            m_ui->grvFond->setScene(m_scene);
            m_scene->setSceneRect(0, 0, m_decor->width(), m_decor->height());
            this->resize(m_decor->width()+26,m_decor->height()+60);
}

InterfaceCompetence::~InterfaceCompetence()
{
    delete m_ui;
}

void InterfaceCompetence::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void InterfaceCompetence::on_btnFermer_clicked()
{
    this->close();
}
