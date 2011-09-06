#include "pixmapmaison.h"
#include <QGraphicsSceneHoverEvent>

PixmapMaison::PixmapMaison(QPixmap image)
    :QGraphicsPixmapItem(image)
{


}

void PixmapMaison::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
    this->setSelected(true);
}

void PixmapMaison::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
    this->setSelected(false);
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
