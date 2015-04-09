/**
  * Implémentation de la fonction main qui permet soit de lancer l'interface générale, soit de lancer un exercice donné, avec éventuellement un niveau donné
  *
  * @warning nécessité de respecter la syntaxe
  * @see https://redmine.ryxeo.com/projects/leterrier-calculment/
  * @author 2009-2010 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @see The GNU Public License (GPL)
  */

/*
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

#include "abuleduapplicationv1.h"
#include "abuledusplashscreenv1.h"
#include "abuleduetiquettesv1.h"
#include "interface.h"
#include "exerciceoperation.h"
#include "exercicemaisonnombres.h"
#include "editeur.h"
#include "version.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
    AbulEduApplicationV1 a(argc, argv,VER_INTERNALNAME_STR, VER_PRODUCTVERSION_STR, VER_COMPANYDOMAIN_STR, VER_COMPANYNAME_STR, VER_UNITVERSION_STR);
    a.setAbeApplicationLongName(QObject::trUtf8(VER_FILEDESCRIPTION_STR));

    abeApp->setProperty("langageUtilise",QLocale::system().name().section('_', 0, 0));
    /** @todo regarder si c'est encore nécessaire, ça */
    abeApp->setProperty("utilisateur","");
    abeApp->setProperty("afficheBilanExercice",false);

    /** @todo corriger le commentaire ci-dessous */
    /* On peut désormais lancer Calcul Mental avec des arguments
     * par exemple, le lancement en console de "leterrier-calculment --exercice=tableM --option=7 --fond=~/leterrier/abebox/leterrier-calculment/fondecran.jpg"
     * lancera des calculs sur la table de multiplication par 7 en utilisant comme fond d'écran le fichier fondecran.jpg  */

    QStringList arguments = a.arguments();
    qDebug()<<arguments;
    if (arguments.size() == 1)
    {
        AbulEduSplashScreenV1 *splash = new AbulEduSplashScreenV1(0,true);
        splash->show();
        abeApp->processEvents();
        splash->setMainWindow(new interface(0));
        splash->launch(1000);
    }
    else {
        QString exerciseType;
//        QString unitName; --> le jour où on gèrera le travail en modules
        QString background;
        QString option = "-1";
        QString niveau = "-1";
        QStringListIterator iter(arguments);
        while(iter.hasNext()){
            QString current = iter.next();
            if(current.contains("--exercice")){
                exerciseType = current.split("=").last();
            }
            /** @todo utiliser des modules -> version-3.0 (voir aussi les lignes commentées pour la QMessageBox::information et l'instanciation de l'AbulEduFileV1) */
        /*  --> le jour où on gèrera le travail en modules
            else if(current.contains("--module=")){
                unitName = current.split("=").last();
                unitName.replace("~",QDir::homePath());
            } */
            else if(current.contains("--fond=")){
                background = current.split("=").last();
                background.replace("~",QDir::homePath());
            }
            else if(current.contains("--option=")){
                option = current.split("=").last();
            }
            else if(current.contains("--niveau=")){
                niveau = current.split("=").last();
            }
            else if(current.contains("--help")) {
                QMessageBox::information(new QWidget(),"Aide","Vous pourrez lancer ce logiciel avec les arguments suivants:\
                         \n\--help : cette aide\
                         \n\--exercice=type exercice : lance directement un exercice, choisissez parmi addition, soustraction, multiplication, division, maisonDesNombres, OdGrandeur, tableA, tableM, complementA ou complementM\
                         \n\--option= un nombre de 2 à 9 pour les tables, un nombre parmi 5, 10, 15, 20, 25 et 50 pour complementM (multiples), un nombre parmi 10, 100 ou 1000 pour complementA, un nombre parmi 0 (nombres de 1 à 10) et 10 (nombres de 11 à 20) pour maisonDesNombres, un mot parmi Addition, Soustraction et Multiplication pour les ordres de grandeur \
                         \n\--fond=image de fond : indiquez son chemin absolu\n\
                        ");
                        return 0;
            }
        }
        AbulEduEtiquettesV1* g = new AbulEduEtiquettesV1(QPointF(0,0));
        g->setGeometry(QDesktopWidget().frameGeometry());
        g->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        if(!background.isEmpty()){
            QPixmap pixFond(background);
            g->setBackgroundBrush(QBrush(pixFond.scaledToWidth(g->width())));
        }
        /*  --> le jour où on gèrera le travail en modules
        AbulEduFileV1* abeFile = new AbulEduFileV1();
        if(!abeFile->abeFileOpen(unitName)){
            AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(QObject::trUtf8("Problème !!"),QObject::trUtf8("Impossible d'ouvrir le module..."),true,g);
            msg->show();
            QObject::connect(msg, SIGNAL(signalAbeMessageBoxCloseOrHide()),g, SLOT(close()),Qt::UniqueConnection);
        } */

        if(QString::compare(exerciseType, "multiplication", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "division", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "addition", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "soustraction", Qt::CaseInsensitive) == 0){
            ExerciceOperation *ex = new ExerciceOperation(exerciseType, g,0,niveau.toInt());
            QObject::connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else if(QString::compare(exerciseType, "OdGrandeur", Qt::CaseInsensitive) == 0){
            int number = 0;
            if(QString::compare(option, "Addition", Qt::CaseInsensitive) != 0
                    && QString::compare(option, "Soustraction", Qt::CaseInsensitive) != 0
                    && QString::compare(option, "Division", Qt::CaseInsensitive) != 0
                    && QString::compare(option, "Multiplication", Qt::CaseInsensitive) != 0){
                option = QString();
                number = -1;
            }
            option = option.toLower();
            option = option.replace(option.at(0), option.at(0).toUpper());
            ExerciceOperation *ex = new ExerciceOperation(exerciseType.append(option), g,number,niveau.toInt());
            QObject::connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else if(QString::compare(exerciseType, "tableM", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "tableA", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "complementA", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "complementM", Qt::CaseInsensitive) == 0){
            ExerciceOperation *ex = new ExerciceOperation(exerciseType, g,option.toInt(),niveau.toInt());
            ex->connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else if(QString::compare(exerciseType, "maisonDesNombres", Qt::CaseInsensitive) == 0){
            int number = -1;
            int optionInt = option.toInt();
            if(optionInt == 0 || optionInt == 10){
                number = optionInt;
            }
            ExerciceMaisonNombres* ex = new ExerciceMaisonNombres(exerciseType,g,number,niveau.toInt());
            QObject::connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else {
            /** @todo indiquer ici les valeurs possibles pour les noms d'exercice */
            AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(QObject::trUtf8("Problème !!"),QObject::trUtf8("Impossible de lancer l'exercice spécifié..."),true,g);
            msg->show();
            QObject::connect(msg, SIGNAL(signalAbeMessageBoxCloseOrHide()),g, SLOT(close()),Qt::UniqueConnection);
        }
//        qDebug()<<abeFile->abeFileGetContent(0).filePath(); --> le jour où on gèrera le travail en modules
        g->show();
    }
    return a.exec();
}
