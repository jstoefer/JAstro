#include "horoscope.h"

Horoscope::Horoscope (QString name, QDateTime dTime, QString location,
                      CountryCode code, SubCode sCode, float latitude, float longitude) {
    this->name = name;
    this->dTime = dTime;
    this->location = location;
    this->countryCode = code;
    this->subCode = sCode;
    this->latitude = latitude;
    this->longitude = longitude;
    this->setDefaultValues();
    this->empty = FALSE;
    this->calcFromJD = FALSE;
    this->update();

    // qDebug() << "constructor 1, country code = " << countryCode << ", subcode = " << subCode;
}

/*----------------------------------------------------------------------------------------------*/

Horoscope::Horoscope (QString name, QDateTime dTime, QString location,
                      float latitude, float longitude) {
    this->name = name;
    this->dTime = dTime;
    this->location = location;
    this->countryCode = de;
    this->subCode = NONE;
    this->latitude = latitude;
    this->longitude = longitude;
    this->setDefaultValues();
    this->empty = FALSE;
    this->calcFromJD = FALSE;
    this->update();

    // qDebug() << "constructor 2, country code = " << countryCode << ", subcode = " << subCode;
}

/*----------------------------------------------------------------------------------------------*/

Horoscope::Horoscope (QString name) {
    this->name = name;
    this->setDefaultValues();
    this->empty = TRUE;
    this->calcFromJD = FALSE;
}

/*----------------------------------------------------------------------------------------------*/

void Horoscope::setDefaultValues(void) {

    // qDebug("Horoscope::setDefaultValues, begin");

    this->lastPlanet = SE_CHIRON;

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
    aspectOrbis [ASPECT_SEXTILE]        = 5;    /* 60                  */
    aspectOrbis [ASPECT_SQUARE]         = 6;    /* 90                  */
    aspectOrbis [ASPECT_TRINE]          = 5;    /* 120                 */
    aspectOrbis [ASPECT_QUINCUNX]       = 1.5;  /* 150                 */
    aspectOrbis [ASPECT_OPPOSITION]     = 6;    /* 180                 */
    aspectOrbis [ASPECT_SEMISQUARE]     = 1;    /* 45                  */
    aspectOrbis [ASPECT_QUINTILE]       = 1;    /* 72                  */
    aspectOrbis [ASPECT_BIQUINTILE]     = 1;    /* 144                 */

    /* set intrinsic orbis values for planets                          */
    planetOrbis [SE_SUN] = 2.0;
    planetOrbis [SE_MOON] = 1.5;
    for (int i= 2; i< SE_NPLANETS; i++) {
        planetOrbis[i] = 1;
    }

    /* reset retrograde flaggs                                          */
    for (int i= 0; i<SE_NPLANETS; i++) {
        retrograde[i] = FALSE;
    }

}

/*----------------------------------------------------------------------------------------------*/

void Horoscope::setLastPlanet(int lastPlanet) {
    this->lastPlanet = lastPlanet;
    // qDebug() << "Horoscope: last planet = " << lastPlanet;
}

/*----------------------------------------------------------------------------------------------*/

int Horoscope::getLastPlanet() {
   return this->lastPlanet;
}

/*----------------------------------------------------------------------------------------------*/

void Horoscope::setCalcFromJD (bool val) {
    calcFromJD = val;
    /* if a julian day number has been given, e.g. in case of a combine,
    the horoscope isn't empty anymore and can be calculated.                            */
    if (val == TRUE) empty = FALSE;
}

/*----------------------------------------------------------------------------------------------*/

QDateTime Horoscope::getDateTime (void) {
    return this->dTime;
}

/*----------------------------------------------------------------------------------------------*/

CountryCode Horoscope::getCountryCode(void) {
    return this->countryCode;
}

/*----------------------------------------------------------------------------------------------*/

SubCode Horoscope::getSubCode (void) {
    return this->subCode;
}

/*----------------------------------------------------------------------------------------------*/

