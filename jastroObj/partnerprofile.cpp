#include "partnerprofile.h"

PartnerProfile::PartnerProfile(Horoscope *h, int planet[2]) {

    qDebug() << "PartnerProfile of " << h->name;

    this->m_h = h;              // store reference to horoscope

    for (int l=0; l < 2; l++) {
        this->m_planet[l] = planet[l];    // array of partner planets
    }

    for (int l=0; l< N_PLANETS; l++) m_profile[l] = 0;

    calculateScores();

}


/*----------------------------------------------------------------------------------------------*/


void PartnerProfile::calculateScores() {

    // qDebug() << "PartnerProfile::calculateScores";

    // loop over partner planets
    for (int idxPlanet = 0; idxPlanet < 2; idxPlanet++) {

        qDebug() << "\n------------------------------------------\n";

        //"Zeichenqualitaet"
        //------------------

        // get planet number of partner planet
        int ipl = m_planet[idxPlanet];        // planet number
        qDebug() << "planet no " << idxPlanet << ", ipl = " << ipl;

        // check planets sign
        int nSign = this->m_h->getSign(ipl, false);
        qDebug() << "sign of planet " << planetNames[ipl] << " is " << signNames[nSign];

        // check planet rulers sign
        int ruler = this->m_h->getRuler(nSign);
        qDebug() << "Ruler of " << signNames[nSign] << " is " << planetNames[ruler];

        this->m_profile[ruler] += 10;       // add 10 for ruler of planets sign


        //"Grundton"
        //----------

        // check sign of planets ruler
        nSign = this->m_h->getSign(ruler, false);
        qDebug() << planetNames[ruler] << " is in " << signNames[nSign];

        // check ruler of sign of planets ruler
        ruler = this->m_h->getRuler(nSign);
        qDebug() << "Ruler of planets ruler is " << planetNames[ruler] << "\n";

        this->m_profile[ruler] += 5;        // add 5 for ruler of sign of ruler of planets sign



        // "Hausqualität"
        //---------------

        // check planets house
        int nHouse = m_h->getHouse(ipl);
        qDebug() << planetNames[ipl] << " is in house " << nHouse;

        // check analog ruler of planets house
        ruler = m_h->getAnalogRuler(nHouse);
        qDebug() << "analog ruler of planets house is " << planetNames[ruler] << "\n";

        this->m_profile[ruler] += 10;       // add 10 for analog ruler of planets house

        // check aspects with planets and cusps
        float planetPos = m_h->planet[ipl];
        // check all aspects
        for (int k=0; k < m_h->noOfAspects; k++) {
            Aspect *asp = m_h->aspectList[k];
            // check wether this is an aspect of our planet
            if ( (asp->p1 == planetPos) ) {
                checkAspect(asp, ipl, 2);
            } else if  ( (asp->p2 == planetPos) ) {
                checkAspect(asp, ipl, 1);
            }
        }
    }   // end loop over partner planets

    // check sign of DC
    int DCSign = this->m_h->getSign(7, true);
    int DCRuler = this->m_h->getRuler(DCSign);
    qDebug() << "Ruler of DC is " << planetNames[DCRuler];

    this->m_profile[DCRuler] += 10;       // add 10 for ruler of H7

    // check planets in H7
    int planetsInH7 = 0;
    for (int k=0; k <= SE_PLUTO; k++) {
        int house = this->m_h->getHouse (k);
        // qDebug() << "planet " << planetNames[k] << " is in H" << house;
        if (house == 7) {
            qDebug () << "planet " << planetNames[k] << " in H7";
            planetsInH7++;

            this->m_profile[k] += 10;       // add 10 for each planet in house 7
        }
    }
    qDebug () << planetsInH7 << " planets in H7";

    qDebug() << "\n\n";

    // printScore();

}


/*----------------------------------------------------------------------------------------------*/

// check hard analytical aspects: square, oppostion, konjunct
// check light analytical aspects: quincunx, half / 1 1/2  squares
// check synthetic aspects: trine, sextile
void PartnerProfile::checkAspect (Aspect * asp, int nPlanet, int idxPlanet) {

    int nPlanet2 = -1;

    int score = 0;  // will be added to planet2 score value

    int nCusp = 0;
    if (idxPlanet == 2) {
        nPlanet2 = this->m_h->getPlanet(asp->p2);
    } else {
        nPlanet2 = this->m_h->getPlanet(asp->p1);
    }

    if (nPlanet2 == -1){

        // do not consider aspects between planet and H1/H4/H7/H10

        /*
        if (idxPlanet == 2) {
            nCusp = this->m_h->getCusp(asp->p2);
        } else {
            nCusp = this->m_h->getCusp(asp->p1);
        }
        if (nCusp == 0) {
            qDebug() << "no planet, no cusp !";
        } else {
            // reduce orbis to about 5°
            float orbis = asp->orbis*2/3;
            if (asp->deviation < orbis) {
                if ( (asp->angle == 0) ||  (asp->angle == 90) ||  (asp->angle == 180) ){
                    qDebug() << planetNames[nPlanet] << " has analytic aspect of " << asp->angle
                             << "° with a deviation of " << asp->deviation << "° with H" << nCusp;
                    score = 15;
                }
                if ( (asp->angle == 150) ||  (asp->angle == 45) ||  (asp->angle == 135) ){
                    qDebug() << planetNames[nPlanet] << " has weak analytic aspect of " << asp->angle
                             << "° with a deviation of " << asp->deviation << "° with H" << nCusp;
                    score = 5;
                }
                if ( (asp->angle == 60) ||  (asp->angle == 120) ){
                    qDebug() << planetNames[nPlanet] << " has synthetic aspect of " << asp->angle
                             << "° with a deviation of " << asp->deviation << "° with H" << nCusp;
                    score = 5;
                }
                qDebug() << "orbis is " << orbis << "°";
                // get sign of house
                int cSign = this->m_h->getSign(nCusp, true);
                // get ruler of house
                int cRuler = this->m_h->getRuler(cSign);
                qDebug() << "ruler of H" << nCusp << "is "<< planetNames[cRuler];

                this->m_profile[cRuler] += score;       // add score for ruler of planets sign

            }
        }       */
    } else if (nPlanet2 <= SE_PLUTO) {
        if (asp->deviation < asp->orbis) {
            if ( (asp->angle == 0) ||  (asp->angle == 90) ||  (asp->angle == 180) ){
                qDebug() << planetNames[nPlanet] << " has analytic aspect of " << asp->angle << "° with " << planetNames[nPlanet2];
                qDebug() << "adding 15";
                score = 15;
            }
            if ( (asp->angle == 150) ||  (asp->angle == 45) ||  (asp->angle == 135) ){
                qDebug() << planetNames[nPlanet] << " has weak analytic aspect of " << asp->angle << "° with " << planetNames[nPlanet2];
                qDebug() << "adding 5";
                score = 5;
            }
            if ( (asp->angle == 60) ||  (asp->angle == 120) ){
                qDebug() << planetNames[nPlanet] << " has synthetic aspect of " << asp->angle << "° with " << planetNames[nPlanet2];
                qDebug() << "adding 5";
                score = 5;
            }
            this->m_profile[nPlanet2] += score;       // add score for aspecting planet
        }
    }
}


/*----------------------------------------------------------------------------------------------*/

int PartnerProfile::getScore (int planet) {
    return m_profile[planet];
}

/*----------------------------------------------------------------------------------------------*/

void PartnerProfile::printScore () {

    for (int l=0; l <= SE_PLUTO; l++) {
        qDebug () << planetNames[l] << " \t : \t" << m_profile[l];
    }

}
