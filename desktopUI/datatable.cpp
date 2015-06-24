#include "datatable.h"
#include <QDebug>

DataTable::DataTable(QWidget *parent) : QWidget(parent) {
    qDebug() << "DataTable::DataTable";
    dataValid = false;
}

/*----------------------------------------------------------------------------------------------*/

void DataTable::paintEvent(QPaintEvent *evt) {

    int xPos = 200; int yPos = 200;

    /* create painter and select paint device                                                   */
    QPainter painter;
    QColor tableColor(255, 255, 255);
    QPen pen(tableColor, 0);

    painter.begin(this);
    painter.setPen(pen);

    if (dataValid) {
        this->drawInnerPartner(painter, xPos, yPos, true);
        xPos = 500; yPos = 200;
        this->drawInnerPartner(painter, xPos, yPos, false);
    }

    painter.end();
}

/*----------------------------------------------------------------------------------------------*/

void DataTable::drawInnerPartner(QPainter &painter, int &xPos, int &yPos, bool male) {

    QString pp;

    if (male) {
        for (int l=0; l <= SE_PLUTO; l++) {
            // planetLetters[currentIndex]
            painter.drawText(xPos, yPos, planetNames[l]);
            xPos+= 100;
            QString score = QString("%1").arg(partnerProfileM->getScore(l));
            painter.drawText(xPos, yPos, score);
            yPos+=30; xPos-=100;
        }
    } else {
        for (int l=0; l <= SE_PLUTO; l++) {
            painter.drawText(xPos, yPos, planetNames[l]);
            xPos+= 100;
            QString score = QString("%1").arg(partnerProfileF->getScore(l));
            painter.drawText(xPos, yPos, score);
            yPos+=30; xPos-=100;
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void DataTable::calculatePartnerProfiles(Horoscope *h) {

    int partnerPlanet[2];

    partnerPlanet[0] = SE_MOON;
    partnerPlanet[1] = SE_VENUS;
    this->partnerProfileM = new PartnerProfile (h, partnerPlanet);

    partnerPlanet[0] = SE_SUN;
    partnerPlanet[1] = SE_MARS;
    this->partnerProfileF = new PartnerProfile (h, partnerPlanet);

    this->dataValid = true;
}