void Horoscope::update(void) {

    char serr[AS_MAXCH];            /* error string                                     */
    double xx[6], xx_r[6];          /* result array for one planet                      */
    int ipl;                        /* planet number                                    */
    int32 iflag;                    /* flags                                            */
    iflag = SEFLG_SPEED;            /* Speed flag. If true, speed of body is calculated */

    if (!empty) {

        if (!calcFromJD) {

            // qDebug() << "Aktualisiere Horoskop " << name;

            /* show date / time using system locale                                             */
            // qDebug () << "Datum / Zeit dieses Horoskops: " << dTime.toString();

            /* convert to UTC                                                                   */
            this->dTimeUTC = convertToUTC (this->dTime, this->countryCode, this->subCode);

            // qDebug () << "Datum / Zeit in UTC umgerechnet: " << dTimeUTC.toString();

            /**
              * calculate Julian day number
              *
              * hour        decimal representation of hour, like 12.823
              * gregflag    either SE_GREG_CAL (gregorian cal.) or SE_JUL_CAL (julian cal.)
              *
              */
            // float swe_julday(int year, int month, int day, float hour, int gregflag);

            /* calculate time in decimal number format                                          */
            float decimalHour = this->dTimeUTC.time().hour() + this->dTimeUTC.time().minute()/60.0F
                                 + this->dTimeUTC.time().second()/3600.0F;

            /* calculate Julian Date (JD) from UTC and store it as local attribute              */
            this->tjd_ut = swe_julday(this->dTimeUTC.date().year(),
                                this->dTimeUTC.date().month(),
                                this->dTimeUTC.date().day(),
                                decimalHour,
                                SE_GREG_CAL);

            /* another Julian Date, 1 day later ...                                             */
            tjd_ut_r = swe_julday(this->dTimeUTC.date().year(),
                                  this->dTimeUTC.date().month(),
                                  this->dTimeUTC.date().day() + 1,
                                  decimalHour,
                                  SE_GREG_CAL);
        }

        // qDebug() << "Horoscope: " << this->name << " calculating with JD = " << this->tjd_ut ;

        /**
          * calculate planet position
          *
          * tjd_ut  Julian day, Universal Time
          * ipl     body number
          * iflag   32 bit integer containing bit flags that indicate what kind of computation is wanted
          * xx      array of 6 floats for longitude, latitude, distance, speed in long., speed in lat.,
          *         and speed in dist.
          * serr    character string to return error messages in case of error
          */
        // int swe_calc_ut ( float tjd_ut, int ipl, int iflag, float* xx, char* serr);

        /* now, loop over all planets                                                       */
        for (ipl = 0; ipl <= this->lastPlanet; ipl++) {

            swe_calc_ut (this->tjd_ut, ipl, iflag, xx, serr);

            // qDebug () << "horoscope, error from swe_calc_ut : " << serr;

            swe_calc_ut (this->tjd_ut_r, ipl, iflag, xx_r, serr);

            float diff = (float) (xx_r[0] - xx[0]);

            if  ((diff < 0) && ( (ipl == SE_MERCURY) || (ipl == SE_VENUS) || (ipl == SE_MARS)
                                 || (ipl == SE_JUPITER) || (ipl == SE_SATURN) || (ipl == SE_URANUS)
                                 || (ipl == SE_NEPTUNE) || (ipl == SE_PLUTO))) {
                retrograde[ipl] = TRUE;
                // qDebug() << "planet " << ipl << " is retrograde!";
            } else {
                retrograde[ipl] = FALSE;
            }

            planet[ipl] = (float) xx[0];
            // qDebug() << "ipl = " << ipl << ", arc = " << planet[ipl];

        }

        /* house cusps, ascendant and MC */

        // int swe_houses(

        // float tjd_ut,      /* Julian day number, UT */
        // float geolat,      /* geographic latitude, in degrees */
        // float geolon,      /* geographic longitude, in degrees
        /* eastern longitude is positive, western longitude is negative,
            northern latitude is positive, southern latitude is negative */
        // int hsys,           /* house method, ascii code of one of the letters PKORCAEVXHTBG */
        // float *cusps,      /* array for 13 floats */
        // float *ascmc    ); /* array for 10 floats */

        /* calculate houses, placidus manier                                                */
        swe_houses (this->tjd_ut, this->latitude, this->longitude, 'P', this->cusp, this->ascmc);

        /* close library and free resources occupied by swe                                 */
        swe_close();

    }

    // qDebug() << "Horoscope::update(), calculate aspects";

    /* no aspects calculated yet                                                        */
    this->noOfAspects = 0;

    /* calculate aspects between planets                                                */
    for (int i=0; i <= lastPlanet; i++) {                 /* iterate over all planets     */
        if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)) {
            for (int k = i+1; k <= lastPlanet; k++) {         /* iterate over second planet   */
                if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)) {
                    float angle = abs (planet[i] - planet[k]); /* calculate angle              */
                    if (angle < 0) {                            /* shift to positive values     */
                        angle *= -1;
                    }
                    if (angle > 180) {                          /* reduce range to 180         */
                        angle = 360 -angle;
                    }
                    /* calculate mean value of planets intrinsic orbis                          */
                    float orbisPlanets = (planetOrbis[i] + planetOrbis[k])/2;
                    // qDebug ("planetOrbis[%d] = %f, planetOrbis[%d] = %f, orbisPlanets = %f",
                    //        i, planetOrbis[i], k, planetOrbis[k], orbisPlanets);
                    for (int l=0; l< N_ASPECTS; l++) {          /* iterate over aspects         */
                        /* calculate maximum orbis for test aspect                              */
                        float orbis = orbisPlanets * aspectOrbis[l];
                        float deviation = abs(angle - this->angle[l]);
                        /* test angle                                                           */
                        if (deviation < orbis) {
                            /* store aspect                                                     */
                            bool aspectValid = FALSE;
                            /* show only main aspects for the node                              */
                            if ( (k == SE_MEAN_NODE) || (k == SE_TRUE_NODE) ) {
                                if ((l == ASPECT_KONJUNCTION) ||
                                    (l == ASPECT_SEXTILE ) ||
                                    (l == ASPECT_SQUARE  ) ||
                                    (l == ASPECT_TRINE ) ||
                                    (l == ASPECT_OPPOSITION )){
                                    aspectValid = TRUE;
                                }
                            } else {
                                aspectValid = TRUE;
                            }
                            if (aspectValid) {
                                // Aspect(float p1, float p2, int angle, float deviation, float orbis);
                                this->aspectList[noOfAspects++] = new Aspect (
                                        planet[i], planet[k], this->angle[l], deviation, orbis);
                                // qDebug ("aspect = %d , deviaton = %f, orbis = %f", this->angle[l], deviation, orbis);
                            }
                        }
                    } // end loop 'aspects'
                } // end loop 'second planet'
            }
        }
        /* do not calculate aspects between nodes and cusps     */
        if ((i != SE_MEAN_NODE) && (i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)) {
            this->calculateAspectPlanetToCusp(i, 1);
            this->calculateAspectPlanetToCusp(i, 10);
        }
    } // end 'planet loop'

}

