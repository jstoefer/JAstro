#include "radixwheel.h"


/*----------------------------------------------------------------------------------------------*/


RadixWheel::RadixWheel (Horoscope * horoscope, bool mobileUI, QWidget *parent) : QWidget (parent) {

    this->mobileUI = mobileUI;

    this->h1 = horoscope;
    this->h = h1;
    this->hh = NULL;
    this->h2 = NULL;
    this->h3 = NULL;
    this->h4 = NULL;
    this->h5 = NULL;
    this->s = NULL;
    this->c = NULL;
    this->cb = NULL;
    this->sol = NULL;
    this->setDefaults();

    /* settings at start of program                 */
    transitsOnly                = TRUE;
    radix                       = TRUE;

    /* these options may be activated later         */
    showTransits                = FALSE;
    showSecondaryDirections     = FALSE;
    synastrie                   = FALSE;
    composite                   = FALSE;
    combine                     = FALSE;
    solar                       = FALSE;
    synastriePlanetaryAspects   = FALSE;
    synastrieAxisAspects        = FALSE;
    synastrieAxisReverse        = FALSE;

}

/*----------------------------------------------------------------------------------------------*/

RadixWheel::RadixWheel(bool mobileUI, QWidget *parent) : QWidget(parent) {

    this->mobileUI = mobileUI;

    this->h1 = new Horoscope ("Hier und jetzt", QDateTime::currentDateTime(),
                              "Hamburg", 52.77, 9.00);
    this->h2 = NULL;
    this->h3 = NULL;
    this->h4 = NULL;
    this->s = NULL;
    this->c = NULL;
    this->cb = NULL;
    this->setDefaults();
    transitsOnly                = TRUE;
    showTransits                = FALSE;
    radix                       = TRUE;
    synastrie                   = FALSE;
    composite                   = FALSE;
    synastriePlanetaryAspects   = FALSE;
    synastrieAxisAspects        = FALSE;
    synastrieAxisReverse        = FALSE;

    // load images from resource file
    // QString imgPath = ":/img/";

    // qDebug() << "imgPath =" << imgPath;

    // QIcon windowIcon  = QIcon(imgPath+"composite.png");

    /* set application icon                         */
    // this->setWindowIcon(windowIcon);
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setHoroscope(Horoscope * horoscope) {
    this->h1 = horoscope;
    if (this->s != NULL) {
        this->s->setH1(this->h1);
    }
    if (this->c != NULL) {
        this->c->setH1(this->h1);
    }
    radix = TRUE;
    synastrie = FALSE;
    composite = FALSE;
    combine = FALSE;
    solar = FALSE;
    radixAspects = TRUE;
    transitsOnly = FALSE;
    setWindowTitle(this->h->name);
    this->h = this->h1;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::resizeEvent ( QResizeEvent * event ) {
    emit wheelResized (event->size());
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showRadix(void) {
    this->radix = TRUE;
    this->radixAspects = TRUE;
    this->synastrie = FALSE;
    this->composite = FALSE;
    this->combine = FALSE;
    this->solar = FALSE;
    this->h = this->h1;
    setWindowTitle(this->h->name);
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::hideRadix(void) {
    this->radix = FALSE;
    this->radixAspects = FALSE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showRadixAspects(void) {
    this->radixAspects = TRUE;
    this->synastrie = FALSE;
    this->composite = FALSE;
    this->combine = FALSE;
    this->solar = FALSE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::hideRadixAspects(void) {
    this->radixAspects = FALSE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setTransit(Horoscope * transit) {
    this->t = transit;
    showTransits = TRUE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setSecondaryDirection(Horoscope * sd) {
    this->sd = sd;
    showSecondaryDirections = TRUE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setSynastry(Horoscope * horoscope) {
    this->h2 = horoscope;
    transitsOnly = FALSE;
    setWindowTitle("Synastrie von " + this->h->name + " und "+this->h2->name);
    this->synastrie = checkSynastrieData();
    if (this->synastrie) {
        this->s->setH2(horoscope);
        this->radixAspects = false;
        this->hh = horoscope;
    }
    this->hideRadix();
    this->hideComposite();
    this->hideCombine();
    this->hideSolar();
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setCompositAsSynastriePartner(int state) {
    qDebug() << "RadixWheel::setCompositAsSynastriePartner";
    if (state) {
        // this->setSynastry(this->c->h);
        // this->
        this->s->setH2(this->c->h);
        this->radixAspects = false;
        setWindowTitle("RÃŒckbindung Composit auf Radix " + this->h->name);
        this->update();
    } else {
        if (this->synastrie) {
            this->s->setH2(this->h2);
            this->radixAspects = false;
            setWindowTitle("Synastrie von " + this->h->name + " und "+this->h2->name);
            this->update();
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setCompositePartner(Horoscope * horoscope) {
    this->h3 = horoscope;
    transitsOnly = FALSE;
    setWindowTitle("Composit von "+this->h1->name + " und "+this->h3->name);
    this->composite = checkCompositeData(); /* composite will be calculated here    */
    if (this->composite) {
        this->c->setH2(horoscope);
        this->h = this->c->h;

    }
    radix = FALSE;
    synastrie = FALSE;
    combine = FALSE;
    solar = FALSE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setCombinePartner(Horoscope * horoscope) {

    /* make h4 the partner horoscope for the combine                    */
    this->h4 = horoscope;
    transitsOnly = FALSE;
    setWindowTitle("Combin von "+this->h1->name + " und "+this->h4->name);

    /* A new combine will be created here, if it does not exist yet.    */
    this->combine = checkCombineData();
    if (this->combine) {

        /* By setting H2 of combine, the combine will be calculated.    */
        this->cb->setH2(horoscope);

        /* Point main horoscope of the wheel to calculated horoscope of the
           combine object.      */
        this->h = this->cb->h; 
    }
    /* Set proper flags for method paintEvent() so the combine horoscope
       will be drawn.       */
    radix = FALSE;
    synastrie = FALSE;
    composite = FALSE;
    solar = FALSE;
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setSolarYear(int year) {
    // qDebug() << "RadixWheel::setSolarYear, year = " << year;
    this->solarYear = year;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::hidePartner() {
    synastrie = FALSE;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showPartner() {
    this->synastrie = checkSynastrieData();
    if (synastrie) {
        setWindowTitle("Synastrie von " + this->h1->name + " und "+this->h2->name);
        this->s->setH2(this->h2);
        this->hh = h2;
    } else {
        this->hh = NULL;
    }
    this->h = this->h1; /* show radix on outer wheel   */
    this->hideRadix();
    this->hideComposite();
    this->hideCombine();
    this->hideSolar();
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showComposite() {
    this->composite = checkCompositeData();
    if (composite) {
        setWindowTitle("Composit von "+this->h1->name + " und " + this->h3->name);
        this->h = this->c->h;
    }
    this->hideRadix();
    this->hidePartner();
    this->hideCombine();
    this->hideSolar();
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showCombine() {
    this->combine = checkCombineData();
    if (combine) {
        setWindowTitle("Combin von "+this->h1->name + " und " + this->h4->name);
        this->h = this->cb->h;
    }
    this->hideRadix();
    this->hidePartner();
    this->hideComposite();
    this->hideSolar();
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showSolar() {
    // qDebug() << "RadixWheel::showSolar()";
    this->solar = checkSolarData();
    if (solar) {
        QString title(this->h1->name + ", Solar " +QString().setNum(this->solarYear));
        setWindowTitle(title);
        this->s->setH2(this->sol->h);
        this->h = this->h1; /* show radix on outer wheel    */
        this->hh = sol->h;  /* show solar on inner wheel    */
    }
    this->hideRadix();
    this->hidePartner();
    this->hideComposite();
    this->hideCombine();
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::hideComposite() {
    composite = FALSE;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::hideCombine() {
    combine = FALSE;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::hideSolar(void) {
    solar = FALSE;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showSynastriePlanetaryAspects(int state) {
    synastriePlanetaryAspects = (state && checkSynastrieData());
    if (checkSynastrieData()) {
        this->s->calculateAspects();
    }
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showSynastrieAxisAspects(int state) {
    synastrieAxisAspects = (state && checkSynastrieData());
    if (checkSynastrieData()) {
        this->s->calculateAspects();
    }
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::showSynastrieAxisAspectsReverse(int state) {
    synastrieAxisReverse = (state && checkSynastrieData());
    if (checkSynastrieData()) {
        this->s->calculateAspects();
    }
    this->update();
}

/*----------------------------------------------------------------------------------------------*/

bool RadixWheel::checkSynastrieData(void) {
    bool result = TRUE;
    if (this->h1 == NULL) {
        result = FALSE;     /* no synastrie w/o 2 horoscopes    */
    } else if (this->h2 == NULL) {
        result = FALSE;
    } else if (this->s == NULL) {
        this->s = new Synastrie (this->h1, this->h2);
    }
    return result;
}

/*----------------------------------------------------------------------------------------------*/

bool RadixWheel::checkSolarData(void) {
    bool result = TRUE;
    if (this->h1 == NULL) {
        result = FALSE;     /* no synastrie w/o 2 horoscopes    */
    } else if (this->sol == NULL) {
        this->sol = new Solar (this->h1, this->solarYear);
        if (this->s == NULL) {
            this->s = new Synastrie(this->h1, this->sol->h);
        }
    } else {
        this->sol->setYear(this->solarYear);
        this->s->setH2(sol->h);
    }
    return result;
}

/*----------------------------------------------------------------------------------------------*/

bool RadixWheel::checkCompositeData(void) {
    bool result = TRUE;
    if (this->h1 == NULL) {
        result = FALSE;
    } else if (this->h3 == NULL) {
        result = FALSE;
    } else if (this->c == NULL) {
        this->c = new Composite (this->h1, this->h3);
    }
    return result;
}

/*----------------------------------------------------------------------------------------------*/

bool RadixWheel::checkCombineData(void) {
    bool result = TRUE;
    if (this->h1 == NULL) {
        result = FALSE;
    } else if (this->h4 == NULL) {
        result = FALSE;
    } else if (this->cb == NULL) {
        this->cb = new Combine (this->h1, this->h4);
    }
    return result;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setDefaults (void) {

    /* the window title is visible above the
       application area in symbian, too ..      */
    setWindowTitle(this->h->name);

    /*
    #define SE_SUN          0
    #define SE_MOON         1
    #define SE_MERCURY      2
    #define SE_VENUS        3
    #define SE_MARS         4
    #define SE_JUPITER      5
    #define SE_SATURN       6
    #define SE_URANUS       7
    #define SE_NEPTUNE      8
    #define SE_PLUTO        9
    #define SE_MEAN_NODE    10
    #define SE_TRUE_NODE    11
    #define SE_MEAN_APOG    12
    #define SE_OSCU_APOG    13
    #define SE_EARTH        14
    #define SE_CHIRON       15  */

    /* fonts ...                                */
    astroFontHuge       = QFont("Astro-SemiBold");
    astroFontMedium     = QFont("Astro-SemiBold");
    astroFontSmall      = QFont("Astro-SemiBold");
    astroFontTable      = QFont("Astro-SemiBold");
    defaultFontSmall    = QFont("Ubuntu");
    fontHeading         = QFont("Ubuntu");
    fontInfo            = QFont("Ubuntu");
    fontTable           = QFont("Ubuntu Mono");

    /* scale fonts and adapt colors to UI       */
    //if (mobileUI) {
    //    fontHeading.setPixelSize(16);
    //    fontHeading.setBold(true);
    //    fontInfo.setPixelSize(12);
    //    fontInfo.setPixelSize(11);
    //    astroFontHuge.setPixelSize(16);
    //    astroFontMedium.setPixelSize(16);
    //    astroFontSmall.setPixelSize(14);
    //    astroFontTable.setPixelSize(16);
    //    defaultFontSmall.setPixelSize(8);
    //} else {
        fontHeading.setPixelSize(20);
        fontHeading.setBold(true);
        fontInfo.setPixelSize(14);
        fontTable.setPixelSize(14);
        astroFontHuge.setPixelSize(16);
        astroFontMedium.setPixelSize(10);
        astroFontSmall.setPixelSize(8);
        astroFontTable.setPixelSize(18);
        defaultFontSmall.setPixelSize(4);
    //}

    /* more colors ...                          */
    markerColor         = QColor(220, 0, 0);
    aspectColorRed      = QColor(240, 0, 30);
    aspectColorGreen    = QColor(0, 220, 40);
    aspectColorBlue     = QColor(0, 60, 240);

}

/*----------------------------------------------------------------------------------------------*/

int RadixWheel::paintHoroscope (QPainter &painter, int yPos) {

    /* do not draw anything if there is no data yet !                                           */
    if (!(radix || synastrie || composite || combine || solar)) return yPos;

    int side;
    if (printing) {
        side = qMin(printer.width(), printer.height());
    } else {
        side = qMin(this->width(), this->height());
    }
    int r = side/2;

    QPointF center (0, 0);

    /* save context until end of this method                                            */
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(r, r + yPos);             /* find center of plane                 */
    painter.scale(side / 200.0, side / 200.0);  /* scale to -100 ... +100               */

    QPen pen(wheelColor, 0);

    /* TODO: make this adjustable via settings !                                        */
    pen.setWidthF(0.1);
    painter.setPen(pen);

    painter.drawEllipse (center, 90, 90);       /* outer zodiac circle                  */
    painter.drawEllipse (center, 74, 74);       /* inner zodiac circle                  */
    painter.drawEllipse (center, 71, 71);       /* inner scale circle                   */

    if (radix || synastrie || composite || combine || solar) {
        painter.drawEllipse (center, 50, 50);   /* inner house circle                   */
    }

    if (synastrie || solar) {
        painter.drawEllipse (center, 35, 35);   /* inner circle partner houses          */
    }

    if (transitsOnly) {
        painter.drawEllipse (center, 20, 20);   /* center circle                        */
    }

    painter.save();

    if (radix || synastrie || composite || combine || solar) {
        painter.rotate(h->cusp[1] -270.0);     /* get AC to the left                    */
    } else {
        painter.rotate(90.0);                  /* get Aries to the left                 */
    }

    for (int i = 0; i < 12; ++i) {             /* devide into 30 degree sections        */
        painter.drawLine(0, 74, 0, 90);
        painter.rotate(30.0);
    }

    for (int i = 0; i < 36; ++i) {             /* draw decanates                        */
        painter.drawLine(0, 71, 0, 74);
        painter.rotate(10.0);
    }

    for (int j = 0; j < 360; ++j) {            /* degree scale                          */
        if (j%5 == 0) {
            painter.drawLine(0, 71, 0, 73);
        } else {
            painter.drawLine(0, 71, 0, 72);
        }
        painter.rotate(1.0);
    }

    painter.setFont(astroFontHuge);             /* use huge font for zodiac signs           */
    pen.setColor(itemColor);
    painter.setPen(pen);
    painter.save();
    painter.rotate(-15.0);                      /* draw into center of sign segment         */

    QPointF p1 (-7, -76);

    int dAlpha = 0;

    QColor signColor = itemColor;

    for (int i = 0; i < 6; ++i) {               /* draw zodiac signs from libra to pisces   */
        if ((i == 0) || (i == 2) || (i == 5)) {
            dAlpha = 2;
        } else {
            dAlpha = 0;
        }
        painter.rotate (dAlpha);

        switch ((i+6)%4) {
        case 0:
            signColor = fireElementColor;
            break;
        case 1:
            signColor = earthElementColor;
            break;
        case 2:
            signColor = airElementColor;
            break;
        case 3:
            signColor = waterElementColor;
            break;
        default:
            signColor = itemColor;
            break;
        }

        pen.setColor(signColor);
        painter.setPen(pen);

        painter.drawText ( p1, signLetters[i+6]);
        painter.rotate (-dAlpha);
        painter.rotate(-30.0);
    }
    for (int i = 6; i < 12; ++i) {             /* draw zodiac signs from aries to virgo     */
        if ((i == 7) ) {
            dAlpha = 2;
        } else {
            dAlpha = 0;
        }
        painter.rotate (dAlpha);

        switch ((i+6)%4) {
        case 0:
            signColor = fireElementColor;
            break;
        case 1:
            signColor = earthElementColor;
            break;
        case 2:
            signColor = airElementColor;
            break;
        case 3:
            signColor = waterElementColor;
            break;
        default:
            signColor = itemColor;
            break;
        }

        pen.setColor(signColor);
        painter.setPen(pen);
        painter.drawText ( p1, signLetters[i-6]);
        painter.rotate (-dAlpha);
        painter.rotate(-30.0);
    }

    painter.restore();                              /* back 15 degrees                          */
    painter.restore();                              /* x -axis is horizont again                */

    painter.setFont(astroFontMedium);

    if (radix || synastrie || composite || combine || solar) {
        painter.drawLine(-99, 0, -90, 0);           /* draw AC - DC axis                        */
        painter.drawLine( 99, 0,  90, 0);

        if (transitsOnly) {
            painter.drawLine( 71, 0,  20, 0);
            painter.drawLine(-71, 0, -20, 0);
        } else {
            painter.drawLine( 71, 0,  50, 0);
            painter.drawLine(-71, 0, -50, 0);
        }

        QPointF AC (-99.0, -1.0);                   /* draw "ASC" at tip of AC - DC axis        */
        painter.drawText(AC, "a");

        painter.save();
        painter.rotate(h->cusp[1] - h->cusp[10]);   /* rotate back to MC                        */
        painter.drawLine(-99, 0, -90, 0);           /* draw MC - IC axis                        */
        painter.drawLine( 99, 0,  90, 0);

        if (transitsOnly) {
            painter.drawLine( 71, 0,  20, 0);
            painter.drawLine(-71, 0, -20, 0);
        } else {
            painter.drawLine( 71, 0,  50, 0);
            painter.drawLine(-71, 0, -50, 0);
        }

        painter.restore();

        for (int i = 2; i< 7; i++) {                /* draw remaining houses                    */
            if (i != 4) {
                painter.save();
                                                    /* rotate to house cusp                     */
                painter.rotate(h->cusp[1] - h->cusp[i]);
                painter.drawLine(-71, 0, -50, 0);
                painter.drawLine( 71, 0,  50, 0);
                painter.restore();
            }
        }
    }

    /* draw houses of 2nd horoscope in the inner circle                                         */
    if (synastrie || solar) {
        for (int i = 1; i< 7; i++) {                /* draw remaining houses                    */
            painter.save();
                                                    /* rotate to house cusp                     */
            painter.rotate(h->cusp[1] - hh->cusp[i]);
            painter.drawLine(-50, 0, -35, 0);
            painter.drawLine( 50, 0,  35, 0);
            if ((i == 1) || (i == 4)) {             /* draw AC / DC / IC / MC - axis outside    */
                if (solar) {
                    pen.setWidth(0.7);
                    pen.setColor(Qt::darkCyan);
                    painter.setPen(pen);

                }
                painter.drawLine(-99, 0, -90, 0);
                painter.drawLine( 99, 0,  90, 0);
                if (solar) {
                    pen.setWidth(0.4);
                    pen.setColor(itemColor);
                    painter.setPen(pen);
                }
            }
            painter.restore();
        }
    }


    pen.setWidth(0.4);
    pen.setColor(wheelColor);
    painter.setPen(pen);

    if (radix || synastrie || composite || combine || solar) {
        for (int i = 0; i <= h->getLastPlanet(); i++) {       /* draw markers for planet positions        */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                painter.rotate(h->cusp[1] - h->planet[i]);
                painter.drawLine( -52, 0,  -50, 0); /* marker on inner house circle             */
                painter.drawLine( -71, 0,  -69, 0); /* marker on outer house circle             */
                painter.restore();
            }
        }
    }

    if (synastrie || solar) {
        for (int i = 0; i < h->getLastPlanet(); i++) {          /* draw markers for partner planets     */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                pen.setColor(Qt::red);
                painter.setPen(pen);
                painter.rotate(h->cusp[1] - hh->planet[i]);
                painter.drawLine( -36, 0,  -35, 0);     /* marker on inner house circle         */
                painter.drawLine( -49, 0,  -50, 0);     /* marker on outer house circle         */
                painter.restore();
            }
        }
    }

    if (this->showTransits) {
        for (int i = 0; i < h->getLastPlanet(); i++) {       /* draw markers for transit planets         */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                // pen.setColor(Qt::cyan);
                pen.setColor(transitColor);
                painter.setPen(pen);
                painter.rotate(h->cusp[1] - t->planet[i]);
                painter.drawLine( -71, 0,  -69, 0); /* marker on outer house circle             */
                painter.drawLine( -92, 0,  -90, 0); /* marker on outer sign circle              */
                painter.restore();
            }
        }
    }

    if (this->showSecondaryDirections) {
        for (int i = 0; i < h->getLastPlanet(); i++) {       /* draw markers for progressive planets     */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                // pen.setColor(Qt::cyan);
                pen.setColor(progressionColor);
                painter.setPen(pen);
                painter.rotate(h->cusp[1] - sd->planet[i]);
                painter.drawLine( -71, 0,  -69, 0); /* marker on outer house circle             */
                painter.drawLine( -92, 0,  -90, 0); /* marker on outer sign circle              */
                painter.restore();
            }
        }
    }

    pen.setWidthF(0.1);

    float x, y, alpha, alphaRad;
    float current = 0.0;                        /* angle of next planet to be drawn         */
    float previous = 0.0;                       /* angle of previous planet that was drawn  */
    int currentIndex = 0;                       /* index of next planet to be drawn         */
    int previousIndex = -1;                     /* index of previous planet drawn           */
    float ri = 58.0;                            /* radius of planet shifted towards center  */
    float ra = 66.0;                            /* radius of planet shifted outwards        */
    float r0 = ri;                              /* standard radius for planet symbol        */
    int shift =0;                               /* wether last planet was shifted inwards (-1),
                                                   outwards (+1) or not shifted at all (0). */

    /*
     * Current position is initialized with 0.0°
     * Find planet which is closest to current position.
     * Then advance current position to that planets position.
     * Then find that planet, which comes next.
     * At the end, all planets should have been drawn once.
     */
    if (radix || synastrie || composite || combine || solar) {
        int i=0;
        // qDebug() << "h->lastPlanet = " << h->getLastPlanet();
        while (i<= h->getLastPlanet()) {    /* we are going to draw all of the planets in the
                                               array                                            */
            float candidate = 360;          /* start searching from at the end of the circle    */

            for (int k = 0; k<= h->getLastPlanet(); k++) {    /* again, iterate over all planets in
                                                      array, but skip some of them              */
                if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                    if (h->planet[k] > current) {   /* planet has not yet been drawn            */
                        /* check wether this one is closer to last planet drawn, than the
                           others we found so far                                               */
                        if (h->planet[k] < candidate) {
                            candidate = h->planet[k];
                            currentIndex = k;
                            // qDebug() << "candidate: " << candidate << ", currentIndex: " << currentIndex;
                        }
                    }
                }
            }

            /* If we found the same planet once again, we are done!                             */
            if (currentIndex == previousIndex) {
                // qDebug() << "currentIndex == previousIndex = " << currentIndex;
                break;
            }
            /* Now we should have found the next planet to be drawn.
               So we advance our current position to that planets position for the next
               iteration and for drawing.                                                       */
            previous = current;
            previousIndex = currentIndex;   /* for loop termination                             */
            current = h->planet[currentIndex];

            /* correct drawing position, if two planets are too close                           */
            if (current - previous < 5) {
                if (shift == 0) {
                    shift = 1;
                    r0 = ra;                        /* shift planet symbol outwards             */
                } else if (shift == 1) {
                    shift = 0;
                    r0 = ri;                        /* draw planet symbol at normal position    */
                }
            } else {
                shift = 0;
                r0 = ri;
            }
            alpha = current - h->cusp[1];
            alphaRad = alpha*PI/180.0;
            x = -r0 * cos (alphaRad) -4;
            y = r0 * sin (alphaRad)+4;
            QPointF planetPosition(x, y);

            /* draw planet symbol using astro font                                              */
            painter.drawText(planetPosition, planetLetters[currentIndex]);

            /* show little 'r' for retrograde planets       */
            if (h->retrograde[currentIndex]) {
                painter.setFont(defaultFontSmall);
                QPointF rPosition(x - abs(2*sin(alphaRad)) +7, y);
                painter.drawText(rPosition, "r");
                painter.setFont(astroFontMedium);
            }
            // qDebug () << "planet: " << planetLetters[currentIndex] << ",planet index = " << currentIndex << ", iteration = " << i;
            i++;
        }
    }

    if (synastrie || solar) {
        current = 0.0;                              /* angle of next planet to be drawn         */
        previous = 0.0;                             /* angle of previous planet that was drawn  */
        currentIndex = 0;                           /* index of next planet to be drawn         */
        previousIndex = -1;                         /* index of previous planet drawn           */
        ri = 40.0;                                  /* radius of planet shifted towards center  */
        ra = 46.0;                                  /* radius of planet shifted outwards        */
        r0 = ri;                                    /* standard radius for planet symbol        */
        shift =0;                                   /* wether last planet was shifted inwards (-1),
                                                       outwards (+1) or not shifted at all (0). */
        int i = 0;
        while (i <= h->getLastPlanet()) {               /* draw planet symbols in ascending order of
                                                        their angle                             */
            float candidate = 360;

            for (int k = 0; k <= h->getLastPlanet(); k++) {
                if (hh->planet[k] > current) {       /* o.k., here is a candidate                */
                    if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                        if (hh->planet[k] < candidate) {
                            candidate = hh->planet[k];
                            currentIndex = k;
                        }
                    }
                }
            }

            /* If we found the same planet once again, we are done!                             */
            if (currentIndex == previousIndex) break;

            previous = current;
            previousIndex = currentIndex;
            current = hh->planet[currentIndex];
            if (current - previous < 5) {           /* these symbols would overlap              */
                if (shift == 0) {
                    shift = 1;
                    r0 = ra;                        /* shift planet symbol outwards             */
                } else if (shift == 1) {
                    shift = 0;
                    r0 = ri;                        /* draw planet symbol at normal position    */
                }
            } else {
                shift = 0;
                r0 = ri;
            }
            alpha = current - h->cusp[1];
            // qDebug ("alpha = %f", alpha);
            alphaRad = alpha*PI/180.0;
            x = -r0 * cos (alphaRad) -4;
            y = r0 * sin (alphaRad)+4;
            QPointF planetPosition(x, y);
            if (solar) {
                painter.setPen(Qt::darkCyan);
            }
            painter.drawText(planetPosition, planetLetters[currentIndex]);

            /* draw small 'r' for retrograde planets    */
            if (hh->retrograde[currentIndex]) {
                painter.setFont(defaultFontSmall);
                QPointF rPosition(x - abs(2*sin(alphaRad)) +7, y);
                painter.drawText(rPosition, "r");
                painter.setFont(astroFontMedium);
            }

            if (solar) {
                painter.setPen(wheelColor);
            }
            i++;
        }
    }

    if (this->showTransits) {

        // qDebug ("showTransits 1");

        painter.save();

        painter.setPen(transitColor);

        /**
          * Planets will be drawn in ascending order of their absolute angle.
          * Example: If mars is at 10° aries and moon is at 20° taurus, the absolue angles are
          * 10° for mars and 50° for the moon (since taurus starts at 30°).
          * In this example, mars would be drawn first.
          * The algorithm starts with the planet with the smallest angle and draws it.
          * After that, it searches for the planet with the second smalles angle and draws that
          * and so on.
          */

        current = 0.0;                                  /* angle of next planet to be drawn         */
        previous = 0.0;                                 /* angle of previous planet that was drawn  */
        currentIndex = 0;                               /* index of next planet to be drawn         */
        previousIndex = -1;                             /* index of previous planet that was drawn  */

       /**
        * Planets which would overlap in the drawing with the planet previously will be
        * shifted outwards or inwards.
        */

        ri = 95.0;                                      /* radius of planet shifted towards center  */
        ra = 99.0;                                      /* radius of planet shifted outwards        */
        r0 = ri;                                        /* standard radius for planet symbol        */
        shift =0;                                       /* wether last planet was shifted inwards
                                                           (-1), outwards (+1) or not shifted at
                                                           all (0).                                 */
        int i = 0;
        while (i <= t->getLastPlanet()) {               /* Go through list of planets from moon to
                                                           Chiron.                                  */
            float candidate = 360;                      /* any planet that has a lower angle than
                                                           360° will be the next candidate.         */
            for (int k = 0; k <= t->getLastPlanet(); k++) {
                if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                    if (t->planet[k] > current) {       /* check wether planet has not yet been
                                                           drawn                                    */
                        if (t->planet[k] < candidate) { /* check wether it is the next planet to be
                                                           drawn                                    */
                            candidate = t->planet[k];
                            currentIndex = k;           /* index points to the planet that would be
                                                           drawn if none of the remaining planets
                                                           has a lower angle.                       */
                        }
                    }
                }
            }

            /* If we found the same planet once again, we are done!                                 */
            // qDebug ("transits: currentIndex = %d,  previousIndex = %d", currentIndex, previousIndex);

            if (currentIndex == previousIndex) {
                // qDebug ("break: currentIndex = %d,  previousIndex = %d", currentIndex, previousIndex);
                break;
            }
            /* prepare search for next planet to be drawn                                           */
            previous = current;
            previousIndex = currentIndex;

            /* Now draw planet including any necessary shift to avoid overlap with previous planet  */
            current = t->planet[currentIndex];
            if (current - previous < 5) {               /* these symbols would overlap              */
                if (shift == 0) {
                    shift = 1;
                    r0 = ra;                            /* shift planet symbol outwards             */
                } else if (shift == 1) {
                    shift = 0;
                    r0 = ri;                            /* draw planet symbol at normal position    */
                }
            } else {
                shift = 0;
                r0 = ri;
            }
            alpha = current - h->cusp[1];
            // qDebug ("alpha = %f", alpha);
            alphaRad = alpha*PI/180.0;
            x = -r0 * cos (alphaRad) -4;
            y = r0 * sin (alphaRad)+4;
            QPointF planetPosition(x, y);
            painter.drawText(planetPosition, planetLetters[currentIndex]);

            /* draw small 'r' for retrograde planets    */
            if (t->retrograde[currentIndex]) {
                painter.setFont(defaultFontSmall);
                QPointF rPosition(x - abs(2*sin(alphaRad)) +7, y);
                painter.drawText(rPosition, "r");
                painter.setFont(astroFontMedium);
            }

        }
        painter.restore();
    }

    if (this->showSecondaryDirections) {


        painter.save();

        painter.setPen(progressionColor);

        current = 0.0;                              /* angle of next planet to be drawn         */
        previous = 0.0;                             /* angle of previous planet that was drawn  */
        currentIndex = 0;                           /* index of next planet to be drawn         */
        previousIndex = -1;                         /* index of previous planet drawn           */
        ri = 95.0;                                  /* radius of planet shifted towards center  */
        ra = 99.0;                                  /* radius of planet shifted outwards        */
        r0 = ri;                                    /* standard radius for planet symbol        */
        shift =0;                                   /* wether last planet was shifted inwards (-1),
                                                       outwards (+1) or not shifted at all (0). */
        int i = 0;
        while (i <= h->getLastPlanet()) {               /* draw planet symbols in ascending order of
                                                        their angle                             */
            float candidate = 360;

            for (int k = 0; k <= h->getLastPlanet(); k++) {
                if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                    if (sd->planet[k] > current) {       /* o.k., here is a candidate                */
                        if (sd->planet[k] < candidate) {
                            candidate = sd->planet[k];
                            currentIndex = k;
                        }
                    }
                }
            }

            /* If we found the same planet once again, we are done!                             */
            if (currentIndex == previousIndex) break;

            previous = current;
            previousIndex = currentIndex;
            current = sd->planet[currentIndex];
            if (current - previous < 5) {           /* these symbols would overlap              */
                if (shift == 0) {
                    shift = 1;
                    r0 = ra;                        /* shift planet symbol outwards             */
                } else if (shift == 1) {
                    shift = 0;
                    r0 = ri;                        /* draw planet symbol at normal position    */
                }
            } else {
                shift = 0;
                r0 = ri;
            }
            alpha = current - h->cusp[1];
            // qDebug ("alpha = %f", alpha);
            alphaRad = alpha*PI/180.0;
            x = -r0 * cos (alphaRad) -4;
            y = r0 * sin (alphaRad)+4;
            QPointF planetPosition(x, y);
            painter.drawText(planetPosition, planetLetters[currentIndex]);

            /* draw small 'r' for retrograde planets    */
            if (sd->retrograde[currentIndex]) {
                painter.setFont(defaultFontSmall);
                QPointF rPosition(x - abs(2*sin(alphaRad)) +7, y);
                painter.drawText(rPosition, "r");
                painter.setFont(astroFontMedium);
            }

        }
        painter.restore();
    }


    if (radix || synastrie || composite || combine || solar) {
        r0 = 93;                                    /* draw "MC" at tip of 10th house           */
        alpha = h->cusp[10] - h->cusp[1];
        if (alpha < 0) alpha += 360;
        // qDebug ("alpha = %f", alpha);
        alphaRad = alpha*PI/180.0;
        x = -r0 * cos (alphaRad);
        y = r0 * ( sin (alphaRad) - 0.03* cos (alphaRad));
        QPointF MCPosition(x, y);
        painter.drawText(MCPosition, "b");

        r0 = 52;                                    /* draw house numbers                       */
        painter.setFont(defaultFontSmall);
        for (int i=1; i< 13; i++) {
            alpha = h->cusp[i] - h->cusp[1]+3;
            alphaRad = alpha*PI/180.0;
            x = -r0 * cos (alphaRad)-2;
            y = r0 * sin (alphaRad)+2;
            if ((i == 11) || (i == 12)) {
                x-=1; y -=1;
            }
            if ((i == 2) || (i == 3)) {
                x+=1; y -=1;
            }
            if (i == 6) {
                x-=1;
            }
            QPointF p(x, y);
            QString num;
            if ((i!=1) && (i!=4) && (i!=7) && (i!=10)) {
                painter.drawText(p, num.setNum(i));
            }
        }
    }

    if (synastrie || solar) {                       /* draw house numbers for hh                */
        if (solar) {
            painter.setPen(itemColor2);
        }
        for (int i=1; i< 13; i++) {
            if (i==1) {
                r0 = 94;
                alpha = hh->cusp[i] - h->cusp[1];
                if ((alpha > 270) || (alpha < 90)) {
                    alpha -= 2;
                } else {
                    alpha += 2;
                }
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad)-3;
                y = r0 * sin (alphaRad)+1;
                QPointF p(x, y);
                QString num;
                painter.setFont(astroFontSmall);           /* draw "ASC" at tip of AC - DC axis        */
                painter.drawText(p, "a");
            } else if (i==10) {
                r0 = 94;
                alpha = hh->cusp[i] - h->cusp[1];
                if ((alpha > 270) || (alpha < 90)) {
                    alpha -= 2;
                } else {
                    alpha += 2;
                }
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad)-3;
                y = r0 * sin (alphaRad)+1;
                QPointF p(x, y);
                painter.setFont(astroFontSmall);
                painter.drawText(p, "b");
            } else {
                r0 = 37;
                alpha = hh->cusp[i] - h->cusp[1]+4;
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad)-1;
                y = r0 * sin (alphaRad)+1;
                QPointF p(x, y);
                QString num;
                painter.setFont(defaultFontSmall);
                painter.drawText(p, num.setNum(i));
            }
        }
        if (solar) {
            painter.setPen(itemColor);
        }
    }

    if (this->transitsOnly) {
        pen.setWidth(0);                 /* center point                              */
        pen.setBrush(wheelColor);
        painter.setPen(pen);
        painter.drawEllipse (center, 1, 1);
    }

    if (synastrie || solar)  {
        r0 = 35;
        if (synastriePlanetaryAspects) {
            // drawSynastrieAspectRange (0, s->noOfAspects,  r0, pen, painter);

            for (int i=0; i < s->noOfAspects; i++) {    /* draw aspects                   */
                float x1, y1, x2, y2;
                QColor color;
                alpha = s->aspectList[i]->p1 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x1 = -r0 * cos (alphaRad);
                y1 = r0 * sin (alphaRad);
                alpha = s->aspectList[i]->p2 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x2 = -r0 * cos (alphaRad);
                y2 = r0 * sin (alphaRad);
                bool mainAspect = TRUE;
                switch (s->aspectList[i]->angle) {
                    case 0:
                    case 90:
                    case 180:
                        color = aspectColorRed;
                        break;
                    case 60:
                    case 120:
                        color = aspectColorBlue;
                        break;
                    case 30:
                    case 150:
                        color = aspectColorGreen;
                        mainAspect = FALSE;
                        break;
                    default:
                        color = wheelColor;
                        mainAspect = FALSE;
                        break;
                }
                float quot = 10*(s->aspectList[i]->deviation/s->aspectList[i]->orbis);
                // qDebug ("i = %d, quot = %f", i, quot);
                if ((quot <3) && (mainAspect == TRUE)) {
                    pen.setWidthF(0.8);
                } else if (quot <5) {
                    pen.setWidthF(0.5);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                } else {
                    pen.setWidthF(0.3);
                    if (side < 300) {
                        pen.setWidthF(0.2);
                    }
                }
                /* for now, do not show semi-squares, quintiles and biquintiles      */
                if ((s->aspectList[i]->angle == 45)
                    || (s->aspectList[i]->angle == 72)
                    || (s->aspectList[i]->angle == 144)) {
                    color.setAlpha(0);
                }
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                painter.setPen(pen);

                painter.drawLine(x1, y1, x2, y2);
            }

        }
        if (synastrieAxisAspects) {
            // drawSynastrieAspectRange (MAX_SYNASTRIE_PLANETARY_ASPECTS,
            // MAX_SYNASTRIE_PLANETARY_ASPECTS + s->noOfAxisAspects, r0, pen, painter);
            // qDebug() << "no of axis aspects = " << s->noOfAxisAspects;
            int boundary = MAX_SYNASTRIE_PLANETARY_ASPECTS + s->noOfAxisAspects;
            for (int i=MAX_SYNASTRIE_PLANETARY_ASPECTS; i < boundary; i++) {    /* draw aspects                   */
                float x1, y1, x2, y2;
                QColor color;
                alpha = s->aspectList[i]->p1 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x1 = -r0 * cos (alphaRad);
                y1 = r0 * sin (alphaRad);
                alpha = s->aspectList[i]->p2 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x2 = -r0 * cos (alphaRad);
                y2 = r0 * sin (alphaRad);
                bool mainAspect = TRUE;
                switch (s->aspectList[i]->angle) {
                    case 0:
                    case 90:
                    case 180:
                        color = aspectColorRed;
                        break;
                    case 60:
                    case 120:
                        color = aspectColorBlue;
                        break;
                    case 30:
                    case 150:
                        color = aspectColorGreen;
                        mainAspect = FALSE;
                        break;
                    default:
                        color = wheelColor;
                        mainAspect = FALSE;
                        break;
                }
                float quot = 10*(s->aspectList[i]->deviation/s->aspectList[i]->orbis);
                // qDebug ("i = %d, quot = %f", i, quot);
                if ((quot <3) && (mainAspect == TRUE)){
                    // color.setAlpha(255);
                    pen.setWidthF(1.2);
                } else if (quot <5) {
                    // color.setAlpha(160);
                    pen.setWidthF(0.8);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                } else {
                    // color.setAlpha(80);
                    pen.setWidthF(0.5);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                }
                /* for now, do not show semi-squares, quintiles and biquintiles      */
                if ((s->aspectList[i]->angle == 45)
                    || (s->aspectList[i]->angle == 72)
                    || (s->aspectList[i]->angle == 144)) {
                    color.setAlpha(0);
                }
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                painter.setPen(pen);

                painter.drawLine(x1, y1, x2, y2);
            }
        }
        if (synastrieAxisReverse) {
            // drawSynastrieAspectRange (MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS,
            // MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS + s->noOfReverseAxisAspects,
            // r0, pen, painter);

            // qDebug() << "no of reverse axis aspects = " << s->noOfReverseAxisAspects;
            int lowerBoundary = MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS;
            int upperBoundary = MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS
                                + s->noOfReverseAxisAspects;
            for (int i=lowerBoundary; i < upperBoundary; i++) {    /* draw aspects                   */
                float x1, y1, x2, y2;
                QColor color;
                alpha = s->aspectList[i]->p1 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x1 = -r0 * cos (alphaRad);
                y1 = r0 * sin (alphaRad);
                alpha = s->aspectList[i]->p2 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x2 = -r0 * cos (alphaRad);
                y2 = r0 * sin (alphaRad);
                bool mainAspect = TRUE;
                switch (s->aspectList[i]->angle) {
                    case 0:
                    case 90:
                    case 180:
                        color = aspectColorRed;
                        break;
                    case 60:
                    case 120:
                        color = aspectColorBlue;
                        break;
                    case 30:
                    case 150:
                        color = aspectColorGreen;
                        mainAspect = FALSE;
                        break;
                    default:
                        color = wheelColor;
                        mainAspect = FALSE;
                        break;
                }
                float quot = 10*(s->aspectList[i]->deviation/s->aspectList[i]->orbis);
                // qDebug ("i = %d, quot = %f", i, quot);
                if ((quot <3) && (mainAspect == TRUE)){
                    // color.setAlpha(255);
                    pen.setWidthF(1.2);
                } else if (quot <5) {
                    // color.setAlpha(160);
                    pen.setWidthF(0.8);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                } else {
                    // color.setAlpha(80);
                    pen.setWidthF(0.5);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                }
                /* for now, do not show semi-squares, quintiles and biquintiles      */
                if ((s->aspectList[i]->angle == 45)
                    || (s->aspectList[i]->angle == 72)
                    || (s->aspectList[i]->angle == 144)) {
                    color.setAlpha(0);
                }
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                painter.setPen(pen);
                painter.drawLine(x1, y1, x2, y2);
            }
        }

    }

    if (radixAspects || composite || combine ) {

        r0 = 50;

        for (int i=0; i < h->noOfAspects; i++) {    /* draw aspects                   */
            float x1, y1, x2, y2;
            QColor color;
            alpha = h->aspectList[i]->p1 - h->cusp[1];
            alphaRad = alpha*PI/180.0;
            x1 = -r0 * cos (alphaRad);
            y1 = r0 * sin (alphaRad);
            alpha = h->aspectList[i]->p2 - h->cusp[1];
            alphaRad = alpha*PI/180.0;
            x2 = -r0 * cos (alphaRad);
            y2 = r0 * sin (alphaRad);
            bool mainAspect = TRUE;
            switch (h->aspectList[i]->angle) {
                case 0:
                case 90:
                case 180:
                    color = aspectColorRed;
                    break;
                case 60:
                case 120:
                    color = aspectColorBlue;
                    break;
                case 30:
                case 150:
                    color = aspectColorGreen;
                    mainAspect = FALSE;
                    break;
                default:
                    color = wheelColor;
                    mainAspect = FALSE;
                    break;
            }
            float quot = 10*(h->aspectList[i]->deviation/h->aspectList[i]->orbis);
            // qDebug ("i = %d, quot = %f", i, quot);
            if ((quot <3) && (mainAspect == TRUE)){
                pen.setWidthF(0.8);
            } else if (quot <5) {
                pen.setWidthF(0.4);
                if (side < 300) {
                    pen.setWidth(0);
                }
            } else {
                pen.setWidthF(0.2);
                if (side < 300) {
                    pen.setWidth(0);
                }
            }
            /* for now, do not show semi-squares, quintiles and biquintiles      */
            if ((h->aspectList[i]->angle == 45)
                || (h->aspectList[i]->angle == 72)
                || (h->aspectList[i]->angle == 144)) {
                color.setAlpha(0);
            }
            pen.setColor(color);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(pen);

            if (!transitsOnly) {
                painter.drawLine(x1, y1, x2, y2);
            }
        }
    }

    painter.restore();

    return (yPos + 2*r);
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::paintEvent(QPaintEvent *evt) {

    int yPos = 0;

    printing = false;

    /* if mobile UI is activated, draw small wheel and return                                   */
    // if (mobileUI) {
    //    paintSmallWheel (evt);
    //    return;
    //}

    /* create painter and select paint device                                                   */
    QPainter painter;
    painter.begin(this);

    this->paintHoroscope(painter, yPos);

    painter.end();
}

/*----------------------------------------------------------------------------------------------*/


void RadixWheel::print() {

    #if !defined(Q_OS_ANDROID)      // No printing on Android !

    int margin = 10;
    int xPos = margin;
    int yPos = margin;

    int k;      /*' y-position counter                  */

    printing = true;

    /* create printer dialog                                                                    */
    QPrintDialog dialog (&printer, this);
    dialog.setOption(QAbstractPrintDialog::PrintToFile, TRUE);
    dialog.setWindowTitle(tr("Drucke Horoskopgrafik"));

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    /* create painter                                                                           */
    QPainter painter;

    /* set proper colors for printing                                                           */
    wheelColor = QColor(0, 0, 0);
    itemColor = QColor(0, 0, 0);
    itemColor2 = QColor(0, 0, 0);

    painter.begin(&printer);

    /* get metrics of print surface                                                             */
    int pWidth = printer.width();

    /* get font metrics of heading font                                                         */
    painter.setFont(fontHeading);
    QFontMetrics fm = painter.fontMetrics();
    int lh = fm.lineSpacing();      /* line height                                              */
    int dh = lh - fm.ascent()/2;    /* correction required to place text properly in box        */

    /* draw box around heading                                                                  */
    QPen p1;
    p1.setWidth(1);
    painter.setPen(p1);
    painter.drawLine(margin, yPos, pWidth -margin, yPos);
    painter.drawLine(margin, yPos, margin, yPos + 2*lh);
    painter.drawLine(pWidth -margin, yPos, pWidth -margin, yPos + 2*lh);

    yPos += 2*lh;
    int logoStart = yPos;

    painter.drawLine(margin, yPos, pWidth -margin, yPos);

    /* draw heading using info from horoscopes                                                  */
    QString heading;

    /* print type of horoscope                                                                  */
    if (transitsOnly) {
        heading.append (h1->name);
    } else if (radix) {
        heading.append("Horoskop von ");
        heading.append (h1->name);
    } else if (composite) {
        heading.append("Composit von ");
        heading.append (h1->name);
        heading.append(" und ");
        heading.append(h3->name);
    } else if (combine) {
        heading.append("Combin von ");
        heading.append (h1->name);
        heading.append(" und ");
        heading.append(h4->name);
    } else if (synastrie) {
        heading.append("Synastriehoroskop von ");
        heading.append (h1->name);
        heading.append(" und ");
        heading.append(h2->name);
    } else if (solar) {
        heading.append("Solar ");
        heading.append(QString("").number(sol->getYear()));
        heading.append(" von ");
        heading.append (h1->name);
    }


    /* print heading text                                                                       */
    int textWidth = fm.width(heading);
    xPos = (pWidth - textWidth)/2;
    yPos -= dh; /* position text in box                                                         */
    painter.drawText(xPos, yPos, heading);
    yPos += dh; /* prepare to draw line                                                         */

    /* switch to smaller font for horoscope info                                                */
    painter.setFont(fontInfo);
    fm = painter.fontMetrics();
    lh = fm.lineSpacing();
    dh = lh - fm.ascent()/2;

    int stop1 = margin + (pWidth - 2*margin)*0.25;
    int stop2 = margin + (pWidth - 2*margin)*0.5;
    int stop3 = margin + (pWidth - 2*margin)*0.75;
    int stop4 = pWidth - margin;

    /* draw info table                                                                          */
    painter.drawLine(margin, yPos, margin, yPos + 6*lh);            /* erste Senkrechte         */
    if (radix) {
        painter.drawLine(stop1, yPos + 4*lh, stop1, yPos + 6*lh);   /* 2. Senkrechte            */
    } else {
        painter.drawLine(stop1, yPos, stop1, yPos + 6*lh);          /* dito                     */
    }
    painter.drawLine(stop2, yPos, stop2, yPos + 6*lh);              /* 3. Senkrechte            */
    painter.drawLine(stop3, yPos, stop3, yPos + 6*lh);              /* 4. Senkrechte            */
    painter.drawLine(stop4, yPos, stop4, yPos + 6*lh);              /* 5. Senkrechte            */
    yPos += 2*lh;
    painter.drawLine(margin, yPos, stop3, yPos);                    /* 2. Waagerechte           */
    yPos += 2*lh;
    painter.drawLine(margin, yPos, stop3, yPos);                    /* 3. Waagerechte           */
    yPos += 2*lh;
    painter.drawLine(margin, yPos, pWidth-margin, yPos);            /* 4. Waagerechte           */

    /* fill info table                                                                          */
    yPos -= (4*lh + dh);
    xPos = margin + 10;

    /* 1st row, 1st cell :                                                                      */
    QString info;
    if (transitsOnly) {
        info.append("Datum  :  ");
        QString sdate = h1->getDateTime().date().toString(Qt::DefaultLocaleLongDate);
        info.append(sdate);
    } else if (radix) {
        info.append("geboren am  :  ");
        QString sdate = h1->getDateTime().date().toString(Qt::DefaultLocaleLongDate);
        info.append(sdate);
    } else {
        info.append(h1->name);   /* name                                                        */
    }

    painter.drawText(xPos, yPos, info);

    xPos = stop1 + 10;      /* 1st row, 2nd cell                                                */
    if (!radix){
         /* not used with radix                                                                 */
        info = h1->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        info.append(", ");
        info.append(h1->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    }

    xPos = stop2 + 10;      /* 1st row, 3rd cell                                                */
    if (transitsOnly) {
        /* show locations length for transits                                                      */
        float angle = h1->longitude;
        int degrees = (int)angle;
        int minutes = (int)60*(angle - degrees);
        QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
        info = "Breite : " + ang;
        painter.drawText(xPos, yPos, info);
    } else if (radix) {
        /* show locations length for radix                                                      */
        info = QString("Länge : %1e%2").arg(panel->spinLongitudeDeg->value())
               .arg(panel->spinLongitudeMin->value(), 2, 10, QChar('0'));
        painter.drawText(xPos, yPos, info);
    } else if (solar) {
        info = this->sol->h->location;
        painter.drawText(xPos, yPos, info);
    } else {
        /* show first horoscopes location name for all horoscopes except radix                  */
        info = this->panel->lineEditLocation->text();
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(h1->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    }

    yPos += 2*lh;          /* advance to next table row                                         */
    xPos = margin + 10;


    if (radix) {
        /* show location for radix                                                              */
        info = "in  :  ";
        QString locus = this->h1->location;
        info.append(locus);
        info.append(", ");
        info.append(h1->getCountryStringShort());
    } else if (solar) {
        /* show "Solar" for solar horoscope                                                     */
        info = "Solar";
    } else if (synastrie) {
        /* for all others, show name of 2nd person                                              */
        info = h2->name;   /* name                                                              */
    } else if (composite) {
        info = h3->name;
    } else if (combine) {
        info = h4->name;
    }
    painter.drawText(xPos, yPos, info);

    xPos = stop1 + 10;  /* not used with radix                                                  */
    if (synastrie) {
        /* show 2nd horoscopes date for synastrie                                               */
        info = h2->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        info.append(", ");
        info.append(h2->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    } else if (solar) {
        /* show solar date                                                                       */
        info = sol->h->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        info.append(", ");
        info.append(sol->h->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    } else if (composite) {
        info = h3->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        info.append(", ");
        info.append(h3->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    } else if (combine) {
        info = h4->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        info.append(", ");
        info.append(h4->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    }

    xPos = stop2 + 10;

    if (transitsOnly) {
        float angle = h1->latitude;
        int degrees = (int)angle;
        int minutes = (int)60*(angle - degrees);
        QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
        info = "Breite : " + ang;
        painter.drawText(xPos, yPos, info);
    } else if (radix) {
        info = QString("Breite : %1n%2").arg(panel->spinLatitudeDeg->value())
               .arg(panel->spinLatitudeMin->value(), 2, 10, QChar('0'));
        painter.drawText(xPos, yPos, info);
    } else if (synastrie) {
        /* print location of 2nd horoscope                                                      */
        info = h2->location;
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(h2->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    }  else if (solar) {
        /* get location of solar from form                                                      */
        info = this->panel->lineEditSolLocation->text();
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(sol->h->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    } else if (composite) {
        info = h3->location;
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(h3->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    } else if (combine) {
        info = h4->location;
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(h4->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    }

    yPos += 2*lh;           /* advance to next table row                                        */
    xPos = margin + 10;

    if (showTransits) {
        /* 3rd row for transit data                                                             */
        info = QString("Transite");
    } else if (showSecondaryDirections) {
        /* 3rd row for progression data                                                         */
        info = QString("Sekundärdirektionen");
    } else {
        if (radix || synastrie) {
            QTime t1 = h->getDateTime().time();
            info = QString("Uhrzeit : %1:%2").arg(t1.hour()).arg(t1.minute(), 2, 10, QChar('0'));
        } else if (solar) {
            QTime t1 = hh->getDateTime().time();
            info = QString("Solarzeit : %1:%2").arg(t1.hour()).arg(t1.minute(), 2, 10, QChar('0'));
        } else if (composite) {
            info = "Schnittpunktmethode";
        } else if (combine) {
            info = "nicht korrigiert";
        }
    }
    painter.drawText(xPos, yPos, info);

    xPos = stop1 +10;       /* advance to next table cell                                       */

    if (showTransits) {
        /* show transit date                                                                    */
        info = t->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        info.append(", ");
        info.append(t->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    } else if (showSecondaryDirections) {
        info = this->panel->transitDateEdit->date().toString(Qt::DefaultLocaleShortDate);
        // info = sd->getDateTime().date().toString(Qt::DefaultLocaleShortDate);
        // info.append(", ");
        // info.append(sd->getDateTime().time().toString(Qt::DefaultLocaleShortDate));
        painter.drawText(xPos, yPos, info);
    } else {
        if (radix || synastrie ) {
            QTime t1 = h->convertToUTC (h->getDateTime(), h->getCountryCode(), h->getSubCode()).time();
            info = QString("Weltzeit : %1:%2").arg(t1.hour()).arg(t1.minute(), 2, 10, QChar('0'));
            painter.drawText(xPos, yPos, info);
        } else if (solar) {
            QTime t1 = hh->convertToUTC (hh->getDateTime(), h->getCountryCode(), h->getSubCode()).time();
            info = QString("Weltzeit : %1:%2").arg(t1.hour()).arg(t1.minute(), 2, 10, QChar('0'));
            painter.drawText(xPos, yPos, info);
        }
    }

    xPos = stop2 +10;       /* advance to next table cell                                       */


    if (showTransits) {
        /* show transit location                                                                */
        info = t->location;
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(t->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    } else if (showSecondaryDirections) {
        /* show progression location                                                            */
        info = sd->location;
        int idx = info.indexOf(" ");
        info.remove(0, idx+1);
        info.append(", ");
        info.append(sd->getCountryStringShort());
        painter.drawText(xPos, yPos, info);
    } else {
        info = "Häuser : Placidus";
        painter.drawText(xPos, yPos, info);
    }

    /* draw Logo                                                                                */
    xPos = stop3 + (stop4 - stop3) / 2 + 15;
    /* save y position                                                                          */
    int yPosOld = yPos;

    yPos = logoStart +10;
    QImage logo ("../img/logo_80.png");
    QPoint pLogo (xPos, yPos);
    painter.drawImage(pLogo, logo);

    xPos = stop3 + margin;
    yPos += 2*lh -dh -10 ;
    info = "created with:";
    painter.setFont(fontTable);
    painter.drawText(xPos, yPos, info);
    yPos += lh + dh;
    painter.setFont(fontHeading);
    info = "JAstro";
    painter.drawText(xPos, yPos, info);
    yPos += 3*lh -dh;
    info = "www.jastro.net";
    painter.setFont(fontTable);
    painter.drawText(xPos, yPos, info);
    painter.setFont(fontInfo);

    xPos = stop3 + 10;

    /* restore y position                                                                       */
    yPos = yPosOld;

    /* shift origin downwards                                                                   */
    yPos += dh;

    /*************************************************/

    /* draw radix                                                                               */
    yPos = paintHoroscope(painter, yPos);

    // yPos = 918;

    qDebug() << "print() after paintHoroscope, yPos = " << yPos;

    /*************************************************/

    /* restore line width, which was modified during drawing of radix                           */
    painter.setPen(p1);

    /* switch to smaller font for planets and houses                                            */
    painter.setFont(fontTable);
    fm = painter.fontMetrics();
    lh = fm.lineSpacing();
    dh = lh - fm.ascent()/2;

    /* draw box for planet and house positions                                                  */
    xPos = margin;
    painter.drawLine(xPos, yPos, stop2, yPos);
    painter.drawLine(xPos, yPos, xPos, yPos + 10*lh + dh);   /* 10 rows                         */
    painter.drawLine(stop2, yPos, stop2, yPos +10*lh + dh);
    painter.drawLine(xPos, yPos +10*lh + dh, stop2, yPos + 10*lh + dh);

    yPos += lh+4;   /* align the following lines within the height of the surrounding box       */
    // yPos += lh;

    /* Planets 1st Horoscope                                                                    */
    painter.setFont(astroFontTable);
    this->n_planets = 10;
    for (int i = 0; i< n_planets; i++) {    /* draw planet symbols                              */
        if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)) {
            QPoint planetPosition (xPos + 10, yPos + i*lh );
            QPoint signPosition (xPos + 50, yPos + i*lh );
            QPoint degPosition (xPos + 70, yPos + i*lh);
            QPoint rPosition (xPos + 124, yPos + i*lh);
            painter.drawText(planetPosition, planetLetters[i]);
            int sidx = (int)((h->planet[i])/30);
            double angle = (h->planet[i]) - sidx*30;
            int degrees = (int)angle;
            int minutes = (int)60*(angle - degrees);
            QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
            painter.drawText(signPosition, signLetters[sidx]);
            painter.setFont(fontTable);
            painter.drawText(degPosition, ang);
            if (h->retrograde[i] == TRUE) {
                fontTable.setPixelSize(10);
                painter.setFont(fontTable);
                painter.drawText(rPosition, "r");
                fontTable.setPixelSize(14);
                painter.setFont(fontTable);
            }
            painter.setFont(astroFontTable);
        }

    }

    xPos = stop1;   /* jump to right column - ypos didn't change during printing of lines       */

    /* remaining Planets and Node 1st Horoscope                                                 */
    for (int i = n_planets; i < N_PLANETS; i++) {    /* draw more planet symbols                */
        if ((i == SE_MEAN_NODE) || (i == SE_MEAN_APOG) || (i == SE_CHIRON)) {
            if (i == SE_MEAN_NODE)  k = 0;
            if (i == SE_MEAN_APOG)  k = 1;
            if (i == SE_CHIRON)     k = 2;
            QPoint planetPosition (xPos + 10, yPos + k*lh );
            QPoint signPosition (xPos + 50, yPos + k*lh );
            QPoint degPosition (xPos + 70, yPos + k*lh);
            painter.drawText(planetPosition, planetLetters[i]);
            int sidx = (int)((h->planet[i])/30);
            double angle = (h->planet[i]) - sidx*30;
            int degrees = (int)angle;
            int minutes = (int)60*(angle - degrees);
            QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
            qDebug() << "sidx = " << sidx;
            qDebug() << "signPosition = " << signPosition;
            painter.drawText(signPosition, signLetters[sidx]);
            painter.setFont(fontTable);
            painter.drawText(degPosition, ang);
            painter.setFont(astroFontTable);
        }
    }

    yPos += 3*lh;   /* houses start at line 3                                                   */

    /* Houses 1st Horoscope                                                                     */
    for (int i = 0; i < 12; i++) {
        k = i;
        if ( (i < 3) || (i > 8)) {
            if (i > 8) k = i-6;
            painter.setFont(fontTable);
            QPoint cuspPosition (xPos + 10, yPos + k*lh );
            QPoint signPosition (xPos + 50, yPos + k*lh );
            QPoint degPosition (xPos + 70, yPos + k*lh);
            QString cuspName;
            if (i==0) {
                cuspName = "AC";
            } else if (i==9) {
                cuspName = "MC";
            } else {
                cuspName = QString("H%1").arg(i+1);
            }
            painter.drawText(cuspPosition, cuspName);
            int sidx = (int)((h->cusp[i+1])/30);
            double angle = (h->cusp[i+1]) - sidx*30;
            int degrees = (int)angle;
            int minutes = (int)60*(angle - degrees);
            QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
            painter.setFont(astroFontTable);
            painter.drawText(signPosition, signLetters[sidx]);
            painter.setFont(fontTable);
            painter.drawText(degPosition, ang);
        }
    }

    /* Box 2nd Horoscope in case we have a synastrie chart or a solar                               */


    if (synastrie || solar) {

        /* jump back to the box origin                                                              */
        yPos -= (4*lh+4);
        xPos = stop2;

        /* draw 3 lines, middle line is already there                                               */
        painter.drawLine(xPos, yPos, stop4, yPos);
        painter.drawLine(stop4, yPos, stop4, yPos +10*lh + dh);
        painter.drawLine(xPos, yPos +10*lh + dh, stop4, yPos + 10*lh + dh);

        /* align the following text within the box                                                  */
        yPos += lh +4;
    }

    /* Planets 2nd Horoscope                                                                        */
    if (synastrie || solar) {
        painter.setFont(astroFontTable);
        for (int i = 0; i< n_planets; i++) {    /* draw planet symbols                              */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)) {
                QPoint planetPosition (xPos + 10, yPos + i*lh );
                QPoint signPosition (xPos + 50, yPos + i*lh );
                QPoint degPosition (xPos + 70, yPos + i*lh);
                painter.drawText(planetPosition, planetLetters[i]);
                int sidx = (int)((hh->planet[i])/30);
                double angle = (hh->planet[i]) - sidx*30;
                int degrees = (int)angle;
                int minutes = (int)60*(angle - degrees);
                QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
                painter.drawText(signPosition, signLetters[sidx]);
                painter.setFont(fontTable);
                painter.drawText(degPosition, ang);
                painter.setFont(astroFontTable);
            }

        }
    }

    /* remaining Planets and Node 2nd Horoscope                                                     */
    xPos = stop3;
    if (synastrie || solar) {

        for (int i = n_planets; i < N_PLANETS; i++) {    /* draw more planet symbols                */
            if ((i == SE_MEAN_NODE) || (i == SE_MEAN_APOG) || (i == SE_CHIRON)) {
                if (i == SE_MEAN_NODE)  k = 0;
                if (i == SE_MEAN_APOG)  k = 1;
                if (i == SE_CHIRON)     k = 2;
                QPoint planetPosition (xPos + 10, yPos + k*lh );
                QPoint signPosition (xPos + 50, yPos + k*lh );
                QPoint degPosition (xPos + 70, yPos + k*lh);
                painter.drawText(planetPosition, planetLetters[i]);
                int sidx = (int)((hh->planet[i])/30);
                double angle = (hh->planet[i]) - sidx*30;
                int degrees = (int)angle;
                int minutes = (int)60*(angle - degrees);
                QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
                painter.drawText(signPosition, signLetters[sidx]);
                painter.setFont(fontTable);
                painter.drawText(degPosition, ang);
                painter.setFont(astroFontTable);
            }
        }
    }

    yPos += 3*lh;

    /* Houses 2nd Horoscope                                                                     */
    if (synastrie || solar) {
        for (int i = 0; i < 12; i++) {
            k = i;
            if ( (i < 3) || (i > 8)) {
                if (i > 8) k = i-6;
                painter.setFont(fontTable);
                QPoint cuspPosition (xPos + 10, yPos + k*lh );
                QPoint signPosition (xPos + 50, yPos + k*lh );
                QPoint degPosition (xPos + 70, yPos + k*lh);
                QString cuspName;
                if (i==0) {
                    cuspName = "AC";
                } else if (i==9) {
                    cuspName = "MC";
                } else {
                    cuspName = QString("H%1").arg(i+1);
                }
                painter.drawText(cuspPosition, cuspName);
                int sidx = (int)((hh->cusp[i+1])/30);
                double angle = (hh->cusp[i+1]) - sidx*30;
                int degrees = (int)angle;
                int minutes = (int)60*(angle - degrees);
                QString ang = QString(" %1°%2\"").arg(degrees, 2, 10, QChar(' ')).arg(minutes, 2, 10, QChar('0'));
                painter.setFont(astroFontTable);
                painter.drawText(signPosition, signLetters[sidx]);
                painter.setFont(fontTable);
                painter.drawText(degPosition, ang);
            }
        }
    }

    painter.end();

    /* signal to controller that printing has been finished so screen colors get
       restored according to current theme                                                  */
    finishedPrinting();

    printing = false;

    #endif

}



/*----------------------------------------------------------------------------------------------*/

void RadixWheel::paintSmallWheel (QPaintEvent *evt) {

    /* do not draw anything if there is no data yet !                                           */
    if (!(radix || synastrie || composite || combine || solar)) return;

    this->n_planets = N_PLANETS;

    /* don't show Chiron / Lilith in composite/combine/synastrie                                */
    if (!radix) {
        this->n_planets = 10;
    }

    int side = qMin(width(), height());
    int r = side/2;

    QPointF center (0, 0);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(r, r);                    /* find center of plane                 */
    painter.scale(side / 200.0, side / 200.0);  /* scale to -100 ... +100               */

    QPen pen(wheelColor, 0);
    painter.setPen(pen);

    painter.drawEllipse (center, 90, 90);       /* outer zodiac circle                   */
    painter.drawEllipse (center, 74, 74);       /* inner zodiac circle                   */
    painter.drawEllipse (center, 71, 71);       /* inner scale circle                    */

    if (radix || synastrie || composite || combine) {
        painter.drawEllipse (center, 50, 50);   /* inner house circle                    */
    }

    if (synastrie) {
        painter.drawEllipse (center, 35, 35);   /* inner circle partner houses           */
    }

    if (transitsOnly) {
        painter.drawEllipse (center, 20, 20);   /* center circle                         */
    }

    painter.save();
    if (radix || synastrie || composite || combine) {
        painter.rotate(h->cusp[1] -270.0);     /* get AC to the left                    */
    } else {
        painter.rotate(90.0);                  /* get Aries to the left                 */
    }

    for (int i = 0; i < 12; ++i) {              /* devide into 30 degree sections        */
        painter.drawLine(0, 74, 0, 90);
        painter.rotate(30.0);
    }

    for (int i = 0; i < 36; ++i) {             /* draw decanates                        */
        painter.drawLine(0, 71, 0, 74);
        painter.rotate(10.0);
    }

    for (int j = 0; j < 360; ++j) {            /* degree scale                          */
        if (j%5 == 0) {
            painter.drawLine(0, 71, 0, 73);
        } else {
            painter.drawLine(0, 71, 0, 72);
        }
        painter.rotate(1.0);
    }

    pen.setColor(itemColor);
    painter.setPen(pen);
    painter.setFont(astroFontHuge);             /* use huge font for zodiac signs           */

    painter.save();
    painter.rotate(-15.0);                      /* draw into center of sign segment         */

    QPointF p1 (-7, -76);

    for (int i = 0; i < 6; ++i) {               /* draw zodiac signs from libra to pisces   */
        painter.drawText ( p1, signLetters[i+6]);
        painter.rotate(-30.0);
    }
    for (int i = 6; i < 12; ++i) {             /* draw zodiac signs from aries to virgo     */
        painter.drawText ( p1, signLetters[i-6]);
        painter.rotate(-30.0);
    }

    painter.restore();                          /* back 15 degrees                          */
    painter.restore();                          /* x -axis is horizont again                */

    painter.setFont(astroFontMedium);

    if (radix || synastrie || composite || combine) {
        painter.drawLine(-99, 0, -90, 0);           /* draw AC - DC axis                        */
        painter.drawLine( 99, 0,  90, 0);

        if (transitsOnly) {
            painter.drawLine( 71, 0,  20, 0);
            painter.drawLine(-71, 0, -20, 0);
        } else {
            painter.drawLine( 71, 0,  50, 0);
            painter.drawLine(-71, 0, -50, 0);
        }

        QPointF AC (-99.0, -1.0);                   /* draw "ASC" at tip of AC - DC axis        */
        painter.drawText(AC, "a");

        painter.save();
        painter.rotate(h->cusp[1] - h->cusp[10]);   /* rotate back to MC                        */
        painter.drawLine(-99, 0, -90, 0);           /* draw MC - IC axis                        */
        painter.drawLine( 99, 0,  90, 0);

        if (transitsOnly) {
            painter.drawLine( 71, 0,  20, 0);
            painter.drawLine(-71, 0, -20, 0);
        } else {
            painter.drawLine( 71, 0,  50, 0);
            painter.drawLine(-71, 0, -50, 0);
        }

        painter.restore();

        for (int i = 2; i< 7; i++) {                /* draw remaining houses                    */
            if (i != 4) {
                painter.save();
                                                    /* rotate to house cusp                     */
                painter.rotate(h->cusp[1] - h->cusp[i]);
                painter.drawLine(-71, 0, -50, 0);
                painter.drawLine( 71, 0,  50, 0);
                painter.restore();
            }
        }
    }

    /* draw houses of 2nd horoscope in the inner circle                                     */
    if (synastrie) {
        for (int i = 1; i< 7; i++) {            /* draw remaining houses                    */
            painter.save();
                                                /* rotate to house cusp                     */
            painter.rotate(h->cusp[1] - hh->cusp[i]);
            painter.drawLine(-50, 0, -35, 0);
            painter.drawLine( 50, 0,  35, 0);
            if ((i == 1) || (i == 4)) {         /* draw AC / DC / IC / MC - axis outside    */
                painter.drawLine(-99, 0, -90, 0);
                painter.drawLine( 99, 0,  90, 0);
            }
            painter.restore();
        }
    }


    if (radix || synastrie || composite || combine) {
        for (int i = 0; i < n_planets; i++) {        /* draw markers for planet positions        */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                painter.rotate(h->cusp[1] - h->planet[i]);
                painter.drawLine( -52, 0,  -50, 0);     /* marker on inner house circle             */
                painter.drawLine( -71, 0,  -69, 0);     /* marker on outer house circle             */
                painter.restore();
            }
        }
    }

    if (synastrie) {
        for (int i = 0; i < n_planets; i++) {          /* draw markers for partner planets     */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                pen.setColor(Qt::red);
                painter.setPen(pen);
                painter.rotate(h->cusp[1] - hh->planet[i]);
                painter.drawLine( -36, 0,  -35, 0);     /* marker on inner house circle         */
                painter.drawLine( -49, 0,  -50, 0);     /* marker on outer house circle         */
                painter.restore();
            }
        }
    }

    if (this->showTransits) {
        for (int i = 0; i < n_planets; i++) {          /* draw markers for transit planets         */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                painter.save();
                pen.setColor(Qt::cyan);
                painter.setPen(pen);
                painter.rotate(h->cusp[1] - t->planet[i]);
                painter.drawLine( -71, 0,  -69, 0); /* marker on outer house circle             */
                painter.drawLine( -92, 0,  -90, 0); /* marker on outer sign circle              */
                painter.restore();
            }
        }
    }



    float x, y, alpha, alphaRad;
    float current = 0.0;                        /* angle of next planet to be drawn         */
    float previous = 0.0;                       /* angle of previous planet that was drawn  */
    int currentIndex = 0;                       /* index of next planet to be drawn         */
    float ri = 57.0;                            /* radius of planet shifted towards center  */
    float ra = 67.0;                            /* radius of planet shifted outwards        */
    float r0 = ri;                              /* standard radius for planet symbol        */
    int shift =0;                               /* wether last planet was shifted inwards (-1),
                                                   outwards (+1) or not shifted at all (0). */
    if (radix || synastrie || composite || combine) {
        for (int i = 0; i< n_planets; i++) {    /* draw planet symbols in ascending order of
                                                their angle                             */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                float candidate = 360;

                for (int k = 0; k< n_planets; k++) {
                    if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                        if (h->planet[k] > current) {       /* o.k., here is a candidate                */
                            if (h->planet[k] < candidate) {
                                candidate = h->planet[k];
                                currentIndex = k;
                            }
                        }
                    }
                }
                previous = current;
                current = h->planet[currentIndex];
                if (current - previous < 5) {
                    if (shift == 0) {
                        shift = 1;
                        r0 = ra;                        /* shift planet symbol outwards             */
                    } else if (shift == 1) {
                        shift = 0;
                        r0 = ri;                        /* draw planet symbol at normal position    */
                    }
                } else {
                    shift = 0;
                    r0 = ri;
                }
                alpha = current - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad) -4;
                y = r0 * sin (alphaRad)+4;
                QPointF planetPosition(x, y);
                painter.drawText(planetPosition, planetLetters[currentIndex]);
                // qDebug () << "planet idx " << currentIndex;
            }
        }
    }

    if (synastrie) {
        current = 0.0;                              /* angle of next planet to be drawn         */
        previous = 0.0;                             /* angle of previous planet that was drawn  */
        currentIndex = 0;                           /* index of next planet to be drawn         */
        ri = 40.0;                                  /* radius of planet shifted towards center  */
        ra = 46.0;                                  /* radius of planet shifted outwards        */
        r0 = ri;                                    /* standard radius for planet symbol        */
        shift =0;                                   /* wether last planet was shifted inwards (-1),
                                                       outwards (+1) or not shifted at all (0). */
        for (int i = 0; i<n_planets; i++) {         /* draw planet symbols in ascending order of
                                                        their angle                             */
            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){
                float candidate = 360;

                for (int k = 0; k< n_planets; k++) {
                    if (hh->planet[k] > current) {       /* o.k., here is a candidate                */
                        if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                            if (hh->planet[k] < candidate) {
                                candidate = hh->planet[k];
                                currentIndex = k;
                            }
                        }
                    }
                }
                previous = current;
                current = hh->planet[currentIndex];
                if (current - previous < 5) {           /* these symbols would overlap              */
                    if (shift == 0) {
                        shift = 1;
                        r0 = ra;                        /* shift planet symbol outwards             */
                    } else if (shift == 1) {
                        shift = 0;
                        r0 = ri;                        /* draw planet symbol at normal position    */
                    }
                } else {
                    shift = 0;
                    r0 = ri;
                }
                alpha = current - h->cusp[1];
                // qDebug ("alpha = %f", alpha);
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad) -4;
                y = r0 * sin (alphaRad)+4;
                QPointF planetPosition(x, y);
                painter.drawText(planetPosition, planetLetters[currentIndex]);
            }
        }
    }

    if (this->showTransits) {
        current = 0.0;                              /* angle of next planet to be drawn         */
        previous = 0.0;                             /* angle of previous planet that was drawn  */
        currentIndex = 0;                           /* index of next planet to be drawn         */
        ri = 95.0;                                  /* radius of planet shifted towards center  */
        ra = 99.0;                                  /* radius of planet shifted outwards        */
        r0 = ri;                                    /* standard radius for planet symbol        */
        shift =0;                                   /* wether last planet was shifted inwards (-1),
                                                       outwards (+1) or not shifted at all (0). */
        for (int i = 0; i<n_planets; i++) {         /* draw planet symbols in ascending order of
                                                        their angle                             */
            float candidate = 360;

            if ((i != SE_TRUE_NODE) && (i != SE_OSCU_APOG) && (i != SE_EARTH)){

                for (int k = 0; k< n_planets; k++) {
                    if ((k != SE_TRUE_NODE) && (k != SE_OSCU_APOG) && (k != SE_EARTH)){
                        if (t->planet[k] > current) {       /* o.k., here is a candidate                */
                            if (t->planet[k] < candidate) {
                                candidate = t->planet[k];
                                currentIndex = k;
                            }
                        }
                    }
                }
                previous = current;
                current = t->planet[currentIndex];
                if (current - previous < 5) {           /* these symbols would overlap              */
                    if (shift == 0) {
                        shift = 1;
                        r0 = ra;                        /* shift planet symbol outwards             */
                    } else if (shift == 1) {
                        shift = 0;
                        r0 = ri;                        /* draw planet symbol at normal position    */
                    }
                } else {
                    shift = 0;
                    r0 = ri;
                }
                alpha = current - h->cusp[1];
                // qDebug ("alpha = %f", alpha);
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad) -4;
                y = r0 * sin (alphaRad)+4;
                QPointF planetPosition(x, y);
                painter.drawText(planetPosition, planetLetters[currentIndex]);
            }
        }
    }

    if (radix || synastrie || composite || combine) {
        r0 = 98;                                    /* draw "MC" at tip of 10th house           */
        alpha = h->cusp[10] - h->cusp[1];
        // qDebug ("alpha = %f", alpha);
        alphaRad = alpha*PI/180.0;
        x = -r0 * cos (alphaRad) +1;
        y = r0 * sin (alphaRad)+4;
        QPointF MCPosition(x, y);
        painter.drawText(MCPosition, "b");

        r0 = 54;                                    /* draw house numbers                       */
        painter.setFont(defaultFontSmall);
        for (int i=1; i< 13; i++) {
            alpha = h->cusp[i] - h->cusp[1]+3;
            alphaRad = alpha*PI/180.0;
            x = -r0 * cos (alphaRad)-2;
            y = r0 * sin (alphaRad)+2;
            QPointF p(x, y);
            QString num;
            if ((i!=1) && (i!=4) && (i!=7) && (i!=10)) {
                painter.drawText(p, num.setNum(i));
            }
        }
    }

    if (synastrie) {                            /* draw house numbers for hh                */
        for (int i=1; i< 13; i++) {
            if (i==1) {
                r0 = 96;
                alpha = hh->cusp[i] - h->cusp[1];
                if ((alpha > 270) || (alpha < 90)) {
                    alpha -= 2;
                } else {
                    alpha += 2;
                }
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad)-3;
                y = r0 * sin (alphaRad)+1;
                QPointF p(x, y);
                QString num;
                painter.setFont(astroFontSmall);           /* draw "ASC" at tip of AC - DC axis        */
                painter.drawText(p, "a");
            } else if (i==10) {
                r0 = 96;
                alpha = hh->cusp[i] - h->cusp[1];
                if ((alpha > 270) || (alpha < 90)) {
                    alpha -= 2;
                } else {
                    alpha += 2;
                }
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad)-3;
                y = r0 * sin (alphaRad)+1;
                QPointF p(x, y);
                painter.setFont(astroFontSmall);
                painter.drawText(p, "b");
            } else {
                r0 = 37;
                alpha = hh->cusp[i] - h->cusp[1]+4;
                alphaRad = alpha*PI/180.0;
                x = -r0 * cos (alphaRad)-1;
                y = r0 * sin (alphaRad)+1;
                QPointF p(x, y);
                QString num;
                painter.setFont(defaultFontSmall);
                painter.drawText(p, num.setNum(i));
            }
        }
    }

    if (this->transitsOnly) {
        pen.setWidth(0);                 /* center point                              */
        pen.setBrush(wheelColor);
        painter.setPen(pen);
        painter.drawEllipse (center, 1, 1);
    }

    if (synastrie)  {
        r0 = 35;
        if (synastriePlanetaryAspects) {
            // drawSynastrieAspectRange (0, s->noOfAspects,  r0, pen, painter);

            for (int i=0; i < s->noOfAspects; i++) {    /* draw aspects                   */
                float x1, y1, x2, y2;
                QColor color;
                alpha = s->aspectList[i]->p1 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x1 = -r0 * cos (alphaRad);
                y1 = r0 * sin (alphaRad);
                alpha = s->aspectList[i]->p2 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x2 = -r0 * cos (alphaRad);
                y2 = r0 * sin (alphaRad);
                bool mainAspect = TRUE;
                switch (s->aspectList[i]->angle) {
                    case 0:
                    case 90:
                    case 180:
                        color = aspectColorRed;
                        break;
                    case 60:
                    case 120:
                        color = aspectColorBlue;
                        break;
                    case 30:
                    case 150:
                        color = aspectColorGreen;
                        mainAspect = FALSE;
                        break;
                    default:
                        color = wheelColor;
                        mainAspect = FALSE;
                        break;
                }
                float quot = 10*(s->aspectList[i]->deviation/s->aspectList[i]->orbis);
                // qDebug ("i = %d, quot = %f", i, quot);
                if ((quot <3) && (mainAspect == TRUE)){
                    // color.setAlpha(255);
                    pen.setWidthF(1.2);
                } else if (quot <5) {
                    // color.setAlpha(160);
                    pen.setWidthF(0.8);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                } else {
                    // color.setAlpha(80);
                    pen.setWidthF(0.5);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                }
                /* for now, do not show semi-squares, quintiles and biquintiles      */
                if ((s->aspectList[i]->angle == 45)
                    || (s->aspectList[i]->angle == 72)
                    || (s->aspectList[i]->angle == 144)) {
                    color.setAlpha(0);
                }
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                painter.setPen(pen);

                painter.drawLine(x1, y1, x2, y2);
            }

        }
        if (synastrieAxisAspects) {
            // drawSynastrieAspectRange (MAX_SYNASTRIE_PLANETARY_ASPECTS,
            // MAX_SYNASTRIE_PLANETARY_ASPECTS + s->noOfAxisAspects, r0, pen, painter);
            // qDebug() << "no of axis aspects = " << s->noOfAxisAspects;
            int boundary = MAX_SYNASTRIE_PLANETARY_ASPECTS + s->noOfAxisAspects;
            for (int i=MAX_SYNASTRIE_PLANETARY_ASPECTS; i < boundary; i++) {    /* draw aspects                   */
                float x1, y1, x2, y2;
                QColor color;
                alpha = s->aspectList[i]->p1 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x1 = -r0 * cos (alphaRad);
                y1 = r0 * sin (alphaRad);
                alpha = s->aspectList[i]->p2 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x2 = -r0 * cos (alphaRad);
                y2 = r0 * sin (alphaRad);
                bool mainAspect = TRUE;
                switch (s->aspectList[i]->angle) {
                    case 0:
                    case 90:
                    case 180:
                        color = aspectColorRed;
                        break;
                    case 60:
                    case 120:
                        color = aspectColorBlue;
                        break;
                    case 30:
                    case 150:
                        color = aspectColorGreen;
                        mainAspect = FALSE;
                        break;
                    default:
                        color = wheelColor;
                        mainAspect = FALSE;
                        break;
                }
                float quot = 10*(s->aspectList[i]->deviation/s->aspectList[i]->orbis);
                // qDebug ("i = %d, quot = %f", i, quot);
                if ((quot <3) && (mainAspect == TRUE)){
                    // color.setAlpha(255);
                    pen.setWidthF(1.2);
                } else if (quot <5) {
                    // color.setAlpha(160);
                    pen.setWidthF(0.8);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                } else {
                    // color.setAlpha(80);
                    pen.setWidthF(0.5);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                }
                /* for now, do not show semi-squares, quintiles and biquintiles      */
                if ((s->aspectList[i]->angle == 45)
                    || (s->aspectList[i]->angle == 72)
                    || (s->aspectList[i]->angle == 144)) {
                    color.setAlpha(0);
                }
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                painter.setPen(pen);

                painter.drawLine(x1, y1, x2, y2);
            }
        }
        if (synastrieAxisReverse) {
            // drawSynastrieAspectRange (MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS,
            // MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS + s->noOfReverseAxisAspects,
            // r0, pen, painter);

            // qDebug() << "no of reverse axis aspects = " << s->noOfReverseAxisAspects;
            int lowerBoundary = MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS;
            int upperBoundary = MAX_SYNASTRIE_PLANETARY_ASPECTS + MAX_SYNASTRIE_AXES_ASPECTS
                                + s->noOfReverseAxisAspects;
            for (int i=lowerBoundary; i < upperBoundary; i++) {    /* draw aspects                   */
                float x1, y1, x2, y2;
                QColor color;
                alpha = s->aspectList[i]->p1 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x1 = -r0 * cos (alphaRad);
                y1 = r0 * sin (alphaRad);
                alpha = s->aspectList[i]->p2 - h->cusp[1];
                alphaRad = alpha*PI/180.0;
                x2 = -r0 * cos (alphaRad);
                y2 = r0 * sin (alphaRad);
                bool mainAspect = TRUE;
                switch (s->aspectList[i]->angle) {
                    case 0:
                    case 90:
                    case 180:
                        color = aspectColorRed;
                        break;
                    case 60:
                    case 120:
                        color = aspectColorBlue;
                        break;
                    case 30:
                    case 150:
                        color = aspectColorGreen;
                        mainAspect = FALSE;
                        break;
                    default:
                        color = wheelColor;
                        mainAspect = FALSE;
                        break;
                }
                float quot = 10*(s->aspectList[i]->deviation/s->aspectList[i]->orbis);
                // qDebug ("i = %d, quot = %f", i, quot);
                if ((quot <3) && (mainAspect == TRUE)){
                    // color.setAlpha(255);
                    pen.setWidthF(1.2);
                } else if (quot <5) {
                    // color.setAlpha(160);
                    pen.setWidthF(0.8);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                } else {
                    // color.setAlpha(80);
                    pen.setWidthF(0.5);
                    if (side < 300) {
                        pen.setWidth(0);
                    }
                }
                /* for now, do not show semi-squares, quintiles and biquintiles      */
                if ((s->aspectList[i]->angle == 45)
                    || (s->aspectList[i]->angle == 72)
                    || (s->aspectList[i]->angle == 144)) {
                    color.setAlpha(0);
                }
                pen.setColor(color);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                painter.setPen(pen);
                painter.drawLine(x1, y1, x2, y2);
            }
        }

    }

    if (radixAspects || composite || combine) {

        r0 = 50;

        for (int i=0; i < h->noOfAspects; i++) {    /* draw aspects                   */
            float x1, y1, x2, y2;
            QColor color;
            alpha = h->aspectList[i]->p1 - h->cusp[1];
            alphaRad = alpha*PI/180.0;
            x1 = -r0 * cos (alphaRad);
            y1 = r0 * sin (alphaRad);
            alpha = h->aspectList[i]->p2 - h->cusp[1];
            alphaRad = alpha*PI/180.0;
            x2 = -r0 * cos (alphaRad);
            y2 = r0 * sin (alphaRad);
            bool mainAspect = TRUE;
            switch (h->aspectList[i]->angle) {
                case 0:
                case 90:
                case 180:
                    color = aspectColorRed;
                    break;
                case 60:
                case 120:
                    color = aspectColorBlue;
                    break;
                case 30:
                case 150:
                    color = aspectColorGreen;
                    mainAspect = FALSE;
                    break;
                default:
                    color = wheelColor;
                    mainAspect = FALSE;
                    break;
            }
            float quot = 10*(h->aspectList[i]->deviation/h->aspectList[i]->orbis);
            // qDebug ("i = %d, quot = %f", i, quot);
            if ((quot <3) && (mainAspect == TRUE)){
                // color.setAlpha(255);
                pen.setWidthF(1.2);
            } else if (quot <5) {
                // color.setAlpha(160);
                pen.setWidthF(0.8);
                if (side < 300) {
                    pen.setWidth(0);
                }
            } else {
                // color.setAlpha(80);
                pen.setWidthF(0.5);
                if (side < 300) {
                    pen.setWidth(0);
                }
            }
            /* for now, do not show semi-squares, quintiles and biquintiles      */
            if ((h->aspectList[i]->angle == 45)
                || (h->aspectList[i]->angle == 72)
                || (h->aspectList[i]->angle == 144)) {
                color.setAlpha(0);
            }
            pen.setColor(color);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(pen);

            if (!transitsOnly) {
                painter.drawLine(x1, y1, x2, y2);
            }
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setWheelColor (QColor c) {
    wheelColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setFireElementColor (QColor c) {
    fireElementColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setWaterElementColor (QColor c) {
    waterElementColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setAirElementColor (QColor c) {
    airElementColor = c;
}


/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setEarthElementColor (QColor c) {
    earthElementColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setItemColor (QColor c) {
    itemColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setItemColor2 (QColor c) {
    itemColor2 = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setMarkerColor (QColor c) {
    markerColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setAspectColorBlue (QColor c) {
    aspectColorBlue = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setAspectColorGreen (QColor c) {
    aspectColorGreen = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setAspectColorRed (QColor c) {
    aspectColorRed = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::closeEvent(QCloseEvent *event) {
    emit (wheelClosed());
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::moveEvent ( QMoveEvent * event ) {
    emit (wheelMoved (event));
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::mousePressEvent ( QMouseEvent * event ) {
    emit (wheelClicked(event));
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setTransitsOnly(bool transitsOnly ) {
    this->transitsOnly = transitsOnly;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setTransitColor(QColor c) {
    this->transitColor = c;
}

/*----------------------------------------------------------------------------------------------*/

void RadixWheel::setProgressionColor(QColor c) {
    this->progressionColor = c;
}

/*----------------------------------------------------------------------------------------------*/

RadixWheel::~RadixWheel() {
//
}
