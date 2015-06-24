#ifndef COMBINE_H
#define COMBINE_H

#include <QObject>
#include "horoscope.h"

class Combine : public QObject {
public:

    /**
      * constructor
      */
    Combine(Horoscope *h1, Horoscope *h2);

    /**
      * setter method for 1st horoscope
      */
    void setH1(Horoscope * horoscope);

    /**
      * setter method for 2nd horoscope
      */
    void setH2(Horoscope * horoscope);

    /**
      * calculate combine horoscope
      */
    void calculate();

    /**
      * resulting combine horoscope
      */
    Horoscope *h;

private:

    /* local number of planets used for combine calculation    */
    int n_planets;

    /* pointers to partner horoscopes                          */
    Horoscope *h1, *h2;
};

#endif // COMBINE_H
