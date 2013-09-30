#ifndef PIXMAPMAISON_H
#define PIXMAPMAISON_H

#include <QGraphicsPixmapItem>

class PixmapMaison : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    PixmapMaison(QPixmap image = 0);
    QPixmap getMPixmapInitial();
    void setMPixmapInitial();
protected:
//    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
//    bool m_isChangeableAuSurvol;
private:
    QPixmap m_pixmapInitial;
private slots:
//    void rendSelectionnable();
//    void rendNonSelectionnable();
//    void peutChangerImage();
//    void nePeutPasChangerImage();
};

#endif // PIXMAPMAISON_H
