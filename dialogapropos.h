#ifndef DIALOGAPROPOS_H
#define DIALOGAPROPOS_H

#include <QtGui/QDialog>

namespace Ui {
    class DialogAPropos;
}

class DialogAPropos : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DialogAPropos)
public:
    explicit DialogAPropos(QWidget *parent = 0);
    virtual ~DialogAPropos();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogAPropos *m_ui;
};

#endif // DIALOGAPROPOS_H
