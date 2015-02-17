#include "abuledupixmapwidgetv1.h"

AbulEduPixmapWidgetV1::AbulEduPixmapWidgetV1(QWidget *parent) :
    QWidget(parent),
    m_picturePath(QString()),
    m_ratioMode(KeepByExpanding)
{

}

AbulEduPixmapWidgetV1::AbulEduPixmapWidgetV1(QString picturePath, QWidget *parent) :
    QWidget(parent),
    m_picturePath(picturePath),
    m_ratioMode(KeepByExpanding)
{

}

bool AbulEduPixmapWidgetV1::abePixmapWidgetSetPixmap(const QString &picturePath)
{
    if(picturePath.isNull()){
        m_picturePath = ":/abuledupixmapwidgetv1/backgrounds/empty";
    }
    else {
        if(QFile(picturePath).exists()){
            m_picturePath = picturePath;
        }
        else {
            return false;
        }
    }
    return true;
}

void AbulEduPixmapWidgetV1::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    if(!m_picturePath.isNull()){
        switch(m_ratioMode){
        case KeepByExpanding:
            painter.drawPixmap(QPoint(0,0), QPixmap(m_picturePath).scaled(size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
            break;
        case KeepRatio:
            painter.drawPixmap(QPoint(0,0), QPixmap(m_picturePath).scaled(size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
            break;
        case IgnoreRatio:
            painter.drawPixmap(QPoint(0,0), QPixmap(m_picturePath).scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
            break;
        }
    }
    else {
        /* Je pense qu'on ne vient jamais là */
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0,0,width(),height());
    }
}


