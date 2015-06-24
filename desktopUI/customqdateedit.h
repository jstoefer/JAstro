#ifndef CUSTOMQDATEEDIT_H
#define CUSTOMQDATEEDIT_H

#include <QDateEdit>

class CustomQDateEdit : public QDateEdit {


    Q_OBJECT

public:

    CustomQDateEdit(QWidget *parent = 0);
    CustomQDateEdit(QDate &date, QWidget *parent = 0);
    QDateTimeEdit::StepEnabled stepEnabled() const;

    void stepBy(int steps);

};

#endif // CUSTOMQDATEEDIT_H
