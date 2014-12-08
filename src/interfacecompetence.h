#ifndef INTERFACECOMPETENCE_H
#define INTERFACECOMPETENCE_H

#include <QWidget>
#include <QGraphicsScene>
#include "boutonspolygone.h"

namespace Ui {
    class InterfaceCompetence;
}

class InterfaceCompetence : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(InterfaceCompetence)
public:
    explicit InterfaceCompetence(QString,QWidget *parent = 0);
    virtual ~InterfaceCompetence();

protected:
    virtual void changeEvent(QEvent *e);
    void showEvent(QShowEvent *);

private:
    Ui::InterfaceCompetence *m_ui;
    void adapte(QPixmap);
    QGraphicsScene* m_scene;
    QPixmap* m_decor;
    double m_ratioTaille;
    QWidget* m_parent;
    QList<boutonsPolygone*> m_buttons;

private slots:
    void on_btnFermer_clicked();
signals:
    void signalInterfaceCompetenceClose();
};

#endif // INTERFACECOMPETENCE_H
