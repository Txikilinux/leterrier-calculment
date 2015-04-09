/** Lanceur sélectif pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2011-2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "abuledulanceurv1.h"
#include "ui_abuledulanceurv1.h"
#include "interface.h"

AbuleduLanceurV1::AbuleduLanceurV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbuleduLanceurV1),
    m_interface(0),
    m_nomFichierConfExercices(QString()),
    m_nomExercice(QString()),
    m_intituleExercice(QString()),
    m_listeExercices(QStringList()),
    m_listeNiveaux(QStringList())
{
    ui->setupUi(this);
    installEventFilter(this);
    setWindowTitle(trUtf8("Lanceur"));
    fillCbExercice();
    associeNomIntitule(ui->cbExercice->currentText());
    ui->leNom->setText(abeApp->getAbeIdentite()->abeGetNom());
    if (ui->leNom->text().isEmpty())
    {
        ui->leNom->setText(abeApp->getAbeIdentite()->abeGetLogin());
    }
    else if(!abeApp->getAbeIdentite()->abeGetPreNom().isEmpty())
    {
        ui->leNom->setText(ui->leNom->text().prepend(abeApp->getAbeIdentite()->abeGetPreNom()+" "));
    }
    ui->leNom->setReadOnly(true);
    fillCbNiveau("sertARien");
    fillCbNombre("sertARien");
    connect(ui->cbExercice, SIGNAL(currentIndexChanged(QString)), this, SLOT(associeNomIntitule(QString)));
    connect(this,SIGNAL(signalCbExerciceFilled(QString)),this, SLOT(fillCbNombre(QString)));
    connect(this,SIGNAL(signalCbExerciceFilled(QString)),this, SLOT(fillCbNiveau(QString)));
    setMaximumSize(800*abeApp->getAbeApplicationDecorRatio(),400*abeApp->getAbeApplicationDecorRatio());
    setWindowModality(Qt::ApplicationModal);
}

AbuleduLanceurV1::~AbuleduLanceurV1()
{
    delete ui;
}

void AbuleduLanceurV1::abeLanceurSetIdentite(AbulEduIdentitesV1 *identite)
{
    ui->leNom->setText(identite->abeGetNom());
    if (ui->leNom->text().isEmpty())
    {
        ui->leNom->setText(identite->abeGetLogin());
    }
    else if(!identite->abeGetPreNom().isEmpty())
    {
        ui->leNom->setText(ui->leNom->text().prepend(identite->abeGetPreNom()+" "));
    }
}

void AbuleduLanceurV1::abeLanceurSetInterface(QMainWindow *interface)
{
    m_interface = interface;
}


void AbuleduLanceurV1::fillCbExercice()
{
    QString locale = qApp->property("langageUtilise").toString();
    ui->cbExercice->clear();
    QString nomFichierConf = "./conf/alacarte.conf";
    if (!QFile(nomFichierConf).exists()) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration non trouvé"));
        alertBox->show();
        on_btnAnnuler_clicked();
    }
    else {
        m_nomFichierConfExercices = nomFichierConf;
    }

    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
    configExo.setIniCodec("UTF-8");
    if (!configExo.childKeys().contains("niveaux_"+locale)) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Le fichier de configuration ne contient pas la clef : niveaux_")+locale);
        alertBox->show();
        this->deleteLater();
        return;
    }
    m_listeNiveaux = configExo.value("niveaux_"+locale,"Niveau1;Niveau2;Niveau3;Personnel").toString().split(";");

    configExo.beginGroup("Exercices");
    m_listeExercices = configExo.childGroups();
    QStringList intitulesExercicesProposes;
    QStringListIterator iter(m_listeExercices);
    while (iter.hasNext()){
        QString exerc = iter.next();
        configExo.beginGroup(exerc);
        //qDebug()<<"Je rentre dans "<<exerc<<" et j'ai "<<configExo.childKeys();
        if (!configExo.childKeys().contains("intitule_"+locale)) {
            QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Le fichier de configuration ne contient pas toutes les clefs : intitule_")+locale);
            alertBox->show();
            this->deleteLater();
            return;
        }
        intitulesExercicesProposes.append(QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str()));
        configExo.endGroup();
    }
    ui->cbExercice->addItems(intitulesExercicesProposes);
}

void AbuleduLanceurV1::fillCbNiveau(QString)
{
    QString locale = qApp->property("langageUtilise").toString();
    ui->cbNiveau->clear();
    QString nomFichierConf = "./conf/alacarte.conf";
    if (!QFile(nomFichierConf).exists()) {
        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Fichier de configuration non trouvé"));
        alertBox->show();
        on_btnAnnuler_clicked();
    }
    else {
        m_nomFichierConfExercices = nomFichierConf;
    }

    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
     configExo.setIniCodec("UTF-8");
    QString niv = configExo.value("niveaux_"+locale,"Niveau1;Niveau2;Niveau3;Personnel").toString();
    QStringList listeNiv = niv.split(";");
    if (niv.isEmpty()) {
        ui->cbNiveau->hide();
        ui->lblNiveau->hide();
    }
    else {
        ui->cbNiveau->addItems(listeNiv);
        ui->cbNiveau->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        ui->cbNiveau->show();
        ui->lblNiveau->show();
    }
}

void AbuleduLanceurV1::fillCbNombre(QString jsaispasquoi)
{
    /* Attention : vérifier si les espaces avant et après ajoutés au moment de l'enfournage des signes d'opération ne pose pas problème  */
    Q_UNUSED(jsaispasquoi)
    ABULEDU_LOG_TRACE()<<__PRETTY_FUNCTION__<<m_nomExercice ;
    ui->cbNombre->hide();
    ui->lblNombre->hide();
    ui->cbNombre->clear();
    bool trouve = false;
    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
    configExo.beginGroup("Exercices");
    QStringListIterator iter(m_listeExercices);
    while (iter.hasNext() && trouve==false){
        QString exerc = iter.next();
        configExo.beginGroup(exerc);
        ui->cbNombre->clear();
        if (configExo.value("nom").toString() == m_nomExercice) {
            trouve=true;
            QString val = configExo.value("liste").toString();
            QStringList listeVal;
            foreach(QString elt,val.split(";")){
                listeVal << " "+elt+" ";
            }
            if (val.isEmpty())
            {
                ui->cbNombre->hide();
                ui->lblNombre->hide();
            }
            else {
                ui->cbNombre->addItems(listeVal);
                ui->cbNombre->setSizeAdjustPolicy(QComboBox::AdjustToContents);
                ui->cbNombre->show();
                ui->lblNombre->show();
//                adapte();
            }
            return;
        }
        configExo.endGroup();
    }
}

