#include "pixmapmaison.h"
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

PixmapMaison::PixmapMaison(QPixmap image)
    :QGraphicsPixmapItem(image)
{
    setAcceptHoverEvents(true);
    m_isChangeableAuSurvol = false;
}

void PixmapMaison::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<"PixmapMaison::hoverEnterEvent "<<m_isChangeableAuSurvol;
    if (m_isChangeableAuSurvol)
    {
    event->accept();
    qDebug()<<"Survol de "<<toolTip();
    if (toolTip().right(1) == "0")
        setPixmap(QPixmap("./data/images/maison"+toolTip().right(2)+"b.png"));
    else
        setPixmap(QPixmap("./data/images/maison"+toolTip().right(1)+"b.png"));
    this->setSelected(true);
    }
}

void PixmapMaison::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<"PixmapMaison::hoverLeaveEvent "<<m_isChangeableAuSurvol;
    if (m_isChangeableAuSurvol)
    {
    event->accept();
    if (toolTip().right(1) == "0")
        setPixmap(QPixmap("./data/images/maison"+toolTip().right(2)+".png"));
    else
        setPixmap(QPixmap("./data/images/maison"+toolTip().right(1)+".png"));
    this->setSelected(false);
    }
}

void PixmapMaison::rendSelectionnable()
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
}

void PixmapMaison::rendNonSelectionnable()
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    this->setAcceptHoverEvents(false);
}

void PixmapMaison::peutChangerImage()
{
    m_isChangeableAuSurvol = true;
}

void PixmapMaison::nePeutPasChangerImage()
{
    m_isChangeableAuSurvol = false;
}
