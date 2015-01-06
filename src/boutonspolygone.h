/** Bouton de lancement des exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2010-2015 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2010-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
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

#ifndef BOUTONSPOLYGONE_H
#define BOUTONSPOLYGONE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>

class boutonsPolygone : public QPushButton, public QGraphicsItem
{
    Q_OBJECT
          //Q_INTERFACES(QGraphicsItem)
public:
    boutonsPolygone(QString,int = 0);//
    void deplace(int,int);
    void bouge(int,int);
    //void eclate();
    //void tombe(int,int,int,QGraphicsScene);
    void retaille(int,int);
    void tourne(int);
    void setImage(QPixmap);
    void setTexte(QString);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPoint getMBase();
    QSize getMTaille();
    void setMTransformable(int);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);

signals:
    void sortie();
    void signalBoutonPolygonePressed(int,QString);

private:
    QPoint* m_base;
    int m_angle;
    int m_zoom;
    QSize* m_taille;
    QString* m_action;
    QString* m_texte;
    int m_val;
    QPixmap* m_image;
    int m_transformable;

private slots:
    void slotMontreLanceur();
    void slotMontreErreurId();

};

#endif // BOUTONSPOLYGONE_H
