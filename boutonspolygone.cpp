/**
  * Implémentation de zones cliquables sur les interfaces
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

#include "boutonspolygone.h"
#include "exercice.h"
#include "interfacecompetence.h"
#include "editeur.h"
#include <QtGui>
#include <QPropertyAnimation>

boutonsPolygone::boutonsPolygone(QString operation, int val)
        : QGraphicsItem()
        //:QPushButton() 
{
    //this->setObjectName("bouton"+operation+val);
    //qDebug()<<"Nom du bouton : "<<this->objectName();     Plus besoin de le nommer si pas de filiation
    m_base = new QPoint(0,0);
    m_taille = new QSize(100,100);
    m_val = val;
    m_angle = 0;
    m_zoom = 0;
    m_image = new QPixmap("");
    m_action = new QString(operation);
    this->QGraphicsItem::setCursor(Qt::PointingHandCursor);
    this->setIcon(*m_image);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    m_transformable = 0;
    m_texte = new QString("");
}

//redéfinition de la zone sensible : je la mets comme le bouton
QRectF boutonsPolygone::boundingRect() const
    {
    return QRectF(m_base->x(),m_base->y(),m_taille->width(),m_taille->height());
    }

void boutonsPolygone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    //changer laquelle des 2 lignes ci-dessous est commentée selon qu'on veut que les contours du bouton soient visibles ou pas
    painter->setPen(Qt::NoPen);
    //painter->setPen(Qt::black);
    //painter->setBrush(Qt::NoBrush);
    QRect rect(m_base->x(), m_base->y(), m_taille->width(), m_taille->height());
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
    painter->drawPixmap(rect, *m_image);
    painter->setPen(Qt::NoPen);
    //painter->setPen(Qt::black);
    painter->drawText(rect,Qt::AlignCenter, *m_texte);
}

void boutonsPolygone::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    QChar initialeAction;
    initialeAction=(QChar)m_action->operator [](0);
    
    qDebug() << "initialeAction : "<<initialeAction << " mais début de m_action : " << m_action->operator [](0);
    QGraphicsItem::mousePressEvent(e);
    if (*m_action=="editeur") {
        Editeur* ed = new Editeur;
        ed->show();
        }
    else if (*m_action=="sortie") emit sortie();
        else if (initialeAction.isNumber()) {
                InterfaceCompetence* inter = new InterfaceCompetence(*m_action);
                inter->show();
                }
            else if (*m_action=="travaux") {
                    //QMessageBox::warning(this, "En travaux", QString::fromUtf8(tr("Fonctionnalité non encore disponible").toStdString().c_str()));
                QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,tr("En travaux"),QString::fromUtf8(tr("Fonctionnalité non encore disponible").toStdString().c_str()));
                alertBox->setGeometry(m_base->x(), m_base->y(), m_taille->width(), m_taille->height());
                alertBox->show();
                        }
                else {
                    exercice* ex = new exercice(*m_action,0,m_val);
                    ex->show();
                    }
    e->accept();
    switch (m_transformable) {
        case 1 :bouge(0,300);break;
        case 2 : /*QPixmap img("./data/images/pomme2.png"); setImage(img);*/setTexte("Fait");break;
        }

}

void boutonsPolygone::deplace(int x, int y)
{
    m_base->setX(x);
    m_base->setY(y);
}

void boutonsPolygone::retaille(int w, int h)
{
    m_taille->setWidth(w);
    m_taille->setHeight(h);
}

void boutonsPolygone::tourne(int angle)
{
    m_angle = angle;
    //this->rotate(angle);
    this->setTransform(QTransform().translate(this->m_base->x(), this->m_base->y()).rotate(angle).translate(-1*this->m_base->x(),-1*this->m_base->y()));

}

void boutonsPolygone::setImage(QPixmap image)
{
    *m_image = image;
    QGraphicsItem::update(m_base->x(), m_base->y(), m_taille->width(), m_taille->height());
}

void boutonsPolygone::setTexte(QString texte)
{
    *m_texte = texte;
    QGraphicsItem::update(m_base->x(), m_base->y(), m_taille->width(), m_taille->height());
}

QPoint boutonsPolygone::getMBase()
{
    return *m_base;
}

QSize boutonsPolygone::getMTaille()
{
    return *m_taille;
}

void boutonsPolygone::setMTransformable(int entier)
{
    m_transformable = entier;
}

void boutonsPolygone::bouge(int dx, int dy)
{
    QGraphicsItem::hide();
    deplace(m_base->x() + dx, m_base->y()+dy);
    QGraphicsItem::update(m_base->x(), m_base->y(), m_taille->width(), m_taille->height());
    QGraphicsItem::show();
}
