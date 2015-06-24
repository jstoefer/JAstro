#ifndef SYNASTRIE_H
#define SYNASTRIE_H

#include <QObject>
#include "horoscope.h"
#include "aspect.h"
#include "constants.h"

using namespace saturnia;

/**
  * holds aspects of a synastrie chart
  */
class Synastrie : public QObject {

    // Q_OBJECT

    public:

        int noOfAspects, noOfAxisAspects, noOfReverseAxisAspects;
        Aspect * aspectList[MAX_SYNASTRIE_PLANETARY_ASPECTS
                            + MAX_SYNASTRIE_AXES_ASPECTS
                            + MAX_SYNASTRIE_AXES_REVERSE_ASPECTS];

        /**
          * standard constructor
          */
        Synastrie(Horoscope *h1, Horoscope *h2);

        /**
          * initialize some constants like aspect orbs etc.
          */
        void setDefaultValues(void);

        /**
          * set horoscope h1 and recalculate aspects
          */
        void setH1(Horoscope * horoscope);

        /**
          * set horoscope h2 and recalculate aspects
          */
        void setH2(Horoscope * horoscope);

        /**
          * calculate inter - aspects between h1 and h2
          */
        void calculateAspects();

        /**
          * calculate aspect between planet and cusp
          *
          * @param int  index of planet
          * @param int  index of cusp (AC = 1, MC = 10)
          * @param bool wether from palnet of h1 to cusp of h2 or reverse
          *
          * modifies global 'noOfAspects'
          */
        void calculateApectPlanetToCusp (int planetIndex, int cuspIndex, bool reverse);

    private:

        /* local number of planets used for synastrie calculations
           set e.g. to '10' avoids lilith, chiron, nodes ...      */
        int n_planets;

        Horoscope *h1, *h2;

        /* angles: 0, 30, 60, 90, 120, 150, 180, 45, 72, 144      */
        int angle [10];

        /* orbis for: 0, 30, 60, 90, 120, 150, 180, 45, 72, 144   */
        float aspectOrbis [10];

        /* orbis for planets                                      */
        float planetOrbis [SE_NPLANETS];

    };

#endif // SYNASTRIE_H
