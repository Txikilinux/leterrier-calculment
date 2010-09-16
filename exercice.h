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
    explicit exercice(QString, int val=0, QString niveau=0,QWidget *parent = 0);//
    virtual ~exercice();
    void setImgFond();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::exercice *m_ui;
    int m_score;
    int m_total;
    int m_nbMaxBallons;
    QString m_operation;
    QGraphicsScene* m_scene;
    QPixmap* m_imgFond;
    baudruche* m_baudruche;
    QString* m_trace;
    QString* m_niveau;
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;
    int m_temps;
    int m_resultatEnCours;
    int m_cible;
    void chargerParametres();

private slots:
    void on_btnRejouer_clicked();
    void on_btnEditeur_clicked();
    void on_leResultat_returnPressed();
    void on_btnFeu_clicked();
    void on_btnBallon_clicked();
    void on_btnQuitter_clicked();
};

#endif // EXERCICE_H
