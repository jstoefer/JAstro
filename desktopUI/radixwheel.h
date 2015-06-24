#ifndef RADIXWHEEL_H
#define RADIXWHEEL_H

/* Qt classes:          */
#include <QtWidgets>
#include <QMoveEvent>
#include <QMouseEvent>
#include <QIcon>
#include <QPrinter>
#include <QPrintDialog>
#include <QCoreApplication>

/* JAstro classes:      */
#include "horoscope.h"
#include "aspect.h"
#include "synastrie.h"
#include "composite.h"
#include "combine.h"
#include "solar.h"

#include "sidepanel.h"

#define PI 3.14159265

class RadixWheel : public QWidget {

    Q_OBJECT

public:
    /**
      * parameterless constructor which uses default values to display a test radix
      */
    RadixWheel(bool mobileUI = false, QWidget *parent = 0);

    /**
      * constructor of class RadixWheel
      *
      * @param horoscope    value object which holds planet positions, houses and aspects
      * @param QWidget      parent widget
      */
    RadixWheel (Horoscope * horoscope, bool mobileUI = false, QWidget *parent = 0);

    ~RadixWheel();

    void setDefaults (void);

    void setWheelColor (QColor c);
    void setFireElementColor (QColor c);
    void setWaterElementColor (QColor c);
    void setAirElementColor (QColor c);
    void setEarthElementColor (QColor c);
    void setItemColor (QColor c);
    void setItemColor2 (QColor c);
    void setMarkerColor (QColor c);
    void setAspectColorBlue (QColor c);
    void setAspectColorGreen (QColor c);
    void setAspectColorRed (QColor c);
    void setTransitColor(QColor c);
    void setProgressionColor(QColor c);

    void setTransitsOnly (bool transitsOnly );
    void setHoroscope(Horoscope * horoscope);
    void setSynastry(Horoscope * horoscope);
    void setCompositePartner(Horoscope * horoscope);
    void setCombinePartner(Horoscope * horoscope);
    void setTransit(Horoscope * transit);
    void setSecondaryDirection(Horoscope * sd);

    /**
      * check wether h, h2 and s are valid; claculate s, if it is still invalid ("NULL")
      *
      * @return FALSE, if anything is missing
      */
    bool checkSynastrieData(void);

    /**
      * check wether h, h3 and c are valid; calculate c, if it is still invalid ("NULL")
      *
      * @return FALSE, if anything is missing
      */
    bool checkCompositeData(void);

    /**
      * check wether h, h4 and cb are valid; calculate cb, if it is still invalid ("NULL")
      *
      * @return FALSE, if anything is missing
      */
    bool checkCombineData(void);

    /**
      * check wether h is valid; calculate sol, if it still invalid ("NULL")
      *
      * @return FALSE, if anything is missing
      */
    bool checkSolarData(void);

