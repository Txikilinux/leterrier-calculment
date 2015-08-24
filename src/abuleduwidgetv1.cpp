/** Réimplémentation de la classe QWidget afin de lui donner une image de fond
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  *
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

#include "abuleduwidgetv1.h"


AbulEduWidgetV1::AbulEduWidgetV1(QWidget *parent) :
    QWidget(parent),
    m_picturePath(QString())
{

}

AbulEduWidgetV1::AbulEduWidgetV1(QString picturePath, QWidget *parent) :
    QWidget(parent),
    m_picturePath(picturePath)
{

}

bool AbulEduWidgetV1::abeWidgetSetBackgroundPixmap(const QString &picturePath)
{
    if(QFile(picturePath).exists()){
        m_picturePath = picturePath;
    }
    else {
        return false;
    }
    return true;
}

void AbulEduWidgetV1::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    if(!m_picturePath.isNull()){
        QPainter painter(this);
        painter.drawPixmap(QPoint(0,0), QPixmap(m_picturePath).scaled(size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
    }
}
