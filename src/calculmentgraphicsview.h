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
