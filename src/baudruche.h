/** Objet qui porte les opérations pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2010-2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2010-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

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
    baudruche(int intMinG, int intMaxG, int intMinD, int intMaxD, int tempsAccorde, QString op,QPoint pos, QObject* parent = 0,QString image = QString());
    baudruche(int intMaxG, int intMaxD, int tempsAccorde,QString op,QPoint pos,QObject* parent = 0,QString image = QString());
    baudruche(int intDroite, int tempsAccorde,QString op,QPoint pos, QObject* parent = 0,QString image = QString());
    QPoint getMPosition();
    float getMResultat();
    QList<int> getMApproximation();
    float getMGOperande();
    float getMDOperande();
    QString getMLigne();
    QString getMOperation();
    QString getMAffichage();
    QString getMDropValeur();
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
    inline bool getBaudrucheIsDetructionPlanified(){return m_isDetructionPlanified;}

public slots:
    void detruire();
    void detruireTps();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
     void valueChanged(int newValue);
     void lacheIci(QPoint pos);
     void destroyed(bool);
     void destroyed();
     void tempsFini(QString msg);
     void tempsFini(QPixmap pix);
     void baudrucheSurvole(QString nom);
     void baudrucheSurvoleRien();

private:
     bool m_localDebug;
    QString m_op;
    QString m_nomOperation;
    float g_operande;
    float d_operande;
    float m_resultat;
    QList<int> m_approximation;
    QString m_ligne;
    QString m_affichage;
    QString m_dropValeur;
    QObject* m_parent;
    bool m_isMaisonSurvolee;
    ///
    /// \brief Permet de savoir si une QTimeLine aboutissant à la destruction de l'objet a été lancée
    /// \value true si la QTimeLine a démarré
    ///
    bool m_isDetructionPlanified;
    void dessineMoi(QString);
    void construisAffichage();

    int arrondis(float nombre, bool yComprisInfDix = true);
    int arrondisToMax(float nombre, int max);
};

#endif // BAUDRUCHE_H
