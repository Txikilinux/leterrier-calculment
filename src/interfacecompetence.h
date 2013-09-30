#ifndef INTERFACECOMPETENCE_H
#define INTERFACECOMPETENCE_H

#include <QtGui/QWidget>
#include <QGraphicsScene>

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

private:
    Ui::InterfaceCompetence *m_ui;
    void adapte(QPixmap);
    QGraphicsScene* m_scene;
    QPixmap* m_decor;
    double m_ratioTaille;

private slots:
    void on_btnFermer_clicked();
};

#endif // INTERFACECOMPETENCE_H