void AbuleduLanceurV1::on_btnAnnuler_clicked()
{
    interface* i = (interface*) m_interface;
    i->slotInterfaceBackFromExercise();
}

void AbuleduLanceurV1::on_btnLancer_clicked()
{
    /** et le niveau ? */
//    qDebug()<<parent()->parent()->parent()->parent()->objectName();
    interface* i = (interface*) m_interface;
    QString nom;
    qApp->setProperty("utilisateur",ui->leNom->text());
    int val = 0;
    if(ui->cbNombre->isVisible()){
        val = ui->cbNombre->currentText().toInt();
    }
    int niveau = -1;
    if(ui->cbNiveau->isVisible()){
        niveau = ui->cbNiveau->currentIndex()+1;
    }

    /*  Attention à changer ligne 203 de exercice.cpp si on change le caractère de concaténation dans la ligne au dessus */
//    qApp->setProperty("utilisateur",qApp->property("utilisateur").toString().replace(";"," "));
//    qDebug()<<m_nomExercice<<" -> "<<i->abeInterfaceGetExerciceNames().value(m_nomExercice)<< ":: "<<val;
    if (m_nomExercice == "maisonDesNombres")
    {
        if(ui->cbNombre->currentText().contains("20")){
            val = 10;
        }
        else if(ui->cbNombre->currentText().contains("10")){
            val = 0;
        }
        i->slotInterfaceLaunchExercise(val,"Maisons",niveau);
    }
    else if(m_nomExercice == "OdGrandeur"){
        if(ui->cbNombre->currentText().simplified() == "+"){
            nom = "OdGrandeurAddition";
        }
        else if(ui->cbNombre->currentText().simplified() == "-"){
            nom = "OdGrandeurSoustraction";
        }
        else if(ui->cbNombre->currentText().simplified() == "x"){
            nom = "OdGrandeurMultiplication";
        }
        else if(ui->cbNombre->currentText().simplified() == QString::fromUtf8("÷")){
            nom = "OdGrandeurDivision";
        }
        else{
            qDebug()<<"Incohérence dans le lanceur";
        }
        i->slotInterfaceLaunchExercise(val,nom,niveau);
    }
    else
    {
        i->slotInterfaceLaunchExercise(val,i->abeInterfaceGetExerciceNames().value(m_nomExercice), niveau);
    }
}

void AbuleduLanceurV1::associeNomIntitule(QString intitule)
{
//    qDebug()<<"AbuleduLanceurV1::associeNomIntitule(1) : "<<intitule;
    QString locale = qApp->property("langageUtilise").toString();
    QSettings configExo(m_nomFichierConfExercices, QSettings::IniFormat);
    configExo.setIniCodec("UTF-8");
    configExo.beginGroup("Exercices");
//    int i = 1;
    bool trouve = false;
    QStringListIterator iterateur(m_listeExercices);
    while (iterateur.hasNext() && !trouve) {
        QString exercice = iterateur.next();
        configExo.beginGroup(exercice);
        if (QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str())==intitule) {
            m_nomExercice = configExo.value("nom").toString();
            m_intituleExercice = QString::fromUtf8((configExo.value("intitule_"+locale).toString()).toStdString().c_str());
            trouve = true;
        }
        configExo.endGroup();
//        i++;
    }
    if(m_nomExercice == "OdGrandeur"){
        ui->lblNombre->setText(trUtf8("Opération"));
    }
    else{
        ui->lblNombre->setText(trUtf8("Nombre"));
    }
    emit signalCbExerciceFilled(m_nomExercice);
}

bool AbuleduLanceurV1::eventFilter(QObject *obj, QEvent *event)
{
    /* Pas localDebug car il y a trop de message */
    if (event->type() == QEvent::KeyRelease)
    {
        qDebug()<<"qd";
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        /* Protection contre les clics frénétiques */
        if(!keyEvent->isAutoRepeat())
        {
            /* Navigation avec la touche Entrée : l'appui sur la touche Entrée provoque : */
            switch(keyEvent->key())
            {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                ui->btnLancer->click();
                break;
            case Qt::Key_Escape:
                ui->btnAnnuler->click();
                break;
            default:
                break;
            }
        }
        return true;
    }
    else
    {
        /* On fait suivre l'évènement, sinon tout est bloqué */
        return QObject::eventFilter(obj, event);
    }
}

void AbuleduLanceurV1::on_chbBilan_clicked()
{
    if (ui->chbBilan->isChecked())
    {
        qApp->setProperty("afficheBilanExercice",true);
    }
    else
    {
        qApp->setProperty("afficheBilanExercice",false);
    }
}