    void closeEvent(QCloseEvent *event);
    void moveEvent (QMoveEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void resizeEvent ( QResizeEvent * event );

    QFont fontHeading, fontInfo, fontTable, astroFontHuge, astroFontMedium, astroFontSmall,
            astroFontTable, defaultFontSmall;
    QPalette * palette;

    bool transitsOnly;              /* if true, show transits as radix, but w/o aspects         */
    bool showTransits;              /* if true, show transits at the outer rim of radix         */
    bool showSecondaryDirections;   /* if true, show secondary directions                       */
    bool radix;                     /* if true, show radix planets and houses                   */
    bool radixAspects;              /* if true, show radix aspects                              */
    bool synastrie;                 /* if true, show additional wheel with 2nd horoscope h2     */
    bool synastriePlanetaryAspects; /* show aspects between planets of h1 and h2                */
    bool synastrieAxisAspects;      /* show aspects between planets of h1 and main axes of h2   */
    bool synastrieAxisReverse;      /* show aspects between planets of h2 and main axes of h1   */
    bool composite;                 /* if true, show composite horoscope of h and h2            */
    bool combine;                   /* if true, show combine horoscope of h and h2              */
                                    /* if true, solar horoscope of h                            */
                                    /* if 'synastrie' is true as well, show synastry of h and   */
    bool solar;                     /* solar                                                    */

    /* the side panel holding the input forms                                                   */
    SidePanel * panel;

    /* special jastro objects                                                                   */
    Horoscope *t;                   /* transit Horoscope                                        */
    Horoscope *sd;                  /* secondary directions horoscope                           */
    Synastrie *s;                   /* synastrie horoscope                                      */
    Composite *c;                   /* composite horoscope                                      */
    Combine *cb;                    /* combine horoscope                                        */
    Solar * sol;                    /* solar horoscope                                          */

    signals:
        void wheelClosed (void);
        void wheelMoved (QMoveEvent * event);
        void wheelClicked(QMouseEvent * event);
        void wheelResized (QSize);
        void finishedPrinting(void);

    public slots:
        void showRadix(void);
        void hideRadix(void);
        void showRadixAspects(void);
        void hideRadixAspects(void);
        void showPartner(void);
        void hidePartner(void);
        void showComposite(void);
        void showCombine(void);
        void showSolar(void);
        void hideComposite(void);
        void hideCombine(void);
        void hideSolar(void);
        void showSynastriePlanetaryAspects(int state);
        void showSynastrieAxisAspects(int state);
        void showSynastrieAxisAspectsReverse(int state);
        void setCompositAsSynastriePartner(int state);
        void setSolarYear(int year);

        /**
          * print method
          */
        void print ();

protected:

    /**
      * all painting happens here
      */
    void paintEvent(QPaintEvent *event);

    /**
      * paint small wheel optimized for mobile screens
      */
    void paintSmallWheel(QPaintEvent *event);

    /**
      * draws currently selected horoscope type at given y - position
      *
      * called by paintEvent () as well as by print ();
      *
      * @param  QPainter    QPainter object to be used for drawing
      * @param  int         y - position where to start drawing
      *
      * @return int         resulting y - position at end of wheel
      *
      */
    int paintHoroscope (QPainter &p, int yPos);


    /* horoscope objects that deliver data for drawing and point to one of the below horoscopes */
    Horoscope *h;                   /* pointer to horoscope (=h1 or =c), wheel outer horoscope  */
    Horoscope *hh;                  /* pointer to hor. (=h2 or sol->h), wheel inner horoscope   */

    /* further horoscope objects, referenced by the above pointers                              */
    Horoscope *h1;                  /* 1st horoscope; 1st partner synastrie and composite       */
    Horoscope *h2;                  /* 2nd horoscope, 2nd partner for synastrie                 */
    Horoscope *h3;                  /* 3rd horoscope, 2nd partner for composite                 */
    Horoscope *h4;                  /* 4th horoscope, 2nd partner for combine                   */
    Horoscope *h5;                  /* 5th horoscope, solar                                     */

    /* printer object                                                                           */
    QPrinter printer;

    /* colors used in the radix drawing                                                         */
    QColor wheelColor;              /* used to draw the wheel structure                         */
    QColor fireElementColor;        /* used to draw sign symbols of the fire element            */
    QColor waterElementColor;       /* used to draw sign symbols of the water element           */
    QColor airElementColor;         /* used to draw sign symbols of the air element             */
    QColor earthElementColor;       /* used to draw sign symbols of the earth element           */
    QColor itemColor;               /* used to draw planets and house numbers                   */
    QColor itemColor2;              /* used to draw solar planets                               */
    QColor markerColor;             /* used to draw the markers for the planets                 */
    QColor aspectColorRed;          /* color for conjunct, square and opposition aspects        */
    QColor aspectColorGreen;        /* color used for quintile and semsextile spects            */
    QColor aspectColorBlue;         /* color used for trine and sectile                         */
    QColor transitColor;            /* color used to draw transit planets                       */
    QColor progressionColor;        /* color used to draw progressive planets                   */

    int n_planets;

    int solarYear;                  /* year used for solar calculation                          */

    bool mobileUI;                  /* if true, a mobile version of the UI will be created      */
    bool printing;                  /* set true while printing                                  */

};

#endif // RADIXWHEEL_H
