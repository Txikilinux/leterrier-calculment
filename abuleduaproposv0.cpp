/** Implémentation Automatique du menu Aide
 *
 * @see https://redmine.ryxeo.com/projects/leterrier-developpeu/
 * @author 2010 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
 * @see The GNU Public License (GPL)
 */

/**
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
#include "abuleduaproposv0.h"
#include "ui_abuleduaproposv0.h"
#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include <QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QtXml>




AbulEduAproposV0::AbulEduAproposV0(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AbulEduAproposV0)
{
    ui->setupUi(this);
    this->setWindowTitle(trUtf8("A propos de")+" "+qApp->applicationName()+" "+qApp->applicationVersion());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    installeMenu(); //Installe le menu Aide dans la menuBar

    afficheRSS(); // Récupère, s'il exite, le flux RSS du forum lié au logiciel (entraide.abuledu-fr.org/rss/forum/leterrier-MonLogiciel)


}

AbulEduAproposV0::~AbulEduAproposV0()
{
    delete ui;
}


void AbulEduAproposV0::installeMenu()
{
    QString titreAbout=trUtf8("<center>%1 %2</center").arg(qApp->applicationName()) //Récupère les paramètres
                       .arg(qApp->applicationVersion());
    ui->textBrowser_3->setHtml(titreAbout+" "+ui->textBrowser_3->toHtml());         // Ajoute Nom application et Version en haut de la page

    QMenuBar *b = parentWidget()->findChild<QMenuBar *>(); // Cherche la barre de menu
    w=b->parentWidget();
    QMenu *m = w->findChild<QMenu *>("menuAide"); //On cherche si le menu aide existe

    if(!m)  //sinon, on crée le menu Aide
    {
        QMenu *menuAide = new QMenu("Aide",b);
        menuAide->setObjectName("menuAide");
        b->addMenu(menuAide); //Ajoute l'entrée Aide au menu

        //Ajout des entrées de menu

        //Aide
        w= b->parent(); //Recupère la fenetre parente
        QString nomItem=trUtf8("Aide");
        actionAide=new QAction(nomItem,w);
        menuAide->addAction(actionAide);
        if(qApp->organizationDomain()=="")
        {
            actionAide->setDisabled(true);
        }
        connect(actionAide, SIGNAL(triggered()), this, SLOT(aide()));

        //
        nomItem=trUtf8("Aide intégrée...");
        actionAideIntegree=new QAction(nomItem,w);
        actionAideIntegree->setShortcut(Qt::Key_F1);
        menuAide->addAction(actionAideIntegree);
        connect(actionAideIntegree, SIGNAL(triggered()),this,SLOT(montreAide()));

        // News
        nomItem=trUtf8("Nouveautés...");
        actionNews=new QAction(nomItem,w);
        menuAide->addAction(actionNews);
        connect(actionNews, SIGNAL(triggered()),this,SLOT(montreNews()));

        // Contacts
        nomItem=trUtf8("Contacts...");
        actionContacts=new QAction(nomItem,w);
        menuAide->addAction(actionContacts);
        connect(actionContacts, SIGNAL(triggered()),this,SLOT(montreContacts()));

        // Traductions
        nomItem=trUtf8("Traduire ce logiciel...");
        actionTraductions=new QAction(nomItem,w);
        menuAide->addAction(actionTraductions);
        connect(actionTraductions, SIGNAL(triggered()),this,SLOT(montreTraduire()));

        // A propos
        nomItem=trUtf8("A Propos de ce logiciel...");
        actionTraductions=new QAction(nomItem,w);
        menuAide->addAction(actionTraductions);
        connect(actionTraductions, SIGNAL(triggered()),this,SLOT(montreAPropos()));


    }

}

//--------------------------------------------------------------------------------------
//
//------------------------------Gestion fenêtre A Propos--------------------------------
//
//--------------------------------------------------------------------------------------


/**
  *les fonctions montreAide(), montreNews(), montreContacts(), montreTraduire(), montreAPropos()
  *créent la fenêtre d'APropos en mode modal et positionnent le bon onglet
  *en émettant le signal setIndex(int)
  */
void AbulEduAproposV0::montreAide()                                                   //
{
    connect(this, SIGNAL(setindex(int)),this,SLOT(changeTab(int)));
    emit setindex(0);
    this->show();
}

