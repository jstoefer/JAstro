#include "synastrie.h"

Synastrie::Synastrie(Horoscope *h1, Horoscope *h2) {
    this->h1 = h1;
    this->h2 = h2;
    this->setDefaultValues();
    this->calculateAspects();
}

void Synastrie::setDefaultValues(void) {

    // qDebug() << "Synastrie::setDefaultValues, begin";

    this->n_planets = 10;

    /* fill array of angles                                            */
    for (int i=0; i < 7; i++) {
        angle[i]                        = 30*i;
    }

    angle[ASPECT_SEMISQUARE]            = 45;
    angle[ASPECT_QUINTILE]              = 72;
    angle[ASPECT_BIQUINTILE]            = 144;

    /* set default orbis values for aspects                            */
    aspectOrbis [ASPECT_KONJUNCTION]    = 6;    /* 0                   */
    aspectOrbis [ASPECT_SEMISEXTILE]    = 1.5;  /* 30                  */
    aspectOrbis [ASPECT_SEXTILE]        = 4;    /* 60                  */
    aspectOrbis [ASPECT_SQUARE]         = 5;    /* 90                  */
    aspectOrbis [ASPECT_TRINE]          = 5;    /* 120                 */
    aspectOrbis [ASPECT_QUINCUNX]       = 1.5;  /* 150                 */
    aspectOrbis [ASPECT_OPPOSITION]     = 5;    /* 180                 */
    aspectOrbis [ASPECT_SEMISQUARE]     = 0;    /* 45                  */
    aspectOrbis [ASPECT_QUINTILE]       = 0;    /* 72                  */
    aspectOrbis [ASPECT_BIQUINTILE]     = 0;    /* 144                 */

    /* set intrinsic orbis values for planets                          */
    planetOrbis [SE_SUN] = 2.0;
    planetOrbis [SE_MOON] = 1.5;
    for (int i= 2; i< SE_NPLANETS; i++) {
        planetOrbis[i] = 1;
    }
    // qDebug() << "Synastrie::setDefaultValues, end";
}


void Synastrie::setH1(Horoscope * horoscope) {
    this->h1 = horoscope;
    qDebug() << "name of h1 = " << horoscope->name;
    calculateAspects();
}


void Synastrie::setH2(Horoscope * horoscope) {
    this->h2 = horoscope;
    qDebug() << "name of h2 = " << horoscope->name;
    qDebug() << "absolute angle of sun relative to 0° aries is = " << horoscope->planet[0] << "°";
    calculateAspects();
}


