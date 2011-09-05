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
    baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD, int tempsAccorde, QString op,QPoint pos, QObject* parent=0,QString image="");
    baudruche(int intMaxG, int intMaxD, int tempsAccorde,QString op,QPoint pos,QObject* parent=0,QString image="");
    baudruche(int intDroite, int tempsAccorde,QString op,QPoint pos, QObject* parent=0,QString image="");
    baudruche(int pts, QPoint pos, QObject* parent=0,QString image="");
    baudruche(float operandeG, float operandeD, int tempsAccorde, QString op,QPoint pos,QObject* parent=0,QString image="");
    QPoint getMPosition();
    float getMResultat();
    float getMApproximation();
    float getMGOperande();
    float getMDOperande();
    QString getMLigne();
    QString getMOperation();
    QString getMAffichage();
    int valeurApprochee(int,int);
//    QTimeLine getMTimer();
    QTimeLine* m_timer;
    void emetRes();
    void emetApprox();
    void emetMort();
    void changeImage(QString);
    QGraphicsTextItem* m_texteAffiche;
    QGraphicsPixmapItem m_image;
    QPoint m_position;
    QString m_nomImage;

public slots:
    void detruire();
    void detruireTps();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
     void valueChanged(int newValue);
     void lacheIci(QPoint pos);
//     void lache();
     void destroyed(bool);
     void destroyed();
     void tempsFini(QString msg);
     void tempsFini(QPixmap pix);

private:
    QString m_op;
    QString m_nomOperation;
    float g_operande;
    float d_operande;
    float m_resultat;
    float m_approximation;
    QString m_ligne;
    QString m_affichage;
    void dessineMoi(QString,int);
    void construisAffichage();
};

#endif // BAUDRUCHE_H
