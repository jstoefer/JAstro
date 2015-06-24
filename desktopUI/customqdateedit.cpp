#include "customqdateedit.h"

CustomQDateEdit::CustomQDateEdit(QWidget *parent) : QDateEdit( parent) {

}


CustomQDateEdit::CustomQDateEdit(QDate &date, QWidget *parent) : QDateEdit(date, parent) {

}

/**
* this effectively disables step checking on QDateEdits
* do not use this if you want min and max ranges defined...unless you want it wrapping
*/
QDateTimeEdit::StepEnabled CustomQDateEdit::stepEnabled() const {
    return QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
}

/**
* custom stepBy...this allows rollover of months and years on QDateEdits
*/
void CustomQDateEdit::stepBy(int steps) {

    QDate date(QDateEdit::date());
    QDateEdit::Section temp_section = currentSection();

    if(temp_section == QDateEdit::DaySection) {
        QDate next_date(date.addDays(steps));
        if(next_date.month() != date.month() || next_date.year() != date.year()) {
            QDateEdit::setDate(next_date);
            steps = 0;
        }
    } else if(temp_section == QDateEdit::MonthSection) {
        QDate next_date(date.addMonths(steps));
        if(next_date.year() != date.year()){
            QDateEdit::setDate(next_date);
            steps = 0;
        }
    }
    QDateEdit::stepBy(steps);

}
