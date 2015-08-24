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
#include "interface.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
//#include <QPropertyAnimation>

InterfaceCompetence::InterfaceCompetence(QString competence,QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::InterfaceCompetence)
{
    m_ui->setupUi(this);
    m_scene = new QGraphicsScene(this);
    qDebug()<<"Appel de l'interface compétence pour "<<competence<<"::"<<parent->size();
    setFixedHeight(parent->height());
    update();
    m_parent = parent;
    setAttribute(Qt::WA_DeleteOnClose);

    if(competence == "tableM") {
            m_decor = new QPixmap(":/calculment/backgrounds/backgroundInterfaceTablesM");
            this->setWindowTitle(trUtf8("Tables de multiplication"));
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
                btn->retaille(100*abeApp->getAbeApplicationDecorRatio(),100*abeApp->getAbeApplicationDecorRatio());
                btn->QGraphicsItem::setToolTip(trUtf8("Table x%1").arg(QString::number(i+2)));
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            }

    else if(competence =="complementA") {
            m_decor = new QPixmap(":/calculment/backgrounds/backgroundInterfaceComplements");
            this->setWindowTitle(trUtf8("Trouve le complément à..."));
            //QPoint origine(m_decor->width()/4 ,m_decor->height()/4);
            QPoint origine((m_decor->width()/4)-80,(m_decor->height()/4)-80);
            QPixmap im(":/calculment/elements/fantomeRose");
            boutonsPolygone* btn1 = new boutonsPolygone("complementA10",10);
                btn1->QGraphicsItem::setToolTip(trUtf8("Compléments à 10"));
                btn1->deplace(origine.x(), origine.y());
                btn1->retaille(im.width()*abeApp->getAbeApplicationDecorRatio(),im.height()*abeApp->getAbeApplicationDecorRatio());
                btn1->setImage(im);
                btn1->setMTransformable(2);
                m_scene->addItem(btn1);
            boutonsPolygone* btn2 = new boutonsPolygone("complementA100",100);
                btn2->QGraphicsItem::setToolTip(trUtf8("Compléments à 100"));
                btn2->deplace((origine.x()+ m_decor->width()/2), origine.y());
                btn2->retaille(im.width()*abeApp->getAbeApplicationDecorRatio(),im.height()*abeApp->getAbeApplicationDecorRatio());
                btn2->setImage(im);
                btn2->setMTransformable(2);
                m_scene->addItem(btn2);
            boutonsPolygone* btn3 = new boutonsPolygone("complementA1000",1000);
                btn3->QGraphicsItem::setToolTip(trUtf8("Compléments à 1000"));
                btn3->deplace(origine.x(), (origine.y()+ m_decor->height()/2));// en attendant l'implémentation de la maison des nombres
//                btn3->deplace(origine.x() + m_decor->width()/4, (origine.y()+ m_decor->height()/2));
                btn3->retaille(im.width()*abeApp->getAbeApplicationDecorRatio(),im.height()*abeApp->getAbeApplicationDecorRatio());
                btn3->setImage(im);
                btn3->setMTransformable(2);
                m_scene->addItem(btn3);

                //fantome pour la maison des nombres
            boutonsPolygone* btn4 = new boutonsPolygone("maisonDesNombres",0);
                btn4->QGraphicsItem::setToolTip(trUtf8("La maison des nombres, niveau1"));
                btn4->deplace((origine.x()+ m_decor->width()/3), (origine.y()+ m_decor->height()/2));
                btn4->retaille(im.width()*abeApp->getAbeApplicationDecorRatio(),im.height()*abeApp->getAbeApplicationDecorRatio());
                btn4->setImage(im);
                btn4->setMTransformable(2);
                m_scene->addItem(btn4);

            boutonsPolygone* btn5 = new boutonsPolygone("maisonDesNombres",10);
                btn5->QGraphicsItem::setToolTip(trUtf8("La maison des nombres, niveau2"));
                btn5->deplace((origine.x()+ 2*m_decor->width()/3), (origine.y()+ m_decor->height()/2));
                btn5->retaille(im.width()*abeApp->getAbeApplicationDecorRatio(),im.height()*abeApp->getAbeApplicationDecorRatio());
                btn5->setImage(im);
                btn5->setMTransformable(2);
                m_scene->addItem(btn5);
            }
      else if(competence =="complementM") {
            m_decor = new QPixmap(":/calculment/backgrounds/backgroundInterfaceMultiples");
            this->setWindowTitle(trUtf8("Les multiples"));
            QPoint origine(m_decor->width()/6,m_decor->height()/8);
            QPoint origine2(m_decor->width()/6, m_decor->height()/2);
            for (int i=0;i<4;i++) {
                boutonsPolygone* btn = new boutonsPolygone("complementM"+QString::number((i+1)*5),(i+1)*5);
                btn->deplace(origine.x()+m_decor->width()/5*i, origine.y()+m_decor->height()/16*i);
                btn->retaille(60*abeApp->getAbeApplicationDecorRatio(),77*abeApp->getAbeApplicationDecorRatio());
                btn->QGraphicsItem::setToolTip(trUtf8("Multiples de %1").arg(QString::number((i+1)*5)));
                QPixmap im(":/calculment/elements/carabine");
                QPixmap im2 = im.scaled(60*abeApp->getAbeApplicationDecorRatio(),77*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                btn->setImage(im2);

                btn->setMTransformable(3);
                btn->setTexte(QString::number(i));
                m_scene->addItem(btn);
                }
            for (int i=0;i<2;i++) {
                boutonsPolygone* btn = new boutonsPolygone("complementM"+QString::number((i+1)*25),(i+1)*25);
                btn->deplace(origine2.x()+m_decor->width()/5*i, origine2.y()+m_decor->height()/16*i);
                btn->retaille(60*abeApp->getAbeApplicationDecorRatio(),77*abeApp->getAbeApplicationDecorRatio());
                btn->QGraphicsItem::setToolTip(trUtf8("Multiples de %1").arg(QString::number((i+1)*25)));
                QPixmap im(":/calculment/elements/carabine");
                QPixmap im2 = im.scaled(60*abeApp->getAbeApplicationDecorRatio(),77*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                btn->setImage(im2);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }

            }
      else if(competence =="tableA") {
            m_decor = new QPixmap(":/calculment/backgrounds/backgroundInterfaceTablesA");
            this->setWindowTitle(trUtf8("Tables d'addition"));
            QPoint origine(m_decor->width()/2 - 35,m_decor->height()/4);
            int R=m_decor->width()/2.7;
            int nb=8;
            float deformation=0.5;
            for (int i=0;i<nb;i++) {
                boutonsPolygone* btn = new boutonsPolygone("tableA"+QString::number(i+2),i+2);
                btn->deplace((origine.x()+R*cos((2*3.14/nb)*i)), (origine.y()+deformation*R*sin((2*3.14/nb)*i)));
                btn->retaille(66*abeApp->getAbeApplicationDecorRatio(),65*abeApp->getAbeApplicationDecorRatio());
                btn->QGraphicsItem::setToolTip(trUtf8("Table +%1").arg(QString::number(i+2)));
                QPixmap im(":/calculment/elements/nausee"+QString::number(i%3+1));
                QPixmap im2 = im.scaled(66*abeApp->getAbeApplicationDecorRatio(),65*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                btn->setImage(im2);
                btn->setMTransformable(2);
                m_scene->addItem(btn);
                }
            }
        else if(competence =="OdGrandeur") {
            m_decor = new QPixmap(":/calculment/backgrounds/backgroundInterfaceODG");
            this->setWindowTitle(trUtf8("Ordres de grandeur"));
            QPoint origine(m_decor->width()/5.4,m_decor->height()/2.7);
                boutonsPolygone* btnA = new boutonsPolygone("OdGrandeurAddition",100);
                btnA->deplace(origine.x(), origine.y());
                btnA->retaille(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio());
                btnA->QGraphicsItem::setToolTip(trUtf8("... d'additions"));
                QPixmap im = QPixmap(":/calculment/elements/aie"+ QString::number(rand()%3 +1));
                QPixmap im2 = im.scaled(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                btnA->setImage(im2);
                btnA->setMTransformable(2);
                m_scene->addItem(btnA);

                boutonsPolygone* btnS = new boutonsPolygone("OdGrandeurSoustraction",100);
                btnS->deplace(origine.x()+m_decor->width()/4, origine.y()+m_decor->height()/6);
                btnS->retaille(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio());
                btnS->QGraphicsItem::setToolTip(trUtf8("... de soustractions"));
                im = QPixmap(":/calculment/elements/aie"+ QString::number(rand()%3 +1));
                im2 = im.scaled(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                btnS->setImage(im2);
                btnS->setMTransformable(2);
                m_scene->addItem(btnS);

                boutonsPolygone* btnM = new boutonsPolygone("OdGrandeurMultiplication",100);
                btnM->deplace(origine.x()+2*m_decor->width()/4, origine.y()+2*m_decor->height()/6);
                btnM->retaille(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio());
                btnM->QGraphicsItem::setToolTip(trUtf8("... de multiplications"));
                im = QPixmap(":/calculment/elements/aie"+ QString::number(rand()%3 +1));
                im2 = im.scaled(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                btnM->setImage(im2);
                btnM->setMTransformable(2);
                m_scene->addItem(btnM);

            }

// ----------------------------- Implémentation des additions à trou en suspens -----------------------------
//        case 6 : {
//            m_decor = new QPixmap("./data/images/6autoTampon.jpg");
//            this->setGeometry(0,0, m_decor->width()+26,m_decor->height()+60);
//            this->setWindowTitle(trUtf8("Additions"));
//            adapte(*m_decor);
//            move(200,0);
//            QPoint origine(m_decor->width()/5.4,m_decor->height()/2.7);
//                boutonsPolygone* btnNormales = new boutonsPolygone("OdGrandeurAddition",100);
//                btnA->deplace(origine.x(), origine.y());
//                btnA->retaille(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio());
//                btnA->QGraphicsItem::setToolTip(trUtf8("... d'additions"));
//                QPixmap im("./data/images/aie0"+ QString::number(rand()%3 +1)+".png");
//                QPixmap im2 = im.scaled(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//                btnA->setImage(im2);
//                btnA->setMTransformable(2);
//                m_scene->addItem(btnA);

//                boutonsPolygone* btnATrous = new boutonsPolygone("OdGrandeurSoustraction",100);
//                btnS->deplace(origine.x()+m_decor->width()/4, origine.y()+m_decor->height()/6);
//                btnS->retaille(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio());
//                btnS->QGraphicsItem::setToolTip(trUtf8("... de soustractions"));
//                im = QPixmap("./data/images/aie0"+ QString::number(rand()%3 +1)+".png");
//                im2 = im.scaled(103*abeApp->getAbeApplicationDecorRatio(),96*abeApp->getAbeApplicationDecorRatio(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//                btnS->setImage(im2);
//                btnS->setMTransformable(2);
//                m_scene->addItem(btnS);
//            }
//        break;

}

void InterfaceCompetence::adapte(QPixmap cheminImage)
{
    qDebug()<<m_ui->grvFond->size();
    QPixmap imgFond2 = cheminImage.scaledToHeight(m_ui->grvFond->height(), Qt::SmoothTransformation);
    qDebug()<<"largeur imageAvant = "<<cheminImage.width()<<" Largeur imageApres = "<<imgFond2.width();

//    abeApp->getAbeApplicationDecorRatio() = static_cast<double>(imgFond2.width())/static_cast<double>(cheminImage.width());
    qDebug()<<"Ratio taille = "<<abeApp->getAbeApplicationDecorRatio();

//Pour animation d'ouverture, décommenter les lignes marquées "1" et commenter les lignes marquées "2" à la fin
//1    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
//1        animation->setDuration(2000);
//1        animation->setStartValue(QRect(0, 0, 10, 10));
//1       animation->setEndValue(QRect(0, 0, imgFond2.width()+26, imgFond2.height()+60));
//1       animation->start();
//    this->setGeometry(10,20, imgFond2.width()+26,imgFond2.height()+60);//2
//    this->setFixedSize(imgFond2.width()+26,imgFond2.height()+60);//2
    qDebug()<<m_decor->size();
    *m_decor = imgFond2;
    qDebug()<<m_decor->size();
    m_ui->grvFond->setFixedSize(imgFond2.size());
    QBrush* fond = new QBrush(imgFond2);
    m_ui->grvFond->setBackgroundBrush(*fond);
    m_ui->grvFond->setScene(m_scene);
    m_scene->setSceneRect(0, 0, imgFond2.width(), imgFond2.height());
    move((m_parent->width()-width())/2,y());
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

void InterfaceCompetence::showEvent(QShowEvent *)
{
    adapte(*m_decor);
}


void InterfaceCompetence::on_btnFermer_clicked()
{
    this->close();
    emit signalInterfaceCompetenceClose();
}
