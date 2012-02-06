#ifndef PIXMAPMAISON_H
#define PIXMAPMAISON_H

#include <QGraphicsPixmapItem>

class PixmapMaison : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    PixmapMaison(QPixmap image = 0);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool m_isChangeableAuSurvol;
private slots:
    void rendSelectionnable();
    void rendNonSelectionnable();
    void peutChangerImage();
    void nePeutPasChangerImage();
};

#endif // PIXMAPMAISON_H
