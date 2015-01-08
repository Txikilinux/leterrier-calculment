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

    /** @todo regarder si c'est encore nécessaire, ça */
    abeApp->setProperty("langageUtilise",QLocale::system().name().section('_', 0, 0));
    abeApp->setProperty("utilisateur","");
    abeApp->setProperty("afficheBilanExercice",false);

    /** @todo corriger le commentaire ci-dessous */
    /* On peut désormais lancer Aller avec des arguments
     * par exemple, le lancement en console de "leterrier-aller --exercice=ttr --module=~/leterrier/abebox/leterrier-aller/Heureux_ulysse_ce1.abe --fond=~/leterrier/abebox/leterrier-aller/fondecran.jpg"
     * lancera closure sur le module ulysse en utilisant comme fond d'écran le fichier fondecran.jpg  */

    QList<AbulEduLogLevelV1> bannishedLogs;
    bannishedLogs << WarnLevel << InfoLevel << TraceLevel;
    a.setAbeApplicationBannishedLogLevel(bannishedLogs);
    QStringList arguments = a.arguments();
    qDebug()<<arguments;
    if (arguments.size() == 1)
    {
        AbulEduSplashScreenV1 *splash = new AbulEduSplashScreenV1(0,true);
        splash->show();
        interface* w = new interface();
        splash->setMainWindow(w);
        splash->launch(1000);
    }
    else {
        QString exerciseType;
//        QString unitName; --> le jour où on gèrera le travail en modules
        QString background;
        QString option = "-1";
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
            else if(current.contains("--help")) {
                QMessageBox::information(new QWidget(),"Aide","Vous pourrez lancer ce logiciel avec les options suivantes:\n\
                                         \n\
                                         --help -> cette aide\n\
                                         \n\
                                         --exercice=type exercice -> lance directement un exercice, choisissez parmi ExerciceClosure, ExerciceOrdreAlpha1, ExerciceMotsMelanges, ExercicePhrasesMelangees, ExerciceParagraphesMelanges, ExercicePhraseSansEspaces ou ExerciceRapido\n\
//  --> le jour où on gèrera le travail en modules                                       \n\
//  --> le jour où on gèrera le travail en modules                                        --module=nom module -> indiquez son chemin absolu\n\
                        \n\
                        --fond=image de fond -> indiquez son chemin absolu\n\
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
            ExerciceOperation *ex = new ExerciceOperation(exerciseType, g);
            QObject::connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else if(QString::compare(exerciseType, "OdGrandeur", Qt::CaseInsensitive) == 0){
            int number = 0;
            if(QString::compare(option, "Addition", Qt::CaseInsensitive) != 0
                    && QString::compare(option, "Soustraction", Qt::CaseInsensitive) != 0
                    && QString::compare(option, "Multiplication", Qt::CaseInsensitive) != 0){
                option = QString();
                number = -1;
            }
            ExerciceOperation *ex = new ExerciceOperation(exerciseType.append(option), g,number);
            QObject::connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else if(QString::compare(exerciseType, "tableM", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "tableA", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "complementA", Qt::CaseInsensitive) == 0
                || QString::compare(exerciseType, "complementM", Qt::CaseInsensitive) == 0){
            ExerciceOperation *ex = new ExerciceOperation(exerciseType, g,option.toInt());
            ex->connect(ex, SIGNAL(signalExerciseExited()),g,SLOT(close()),Qt::UniqueConnection);
        }
        else if(QString::compare(exerciseType, "maisonDesNombres", Qt::CaseInsensitive) == 0){
            int number = -1;
            int optionInt = option.toInt();
            if(optionInt == 0 || optionInt == 10){
                number = optionInt;
            }
            ExerciceMaisonNombres* ex = new ExerciceMaisonNombres(exerciseType,g,number);
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
