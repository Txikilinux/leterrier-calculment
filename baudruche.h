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
    baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD,QString op,QPoint pos);
    baudruche(int pts, QPoint pos);
    QPoint getMPosition();
    float getMResultat();
    float getMGOperande();
    float getMDOperande();
    QString getMLigne();
    QString getMOperation();

//    QTimeLine getMTimer();
    QTimeLine* m_timer;
    void emetRes();
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
    QPoint m_position;
    QString m_ligne;
//    QTimeLine m_timer;
};

#endif // BAUDRUCHE_H
