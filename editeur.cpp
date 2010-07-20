#include "editeur.h"
#include "ui_editeur.h"

 #include <QComboBox>
#include <QSettings>
#include <QCloseEvent>


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

   QSettings config("./maConfig.ini", QSettings::IniFormat);
       config.setValue("langue", "fr");
       for (int i=1;i<4;i++) {
            QString niv = "Niveau";
            QString iString;
            iString.setNum(i);
            niv.append(iString);
            config.beginGroup(niv);
            config.setValue("MinGauche", m_ui->spbGMin->value());
            config.setValue("MaxGauche", m_ui->spbGMax->value());
            config.setValue("MinDroite", m_ui->spbDMin->value());
            config.setValue("MaxDroite", m_ui->spbDMax->value());
            config.endGroup();
        }
       config.beginGroup("Personnel");
            config.setValue("MinGauche", m_ui->spbGMin->value());
            config.setValue("MaxGauche", m_ui->spbGMax->value());
            config.setValue("MinDroite", m_ui->spbDMin->value());
            config.setValue("MaxDroite", m_ui->spbDMax->value());
            config.endGroup();

}

Editeur::~Editeur()
{
    delete m_ui;
}

void Editeur::closeEvent(QCloseEvent *event)
{
    QSettings config("./maConfig.ini", QSettings::IniFormat);
    config.beginGroup(m_ui->cbNiveau->currentText());
        config.setValue("MinGauche", m_ui->spbGMin->value());
        config.setValue("MaxGauche", m_ui->spbGMax->value());
        config.setValue("MinDroite", m_ui->spbDMin->value());
        config.setValue("MaxDroite", m_ui->spbDMax->value());
    config.endGroup();
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
