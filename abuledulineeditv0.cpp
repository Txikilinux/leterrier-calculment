#include "abuledulineeditv0.h"

AbulEduLineEditV0::AbulEduLineEditV0(QWidget* parent)
    :QLineEdit(parent)
{
}

void AbulEduLineEditV0::keyPressEvent(QKeyEvent* event)
{
    //empêcher la propagation de l'événement
    event->accept();
    qDebug()<<"Touche pressée dans AbulEduLineEdit";

    switch (event->key())
    {
    case Qt::Key_Ampersand:
        //Quand je mets la ligne 2 seule, rien...
        // Quand je mets la ligne 1 seule, j'ai le N
        // Quand je mets les 2 lignes, j'ai Nn
        //     ???????????
          /* 1- */setText(this->text()+"1");
        // 2- keyPressEvent(&QKeyEvent(QEvent::KeyPress,Qt::Key_N,Qt::NoModifier,"n"));
        qDebug()<<"Touche AltGr pressée";
        break;

    case Qt::Key_Eacute:
        setText(this->text()+"2");
        break;

    case Qt::Key_QuoteDbl:
        setText(this->text()+"3");
        break;

    case Qt::Key_Apostrophe:
        setText(this->text()+"4");
        break;

    case Qt::Key_BracketLeft:
        setText(this->text()+"5");
        break;

    case Qt::Key_Minus:
        setText(this->text()+"6");
        break;

    case Qt::Key_Egrave:
        setText(this->text()+"7");
        break;

    case Qt::Key_Underscore:
        setText(this->text()+"8");
        break;

    case Qt::Key_Ccedilla:
        setText(this->text()+"9");
        break;

    case Qt::Key_Agrave:
        setText(this->text()+"0");
        break;

    default:
        QLineEdit::keyPressEvent(event);
    }
}
