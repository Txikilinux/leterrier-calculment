#ifndef EDITEUR_H
#define EDITEUR_H

#include <QtGui/QWidget>

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
    int m_minG;
    int m_maxG;
    int m_minD;
    int m_maxD;
    void initialiserOperation(QString);
    void initialiserApproche(QString operation);
    void initialiserApprocheM(QString operation);
    void initialiserComplement(QString operation);

private slots:
    void on_btnQuitter_clicked();
    void sauverNiveau(QString);
    void chargerNiveau(QString);
    void changerNiveau(QString);
    void sauverOperation(QString);
    void chargerOperation(QString);
    void changerOperation(QString);
    void testerValeurs(int);
};

#endif // EDITEUR_H
