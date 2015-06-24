#include "combine.h"

Combine::Combine(Horoscope *h1, Horoscope *h2) {

    this->n_planets = 16;   // including Lilith, Chiron

    this->h = new Horoscope("Combin");
    this->h1 = h1;
    this->h2 = h2;
    // this->calculate();
}

void Combine::setH1(Horoscope * horoscope) {
    this->h1 = horoscope;
    if (this->h2 != NULL) {
        this->calculate();
    } else {
        qDebug() << "Combine, setting H1, but H2 = NULL";
    }
}

void Combine::setH2(Horoscope * horoscope) {
    this->h2 = horoscope;
    if (this->h1 != NULL) {
        this->calculate();
    } else {
        qDebug() << "Combine, setting H2, but H1 = NULL";
    }
}

void Combine::calculate() {

    if (this->h1 == NULL) {
        qDebug() << "Combine::calculate(), h1 == NULL !";
    } else if (this->h2 == NULL) {
        qDebug() << "Combine::calculate(), h2 == NULL !";
    } else {

        /* according to Mona Riegger ("Handbuch der Combin - und Composit - Deutung")
           the combine of 2 horoscopes is calculated in 4 steps:                    */

        /* 1. Calculate JD of combine as mean value of julian dates of the partners.*/
        // qDebug() << "JD of h1 = " << h1->tjd_ut;
        // qDebug() << "JD of h2 = " << h2->tjd_ut;
        h->tjd_ut = (h1->tjd_ut + h2->tjd_ut)/2;
        // qDebug() << "JD of combine = " << h->tjd_ut;
        h->setLastPlanet(SE_CHIRON);     // make configurable later ..

        /* set flag so horoscope is directly calculated from julian day number      */
        h->setCalcFromJD (true);

        /* 2. Calculate combine world time as mean value of the world times of the
           horoscopes.
           This may be omitted, because we calculated the JD including it's fractional
           part, which already contains the time.                                   */

        /* 3. and 4. Calculate combine longitude and latitude as mean values of the
           locations of the partner horoscopes.                                     */
        h->longitude = (h1->longitude + h2->longitude) / 2;
        h->latitude  = (h1->latitude  + h2->latitude ) / 2;

        h->update();

        /* reset retrograde flaggs, because they do not make any sense              */
        for (int ipl = 0; ipl < SE_NPLANETS; ipl++) {
            h->retrograde[ipl] = FALSE;
        }
    }
}
