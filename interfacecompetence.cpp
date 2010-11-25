/**
  * Implémentation de l'interface intermédiaire quand l'exercice appelé requiert une valeur en paramètre. Une zone cliquable correspond à chaque paramètre possible
  *
  * @see https://redmine.ryxeo.com/projects/leterrier-calculment/
  * @author 2009-2010 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @see The GNU Public License (GPL)
  */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "interfacecompetence.h"
#include "ui_interfacecompetence.h"
#include "boutonspolygone.h"
#include "interface.h"

#include <QDebug>
#include <QApplication>
 #include <QDesktopWidget>
#include <QPropertyAnimation>

InterfaceCompetence::InterfaceCompetence(QString competence,QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::InterfaceCompetence)
{
    m_ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    QString initialeCompetence = competence;
        initialeCompetence.truncate(1);
    competence.remove(0,1);
    int choix = initialeCompetence.toInt();
   qDebug() << "choix vaut " << choix << " et compétence : "<<competence;

    switch (choix) {
        case 1 : {
            m_decor = new QPixmap("./data/images/1gdRoue.jpg");
            this->setGeometry(0,0, m_decor->width()+26,m_decor->height()+60);
            this->setWindowTitle(trUtf8("Tables de multiplication"));
            adapte(*m_decor);
            QPoint origine(m_decor->width()/2.4,m_decor->height()/2.3);
            qDebug()<<"Taille décor : "<<m_decor->width()<<" X "<<m_decor->height();
            qDebug()<<"Origine : ("<<m_decor->width()/2<<","<<m_decor->height()/2<<")";
            int R=m_decor->width()/2.5;
            int nb=8;
            double angleDepart=3.14/nb;
            float deformation=1;
            for (int i=0;i<nb;i++) {
                boutonsPolygone* btn = new boutonsPolygone("tableM"+QString::number(i+2),i+2);
                btn->deplace((origine.x()+R*cos((2*3.14/nb)*i+angleDepart)), (origine.y()+deformation*R*sin((2*3.14/nb)*i+angleDepart)));
                btn->retaille(100*m_ratioTaille,100*m_ratioTaille);
                btn->QGraphicsItem::setToolTip(trUtf8("Table x")+QString::number(i+2));
//                QPixmap im("./data/images/nacellePourrie.png");
//                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            }
            break;

        case 2 : {
            m_decor = new QPixmap("./data/images/2trainFantome.jpg");
            this->setWindowTitle(trUtf8("Trouve le complément à..."));
            adapte(*m_decor);
            this->move(150,0);
            //QPoint origine(m_decor->width()/4 ,m_decor->height()/4);
            QPoint origine((m_decor->width()/4)-80,(m_decor->height()/4)-80);
            QPixmap im("./data/images/fantome.png");
            boutonsPolygone* btn1 = new boutonsPolygone("complementA10",10);
                btn1->QGraphicsItem::setToolTip(trUtf8("Compléments à 10"));
                btn1->deplace(origine.x(), origine.y());
                btn1->retaille(im.width()*m_ratioTaille,im.height()*m_ratioTaille);
                btn1->setImage(im);
                btn1->setMTransformable(2);
                m_scene->addItem(btn1);
            boutonsPolygone* btn2 = new boutonsPolygone("complementA20",20);
                btn2->QGraphicsItem::setToolTip(trUtf8("Compléments à 20"));
                btn2->deplace((origine.x()+ m_decor->width()/2), origine.y());
                btn2->retaille(im.width()*m_ratioTaille,im.height()*m_ratioTaille);
                btn2->setImage(im);
                btn2->setMTransformable(2);
                m_scene->addItem(btn2);
            boutonsPolygone* btn3 = new boutonsPolygone("complementA100",100);
                btn3->QGraphicsItem::setToolTip(trUtf8("Compléments à 100"));
                btn3->deplace(origine.x(), (origine.y()+ m_decor->height()/2));
                btn3->retaille(im.width()*m_ratioTaille,im.height()*m_ratioTaille);
                btn3->setImage(im);
                btn3->setMTransformable(2);
                m_scene->addItem(btn3);
            boutonsPolygone* btn4 = new boutonsPolygone("travaux",0);
   /*
     c'est pas bon il faut charger dans le fichier config un nombre aléatoire ré-généré et écrit dans le fichier config à la fin d'exécution d'InterfaceCompetence
     parce qu'avec cette façon de faire on a 2 problèmes : il faut se rappeler que pour avoir "complément libre" il faut lui passer 0 ET la valeur max du nombre
     et écrite en dur dans le constructeur pour compléments de baudruche
   */
                btn4->QGraphicsItem::setToolTip(trUtf8("Les maisons des nombres"));
                btn4->deplace((origine.x()+ m_decor->width()/2), (origine.y()+ m_decor->height()/2));
                btn4->retaille(im.width()*m_ratioTaille,im.height()*m_ratioTaille);
                btn4->setImage(im);
                btn4->setMTransformable(2);
                m_scene->addItem(btn4);
            }
            break;

        case 3 : {
            m_decor = new QPixmap("./data/images/3standTir.jpg");
            this->setWindowTitle(trUtf8("Les multiples"));
            adapte(*m_decor);
            this->move(200,0);
            QPoint origine(m_decor->width()/6,m_decor->height()/8);
            QPoint origine2(m_decor->width()/6, m_decor->height()/2);
            for (int i=0;i<4;i++) {
                boutonsPolygone* btn = new boutonsPolygone("complementM"+QString::number((i+1)*5),(i+1)*5);
                btn->deplace(origine.x()+m_decor->width()/5*i, origine.y()+m_decor->height()/16*i);
                btn->retaille(60*m_ratioTaille,77*m_ratioTaille);
                btn->QGraphicsItem::setToolTip(trUtf8("Multiples de ")+QString::number((i+1)*5));
                QPixmap im("./data/images/carabine.png");
                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            for (int i=0;i<2;i++) {
                boutonsPolygone* btn = new boutonsPolygone("complementM"+QString::number((i+1)*25),(i+1)*25);
                btn->deplace(origine2.x()+m_decor->width()/5*i, origine2.y()+m_decor->height()/16*i);
                btn->retaille(60*m_ratioTaille,77*m_ratioTaille);
                btn->QGraphicsItem::setToolTip(trUtf8("Multiples de ")+QString::number((i+1)*25));
                QPixmap im("./data/images/carabine.png");
                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }

            }
            break;
            
        case 4 : {
            m_decor = new QPixmap("./data/images/4booster.jpg");
            this->setWindowTitle(trUtf8("Tables d'addition"));
            adapte(*m_decor);
            move(250,0);
            QPoint origine(m_decor->width()/2 - 35,m_decor->height()/2 - 130);
            int R=m_decor->width()/2.7;
            int nb=8;
            float deformation=0.5;
            for (int i=0;i<nb;i++) {
                boutonsPolygone* btn = new boutonsPolygone("tableA"+QString::number(i+2),i+2);
                btn->deplace((origine.x()+R*cos((2*3.14/nb)*i)), (origine.y()+deformation*R*sin((2*3.14/nb)*i)));
                btn->retaille(50*m_ratioTaille,42*m_ratioTaille);
                btn->QGraphicsItem::setToolTip(trUtf8("Table +")+QString::number(i+2));
                QPixmap im("./data/images/vomito.png");
                btn->setImage(im);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            }
            break;

        case 5 : {
            m_decor = new QPixmap("./data/images/5palaisGlaces.jpg");
            this->setWindowTitle(trUtf8("Ordres de grandeur"));
            adapte(*m_decor);
            move(300,0);
            QPoint origine(m_decor->width()/5,m_decor->height()/2.5);
                boutonsPolygone* btnA = new boutonsPolygone("approcheA",100);
                btnA->deplace(origine.x(), origine.y());
                btnA->retaille(70*m_ratioTaille,60*m_ratioTaille);
                btnA->QGraphicsItem::setToolTip(trUtf8("... d'additions"));
                QPixmap im("./data/images/aie.png");
                btnA->setImage(im);
                btnA->setMTransformable(2);
                m_scene->addItem(btnA);

                boutonsPolygone* btnS = new boutonsPolygone("approcheS",100);
                btnS->deplace(origine.x()+m_decor->width()/4, origine.y()+m_decor->height()/6);
                btnS->retaille(70*m_ratioTaille,60*m_ratioTaille);
                btnS->QGraphicsItem::setToolTip(trUtf8("... de soustractions"));
                btnS->setImage(im);
                btnS->setMTransformable(2);
                m_scene->addItem(btnS);

                boutonsPolygone* btnM = new boutonsPolygone("approcheM",100);
                btnM->deplace(origine.x()+2*m_decor->width()/4, origine.y()+2*m_decor->height()/6);
                btnM->retaille(70*m_ratioTaille,60*m_ratioTaille);
                btnM->QGraphicsItem::setToolTip(trUtf8("... de multiplications"));
                btnM->setImage(im);
                btnM->setMTransformable(2);
                m_scene->addItem(btnM);

            }
            break;
        }
}

void InterfaceCompetence::adapte(QPixmap cheminImage)
{
    QRect ecran;
    ecran=QApplication::desktop()->screenGeometry();

    QPixmap imgFond2=cheminImage.scaledToHeight(ecran.height()-140, Qt::SmoothTransformation);
    qDebug()<<"largeur imageAvant = "<<cheminImage.width()<<" Largeur imageApres = "<<imgFond2.width();

    m_ratioTaille = static_cast<double>(imgFond2.width())/static_cast<double>(cheminImage.width());
    qDebug()<<"Ratio taille = "<<m_ratioTaille;

//Pour animation d'ouverture, décommenter les lignes marquées "1" et commenter les lignes marquées "2" à la fin
    QPropertyAnimation* animation = new QPropertyAnimation(this, "size");
        animation->setDuration(3000);
        animation->setStartValue(QSize(10,10));
       animation->setEndValue(QSize(imgFond2.width()+26, imgFond2.height()+60));
       //animation->start();
       QPropertyAnimation* animation2 = new QPropertyAnimation(this, "pos");
           animation2->setDuration(3000);
           animation2->setKeyValueAt(0,QPoint(0,0));
           animation2->setKeyValueAt(0.25,QPoint(1200,200));
           animation2->setKeyValueAt(0.5,QPoint(100,800));
           animation2->setKeyValueAt(0.75,QPoint(400,200));
           animation2->setKeyValueAt(1,QPoint(600,100));
          // animation2->setStartValue(QPoint(0,0));
          //animation2->setEndValue(QPoint(200,100));
         // animation2->start();

          QParallelAnimationGroup *group = new QParallelAnimationGroup;
          group->addAnimation(animation);
          group->addAnimation(animation2);
          group->start();
//2    this->setGeometry(10,20, imgFond2.width()+26,imgFond2.height()+60);//2
//2    this->setFixedSize(imgFond2.width()+26,imgFond2.height()+60);//2

    *m_decor = imgFond2;
    QBrush* fond = new QBrush(imgFond2);
            m_ui->grvFond->setBackgroundBrush(*fond);
            m_scene = new QGraphicsScene(this);
            m_ui->grvFond->setScene(m_scene);
            m_scene->setSceneRect(0, 0, imgFond2.width(), imgFond2.height());

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
