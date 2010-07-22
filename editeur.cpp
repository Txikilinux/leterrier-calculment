#include "editeur.h"
#include "ui_editeur.h"

#include <QComboBox>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>


Editeur::Editeur(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Editeur)
{
    m_ui->setupUi(this);
        m_ui->cbNiveau->addItem(tr("Niveau1"), 1);
        m_ui->cbNiveau->addItem(tr("Niveau2"), 2);
        m_ui->cbNiveau->addItem(tr("Niveau3"), 3);
        m_ui->cbNiveau->addItem(tr("Personnel"),4);
        connect(m_ui->sldVitesse, SIGNAL(valueChanged(int)), m_ui->pbVitesse, SLOT(setValue(int)));
        m_niveauEnCours = new QString(m_ui->cbNiveau->currentText());
        initialiser();
        this->chargerNiveau(*m_niveauEnCours);

        connect(m_ui->cbNiveau, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerNiveau(QString)));
}

Editeur::~Editeur()
{
    delete m_ui;
}

QString Editeur::getNiveauEnCours()
{
    return *m_niveauEnCours;
}

void Editeur::initialiser()
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.setValue("NiveauEnCours", "Niveau1");
    config.beginGroup("Niveau1");
        config.setValue("MinGauche", 0);
        config.setValue("MaxGauche", 5);
        config.setValue("MinDroite", 0);
        config.setValue("MaxDroite", 5);
    config.endGroup();
    config.beginGroup("Niveau2");
        config.setValue("MinGauche", 5);
        config.setValue("MaxGauche", 9);
        config.setValue("MinDroite", 5);
        config.setValue("MaxDroite", 9);
    config.endGroup();
    config.beginGroup("Niveau3");
        config.setValue("MinGauche", 10);
        config.setValue("MaxGauche", 99);
        config.setValue("MinDroite", 10);
        config.setValue("MaxDroite", 99);
    config.endGroup();
    nouvelUtilisateur = false;
}
void Editeur::sauverNiveau(QString niveau)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.beginGroup(niveau);
        config.setValue("MinGauche", m_ui->spbGMin->value());
        config.setValue("MaxGauche", m_ui->spbGMax->value());
        config.setValue("MinDroite", m_ui->spbDMin->value());
        config.setValue("MaxDroite", m_ui->spbDMax->value());
    config.endGroup();
    *m_niveauEnCours = niveau;
}

void Editeur::chargerNiveau(QString niveau)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.beginGroup(niveau);
        m_ui->spbGMax->setValue(config.value("MaxGauche").toInt());
        m_ui->spbGMin->setValue(config.value("MinGauche").toInt());
        m_ui->spbDMax->setValue(config.value("MaxDroite").toInt());
        m_ui->spbDMin->setValue(config.value("MinDroite").toInt());
    config.endGroup();
}

void Editeur::changerNiveau(QString chaine)
{
    this->sauverNiveau(*m_niveauEnCours);
    this->chargerNiveau(chaine);
    *m_niveauEnCours = chaine;
}

void Editeur::closeEvent(QCloseEvent *event)
{
    this->sauverNiveau(*m_niveauEnCours);
    event->accept();

}

void Editeur::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Editeur::on_btnQuitter_clicked()
{
    this->close();

}
