#ifndef EDITEUR_H
#define EDITEUR_H

#include <QWidget>
#include <QSettings>

namespace Ui {
    class Editeur;
}

class Editeur : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Editeur)
public:
    explicit Editeur(QWidget *parent = 0);
    virtual ~Editeur();
    QString getNiveauEnCours();
    void initialiser();

protected:
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    Ui::Editeur *m_ui;
    QString *m_niveauEnCours;
    QString *m_operationEnCours;
    QString *m_nomOperationEnCours;
    QSettings* m_settings;
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;
    void initialiserOperation(QString);
    void initialiserApproche(QString operation);
    void initialiserApprocheM(QString operation);
    void initialiserComplement(QString operation);
    QString associeNomIntitule(QString intitule);

private slots:
    void on_btnQuitter_clicked();
    void sauverNiveau(QString);
    void chargerNiveau(QString);
    void changerNiveau(QString);
    void sauverOperation(QString);
    void chargerOperation(QString);
    void changerOperation(QString);
    void ajusterValeurs(int);
};

#endif // EDITEUR_H
