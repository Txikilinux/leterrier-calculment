#ifndef EXERCICEMAISONNOMBRES_H
#define EXERCICEMAISONNOMBRES_H
#include "exerciceoperation.h"

class ExerciceMaisonNombres : public ExerciceOperation
{
    Q_OBJECT
public:
    ExerciceMaisonNombres(QString exo,QWidget *parent = 0,int val = 0, QString niveau = "");
protected:
    int m_valeurSurvolee;
    int m_valeurBase;
    void on_btn2chance_clicked();
    virtual void mousePressEvent(QMouseEvent *);
    void ajouteErreur(QString msg);

signals:
    /** Signal destiné à prévenir qu'une baudruche est lancée
      * Créé pour avoir un comportement d'un autre objet SI une baudruche est lancée
      */
    void baudrucheLancee();

protected slots:
    void affichePosBaudruche(QPoint point);
    void selectionChanged();

    /** Affecte l'image en couleurs inversées pour une PixmapMaison survolée
      * Connecté au signal baudrucheSurvole(QString)
      * @param bulleAide est le toolTip() de la PixmapMaison envoyé par le signal.
      * L'envoi du toolTip plutôt que la QProperty("Valeur") évite un cast dans le mouseMoveEvent de la baudruche
      */
    void trouveMaisonSurvolee(QString bulleAide);

    /** Réinitialise les dessins de PixmapMaisons contenus dans la scène
      * en leur donnant comme image l'image initiale
      * Connecté au signal baudrucheSurvoleRien()
      */
    void zeroMaisonSurvolee();

    /** Entrée dans l'état "initQuestion" de l'AbulEduStateMachineV1
      * Passage en boucle à chaque nouvelle question
      * Par contre, on n'y repasse pas si on refait la même question */
    virtual void slotInitQuestionEntered();

    /** Réimplémentation destinée à corriger l'accessibilité aux objets de la télécommande */
    virtual void slotSequenceEntered();
};

#endif // EXERCICEMAISONNOMBRES_H
