#ifndef PARTNERPROFILE_H
#define PARTNERPROFILE_H

#include "horoscope.h"

class PartnerProfile {

public:

    /**
     * @brief construct a PartnerProfile from a horoscope
     *
     * @param h         the horoscope from which to extract the profile
     * @param planet    the planetary significators of the profile
     *
     */
    PartnerProfile(Horoscope *h, int planet[2]);

    /**
     * @brief   getScore
     * @param   planet
     */
    int getScore (int planet);

protected:


private:

    /**
     * @brief calculates the planets scores
     */
    void calculateScores ();

    /**
     * @brief check wether aspect is relevant for partner profile
     *
     * Check wether it is an aspect with a planet or with a cusp.
     * Check wether it is analytical, synthetical or a minor analytical aspect.
     *
     * @param asp
     * @param nPlanet
     * @param idxPlanet
     */
    void checkAspect (Aspect * asp, int nPlanet, int idxPlanet);

    /**
     * @brief debug print out planet score
     */
    void printScore ();

    Horoscope *m_h;             // horoscope from which the profile will be calculated
    int m_planet [2];           // should be either {SE_SUN, SE_MARS} or {SE_MOON, SE_VENUS}
    int m_n;                    // number of planets that are significant for the profile (normally 2)
    int m_profile [N_PLANETS];  // the profile, expressed as an array of scores

};

#endif // PARTNERPROFILE_H
