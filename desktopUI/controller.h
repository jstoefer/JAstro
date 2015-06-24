#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <qtimer.h>
#include "muiwin.h"
#include "radixwheel.h"
#include "sidepanel.h"
#include "persondb.h"
#include "locationdb.h"
#include "partnerprofile.h"
#include <QMoveEvent>
#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QDesktopWidget>
#include <QApplication>
#include <QFontDatabase>


/**
  * This class contains all those parts of the application logic which are
  * specific to the desktop UI.
  */
class Controller : public QObject {

    Q_OBJECT

    public:
        Controller();

    signals:
        void locationFound (QStringList locations);
        void radixFound (QStringList radices);
        void partnerFound (QStringList radices);
        void compositeFound (QStringList radices);
        void combineFound (QStringList radices);

        /**
         * @brief emitted after a new horoscope has been calculated
         */
        void radixCalculated ();

    public slots:
        void correctOwnSize(void);
        void setPanelSize(QSize size);
        void moveOutPanel(void);
        void moveInPanel(void);
        void onMainWidgetClose(void);
        void onMainWidgetMove (QMoveEvent * event);
        void onMainWidgetClicked(QMouseEvent * event);
        void animatePanelMovement (bool in);
        void searchLocation (QString searchString);
        void setLocation (int index);
        void updateLocations (QStringList locations);
        void saveRadix();
        void searchRadix (const QString name);
        void calculateRadix(void);
        void calculateSynastry (void);
        void calculateComposite (void);
        void calculateCombine (void);
        void calculateSolar(void);
        void calculatePartnerProfile(void);
        void updateRadices (QStringList radices);
        void updatePartners (QStringList radices);
        void updateComposites (QStringList radices);
        void updateCombines (QStringList radices);

        /**
          * can parse a horoscope data file in .aph - format
          */
        void setRadix (int index);

        /**
          * parses selected line of .aph - format file from comboPartner and stores values
          */
        void setPartner (int index);

        /**
          * parses selected line of .aph - format file from comboComposite and stores values
          */
        void setCompositePartner (int index);

        /**
          * parses selected line of .aph - format file from comboCombine and stores values
          */
        void setCombinePartner (int index);

        /**
          * can parse a horoscope data file in old .txt format.
          */
        void setRadixOldFormat (int index);

        /**
          * clear form data
          */
        void clearRadixData();

        /**
         * @brief updates date value of either main horoscope (if this shows transits only) or transit horoscope t
         * @param date  the date as read from the form fields
         */
        void updateTransitDate(const QDate date);

        /**
         * @brief updates time value of either main horoscope (if this shows transits only) or transit horoscope t
         * @param time  the time as read from the form fields
         */
        void updateTransitTime(const QTime time);

        /**
         * @brief updates date value of seconddary directions horoscope sd
         * @param date  the date as read from the form fields
         */
        void updateSecondaryDirectionsDate(const QDate date);

        /**
         * @brief updates time value of secondary directions horoscope sd
         * @param time  the time as read from the form fields
         */
        void updateSecondaryDirectionsTime(const QTime time);

        /**
         * @brief updates location of secondary directions horoscope sd
         */
        void updateSecondaryDirectionsLoction(void);

        /**
         * @brief updates location of secondary directions horoscope sd
         */
        void updateSolarLocation(void);

        /**
          * Get country code from German country name
          */
        CountryCode getCountryCode (QString country);

        /**
          * print current horoscope or table
          */
        void print ();

        /**
          * set the theme as indicated by combo box under "settings" dialog
          */
        void setTheme ();

        /**
          * set the theme
          *
          * @param      QString name of the theme
          */
        void setTheme (QString themeName);

    private:

        PersonDB * pdb;                 /* person database                      */
        LocationDB * ldb;               /* location database                    */

        QRect appRect, screenSize;      /* av. size for app, screen size        */
        int minSize;                    /* smaller one of width / height        */

        MUIWin * muiwin;                /* main window mobile ui                */
        RadixWheel * wheel;
        SidePanel * panel;
        Horoscope *h;                   /* radix                                */
        Horoscope *t;                   /* transit horoscope                    */
        Horoscope *sd;                  /* secondary directions horoscope       */
        Horoscope *h2;                  /* partner horoscope synastry           */
        Horoscope *h3;                  /* composite horoscope                  */
        Horoscope *h4;                  /* combine horoscope                    */

        PartnerProfile *partnerProfile; /* Partner Profile                      */

        QTimer *timer;
        bool panelOut, panelInstantiated;
        CountryCode countryCode, countryCode2, countryCode3, countryCode4;
        SubCode subCode, subCode2, subCode3, subCode4;
        QDate date2, date3, date4;
        QTime time2, time3, time4;

                                        /* store date/time for transits resp.
                                           date/time for sec. directions        */
        QDateTime sdDateTime, transitDateTime;

                                        /* locations of 2nd horoscope           */
        QString location2, location3, location4;
        QString currentTheme;           /* currently selected sceen theme       */
        float longitude2, latitude2, longitude3, latitude3, longitude4, latitude4;

};

#endif // CONTROLLER_H
