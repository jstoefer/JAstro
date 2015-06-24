#ifndef SOLAR_H
#define SOLAR_H

#include "horoscope.h"

class Solar : public QObject {

public:

    /**
      * constructor
      *
      * @param  *Horoscope  reference to base horoscope of the solar
      * @param  int         year, for which solar will be calculated
      */
    Solar(Horoscope *base, int solarYear);

    /**
      * setter method for solar year
      *
      * after setting the year, the solar will be re-calculated
      */
    void setYear(int solarYear);


    /**
      * getter method for solar year
      */
    int getYear(void);

    /**
      * setter method for longitude / latitude
      *
      * allows to calculate the solar for the location of the
      * current or anticipated place of living.
      *
      */
    void setLocation (QString location, CountryCode cc, float longitude, float latitude);

    /**
      * calculate solar horoscope
      */
    void calculate();

    /**
      * resulting solar horoscope
      */
    Horoscope *h;

private:

    /* local number of planets used for solar calculation    */
    int n_planets;

    /* pointer to base horoscope                             */
    Horoscope *base;

    /**
     * solar year
     *
     * The solar is calculated for the birth day of the person
     * in this year.
     */
    int solarYear;

    /**
      * coordinates of solar location
      *
      * These should be the coordinates of the current location of the person
      */
    float solarLongitude, solarLatitude;

};

#endif // SOLAR_H