/*----------------------------------------------------------------------------------------------*/

void Horoscope::calculateAspectPlanetToCusp (int planetIndex, int cuspIndex) {

    /* calculate angle to cusp                                                  */
    float angle = abs (planet[planetIndex] - cusp[cuspIndex]);
    if (angle < 0) {                            /* shift to positive values     */
        angle *= -1;
    }
    if (angle > 180) {                          /* reduce range to 180         */
        angle = 360 -angle;
    }
    /* calculate mean value of orbs                          */
    float orbisPlanets = (planetOrbis[planetIndex] + 1)/2;
    for (int l=0; l< N_ASPECTS; l++) {          /* iterate over aspects         */
        /* calculate maximum orbis for test aspect                              */
        float orbis = orbisPlanets * aspectOrbis[l];
        float deviation = abs(angle - this->angle[l]);
        /* test angle                                                           */
        if (deviation < orbis) {
            /* store aspect                                                     */
            this->aspectList[noOfAspects++] = new Aspect (
                    planet[planetIndex], cusp[cuspIndex], this->angle[l], deviation, orbis);
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

QDateTime Horoscope::convertToUTC (QDateTime dateTime, CountryCode code, SubCode sCode) {

    QDateTime ret (dateTime);
    int addHours = 0;
    TimeZone tz = this->getTimeZone(code, sCode);

    switch (tz) {
        case IDLW:          /*  International Date Line West                        */
            addHours = -12;
            break;
        case UTCMinus11:    /*  Amerikanisch-Samoa, Midwayinseln, Samoa             */
            addHours = -11;
            break;
        case HAST:          /*  Hawaii-Aleutian Standard Time                       */
            addHours = -10;
            break;
        case UTCMinus9:     /*  Französisch-Polynesien, teilweise                   */
            addHours = -9.5;
        case AKST:          /*  Alaska Standard Time                                */
        case YST:           /*  Yukon Standard Time                                 */
            addHours = -9;
            break;
        case PST:           /*  Pacific Standard Time                               */
        case PT:            /*  Pacific Time                                        */
            addHours = -8;
            break;
        case MST:           /*  Mountain Standard Time                              */
            addHours = -7;
            break;
        case CST:           /*  Central Standard Time                               */
            addHours = -6;
            break;
        case EST:           /*  Eastern Standard Time                               */
            addHours = -5;
            break;
        case AST:           /*  Atlantic Standard Time                              */
            addHours = -4;
            break;
        case VST:           /* Venezuelan Standard Time                             */
            addHours = -4.5;
            break;
        case NST:           /* Newfoundland Standard Time                           */
            addHours = -3.5;
            break;
        case UTCMinus3:     /* Argentinien, Brasilien(teilweise), Französisch-Guayana,
                            Grönland(teilweise), Saint-Pierre und Miquelon, Surinam,
                            Uruguay                                                 */
            addHours = -3;
            break;
        case UTCMinus2:     /*  Brasilien (teilweise)                               */
            addHours = -2;
            break;
        case UTCMinus1:     /*  Grönland (teilweise), Kap Verde, Portugal: Azoren   */
            addHours = -1;
            break;
        case UTC:           /*  Universal Time C .. ?                               */
        case WET:           /*  West European Time                                  */
        case GMT:           /*  Greenwich Mean Time                                 */
            addHours = 0;
            break;
        case CET:           /*  Central European Time                               */
        case MEZ:           /*  Mitteleuropäische Zeit                              */
            addHours = 1;
            break;
        case EET:           /*  East European Time                                  */
            addHours = 2;
            break;
        case MSK:           /*  Moscow Standard Time                                */
        case BT:            /*  Bagdad Time                                         */
            addHours = 3;
            break;
        case IRT:           /* Iran Time                                            */
            addHours = 3.5;
            break;
        case UTCPlus4:      /* Armenien, Aserbaidschan, Georgien, Oman, Réunion,
                            Russland, Republik Udmurtien, Oblast Samara, Seychellen,
                            Vereinigte Arabische Emirate                            */
            addHours = 4;
            break;
        case UTCPlus4_5:    /*  Afghanistan                                         */
            addHours = 4.5;
            break;
        case UTCPlus5:      /*  Kirgisistan, Malediven, Pakistan, Russland, einige
                                Provinzen                                           */
            addHours = 5;
            break;
        case IST:           /*  Indian Standard Time                                */
            addHours = 5.5;
            break;
        case UTCPlus5_75:   /*  Nepal ..                                            */
            addHours = 5.75;
            break;
        case UTCPlus6:      /*  Bangladesch, Kasachstan, Russland, einige Provinzen */
            addHours = 6;
            break;
        case UTCPlus6_5:    /*  Kokosinseln, Myanmar                                */
            addHours = 6.5;
            break;
        case ICT:           /*  Indochina Time                                      */
            addHours = 7;
            break;
        case CNST:          /*  China Standard Time                                 */
            addHours = 8;
            break;
        case UTCPlus8_75:   /*  some parts of Australia                             */
            addHours = 8.75;
            break;
        case JST:           /*  Japan/Korea Standard Time                           */
            addHours = 9;
            break;
        case ACST:          /*  Australian Central Standard Time                    */
            addHours = 9.5;
            break;
        case AEST:          /*  Australian Eastern Standard Time                    */
            addHours = 10;
            break;
        case UTCPlus10_5:   /*  Australien* (teilweise), Lord-Howe-Insel            */
            addHours = 10.5;
            break;
        case UTCPlus11:     /*  Neukaledonien,
                            Russland: Republik Sacha (Jakutien) (östlicher Teil),
                            Oblast Magadan                                          */
            addHours = 10.5;
            break;
        case NFT:           /*  Norfolk (Island) Time                               */
            addHours = 11.5;
            break;
        case IDLE:          /*  International Date Line East                        */
        case NZST:          /*  New Zealand Standard Time                           */
            addHours = 12;
            break;
        case UTCPlus12_75:  /*  Chatham-Inseln (Neuseeland)                         */
            addHours = 12.75;
            break;
        case UTCPlus13:     /*  Kiribati (teilweise: Phoenixinseln),
                                Tonga                                               */
            addHours = 13;
            break;
        case UTCPlus14:     /*  Kiribati (teilweise: Line Islands, Kiritimati)      */
            addHours = 14;
            break;
        default:
            addHours = 0;
            break;
    }
    ret = dateTime.addSecs(-3600* (addHours + calcDST (dateTime, code, sCode)));
    return ret;
}

/*----------------------------------------------------------------------------------------------*/

int Horoscope::calcDST (QDateTime dt, CountryCode code, SubCode subCode) {
    int dst = 0;
    bool doubleDST = FALSE;
    QDateTime lower, lower2, upper, upper2; /* lower2 und upper2 are for double summer time     */
    int year = dt.date().year();
    // int month = dt.date().month();
    switch (code) {
        default:
            dst = 0;
            break;
        case de:        /* Westdeutschland                                  */
        case dd:        /* DDR                                              */
        case at:        /* Östereich, weitestgehend gleich mit de           */
            if (dt.date().year() == 1916) {
                //qDebug() <<  "1916";
                lower.setDate(getLastDayInMonth(year, April, Qt::Sunday));
                lower.setTime(QTime(23, 0, 0));
                upper.setDate(getFirstDayInMonth (year, October, Qt::Monday));
                upper.setTime(QTime(0, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if ((dt.date().year() == 1917) || (dt.date().year() == 1918)) {
                //qDebug() <<  "1917, 1918";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Monday));
                lower.addDays(14);       // get 3rd Monday in month
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getFirstDayInMonth (year, September, Qt::Monday));
                lower.addDays(14);       // get 3rd Monday in month
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if ((dt.date().year() == 1919) || (dt.date().year() == 1920)) {
                if (code == at) {       // only for Austria, but exact boundaries unknown ...
                    //qDebug() <<  "1919, 1920";
                    lower.setDate(getFirstDayInMonth(year, April, Qt::Monday));
                    lower.addDays(14);       // get 3rd Monday in month
                    lower.setTime(QTime(2, 0, 0));
                    upper.setDate(getFirstDayInMonth (year, September, Qt::Monday));
                    lower.addDays(14);       // get 3rd Monday in month
                    upper.setTime(QTime(2, 0, 0));
                    if ((lower <= dt) && (dt <= upper)) {
                        dst = 1;
                    }
                }
            } else if ((dt.date().year() >= 1940) && (dt.date().year() <= 1942)) {
                //qDebug() <<  "1940 .. 1942";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Monday));
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, November, Qt::Monday));
                upper.setTime(QTime(2, 0, 0));
                if (    ((dt.date().year() == 1940) && (lower <= dt)) ||
                        (dt.date().year() == 1941) ||
                        ((dt.date().year() == 1942) && (dt <= upper))   ) {
                    dst = 1;
                }
            } else if (dt.date().year() == 1943) {
                //qDebug() <<  "1943";
                lower.setDate(getFirstDayInMonth(year, March, Qt::Monday));
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, October, Qt::Monday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if (dt.date().year() == 1944) {
                //qDebug() <<  "1944";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Monday));
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, October, Qt::Monday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if (dt.date().year() == 1945) {
                //qDebug() <<  "1945";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Monday));
                lower.setTime(QTime(2, 0, 0));
                if (((code == de) && (subCode == BER)) || (code == dd)) {   // Berlin, sowj. Zone
                    upper.setDate(getFirstDayInMonth (year, November, Qt::Sunday));
                    upper.addDays(14);  // So we get the fucking 3rd Sunday of this month !
                    upper.setTime(QTime(2, 0, 0));  // 3:00 MESZ
                    lower2.setDate(getFirstDayInMonth (year, May, Qt::Wednesday));
                    lower2.setTime(QTime(1, 0, 0));
                    upper2.setDate(getLastDayInMonth(year, September, Qt::Monday));
                    upper2.setTime(QTime(1, 0, 0));
                    doubleDST = TRUE;
                } else {
                    upper.setDate(getFirstDayInMonth (year, September, Qt::Sunday));
                    upper.addDays(14);  // So we get the fucking 3rd Sunday of this month !
                    upper.setTime(QTime(1, 0, 0));  // 2:00 MESZ
                }
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                    if (doubleDST) {
                        if ((lower2 <= dt) && (dt <= upper2)) {
                            dst = 2;
                        }
                    }
                }
            } else if (dt.date().year() == 1946) {
                // qDebug() <<  "1946";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Sunday));
                lower.addDays(7);       // get 2nd Sunday in month
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, September, Qt::Monday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if (dt.date().year() == 1947) {
                // qDebug() <<  "1947";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Sunday));
                lower.setTime(QTime(3, 0, 0));
                upper.setDate(getFirstDayInMonth (year, October, Qt::Sunday));
                upper.setTime(QTime(2, 0, 0));  // 3:00 MESZ
                lower2.setDate(getFirstDayInMonth (year, May, Qt::Sunday));
                lower2.addDays(7);  // get 2nd Sunday in month
                lower2.setTime(QTime(2, 0, 0));
                upper2.setDate(getLastDayInMonth(year, June, Qt::Monday));
                upper2.setTime(QTime(1, 0, 0));
                doubleDST = TRUE;
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                    if (doubleDST) {
                        if ((lower2 <= dt) && (dt <= upper2)) {
                            dst = 2;
                        }
                    }
                }
            }  else if (dt.date().year() == 1948) {
                //qDebug() <<  "1948";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Sunday));
                lower.addDays(14);       // get 3rd Sunday in month
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getFirstDayInMonth (year, October, Qt::Sunday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if (dt.date().year() == 1949) {
                // qDebug() <<  "1949";
                if (code != at) {   // no DST for Austria in 1949 !
                    lower.setDate(getFirstDayInMonth(year, April, Qt::Sunday));
                    lower.addDays(7);       // get 2nd Sunday in month
                    lower.setTime(QTime(2, 0, 0));
                    upper.setDate(getFirstDayInMonth (year, October, Qt::Sunday));
                    upper.setTime(QTime(2, 0, 0));
                    if ((lower <= dt) && (dt <= upper)) {
                        dst = 1;
                    }
                }
            } else if (dt.date().year() == 1980) {
                // qDebug() <<  "1981";
                lower.setDate(getFirstDayInMonth(year, April, Qt::Sunday));
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, September, Qt::Sunday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if ((dt.date().year() >= 1981) && (dt.date().year() <= 1995)) {
                // qDebug() <<  "1981 <= date <= 1995";
                lower.setDate(getLastDayInMonth(year, March, Qt::Sunday));
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, September, Qt::Sunday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else if (dt.date().year() > 1995) {
                // qDebug() <<  "> 1995";
                lower.setDate(getLastDayInMonth(year, March, Qt::Sunday));
                lower.setTime(QTime(2, 0, 0));
                upper.setDate(getLastDayInMonth (year, October, Qt::Sunday));
                upper.setTime(QTime(2, 0, 0));
                if ((lower <= dt) && (dt <= upper)) {
                    dst = 1;
                }
            } else {
                dst = 0;
            }
    }
    return dst;
}

/*----------------------------------------------------------------------------------------------*/

QDate Horoscope::getFirstDayInMonth (int year, int month, int weekday) {
    QDate testDate (year, month, 1 );
    while (testDate.dayOfWeek() != weekday) {
        testDate = testDate.addDays(1);
    }
    return testDate;
}

/*----------------------------------------------------------------------------------------------*/

QDate Horoscope::getLastDayInMonth (int year, int month, int weekday) {
    QDate td (year, month, 1 );
    int dim = td.daysInMonth ();
    QDate testDate (year, month, dim);
    while (testDate.dayOfWeek() != weekday) {
        testDate = testDate.addDays(-1);
    }
    return testDate;
}

/*----------------------------------------------------------------------------------------------*/

TimeZone Horoscope::getTimeZone(CountryCode code, SubCode sCode) {
    TimeZone ret = MEZ;
    switch (code) {
        case de:    // Deutschland
        case dd:    // DDR
        case at:    // Österreich
                    // * Algerien*
                    // * Albanien*
                    // * Andorra*
                    // * Angola (WAT ? West African Time)
                    // * Belgien*
                    // * Bosnien und Herzegowina*
                    // * Dänemark*
        case fr:    // * Frankreich*
        case it:    // * Italien*
                    // * Kamerun*
                    // * Kosovo*
                    // * Kroatien*
                    // * Liechtenstein*
                    // * Luxemburg*
                    // * Malta*
                    // * Mazedonien
                    // * Monaco*
                    // * Montenegro*
                    // * Namibia
        case nl:    // * Niederlande*
                    // * Norwegen*
                    // * Polen*
                    // * San Marino*
                    // * Serbien*
        case se:    // * Schweden*
        case ch:    // * Schweiz*
                    // * Slowakei*
                    // * Slowenien*
        case es:    // * Spanien* (außer Kanarische Inseln)
                    // * Tschechien*
                    // * Tunesien
        case hu:    // * Ungarn*
                    // * Vatikanstadt*
                    // * Zentralafrikanische Republik
            ret = MEZ;
            // qDebug() << "timeZone = MEZ";
            break;
        case gb:
            ret = GMT;
            break;
        case us:
            ret = EST;  // fits for most of them
            break;
        case af:
            ret = UTCPlus4_5;
            break;
        case il:
            ret = EET;
            break;
        default:
            ret = MEZ;
            break;
    }
    return ret;
}

/*----------------------------------------------------------------------------------------------*/

 void Horoscope::setDateTime (QDateTime dTime) {
     this->dTime = dTime;
     this->update();
 }

 /*----------------------------------------------------------------------------------------------*/

  void Horoscope::setLocation (QString location, CountryCode countryCode, float longitude, float latitude) {
      this->location = location;
      this->countryCode = countryCode;
      this->longitude = longitude;
      this->latitude = latitude;
      this->update();
  }

 /*----------------------------------------------------------------------------------------------*/

 double Horoscope::getJulianDate () {
     return this->tjd_ut;
 }

 /*----------------------------------------------------------------------------------------------*/

 QString Horoscope::getCountryStringShort () {

     QString ret;

     switch (this->countryCode) {

     case de:
         ret = "DE";
         break;
     case nowhere:
         ret = "unknown";
         break;
     case at:
         ret = "AT";
         break;
     case gb:
         ret = "GB";
         break;
     case us:
         ret = "US";
         break;
     default:
         ret = "--";
         break;

     }
     return ret;
 }


 /*----------------------------------------------------------------------------------------------*/

 // TODO: correct for planets near cusp !
 int Horoscope::getHouse (int nPlanet) {

     int nHouse = 0;

     for (int l = 1; l <= 12; l++) {
        int _cusp = cusp[l+1];
        // check wether we crossed 360° boundary
        if (_cusp < cusp[l]) {
            _cusp = cusp[l+1] + 360;
        }
        if ( (cusp[l] < planet[nPlanet]) && (_cusp > planet[nPlanet]) ) {
            nHouse = l;
            break;
        }
     }
     return nHouse;
 }


/*----------------------------------------------------------------------------------------------*/


int Horoscope::getSign (int n, bool isCusp) {

    int nSign = 0;

    float absoluteAngle = 0.0f;

    if (isCusp) {
        absoluteAngle = cusp[n];
    } else {
        absoluteAngle = planet[n];
    }
    for (int l=0; l < 12; l++) {
        if (((l+1) * 30.0) >= absoluteAngle) {
            nSign = l;
            break;
        }
    }
    return nSign;
}


/*----------------------------------------------------------------------------------------------*/


int Horoscope::getRuler (int sign) {

     int ruler = 0;

     switch (sign) {
     case 0:
         ruler = SE_MARS;       // aries
         break;
     case 1:
         ruler = SE_VENUS;      // taurus
         break;
     case 2:
         ruler = SE_MERCURY;    // twin
         break;
     case 3:
         ruler = SE_MOON;       // cancer
         break;
     case 4:
         ruler = SE_SUN;        // leo
         break;
     case 5:
         ruler = SE_MERCURY;    // virgin
         break;
     case 6:
         ruler = SE_VENUS;      // libra
         break;
     case 7:
         ruler = SE_PLUTO;      // scorpio
         break;
     case 8:
         ruler = SE_JUPITER;    // sagitarius
         break;
     case 9:
         ruler = SE_SATURN;     // capricorn
         break;
     case 10:
         ruler = SE_URANUS;     // aquarius
         break;
     case 11:
         ruler = SE_NEPTUNE;    // pisces
         break;
     }

     return ruler;
}


/*----------------------------------------------------------------------------------------------*/


int Horoscope::getAnalogRuler (int house) {

    return getRuler (house -1);    // houses start with H1 ==> "1"

}


/*----------------------------------------------------------------------------------------------*/


int Horoscope::getPlanet(float position) {
    int ipl = -1;
    for (int k=0; k < N_PLANETS; k++) {
       //  if ( abs(planet[k] -position) < 0.00001f ) {
        if (planet[k] == position) {
            ipl = k;
            break;
        }
    }
    return ipl;
}


/*----------------------------------------------------------------------------------------------*/


int Horoscope::getCusp(float position) {
    int nCusp = 0;
    for (int k=1; k < 13; k++) {
        if ( abs(cusp[k] -position) < 0.001f ) {
            nCusp = k;
            break;
        }
    }
    return nCusp;
}
