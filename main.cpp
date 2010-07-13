#include <QtGui/QApplication>
#include "interface.h"
#include "exercice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString exo = argv[1];
    interface w;
    w.setWindowTitle("Calcul Mental");
    exercice e(exo);
    e.setWindowTitle("exercice");
    if (exo=="") w.show();
    else e.show();
    return a.exec();
}
