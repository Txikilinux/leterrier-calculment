/** Dérivation de QLineEdit afin de d'avoir en permance les chiffres sur le clavier (touches au-dessus des lettres)
  * Utilisé pour les exercices du logiciel Calcul mental
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#include "abuledulineeditv0.h"
#include <QApplication>

AbulEduLineEditV0::AbulEduLineEditV0(QWidget* parent)
    :QLineEdit(parent)
{

}

void AbulEduLineEditV0::keyPressEvent(QKeyEvent* event)
{
    //empêcher la propagation de l'événement
    if (!qApp->property("VerrouNombres").toBool())
        QLineEdit::keyPressEvent(event);
    else
    {
        event->accept();
//    qDebug()<<event->key()<<" <-> "<<event->text();

    switch (event->key())
    {
    case Qt::Key_Ampersand:
          /* 1- setText(this->text()+"1");*/
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_1,Qt::NoModifier,QString(QChar(Qt::Key_1))));
        break;

    case Qt::Key_Eacute:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_2,Qt::NoModifier,QString(QChar(Qt::Key_2))));
        break;

    case Qt::Key_QuoteDbl:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_3,Qt::NoModifier,QString(QChar(Qt::Key_3))));
        break;

    case Qt::Key_Apostrophe:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_4,Qt::NoModifier,QString(QChar(Qt::Key_4))));
        break;

    case Qt::Key_ParenLeft:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_5,Qt::NoModifier,QString(QChar(Qt::Key_5))));
        break;

    case Qt::Key_Minus:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_6,Qt::NoModifier,QString(QChar(Qt::Key_6))));
        break;

    case Qt::Key_Egrave:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_7,Qt::NoModifier,QString(QChar(Qt::Key_7))));
        break;

    case Qt::Key_Underscore:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_8,Qt::NoModifier,QString(QChar(Qt::Key_8))));
        break;

    case Qt::Key_Ccedilla:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_9,Qt::NoModifier,QString(QChar(Qt::Key_9))));
        break;

    case Qt::Key_Agrave:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_0,Qt::NoModifier,QString(QChar(Qt::Key_0))));
        break;

    // On vérifie si la touche est bien sur le keypad désactivé
    if (!(event->modifiers() & Qt::KeypadModifier) == 0)
    {
    case Qt::Key_Insert:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_0,Qt::NoModifier,QString(QChar(Qt::Key_0))));
        break;

    case Qt::Key_End:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_1,Qt::NoModifier,QString(QChar(Qt::Key_1))));
        break;

    case Qt::Key_Down:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_2,Qt::NoModifier,QString(QChar(Qt::Key_2))));
        break;

    case Qt::Key_PageDown:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_3,Qt::NoModifier,QString(QChar(Qt::Key_3))));
        break;

    case Qt::Key_Left:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_4,Qt::NoModifier,QString(QChar(Qt::Key_4))));
        break;

    case Qt::Key_Clear:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_5,Qt::NoModifier,QString(QChar(Qt::Key_5))));
        break;

    case Qt::Key_Right:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_6,Qt::NoModifier,QString(QChar(Qt::Key_6))));
        break;

    case Qt::Key_Home:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_7,Qt::NoModifier,QString(QChar(Qt::Key_7))));
        break;

    case Qt::Key_Up:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_8,Qt::NoModifier,QString(QChar(Qt::Key_8))));
        break;

    case Qt::Key_PageUp:
        QApplication::sendEvent(this,new QKeyEvent(QEvent::KeyPress,Qt::Key_9,Qt::NoModifier,QString(QChar(Qt::Key_9))));
        break;
    }
    default:
        QLineEdit::keyPressEvent(event);
    }
    }
}
