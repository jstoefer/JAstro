#ifndef HOROSCOPE_H
#define HOROSCOPE_H

#include <QObject>
#include <QDate>
#include <QDebug>

#include "aspect.h"
#include "constants.h"
#include "swephexp.h"


using namespace saturnia;

class Horoscope : public QObject {

    Q_OBJECT

    public:

        /**
          * standard constructor for class horoscope
          */
        Horoscope (QString name, QDateTime dTime, QString location, CountryCode code,
                      SubCode sCode, float latitude, float longitude);

        /**
          * simple constructor for class horoscope
          */
        Horoscope (QString name, QDateTime dTime, QString location, float latitude, float longitude);

        /**
          * constructor for empty horoscope object
          *
          * This is a horoscope object containing no horoscope data yet.
          * Planet positions and aspects cannot be calculated yet.
          *
          */
        Horoscope (QString name);

        /**
          * sets some default values, e.g. for orbis values
          */
        void setDefaultValues(void);

        /**
          * method allows manipulation of date/time for use in transit calculations etc.
          * calls update()
          */
        void setDateTime (QDateTime dTime);

        /**
         * @brief setLocation       sets a new location for this horoscope
         * @param locationName      name of location
         * @param countryCode       country code like "DE"
         * @param longitude         longitud of location
         * @param latitude          latitude of location
         */
        void setLocation (QString locationName, CountryCode countryCode, float longitude, float latitude);

        /**
          * getter method for dateTime
          */
        QDateTime getDateTime (void);

        /**
          * getter method for country code
          */
        CountryCode getCountryCode(void);

        /**
          * getter method for sub code
          */
        SubCode getSubCode (void);

        /**
          * forces re-calculation of horoscope
          */
        void update (void);


        /**
          * calculate aspect between planet and cusp
          * modifies global 'noOfAspects'
          */
        void calculateAspectPlanetToCusp (int planetIndex, int cuspIndex);

        /**
         * @brief get the house position of a planet
         * @param nPlanet
         *
         * @return  house number [1 .. 12]
         */
        int getHouse (int nPlanet);

        /**
         * @brief get the sign number [1 .. 12] of the planets sign
         * @param n number according to swiss ephemeris, e.g. SE_MARS
         *        or house number [1 .. 12], depending on param "cusp"
         * @param cusp  if true, n is the number of a cusp, otherwise
         *        it is a planet number
         * @return sign number [0 .. 11]
         */
        int getSign (int nPlanet, bool isCusp);

        /**
         * @brief get planetary ruler of sign
         * @param nSign
         * @return swiss ephemeris planet number, e.g. SE_SUN
         */
        int getRuler (int nSign);

        /**
         * @brief find the planets number from its position
         * @param position in °
         * @return nPlanet number according to swiss ephemeris, e.g. SE_MARS
         *          -1, if no planet was found
         */
        int getPlanet(float position);

        /**
         * @brief find the cusp number from its position
         * @param position in °
         * @return nCusp number, begiining with 1 for H1
         *          -1, if no cusp was found
         */
        int getCusp(float position);

        /**
         * @brief get ruler of sign, which is analog to house.
         * @param nHouse    house number [1 .. 12]
         * @return swiss ephemeris planet number, e.g. SE_SUN
         */
        int getAnalogRuler (int nHouse);


        /**
          * Convert a given QDateTime plus timezone plus countrycode into UTC.
          * Consider subcode, if not NULL. Subcode may either contain the code of a German federal
          * land or the code of a US federal state.
          */
        QDateTime convertToUTC (QDateTime dateTime, CountryCode code, SubCode scode = NONE);


        /**
          * Deduct time zone from country code and, if given, from sub code
          */
        TimeZone getTimeZone(CountryCode code, SubCode sCode = NONE);


        /**
          * Calculate number of hours of daylight saving time ("DST"), if any,
          * for a given date and country.
          * A subcode will be considered for certain countries. E.g. for Germany,
          * the subcode will be the federal land code like "bav" for Bavaria.
          * For the USA, the subcode contains the code of the federal state, like
          * "mi" for Michigan.
          */
        int calcDST (QDateTime dt, CountryCode code, SubCode sCode);


        /**
          * Get first weekday in month
          */
        QDate getFirstDayInMonth (int year, int month, int weekday);


        /**
          * Get last weekday in month
          */
        QDate getLastDayInMonth (int year, int month, int weekday);

        /**
          * set number of last planet for which to calculate aspects
          */
        void setLastPlanet(int lastPlanet);

        /**
         * @brief getLastPlanet
         * @return last planet of horoscope, e.g. SE_CHIRON
         */
        int getLastPlanet();

        /**
          * returns tjd_ut of this horoscope
          * used for combine calculation
          */
        double getJulianDate ();

        /**
          * setter method for boolean attribute calcFromJD
          */
        void setCalcFromJD (bool val);

        /**
          *convert country codes to strings
          */
        QString getCountryStringShort ();

        /**
          * radix name
          */
        QString name;

        /**
          * location name, required to printout2nd horoscope
          */
        QString location;

        /**
          * coordinates of horoscope location
          *
          * These are accessible so they may be easily used for combine calculation
          */
        float longitude, latitude;

        double tjd_ut;              /* Julian Date of horoscope                 */
        double tjd_ut_r;            /* 2nd Julian Date for retrograde check     */

        double cusp [13];           /* cusp array starts with '1'               */
        double ascmc [10];          /* holds armc etc.                          */
        float planet [SE_NPLANETS]; /* absolute angles of planets               */
        bool retrograde [SE_NPLANETS];  /* retrograde flaggs                    */

        int noOfAspects;
        Aspect * aspectList[MAX_ASPECTS];

        CountryCode countryCode;   /* country code like 'nl', 'dk', 'uk' etc.          */

    protected:

        QDateTime dTime, dTimeUTC;
        TimeZone timeZone;         /* timezone code like 'MEZ'                         */

        /* may be used for federal land (Germany) or for federal state (USA, etc.)     */
        SubCode subCode;

        /* angles: 0, 30, 60, 90, 120, 150, 180, 45, 72, 144      */
        int angle [10];

        /* orbis for: 0, 30, 60, 90, 120, 150, 180, 45, 72, 144   */
        float aspectOrbis [10];

        /* orbis for planets                                      */
        float planetOrbis [SE_NPLANETS];

        /* This flag marks a horoscope object containing no horoscope data yet.
          Planet positions and aspects have not been calculated yet.                    */
        bool empty;

        /* True, if the horoscope should be calculated directly from a given julian
           day number. Otherwise, the julian day number will be calculated from the
           QDateTime and the location data first.                                       */
        bool calcFromJD;

        /* number of last planet for which to calculate aspects     */
        int lastPlanet;

};

#endif // HOROSCOPE_H
