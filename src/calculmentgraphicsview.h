/** Dérivation de la QGraphicsView afin de la rendre insensible au wheelEvent
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#ifndef CALCULMENTGRAPHICSVIEW_H
#define CALCULMENTGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class CalculMentGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CalculMentGraphicsView(QWidget *parent = 0);
private:
    inline void wheelEvent(QWheelEvent *event) {event->ignore();}

signals:

public slots:

};

#endif // CALCULMENTGRAPHICSVIEW_H
