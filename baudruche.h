#ifndef BAUDRUCHE_H
#define BAUDRUCHE_H

#include <QGraphicsItemGroup>
#include <QFont>
#include <QString>
#include <QTimeLine>
#include <QtScript>

class baudruche : public QObject, public QGraphicsItemGroup
{
   Q_OBJECT
public:
    baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD, int tempsAccorde, QString op,QPoint pos, QString image=0);
    baudruche(int intMaxG, int intMaxD, int tempsAccorde,QString op,QPoint pos,QString image=0);
    baudruche(int intDroite, int tempsAccorde,QString op,QPoint pos, QString image=0);
    baudruche(int pts, QPoint pos, QString image=0);
    QPoint getMPosition();
    float getMResultat();
    float getMApproximation();
    float getMGOperande();
    float getMDOperande();
    QString getMLigne();
    QString getMOperation();
    int valeurApprochee(int,int);
//    QTimeLine getMTimer();
    QTimeLine* m_timer;
    void emetRes();
    void emetApprox();
    void emetMort();

public slots:
    void detruire();
    void detruireTps();

signals:
     void valueChanged(int newValue);
     void destroyed(bool);
     void destroyed();
     void tempsFini(QString msg);
     void tempsFini(QPixmap pix);

private:
    QString m_op;
    float g_operande;
    float d_operande;
    float m_resultat;
    float m_approximation;
    QPoint m_position;
    QString m_ligne;
    QString* m_affichage;
//    QTimeLine m_timer;
    void dessineMoi(QString,int);
};

#endif // BAUDRUCHE_H
