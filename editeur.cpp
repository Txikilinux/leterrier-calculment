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
        m_niveau = new QString(m_ui->cbNiveau->currentText());
        this->chargerNiveau(*m_niveau);
            //problème : sauver l'ancien niveau et charger le nouveau au changement de valeur du comboBox (peut être m_niveau dans editeur)

        connect(m_ui->cbNiveau, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerNiveau(QString)));
}

Editeur::~Editeur()
{
    delete m_ui;
}

//void Editeur::sauverNiveau()
//{
//    QSettings config("./maConfig.ini", QSettings::IniFormat);
//    config.beginGroup(m_ui->cbNiveau->currentText());
//        config.setValue("MinGauche", m_ui->spbGMin->value());
//        config.setValue("MaxGauche", m_ui->spbGMax->value());
//        config.setValue("MinDroite", m_ui->spbDMin->value());
//        config.setValue("MaxDroite", m_ui->spbDMax->value());
//    config.endGroup();
//}

void Editeur::sauverNiveau(QString niveau)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.beginGroup(niveau);
        config.setValue("MinGauche", m_ui->spbGMin->value());
        config.setValue("MaxGauche", m_ui->spbGMax->value());
        config.setValue("MinDroite", m_ui->spbDMin->value());
        config.setValue("MaxDroite", m_ui->spbDMax->value());
    config.endGroup();
}

//void Editeur::chargerNiveau()
//{
//    QSettings config("./maConfig.ini", QSettings::IniFormat);
//    config.beginGroup(m_ui->cbNiveau->currentText());
//        m_ui->spbGMax->setValue(config.value("MaxGauche").toInt());
//        m_ui->spbGMin->setValue(config.value("MinGauche").toInt());
//        m_ui->spbDMax->setValue(config.value("MaxDroite").toInt());
//        m_ui->spbDMin->setValue(config.value("MinDroite").toInt());
//    config.endGroup();
//}

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
    this->sauverNiveau(*m_niveau);
    this->chargerNiveau(chaine);
    *m_niveau = chaine;
}

void Editeur::closeEvent(QCloseEvent *event)
{
    this->sauverNiveau(*m_niveau);
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
