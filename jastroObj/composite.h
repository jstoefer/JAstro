#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <QObject>
#include "horoscope.h"

class Composite : public QObject {
public:

    /**
      * resulting composite horoscope
      */
    Horoscope *h;

    /**
      * constructor
      */
    Composite(Horoscope *h1, Horoscope *h2);

    /**
      * setter method for 1st horoscope
      */
    void setH1(Horoscope * horoscope);

    /**
      * setter method for 2nd horoscope
      */
    void setH2(Horoscope * horoscope);

    /**
      * calculate half sum horoscope h3 from h1, h2
      */
    void calculate();

private:

    /* local number of planets used for composite calculation    */
    int n_planets;

    Horoscope *h1, *h2;
};

#endif // COMPOSITE_H
