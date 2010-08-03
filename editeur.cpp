#include "editeur.h"
#include "ui_editeur.h"

#include <QComboBox>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>


Editeur::Editeur(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Editeur)
{
    m_ui->setupUi(this);
        m_ui->cbNiveau->addItem(tr("Niveau1"), 1);
        m_ui->cbNiveau->addItem(tr("Niveau2"), 2);
        m_ui->cbNiveau->addItem(tr("Niveau3"), 3);
        m_ui->cbNiveau->addItem(tr("Personnel"),4);

        m_ui->cbOperation->addItem(tr("Addition"), 1);
        m_ui->cbOperation->addItem(tr("Multiplication"), 2);

        connect(m_ui->sldVitesse, SIGNAL(valueChanged(int)), m_ui->pbVitesse, SLOT(setValue(int)));
        m_niveauEnCours = new QString(m_ui->cbNiveau->currentText());
        QFile* fichierConf = new QFile("./maConfig.ini");
             if (!fichierConf->exists()) initialiser();
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

void Editeur::initialiserOperation(QString operation)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup(tr("Niveau1"));
                config.setValue(tr("MinGauche"), 0);
                config.setValue(tr("MaxGauche"), 5);
                config.setValue(tr("MinDroite"), 0);
                config.setValue(tr("MaxDroite"), 5);
            config.endGroup();
            config.beginGroup(tr("Niveau2"));
                config.setValue(tr("MinGauche"), 5);
                config.setValue(tr("MaxGauche"), 9);
                config.setValue(tr("MinDroite"), 5);
                config.setValue(tr("MaxDroite"), 9);
            config.endGroup();
            config.beginGroup(tr("Niveau3"));
                config.setValue(tr("MinGauche"), 10);
                config.setValue(tr("MaxGauche"), 19);
                config.setValue(tr("MinDroite"), 10);
                config.setValue(tr("MaxDroite"), 19);
            config.endGroup();
            config.beginGroup(tr("Personnel"));
                config.setValue(tr("MinGauche"), 0);
                config.setValue(tr("MaxGauche"), 5);
                config.setValue(tr("MinDroite"), 0);
                config.setValue(tr("MaxDroite"), 5);
            config.endGroup();
    config.setValue("NiveauEnCours"+operation, "Niveau1");
    config.endGroup();
}
void Editeur::initialiser()
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.setValue(tr("NombreBallons"), 10);
    config.setValue(tr("TempsAccorde"),8);
    initialiserOperation("Addition");
    initialiserOperation("Multiplication");
}
void Editeur::sauverNiveau(QString niveau)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.beginGroup(m_ui->cbOperation->currentText());
        config.beginGroup(niveau);
            config.setValue(tr("MinGauche"), m_ui->spbGMin->value());
            config.setValue(tr("MaxGauche"), m_ui->spbGMax->value());
            config.setValue(tr("MinDroite"), m_ui->spbDMin->value());
            config.setValue(tr("MaxDroite"), m_ui->spbDMax->value());
        config.endGroup();
    config.endGroup();
    *m_niveauEnCours = niveau;
}

void Editeur::chargerNiveau(QString niveau)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    m_ui->sldVitesse->setValue(config.value(tr("TempsAccorde")).toInt());
    m_ui->spbNombreBallons->setValue(config.value(tr("NombreBallons")).toInt());
    config.beginGroup(m_ui->cbOperation->currentText());
        config.beginGroup(niveau);
            m_ui->spbGMax->setValue(config.value(tr("MaxGauche")).toInt());
            m_ui->spbGMin->setValue(config.value(tr("MinGauche")).toInt());
            m_ui->spbDMax->setValue(config.value(tr("MaxDroite")).toInt());
            m_ui->spbDMin->setValue(config.value(tr("MinDroite")).toInt());
        config.endGroup();
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
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.setValue(tr("TempsAccorde"),m_ui->sldVitesse->value());
    config.setValue(tr("NombreBallons"), m_ui->spbNombreBallons->value());
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
