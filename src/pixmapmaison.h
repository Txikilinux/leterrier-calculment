/** Objet cible des exercices du type Maison des nombres
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#ifndef PIXMAPMAISON_H
#define PIXMAPMAISON_H

#include <QGraphicsItemGroup>

class PixmapMaison : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    PixmapMaison(int number,QPixmap image = QPixmap(),QColor color= QColor(0,0,0));
    QPixmap getMPixmapInitial();
    void setMPixmapInitial();
    void pixmapMaisonSetPixmap(QPixmap pixmap);
protected:
//    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
//    bool m_isChangeableAuSurvol;
private:
    QPixmap m_pixmapInitial;
    QGraphicsPixmapItem* m_picture;
    QGraphicsTextItem* m_number;
private slots:
//    void rendSelectionnable();
//    void rendNonSelectionnable();
//    void peutChangerImage();
//    void nePeutPasChangerImage();
};

#endif // PIXMAPMAISON_H