void Synastrie::calculateAspects() {

    // qDebug() << "Synastrie::calculateAspects, begin";

    this->noOfAspects               = 0;
    this->noOfAxisAspects           = 0;
    this->noOfReverseAxisAspects    = 0;

    /* calculate aspects between planets                                                */
    for (int i=0; i < n_planets; i++) {                 /* iterate over all planets     */
        for (int k = 0; k < n_planets; k++) {           /* iterate over second planet   */
            float angle = abs (h1->planet[i] - h2->planet[k]);
            if (angle < 0) {                            /* shift to positive values     */
                angle *= -1;
            }
            if (angle > 180) {                          /* reduce range to 180         */
                angle = 360 -angle;
            }
            /* calculate mean value of planets intrinsic orbis                          */
            float orbisPlanets = (this->planetOrbis[i] + this->planetOrbis[k])/2;
            // qDebug ("planetOrbis[%d] = %f, planetOrbis[%d] = %f, orbisPlanets = %f",
            //         i, planetOrbis[i], k, planetOrbis[k], orbisPlanets);
            for (int l=0; l< N_ASPECTS; l++) {          /* iterate over aspects         */
                /* calculate maximum orbis for test aspect                              */
                float orbis = orbisPlanets * this->aspectOrbis[l];
                float deviation = abs(angle - this->angle[l]);
                /* test angle                                                           */
                if (deviation < orbis) {
                    /* store aspect                                                     */
                    bool aspectValid = FALSE;
                    /* Do not calculate aspects for earth                               */
                    if (k == SE_EARTH) {
                        aspectValid = FALSE;
                    } else if (( k == SE_MEAN_NODE) || (k == SE_TRUE_NODE)
                        || (k == SE_MEAN_APOG) || (k == SE_OSCU_APOG)
                        || (k == SE_CHIRON))
                        /* show only (main aspects) konjunctions for the node, Lilith
                                                                   and Chiron           */
                    {
                        if ((l == ASPECT_KONJUNCTION)
                           // ||(l == ASPECT_SEXTILE )
                           // ||(l == ASPECT_SQUARE  )
                           // ||(l == ASPECT_TRINE )
                          //  ||(l == ASPECT_OPPOSITION )
                          ){
                            aspectValid = TRUE;
                        }
                    } else {
                        aspectValid = TRUE;
                    }
                    if (aspectValid) {
                        if (this->noOfAspects < MAX_SYNASTRIE_PLANETARY_ASPECTS) {
                            this->aspectList[noOfAspects++] = new Aspect (
                                    h1->planet[i], h2->planet[k], this->angle[l], deviation, orbis);
                            // qDebug ("i= %d, k= %d, l=%d, aspect = %d , deviation = %f, orbis = %f",
                            //     i, k, l, this->angle[l], deviation, orbis);
                        }
                    }
                }
            } // end loop 'aspects'
        } // end loop 'planet/cusp h2'
        /* calculate aspects between planets of h1 and AC / MC of h2
           but omit the nodes, Chiron, Lilith and Earth*/
        if ((i != SE_MEAN_NODE) && (i != SE_TRUE_NODE)
                && (i != SE_MEAN_APOG) && (i != SE_OSCU_APOG)
                && (i != SE_CHIRON) && (i != SE_EARTH)) {
            this->calculateApectPlanetToCusp(i, 1, FALSE);  /* planet i of h1 to AC of h2 */
            this->calculateApectPlanetToCusp(i, 10, FALSE); /* planet i of h1 to MC of h2 */
        }
    } // end loop 'planet/cusp h1'
    for (int k=0; k < n_planets; k++) {
        /* omit the nodes, Chiron, Lilith and Earth*/
        if ((k != SE_MEAN_NODE) && (k != SE_TRUE_NODE)
                && (k != SE_MEAN_APOG) && (k != SE_OSCU_APOG)
                && (k != SE_CHIRON) && (k != SE_EARTH))
        {
            this->calculateApectPlanetToCusp(k, 1, TRUE);  /* planet k of h2 to AC of h1 */
            this->calculateApectPlanetToCusp(k, 10, TRUE); /* planet k of h2 to MC of h1 */
        }
    }
    // qDebug() << "Synastrie::calculateAspects(), end";
}


void Synastrie::calculateApectPlanetToCusp (int planetIndex, int cuspIndex, bool reverse) {

    float angle = 0;

    /* calculate angle to cusp                                                  */
    if (reverse) {
        angle = abs (h2->planet[planetIndex] - h1->cusp[cuspIndex]);
    } else {
        angle = abs (h1->planet[planetIndex] - h2->cusp[cuspIndex]);
    }
    if (angle < 0) {                            /* shift to positive values     */
        angle *= -1;
    }
    if (angle > 180) {                          /* reduce range to 180         */
        angle = 360 -angle;
    }
    /* calculate mean value of orbs                          */
    float orbisPlanets = (this->planetOrbis[planetIndex] + 1)/2;
    for (int l=0; l< N_ASPECTS; l++) {          /* iterate over aspects         */
        /* calculate maximum orbis for test aspect                              */
        float orbis = orbisPlanets * this->aspectOrbis[l];
        float deviation = abs(angle - this->angle[l]);
        /* test angle                                                           */
        if (deviation < orbis) {
            /* store aspect                                                     */
            if ((reverse) && (noOfReverseAxisAspects < MAX_SYNASTRIE_AXES_REVERSE_ASPECTS)) {
                this->aspectList[MAX_SYNASTRIE_PLANETARY_ASPECTS +  MAX_SYNASTRIE_AXES_ASPECTS
                                 + noOfReverseAxisAspects++] =
                        new Aspect (h2->planet[planetIndex], h1->cusp[cuspIndex],
                            this->angle[l], deviation, orbis);
                // qDebug ("reverse axis: planetIdx= %d, cuspIdx= %d, aspectIdx=%d, aspect = %d , deviation = %f, orbis = %f",
                //          planetIndex, cuspIndex, l, this->angle[l], deviation, orbis);
            } else if (noOfAxisAspects < MAX_SYNASTRIE_AXES_ASPECTS) {
                this->aspectList[MAX_SYNASTRIE_PLANETARY_ASPECTS + noOfAxisAspects++] = new Aspect (
                    h1->planet[planetIndex], h2->cusp[cuspIndex], this->angle[l], deviation, orbis);
                 // qDebug ("axis aspect: planetIdx= %d, cuspIdx= %d, aspectIdx=%d, aspect = %d , deviation = %f, orbis = %f",
                 //         planetIndex, cuspIndex, l, this->angle[l], deviation, orbis);
            }
        }
    }
}

