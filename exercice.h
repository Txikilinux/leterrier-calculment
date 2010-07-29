#ifndef EXERCICE_H
#define EXERCICE_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include "baudruche.h"

namespace Ui {
    class exercice;
}

class exercice : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY(exercice)
public:
   //1 explicit exercice(QString,QWidget *parent = 0);
    explicit exercice(QString, int, QWidget *parent = 0);
    virtual ~exercice();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::exercice *m_ui;
    int m_score;
    int m_total;
    QChar m_operation;
    QGraphicsScene* m_scene;
    baudruche* m_baudruche;
    QString* m_trace;
    QString* m_niveau;
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;

private slots:
    void on_btnEditeur_clicked();
    void on_leResultat_returnPressed();
    void on_btnFeu_clicked();
    void on_btnBallon_clicked();
    void on_btnQuitter_clicked();
};

#endif // EXERCICE_H
