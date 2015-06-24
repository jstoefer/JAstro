#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QPainter>

#include "partnerprofile.h"

class DataTable : public QWidget
{
    Q_OBJECT
public:
    explicit DataTable(QWidget *parent = 0);

    /**
     * @brief drawInnerPartner
     *
     * @param painter   The QPainter object which doies the actual drawing
     * @param xPos      Start / End position, x - direction
     * @param yPos      Start / End position, y - direction
     * @param male      If true, the horoscpoe owner is male and thus it is assumed that the inner partner is female.
     *                  Otherwise, a male inner partner will be derived from the horoscope data.
     */
    void drawInnerPartner(QPainter &painter, int &xPos, int &yPos, bool male);

signals:

public slots:

    /**
     * @brief calculatePartnerProfile
     * Calculate both male and female partner profile
     */
    void calculatePartnerProfiles(Horoscope *h);

protected:

    /**
      * all painting happens here
      */
    void paintEvent(QPaintEvent *event);

private:

     PartnerProfile *partnerProfileM, *partnerProfileF;
     bool dataValid;

};

#endif // DATATABLE_H
