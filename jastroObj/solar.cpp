#include "solar.h"

Solar::Solar(Horoscope *base, int solarYear) {

    this->base = base;              /* store pointer to base horoscope          */
    this->solarYear = solarYear;    /* store solar year                         */
    /* start with location of birth, may be modified by user later              */
    this->solarLatitude = base->latitude;
    this->solarLongitude = base->longitude;
    calculate();                    /* calculate solar                          */

}

/*----------------------------------------------------------------------------------------------*/

void Solar::setYear(int solarYear) {
    this->solarYear = solarYear;
    calculate();
}

/*----------------------------------------------------------------------------------------------*/

int Solar::getYear(void) {
    return this->solarYear;
}

/*----------------------------------------------------------------------------------------------*/

void Solar::setLocation (QString location, CountryCode cc, float longitude, float latitude) {
    qDebug() << "Solar::setLocation, longitude = " << longitude << ", latitude = " << latitude;
    this->solarLongitude = longitude;
    this->solarLatitude = latitude;
    this->h->location = location;
    this->h->countryCode = cc;
    calculate();
    qDebug() << "AC = " << this->h->cusp[1];
}

/*----------------------------------------------------------------------------------------------*/

void Solar::calculate() {

    //qDebug() << "Solar::calculate, begin";

    /* As a starting point, calculate horoscope of the birth date in the solar year
       using birth time.                                                            */
    QDateTime solarDateTime = base->getDateTime();
    QDate d = solarDateTime.date();
    solarDateTime.setDate(QDate(solarYear, d.month(), d.day()));
    this->h = new Horoscope("Solar", solarDateTime, base->location,
                            base->getCountryCode(),
                            base->getSubCode(), solarLatitude, solarLongitude);
    // qDebug() << "Solar::calculate(), longitude = " << solarLongitude << ", latitude = " << solarLatitude;

    /* calculate distance between sun position of base horoscope and solar          */
    double distance = double(base->planet[SE_SUN] - h->planet[SE_SUN]);

    int iteration = 0;  /* count iterations required                                */
    int secs = 0;

    while (sqrt(distance*distance) >= 0.00001) {

        /* calculate time difference in seconds and add them to solar time          */
        secs = 24 * 60 * 60 * distance;
        solarDateTime = solarDateTime.addSecs( secs);

        /* calculate new solar                                                      */
        this->h = new Horoscope("Solar", solarDateTime, base->location,
                                base->getCountryCode(),
                                base->getSubCode(), solarLatitude, solarLongitude);

        /* calculate new deviation                                                  */
        distance = double(base->planet[SE_SUN] - h->planet[SE_SUN]);
        iteration++;
        if (iteration == 10) break;
    }

    // qDebug() << "achieved " << secs << " s deviation after "<< iteration << "iterations.";

}
