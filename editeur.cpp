#include "editeur.h"
#include "ui_editeur.h"

#include <QComboBox>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>


Editeur::Editeur(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Editeur)
{
    this->setWindowModality(Qt::ApplicationModal);
    m_ui->setupUi(this);
        m_ui->cbNiveau->addItem(tr("Niveau1"), 1);
        m_ui->cbNiveau->addItem(tr("Niveau2"), 2);
        m_ui->cbNiveau->addItem(tr("Niveau3"), 3);
        m_ui->cbNiveau->addItem(tr("Personnel"),4);

        m_ui->cbOperation->addItem(tr("Addition"), 1);
        m_ui->cbOperation->addItem(tr("Multiplication"), 2);
        m_ui->cbOperation->addItem(tr("Soustraction"),3);

        connect(m_ui->sldVitesse, SIGNAL(valueChanged(int)), m_ui->pbVitesse, SLOT(setValue(int)));
        m_niveauEnCours = new QString(m_ui->cbNiveau->currentText());
        QFile* fichierConf = new QFile(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf");
             if (!fichierConf->exists()) initialiser();
        this->chargerNiveau(*m_niveauEnCours);

        connect(m_ui->cbNiveau, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerNiveau(QString)));
        connect(m_ui->spbGMin, SIGNAL(valueChanged(int)), this, SLOT(testerValeurs(int)));
        connect(m_ui->spbGMax, SIGNAL(valueChanged(int)), this, SLOT(testerValeurs(int)));
        connect(m_ui->spbDMin, SIGNAL(valueChanged(int)), this, SLOT(testerValeurs(int)));
        connect(m_ui->spbDMax, SIGNAL(valueChanged(int)), this, SLOT(testerValeurs(int)));

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
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup(tr("Niveau1"));
                config.setValue(tr("MinGauche"), 0);
                config.setValue(tr("MaxGauche"), 5);
                config.setValue(tr("MinDroite"), 0);
                config.setValue(tr("MaxDroite"), 5);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Niveau2"));
                config.setValue(tr("MinGauche"), 5);
                config.setValue(tr("MaxGauche"), 9);
                config.setValue(tr("MinDroite"), 5);
                config.setValue(tr("MaxDroite"), 9);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Niveau3"));
                config.setValue(tr("MinGauche"), 10);
                config.setValue(tr("MaxGauche"), 19);
                config.setValue(tr("MinDroite"), 10);
                config.setValue(tr("MaxDroite"), 19);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Personnel"));
                config.setValue(tr("MinGauche"), 0);
                config.setValue(tr("MaxGauche"), 5);
                config.setValue(tr("MinDroite"), 0);
                config.setValue(tr("MaxDroite"), 5);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
    config.endGroup();
}

void Editeur::initialiserApproche(QString operation)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup(tr("Niveau1"));
                config.setValue(tr("MaxGauche"), 100);
                config.setValue(tr("MaxDroite"), 100);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Niveau2"));
                config.setValue(tr("MaxGauche"), 1000);
                config.setValue(tr("MaxDroite"), 100);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Niveau3"));
                config.setValue(tr("MaxGauche"), 1000);
                config.setValue(tr("MaxDroite"), 1000);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Personnel"));
                config.setValue(tr("MaxGauche"), 1000);
                config.setValue(tr("MaxDroite"), 1000);
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
    config.endGroup();
}

void Editeur::initialiserComplement(QString operation)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.beginGroup(operation);
            config.beginGroup(tr("Niveau1"));
                config.setValue(tr("TempsAccorde"),8);
            config.endGroup();
            config.beginGroup(tr("Niveau2"));
                config.setValue(tr("TempsAccorde"),6);
            config.endGroup();
            config.beginGroup(tr("Niveau3"));
                config.setValue(tr("TempsAccorde"),4);
            config.endGroup();
            config.beginGroup(tr("Personnel"));
                config.setValue(tr("TempsAccorde"),6);
            config.endGroup();
            config.setValue("NiveauEnCours"+operation, "Niveau1");
    config.endGroup();
}


void Editeur::initialiser()
{
    //On aurait pu initialiser dans le répertoire conf de l'application, mais l'utilisateur n'aurait pas eu les droits
    //QSettings config(QCoreApplication::applicationDirPath()+"/conf/parametres.conf", QSettings::IniFormat);

    //On initialise donc directement dans le /home de l'utilisateur
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    config.setValue(tr("NombreBallons"), 10);
    initialiserOperation("addition");
    initialiserOperation("multiplication");
    initialiserOperation("soustraction");
    initialiserApproche("approcheA");
    initialiserApproche("approcheS");
    initialiserApproche("approcheM");
    initialiserComplement("complementA10");
    initialiserComplement("complementA20");
    initialiserComplement("complementA100");
    for (int i=1;i<=5;i++) {
        initialiserComplement("complementM"+QString::number(i*5));
        }
    initialiserComplement("complementM50");
}

void Editeur::sauverNiveau(QString niveau)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
        config.setValue(tr("NombreBallons"), m_ui->spbNombreBallons->value());
        config.beginGroup(m_ui->cbOperation->currentText());
            config.beginGroup(niveau);
                config.setValue(tr("MinGauche"), m_ui->spbGMin->value());
                config.setValue(tr("MaxGauche"), m_ui->spbGMax->value());
                config.setValue(tr("MinDroite"), m_ui->spbDMin->value());
                config.setValue(tr("MaxDroite"), m_ui->spbDMax->value());
                config.setValue(tr("TempsAccorde"),m_ui->sldVitesse->value());
            config.endGroup();
        config.endGroup();
        *m_niveauEnCours = niveau;
    //je supprime pas pour l'instant, je garde dans un coin :
}

void Editeur::chargerNiveau(QString niveau)
{
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres.conf", QSettings::IniFormat);
    m_ui->spbNombreBallons->setValue(config.value(tr("NombreBallons")).toInt());
    config.beginGroup(m_ui->cbOperation->currentText());
        config.beginGroup(niveau);
            m_ui->spbGMax->setValue(config.value(tr("MaxGauche")).toInt());
            m_ui->spbGMin->setValue(config.value(tr("MinGauche")).toInt());
            m_ui->spbDMax->setValue(config.value(tr("MaxDroite")).toInt());
            m_ui->spbDMin->setValue(config.value(tr("MinDroite")).toInt());
            m_ui->sldVitesse->setValue(config.value(tr("TempsAccorde")).toInt());
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
    event->accept();
    delete(this);

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
    //delete(this);
}

bool Editeur::testerValeurs(int valeurNouvelle)
{
    if (m_ui->spbDMin<=m_ui->spbDMax && m_ui->spbGMin<=m_ui->spbGMax) return true;

    else {
        QMessageBox::information(this, QString::fromUtf8(tr("Erreur dans les paramètres").toStdString().c_str()), QString::fromUtf8(tr("Les Maxima doivent être supérieurs aux minima !!").toStdString().c_str()));
        return false;
    }
    qDebug()<<valeurNouvelle;
}
