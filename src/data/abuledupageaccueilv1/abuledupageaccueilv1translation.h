#include <QObject>
#include "abuledupageaccueilv1.h"

class AbulEduPageAccueilV1Translation : public QObject
{
   /** Special pour traductions : qt translator pourra extraire ces chaines a traduire
    * attention, le contexte AbulEduPageAccueilV1 est important pour que la localisation
    * s'applique bien Ã  l'objet AbulEduPageAccueilV1
    */
   QString a0000 = AbulEduPageAccueilV1::trUtf8("Multiplications");
   QString a0001 = AbulEduPageAccueilV1::trUtf8("Maison des nombres");
   QString a0002 = AbulEduPageAccueilV1::trUtf8("Ordres de grandeur");
   QString a0003 = AbulEduPageAccueilV1::trUtf8("Additions");
   QString a0004 = AbulEduPageAccueilV1::trUtf8("Soustractions");
   QString a0005 = AbulEduPageAccueilV1::trUtf8("Tables de multiplication");
   QString a0006 = AbulEduPageAccueilV1::trUtf8("Tables d'addition");
   QString a0007 = AbulEduPageAccueilV1::trUtf8("Multiples");
   QString a0008 = AbulEduPageAccueilV1::trUtf8("Compléments additifs");
   QString a0009 = AbulEduPageAccueilV1::trUtf8("Divisions");
}