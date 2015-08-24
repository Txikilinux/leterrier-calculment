#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include "editeur.h"
#include "exercice.h"
#include "interfacecompetence.h"
#include "abuleduapplicationv1.h"
#include "abuledupageaccueilv1.h"
#include "leterrierstringtransition.h"
#include "activityfilter.h"


namespace Ui
{
    class interfaceClass;
}

class interface : public QMainWindow
{
    Q_OBJECT

public:
    interface(QWidget *parent = 0);
    ~interface();
    Editeur* m_editeur;
    exercice* m_exercice;
    int m_hauteurMax;
    /** Appelle la fonction setDimensionsWidget de la page d'accueil */
    void resizeEvent(QResizeEvent *);

protected:
    void paintEvent(QPaintEvent* e );

    /** Installation d'un activity filter qui émet le signal userInactive */
    ActivityFilter *m_activityFilter;

    /** Page d'accueil dont le fichier de paramètres, settings.conf, doit être créé grâce au logiciel AbulEduEditeurPageAccueil */
    AbulEduPageAccueilV1    *m_abuleduPageAccueil;

    /** La stateMachine gère le passage d'une page à l'autre dans le stackedWidget
     *  elle gère aussi la visibilité des éléments d'interface
     */
    QStateMachine                   m_leterrierStateMachine;

    /** Stocke l'envie de l'utilisateur de voir les bulles s'afficher toutes les 7 secondes
      * Initialisé à true dans le constructeur et mis à jour par la case à cocher du menu Option/Mode Démo */
    bool                     m_isDemoAvailable;

    /** AbulEduMessageBoxV1 utilisé dans le slotInterfaceDemo() et passé à la fonction finDemo() */
    AbulEduMessageBoxV1     *m_demoMessageBox;
    QString                  m_messageAide;

    /** Permet de savoir si on est en cours d'édition */
    bool m_isEditorRunning;


private:
    Ui::interfaceClass *ui;
    bool m_localDebug;
    QTranslator qtTranslator;
    QTranslator myappTranslator;
    QString m_locale;
    QSignalMapper* m_signalMapper;
    void creeMenuLangue();

    QMap<QString,QString> m_exerciceNames;

    /** QTimeLine destinée à gérer le temps d'affichage du slot démo
          * Signal finished()) connecté au slotFinDemo() */
    QTimeLine* m_demoTimeLine;

    /* Les états de la stateMachine */
    /** Premier état permettant des initialisations */
    QState          *m_initialState;
    /** Etat Global englobant tous les autres états */
    QState          *m_globalState;
    /** InitialState présentation de la page d'accueil du logiciel */
    QState          *m_homeState;
    /** Etat dans lequel se déroule un exercice */
    QState          *m_exerciseState;
    /** Etat dans lequel on édite les modules */
    QState          *m_editorState;
    /** Etat permettant de quitter proprement la StateMAchine */
    QFinalState     *m_finalState;
    /** Etat dans lequel on choisit les fichiers .abe */
    QState          *m_boxFileManagerState;
    /** PseudoEtat pour HistoryState */
    QHistoryState   *m_historyState;


    /** Crée la stateMachine */
    void createStateMachine();

public slots:

    void slotSessionAuthenticated(bool enable);
    virtual void slotInterfaceLaunchExercise(int number = 0, QString name = QString());

    /** Destiné à être connecté à un signal exited() des exercices */
    virtual void slotInterfaceBackFromExercise();

    /** Permet d'afficher en cas d'inactivité les zones à cliquer pour lancer les exercices
      * Elle est virtuelle pour permettre de déplacer m_abuleduMessageBox en fonction du contenu de la page d'accueil */
    virtual void slotInterfaceDemo();

    /** Cache les zones et bulles de lancement des exercices
      * Appelée par slotInterfaceDemo() après 4 secondes d'affichage */
    virtual void slotInterfaceEndDemo();


    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    virtual void slotInterfaceInitialStateEntered();

    /** On sort de l'état initialState */
    virtual void slotInterfaceInitialStateExited();

    /** On entre dans l'état globalState */
    virtual void slotInterfaceGlobalStateEntered();

    /** On sort de l'état globalState */
    virtual void slotInterfaceGlobalStateExited();

    /** On entre dans l'état homeState */
    virtual void slotInterfaceHomeStateEntered();

    /** On sort de l'état homeState */
    virtual void slotInterfaceHomeStateExited();

    /** On entre dans l'état exerciseState */
    virtual void slotInterfaceExerciseStateEntered();

    /** On sort de l'état exerciseState */
    virtual void slotInterfaceExerciseStateExited();

    /** On entre dans l'état editorState */
    virtual void slotInterfaceEditorStateEntered();

    /** On sort de l'état editorState */
    virtual void slotInterfaceEditorStateExited();

    /** On entre dans l'état finalState */
    virtual void slotInterfaceFinalStateEntered();

    /** On sort de l'état finalState */
    virtual void slotInterfaceFinalStateExited();

private slots:
    void on_actionSur_des_multiplications_triggered();
    void on_actionSur_des_soustractions_triggered();
    void on_actionSur_des_additions_triggered();
    void on_action_un_nombre_al_atoire_triggered();
    void on_action_10_triggered();
    void on_action_20_triggered();
    void on_action_100_triggered();
    void on_actionSoustractions_triggered();
    void on_actionDe_5_triggered();
    void on_actionDe_10_triggered();
    void on_actionDe_15_triggered();
    void on_actionDe_20_triggered();
    void on_actionDe_25_triggered();
    void on_actionDe_50_triggered();
    void on_btnInitialise_clicked();
    void on_actionAfficher_l_diteur_triggered();
    void on_actionX_9_triggered();
    void on_actionX_8_triggered();
    void on_actionX_7_triggered();
    void on_actionX_6_triggered();
    void on_actionX_5_triggered();
    void on_actionX_4_triggered();
    void on_actionX_3_triggered();
    void on_actionX_2_triggered();
    void on_actionp9_triggered();
    void on_actionp8_triggered();
    void on_actionp7_triggered();
    void on_actionp6_triggered();
    void on_actionp5_triggered();
    void on_actionp4_triggered();
    void on_actionp3_triggered();
    void on_actionp2_triggered();
    void on_actionMultiplications_triggered();
    void on_actionAdditions_triggered();
    void on_actionQuitter_triggered();
    void changelangue(QString langue);
    void on_actionMaison_des_nombres_triggered();
    void on_actionVerrouillage_nombres_changed();

    void on_action_Journal_de_mes_activit_s_triggered();
    void on_action_Changer_d_utilisateur_triggered();

    void slotInterfaceShowMainPage();
    void slotInterfaceShowAboutPage();

    void setTitle(int authStatus);

    void slotAskLanceur();
    void slotMontreLanceur();
    void slotMontreErreurId();
};

#endif // INTERFACE_H
