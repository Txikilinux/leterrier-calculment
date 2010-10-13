#include <QtGui/QApplication>
#include "interface.h"
#include "exercice.h"
#include "editeur.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString exo;
    QString nivo="";
    int argument=0;
    if (argc>1) {
        exo= argv[1];
        exo.remove("--exercice=", Qt::CaseInsensitive);
        exo.toLower();}
    qDebug()<<"exo : "<<exo;
    QString deuze;
    QString troize;
    if (argc>2){
        deuze= argv[2];
        troize=argv[3];
        qDebug()<<"deuze : "<<deuze<<"3ème lettre : "<<deuze[2];
        if (deuze[2]=='n') {
            deuze.remove("--niveau=", Qt::CaseInsensitive);
            deuze.prepend("Niveau");
            nivo=deuze;
            qDebug()<<"nivo : "<<nivo;
            }
        else if (deuze[2]=='a') {
            deuze.remove("--argument=", Qt::CaseInsensitive);
            argument=deuze.toInt();
            }
        if (troize[2]=='n') {
            troize.remove("--niveau=", Qt::CaseInsensitive);
            troize.prepend("Niveau");
            nivo=troize;
            qDebug()<<"nivo : "<<nivo;
            }
        else if (troize[2]=='a') {
            troize.remove("--argument=", Qt::CaseInsensitive);
            argument=troize.toInt();
            }
        }
    //à améliorer : dans un cas l'interface est créée pour rien, dans l'autre c'est l'exercice... de toute façon c'est de la mémoire utilisée pour rien !!
    interface w;
    w.setWindowTitle("Calcul Mental");
    exercice e(exo,argument,nivo);
    e.setWindowTitle("exercice");
    if (argc==1) w.show();
    else e.show();
    return a.exec();
}
