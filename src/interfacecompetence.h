#ifndef INTERFACECOMPETENCE_H
#define INTERFACECOMPETENCE_H

#include <QWidget>
#include <QGraphicsScene>
#include "boutonspolygone.h"
#include "exerciceoperation.h"
#include "exercicemaisonnombres.h"

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
    bool m_localDebug;
    void adapte(QPixmap);
    QGraphicsScene* m_scene;
    QPixmap* m_decor;
    double m_ratioTaille;
    QWidget* m_parent;
    QList<boutonsPolygone*> m_buttons;

private slots:
    void on_btnFermer_clicked();
    void slotInterfaceCompetenceGoToExercise(int valeur, QString operation);
signals:
    void signalInterfaceCompetenceClose();
};

#endif // INTERFACECOMPETENCE_H