void AbulEduAproposV0::montreNews()                                                   //
{
    connect(this, SIGNAL(setindex(int)),this,SLOT(changeTab(int)));
    emit setindex(1);
    this->show();
}
void AbulEduAproposV0::montreContacts()
{
    connect(this, SIGNAL(setindex(int)),this,SLOT(changeTab(int)));
    emit setindex(2);
    this->show();
}

void AbulEduAproposV0::montreTraduire()
{
    connect(this, SIGNAL(setindex(int)),this,SLOT(changeTab(int)));
    emit setindex(3);
    this->exec();
}

void AbulEduAproposV0::montreAPropos()
{
    connect(this, SIGNAL(setindex(int)),this,SLOT(changeTab(int)));
    emit setindex(4);
    this->exec();
}

void AbulEduAproposV0::changeTab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

/** Slot lançant un navigateur vers l'Url passée dans qApp->setOrganizationDomain(); */
void AbulEduAproposV0::aide()
{
    QUrl urlSite=qApp->organizationDomain();
    QDesktopServices::openUrl(urlSite);
}

/** Lance le flux RSS des nouveautés */
void AbulEduAproposV0::afficheRSS()
{
    //Message d'erreur
    QString messageErreur = trUtf8("Le site des nouveautés n'est pas accessible");
    //Récupère l'adresse qui est passée dans le textBrowser
    QUrl adresseFlux = ui->textBrowser_5->toPlainText();

    if(!adresseFlux.isValid())

    {

        ui->textBrowser_5->setHtml(messageErreur);
    }
    else
    {
        //On y va
        nam = new QNetworkAccessManager(this);
        connect(nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply*)));

        QNetworkReply* reply = nam->get(QNetworkRequest(adresseFlux));



    }

}

void AbulEduAproposV0::finishedSlot(QNetworkReply* reply)
{
    // Lecture dezs attributs du "reply"
    // par exemple le code des status
    QVariant statusCodeV =
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);


    // Pas d'erreur ?
    if (reply->error() == QNetworkReply::NoError)
    {
        // lectures des données issues du "reply"

        QByteArray bytes = reply->readAll();  // bytes
        QString flux(bytes); // string
        AbulEduAproposV0::montreRSS(flux);

    }
    // Some http error received
    else
    {
        QString messageErreur = trUtf8("Le site des nouveautés n'est pas accessible");
        ui->textBrowser_5->setHtml(messageErreur);
    }

    // Il faudrait détruire le reply
    // mais, pas ici ?

}

void AbulEduAproposV0::montreRSS(QString flux)
{
    xml= new QDomDocument();
    xml->setContent(flux);
    QDomElement xmlElem = xml->documentElement();
    QDomNode node = xmlElem.firstChild();
    while(!node.isNull())
    {
        QDomElement element = node.toElement(); // On essaie de convertir le node en element
        if(!element.isNull())
        {
            if(element.tagName()=="channel")    // Le node est bien un élément.
            {

                QDomNode node=element.firstChild(); //On parcourt le document à partir du tag "channel"
                while(!node.isNull())
                {
                    QDomElement elem=node.toElement(); //On cherche le titre du "Channel"
                    if(elem.tagName()=="title")
                    {
                        QDomNode node=elem.firstChild();
                        QString titre=node.toText().data();
                        ui->textBrowser_5->setHtml("<h4><font color=red>"+titre+"</font></h4>");
                    }
                    else if((elem.tagName()=="item"))
                    {
                        QDomNode node=elem.firstChild();
                        while(!node.isNull())
                        {
                            QDomElement elem=node.toElement(); //Titre du fils de discussion
                            if(elem.tagName()=="title")
                            {
                                QDomNode node=elem.firstChild();
                                itemTitre=node.toText().data();
                                node=node.nextSiblingElement("link");

                            }
                            if(elem.tagName()=="link")      // Lien vers post
                            {
                                QDomNode node=elem.firstChild();
                                itemLink=node.toText().data();
                                node=node.nextSiblingElement("description");

                            }
                            if(elem.tagName()=="description") // Texte du post
                            {
                                QDomNode node=elem.firstChild();
                                itemDescription =node.toText().data();
                                node=node.nextSibling();

                            }

                            node=node.nextSibling();
                        }
                        QString itemView= "<a href="+itemLink+">"
                                          "<font color=green>"+itemTitre+"</font></a>"
                                          +itemDescription;
                        ui->textBrowser_5->append(itemView);

                    }
                    node=node.nextSibling();
                }
            }
            node = node.nextSibling();
        }

    }

}



