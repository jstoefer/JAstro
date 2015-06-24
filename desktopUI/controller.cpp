#include "controller.h"

Controller::Controller() {

    /* find absolute path in file system                                                        */
    QString appDirPath = QCoreApplication::applicationDirPath ();

    qDebug() << "application working directory = " << appDirPath;


    /* Try to load Astro Font                                                                   */
    int fontHandle;
    QString dataPath, fontPath, ephePath;

    dataPath = ":/data/";

    fontPath = dataPath;
    fontPath.append("Astro-SemiBold.ttf");
    qDebug() << "Loading font" << fontPath;
    fontHandle = QFontDatabase::addApplicationFont (fontPath);

    qDebug() << "dataPath =" << dataPath;

    /* create ephemeris                                                                         */
    ephePath = appDirPath;
    ephePath.append("/../data");
    qDebug () << "ephePath = " << ephePath;
    swe_set_ephe_path(ephePath.toLatin1().data());

    this->h = new Horoscope("Aktuelle Konstellation", QDateTime::currentDateTime(),
                            "Hamburg", 52.77, 9.00);

    /* set partner horoscopes to NULL                                                           */
    this->h2 = NULL;
    this->h3 = NULL;
    this->h4 = NULL;

    /* set transit and secondary directions horoscopes to NULL                                  */
    this->t  = NULL;
    this->sd = NULL;

    /* instantiate location database                                                            */
    QStringList ldbFiles;
    ldbFiles << (dataPath+"orte_de_1.txt");
    this->ldb = new LocationDB (ldbFiles);

    /* instantiate person database                                                              */
    QStringList pdbFiles;
    QString pdataPath = appDirPath.append("/../data/");
    pdbFiles << (pdataPath+"personen_1.aph") << (pdataPath+ "Sternwelten.aph")
            << (pdataPath+"VIP-Philosophen_Weise.aph");
    qDebug() << "pdataPath = " << pdataPath << endl;
    this->pdb = new PersonDB(pdbFiles);

    /* get display size and size of area that is available for app                              */
    QDesktopWidget* desktopWidget = QApplication::desktop();
    this->screenSize = desktopWidget->screenGeometry();
    // qDebug() << "screen size is "<< screenSize.width() << "px x " << screenSize.height() << " px";
    this->appRect = desktopWidget->availableGeometry();
    // qDebug() << appRect.width() << "px x " << appRect.height() << " px available for application window";

    /* find minimal screen dimension                                                            */
    minSize = qMin(appRect.width(), appRect.height());

    bool mobileUI = false;

    #if defined(Q_OS_ANDROID)
        /* use mobile UI !                                                                      */
        mobileUI = true; // will be evaluated in RadixWheeel::paintEvent()
        /* resize main window a little bit because we don't have any window frame               */
        qDebug() << "using mobile UI";
        QTimer::singleShot(50, this, SLOT(correctOwnSize()));
    #else
        /* use desktop UI !                                                                     */
        qDebug() << "Using desktop UI" << endl;
    #endif

   if (mobileUI) {
   // if (TRUE) {

        /* The new mobile UI should be started at this point.
           Basically, it has a fixed columnlayout with radixwheel and sidepanel side by side
        */
        this->muiwin = new MUIWin();

        this->wheel = new RadixWheel(h, TRUE, muiwin);

        this->panel = new SidePanel(TRUE, muiwin);
        this->panelInstantiated = TRUE;
        this->timer = new QTimer( this );

        QHBoxLayout *muiLayout = new QHBoxLayout;
        muiLayout->addWidget(wheel);
        muiLayout->addWidget(panel);

        this->muiwin->setLayout(muiLayout);
        this->muiwin->resize(minSize -60, minSize -60);
        this->muiwin->show();

        this->setTheme("Saturnia");

    } else {

        /* Start the desktop UI !                               */

        this->mainwin = new JAstroMainWindow ();
        this->wheel = new RadixWheel(h, mobileUI, mainwin);
        this->table = new DataTable (mainwin);
        int wn = mainwin->addWidget(wheel);
        qDebug() << "adding widget as no " << wn;
        wn = mainwin->addWidget(table);
        qDebug() << "adding widget as no " << wn;
        mainwin->resize(minSize -60, minSize -60);
        mainwin->show();

        this->panelInstantiated = FALSE;
        this->timer = new QTimer( this );

        connect(this->mainwin, SIGNAL(mainwinClosed()),this, SLOT(onMainWidgetClose()));

        connect(this->mainwin, SIGNAL(mainwinMoved(QMoveEvent * )),
                this, SLOT(onMainWidgetMove(QMoveEvent * )));
        connect(this->mainwin, SIGNAL(mainwinClicked(QMouseEvent * )),
                 this, SLOT(onMainWidgetClicked(QMouseEvent * )));
        connect(this->wheel, SIGNAL(wheelClicked(QMouseEvent * )),
                this, SLOT(onWheelClicked(QMouseEvent * )));
        connect(this->wheel, SIGNAL(finishedPrinting()),
                this, SLOT(setTheme()));
        this->setTheme("Saturnia");
    }

}

/*----------------------------------------------------------------------------------------------*/

void Controller::correctOwnSize(void) {
    this->mainwin->resize(minSize, minSize);
    this->timer->disconnect();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setPanelSize(QSize size) {
    this->panel->resize(panel->width(), size.height()-4);
    if (this->panelInstantiated) {
        animatePanelMovement (FALSE);
        panelOut = FALSE;
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setLocation (int index) {
    QString loco = panel->locations.at(index);
    QStringList l = loco.split("   ", QString::SkipEmptyParts);
    QStringList coords = l.at(2).trimmed().split(' ', QString::SkipEmptyParts);
    QString latDegMin = coords.at(0);
    QString lonDegMin = coords.at(1);
    QStringList lat = latDegMin.split('N');
    QStringList lon = lonDegMin.split('E');
    this->panel->spinLatitudeDeg->setValue(lat.at(0).toInt());
    this->panel->spinLatitudeMin->setValue(lat.at(1).toInt());
    this->panel->spinLongitudeDeg->setValue(lon.at(0).toInt());
    this->panel->spinLongitudeMin->setValue(lon.at(1).toInt());
    /* Show currently selected location as location for secondary directions        */
    if (this->sd != NULL) {
        this->panel->lineEditPrgLocation->setText(this->panel->comboLocation->itemText(index));
        this->updateSecondaryDirectionsLoction();
    }
    this->panel->lineEditSolLocation->setText(this->panel->comboLocation->itemText(index));
    this->updateSolarLocation();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateLocations (QStringList locations) {
    this->panel->locations.clear();
    this->panel->comboLocation->clear();
    int i;
    for (i=0; i< locations.size(); i++) {
        this->panel->locations.append(locations.at(i));
        QString loco = locations.at(i);
        QStringList l = loco.split("   ", QString::SkipEmptyParts);
        QStringList plz = l.at(1).trimmed().split(' ', QString::SkipEmptyParts);
        this->panel->comboLocation->addItem(plz.at(1) + " " + l.at(0));
    }
    if (i > 0) {
        this->setLocation(0);   /* select first entry                                   */
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::searchLocation (QString searchString) {

    QStringList locations = ldb->search(searchString);

    if (locations.size() < 10) {
        for (int i = 0; i < locations.size(); i++) {
            // qDebug() << locations.at(i);
        }
        emit (locationFound (locations));
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::animatePanelMovement (bool out) {
    if (out) {
        connect( this->timer, SIGNAL(timeout()), this, SLOT(moveOutPanel()) );
        disconnect( this->timer, SIGNAL(timeout()), this, SLOT(moveInPanel()) );
        this->wheel->activateWindow();      /* these two methods need to be called to put the main    */
        this->wheel->raise();               /* window on top again                                    */
        this->timer->start(20);             /* 20 mseconds single-shot timer                          */
    } else {
        connect( this->timer, SIGNAL(timeout()), this, SLOT(moveInPanel()) );
        disconnect( this->timer, SIGNAL(timeout()), this, SLOT(moveOutPanel()) );
        this->wheel->activateWindow();      /* these two methods need to be called to put the main    */
        this->wheel->raise();               /* window on top again                                    */
        this->timer->start(20);             /* 20 mseconds single-shot timer                          */
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::moveOutPanel(void) {
    int x1 = mainwin->x()+mainwin->width() + 10;
    int x = panel->x() + (x1 - panel->x())/6 + 1;
    this->panel->move(x, mainwin->y());
    if ((x1 -panel->x()) < 1) {
        this->timer->stop();
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::moveInPanel(void) {
    int x1 = mainwin->x() + mainwin->width() -panel->width();
    int x = panel->x() - (panel->x() -x1)/6 - 1;
    this->panel->move(x, mainwin->y());
    if ((panel->x()-x1) < 1) {
        this->timer->stop();
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::onMainWidgetClose(void) {
    this->panel->close();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::onMainWidgetMove (QMoveEvent * event) {
    // QPoint newPosition = event->pos();
    if (panelInstantiated) {
        if (panelOut) {
            this->panel->move(this->mainwin->x() +this->mainwin->width() + 10, this->mainwin->y());
        } else {
            this->panel->move(this->mainwin->x() +this->mainwin->width() -this->panel->width(), this->mainwin->y());
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::calculateRadix (void) {
    if (this->panel->comboName->count() != 0) {
        this->panel->lineEditName->setText(this->panel->comboName->currentText());
    }
    float latitude = panel->spinLatitudeDeg->value() + (panel->spinLatitudeMin->value())/60.0F;
    float longitude = panel->spinLongitudeDeg->value() + (panel->spinLongitudeMin->value())/60.0F;
    QDate date = this->panel->dateEdit->date();
    QTime time = this->panel->timeEdit->time();
    this->h->deleteLater();
    this->h = new Horoscope(this->panel->lineEditName->text(), QDateTime(date,time),
                  this->panel->lineEditLocation->text(), this->countryCode,
                  this->subCode, latitude, longitude);
    this->h->setLastPlanet(SE_CHIRON);
    this->wheel->setHoroscope(this->h);
    this->wheel->showRadix();

    /* secondary directions need to be re - calculated  */
    this->sd = NULL;
    this->wheel->showSecondaryDirections = false;
    this->panel->showTransitEdits(false, false);

    /* show remaining tabs                              */
    panel->synastrieAct->setVisible(TRUE);
    panel->compositeAct->setVisible(TRUE);
    panel->combineAct->setVisible(TRUE);
    panel->solarAct->setVisible(TRUE);

    /* set years range for solar panel                  */
    this->panel->comboSolar->clear();
    for (int idx = 0; idx < 120; idx++) {
        this->panel->comboSolar->insertItem(idx, QString::number(date.year() +idx));
        if ((date.year()+idx) == date.currentDate().year()) {
            this->panel->comboSolar->setCurrentIndex(idx);
        }
    }
    this->wheel->setSolarYear(date.currentDate().year());

    emit (radixCalculated(this->h));

}

/*----------------------------------------------------------------------------------------------*/

void Controller::calculateSynastry (void) {
    if (this->panel->comboPartner->currentText().startsWith("Composit")) {
        this->h2 = this->wheel->c->h;
        this->panel->lineEditPartner->setText(this->panel->comboPartner->currentText());
    } else if (this->panel->comboPartner->currentText().startsWith("Combin")) {
        this->h2 = this->wheel->cb->h;
        this->panel->lineEditPartner->setText(this->panel->comboPartner->currentText());
    } else {
        if (this->panel->comboPartner->count() != 0) {
            this->panel->lineEditPartner->setText(this->panel->comboPartner->currentText());
        }

        this->h2 = new Horoscope(this->panel->lineEditPartner->text(), QDateTime(date2,time2),
                      this->location2, this->countryCode2,
                      this->subCode2, this->latitude2, this->longitude2);
        this->h2->setLastPlanet(SE_CHIRON);
    }
    this->wheel->setSynastry(this->h2);
}

/*----------------------------------------------------------------------------------------------*/

void Controller::calculateComposite (void) {
    if (this->panel->comboComposite->count() != 0) {
        this->panel->lineEditComposite->setText(this->panel->comboComposite->currentText());
    }
    this->h3 = new Horoscope(this->panel->lineEditComposite->text(), QDateTime(date3,time3),
                this->location3, this->countryCode3,
                this->subCode3, this->latitude3, this->longitude3);
    this->wheel->setCompositePartner(this->h3);
    this->updatePartners(QStringList());
}

/*----------------------------------------------------------------------------------------------*/

void Controller::calculateCombine (void) {
    qDebug() << "Controller::calculateCombine";
    if (this->panel->comboCombine->count() != 0) {
        this->panel->lineEditCombine->setText(this->panel->comboCombine->currentText());
    }
    this->h4 = new Horoscope(this->panel->lineEditCombine->text(), QDateTime(date4,time4),
                this->location4,
                this->countryCode4, this->subCode4, this->latitude4, this->longitude4);
    this->wheel->setCombinePartner(this->h4);
    this->updatePartners(QStringList());
}

/*----------------------------------------------------------------------------------------------*/

void Controller::calculateSolar(void) {
    // qDebug() << "Controler::calculateSolar";
    this->wheel->setSolarYear(this->panel->comboSolar->currentText().toInt());
    this->wheel->showSolar();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::saveRadix (void) {
    QString name = this->panel->lineEditName->text();
    QString type = "Radix";
    QString time = this->panel->timeEdit->time().toString("hh:mm:ss");
    QString date = this->panel->dateEdit->date().toString("dd.MM.yyyy");
    // QString zone = this->panel->comboZone->currentText();
    QString latitude = panel->spinLatitudeDeg->text() + ":" + panel->spinLatitudeMin->text();
    QString longitude = panel->spinLongitudeDeg->text() + ":" + panel->spinLongitudeMin->text();
    QString location = this->panel->comboLocation->currentText();
    QString country = this->panel->comboCountry->currentText();
    QString comment = "Created with JAstro.";
    QString zone = "%";
    QString line = name + "\t" + type + "\t" + date + "\t" + time + "\t" + zone + "\t"
                   + longitude + " " + latitude + "\t" + location + "\t" + country + "\t" + comment;
    // qDebug () << line;

    pdb->save(name, line);
}

/*----------------------------------------------------------------------------------------------*/

void Controller::searchRadix (const QString name) {

    QStringList radices = pdb->search(name);

    /* we do not want to show more than 10 results in our drop down box                 */
    if (radices.size() < 10) {
        for (int i = 0; i < radices.size(); i++) {
            // qDebug() << radices.at(i);
        }
        if (this->panel->stackedLayout->currentWidget() == this->panel->radixForm) {
            emit (radixFound (radices));
        } else if (this->panel->stackedLayout->currentWidget() == this->panel->synastrieForm) {
            emit (partnerFound (radices));
        } else if (this->panel->stackedLayout->currentWidget() == this->panel->compositeForm) {
            emit (compositeFound (radices));
        } else if (this->panel->stackedLayout->currentWidget() == this->panel->combineForm) {
            emit (combineFound (radices));
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateRadices (QStringList radices) {
    this->panel->radices.clear();
    this->panel->comboName->clear();
    if (radices.size() == 0) {
        this->clearRadixData();
    }
    int i;
    for (i=0; i< radices.size(); i++) {
        this->panel->radices.append(radices.at(i));
        QString radixLine = radices.at(i);
        // QStringList radixEntries = radixLine.split("   ", QString::SkipEmptyParts);
        QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
        if (this->panel->stackedLayout->currentWidget() == this->panel->radixForm) {
            this->panel->comboName->addItem(radixEntries.at(0));
        } else if (this->panel->stackedLayout->currentWidget() == this->panel->synastrieForm) {
            this->panel->comboPartner->addItem(radixEntries.at(0));
        }
    }
    if (i > 0) {
        this->setRadix(0);
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updatePartners (QStringList radices) {

    /* clear list with partner horoscope data                                   */
    this->panel->partners.clear();
    this->panel->comboPartner->clear();

    int i=0;

    if (radices.size() == 0) {
        // qDebug() << "radices.size() == 0";
        // this->clearRadix();
    } else {
        while (i < radices.size()) {
            // qDebug () << "i = " << i;
            /* rebuild list of partner horoscope data                           */
            this->panel->partners.append(radices.at(i));
            QString radixLine = radices.at(i);
            QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
            this->panel->comboPartner->addItem(radixEntries.at(0));
            i++;
        } 
    }
    if (this->h3 != NULL) {
        /* composite has already been calculated, so append it to combo box     */
        QString co = QString("Composit");
        this->panel->partners.append(QStringList(co));
        this->panel->comboPartner->addItem(co);
        i++;
    }
    if (this->h4 != NULL) {
        /* combine has already been calculated, so append it to combo box       */
        QString cb = QString("Combin");
        this->panel->partners.append(cb);
        this->panel->comboPartner->addItem(cb);
        i++;
    }
    /* check wether at least one dataset found. If yes, take the first one.     */
    if (i >0) {
        // qDebug() << "setPartner(0)";
        this->setPartner(0);
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateComposites (QStringList radices) {
    this->panel->composites.clear();
    this->panel->comboComposite->clear();
    if (radices.size() == 0) {
        // this->clearRadix();
    }
    int i = 0;
    while (i < radices.size()) {
        this->panel->composites.append(radices.at(i));
        QString radixLine = radices.at(i);
        // QStringList radixEntries = radixLine.split("   ", QString::SkipEmptyParts);
        QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
        this->panel->comboComposite->addItem(radixEntries.at(0));
        i++;
    }
    /* check wether only one dataset found. If yes, take this one.          */
    if (i > 0) {
        this->setCompositePartner(0);
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateCombines (QStringList radices) {
    this->panel->combines.clear();          /* clear string list with name of combine horoscopes    */
    this->panel->comboCombine->clear();     /* clear combo box holding selectable combine partners  */
    if (radices.size() == 0) {              /* did not find any data base entries                   */
        // this->clearRadix();
    }
    int i = 0;
    while (i< radices.size()) {             /* go through result list from db query                 */
        this->panel->combines.append(radices.at(i));     /* append them to stringlist               */
        QString radixLine = radices.at(i);  /* get current dataset                                  */
        QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
        this->panel->comboCombine->addItem(radixEntries.at(0));     /* append name to combo box     */
        i++;
    }
    if (i > 0) {
        this->setCombinePartner(0);
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setRadix (int index) {
    QString radixLine = panel->radices.at(index);
    QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
    QString name = radixEntries.at(0);
    QString type = radixEntries.at(1);
    QString dateString = radixEntries.at(2);
    QString timeString = radixEntries.at(3);
    QString timeZoneString = radixEntries.at(4);
    QString lonLatString = radixEntries.at(5);  /* example:  +013:22 +52:30         */
    QStringList lonLat = lonLatString.split(" ");
    QString lonDegMin = lonLat.at(0);
    QString latDegMin = lonLat.at(1);
    QStringList lat = latDegMin.split(':');
    QStringList lon = lonDegMin.split(':');
    QString ort = radixEntries.at(6);
    QString country = radixEntries.at(7);
    QString comment = radixEntries.at(8);
    this->panel->spinLatitudeDeg->setValue(lat.at(0).toInt());
    this->panel->spinLatitudeMin->setValue(lat.at(1).toInt());
    this->panel->spinLongitudeDeg->setValue(lon.at(0).toInt());
    this->panel->spinLongitudeMin->setValue(lon.at(1).toInt());
    QDate date = QDate::fromString(dateString, "dd.MM.yyyy");
    QTime time = QTime::fromString(timeString, "hh:mm:ss");
    this->panel->dateEdit->setDate(date);
    this->panel->timeEdit->setTime(time);
    this->panel->lineEditLocation->setText(ort);
    // qDebug() << "timeZoneString ="<<timeZoneString;
    // for (int i = 0; i< this->panel->comboZone->count(); i++) {
    //    qDebug() << "zone ="<<this->panel->comboZone->itemText(i);
    //    if (this->panel->comboZone->itemText(i) == timeZoneString) {
    //        this->panel->comboCountry->setCurrentIndex(i);
    //    }
    // }
    // qDebug() << "country = " << country;
    for (int i = 0; i< this->panel->comboCountry->count(); i++) {
        if (this->panel->comboCountry->itemText(i) == country) {
            this->panel->comboCountry->setCurrentIndex(i);
    //        qDebug() << "sel. country = " << this->panel->comboCountry->itemText(i);
        }
    }
    this->countryCode = getCountryCode (country);
    this->subCode   = NONE;
    if ((this->countryCode == de) || (this->countryCode == dd)){
        // QString land =
        // this->subcode = getGermanSubCode ();
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setPartner (int index) {

    QString radixLine = panel->partners.at(index);
    QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
    QString name = radixEntries.at(0);

    if (name.startsWith("Composit")) {
        // do nothing here ..
    } else if (name.startsWith("Combin")) {
        // do nothing here ..
    } else {
        QString type = radixEntries.at(1);
        QString dateString = radixEntries.at(2);
        QString timeString = radixEntries.at(3);
        QString timeZoneString = radixEntries.at(4);
        QString lonLatString = radixEntries.at(5);  /* example:  +013:22 +52:30         */
        QStringList lonLat = lonLatString.split(" ");
        QString lonDegMin = lonLat.at(0);
        QString latDegMin = lonLat.at(1);
        QStringList lat = latDegMin.split(':');
        QStringList lon = lonDegMin.split(':');
        QString ort = radixEntries.at(6);
        QString country = radixEntries.at(7);
        QString comment = radixEntries.at(8);

        this->latitude2 = lat.at(0).toInt() + lat.at(1).toInt()/60.0F;
        this->longitude2 = lon.at(0).toInt() + lon.at(1).toInt()/60.0F;
        this->date2 = QDate::fromString(dateString, "dd.MM.yyyy");
        this->time2 = QTime::fromString(timeString, "hh:mm:ss");
        this->location2 = QString(ort);
        this->countryCode2 = getCountryCode (country);
        this->subCode2   = NONE;
        if ((this->countryCode2 == de) || (this->countryCode2 == dd)){
            // QString land =
            // this->subcode = getGermanSubCode ();
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setCompositePartner (int index) {

    // qDebug () << "Controller::setCompositePartner, begin";

    QString radixLine = panel->composites.at(index);
    QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
    QString name = radixEntries.at(0);
    QString type = radixEntries.at(1);
    QString dateString = radixEntries.at(2);
    QString timeString = radixEntries.at(3);
    QString timeZoneString = radixEntries.at(4);
    QString lonLatString = radixEntries.at(5);  /* example:  +013:22 +52:30         */
    QStringList lonLat = lonLatString.split(" ");
    QString lonDegMin = lonLat.at(0);
    QString latDegMin = lonLat.at(1);
    QStringList lat = latDegMin.split(':');
    QStringList lon = lonDegMin.split(':');
    QString ort = radixEntries.at(6);
    QString country = radixEntries.at(7);
    QString comment = radixEntries.at(8);

    this->location3 = QString(ort);
    this->latitude3 = lat.at(0).toInt() + lat.at(1).toInt()/60.0F;
    this->longitude3 = lon.at(0).toInt() + lon.at(1).toInt()/60.0F;

    this->date3 = QDate::fromString(dateString, "dd.MM.yyyy");
    this->time3 = QTime::fromString(timeString, "hh:mm:ss");
    this->countryCode3 = getCountryCode (country);
    this->subCode3   = NONE;
    if ((this->countryCode3 == de) || (this->countryCode3 == dd)){
        // QString land =
        // this->subcode = getGermanSubCode ();
    }
    // qDebug () << "Controller::setCompositePartner, end";
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setCombinePartner (int index) {

    QString radixLine = panel->combines.at(index);
    QStringList radixEntries = radixLine.split("\t", QString::SkipEmptyParts);
    QString name = radixEntries.at(0);
    QString type = radixEntries.at(1);
    QString dateString = radixEntries.at(2);
    QString timeString = radixEntries.at(3);
    QString timeZoneString = radixEntries.at(4);
    QString lonLatString = radixEntries.at(5);  /* example:  +013:22 +52:30         */
    QStringList lonLat = lonLatString.split(" ");
    QString lonDegMin = lonLat.at(0);
    QString latDegMin = lonLat.at(1);
    QStringList lat = latDegMin.split(':');
    QStringList lon = lonDegMin.split(':');
    QString ort = radixEntries.at(6);
    QString country = radixEntries.at(7);
    QString comment = radixEntries.at(8);

    this->latitude4 = lat.at(0).toInt() + lat.at(1).toInt()/60.0F;
    this->longitude4 = lon.at(0).toInt() + lon.at(1).toInt()/60.0F;

    this->location4 = QString(ort);
    this->date4 = QDate::fromString(dateString, "dd.MM.yyyy");
    this->time4 = QTime::fromString(timeString, "hh:mm:ss");
    this->countryCode4 = getCountryCode (country);
    this->subCode4   = NONE;
    if ((this->countryCode4 == de) || (this->countryCode4 == dd)){
        // QString land =
        // this->subcode = getGermanSubCode ();
    }
}

/*----------------------------------------------------------------------------------------------*/

CountryCode Controller::getCountryCode (QString country) {
    CountryCode ret = de;
    if (country == "Deutschland") {
        ret = de;
    } else if (country == "Österreich") {
        ret = at;
    } else if (country == "DDR") {
        ret = dd;
    } else if (country == "Frankreich") {
        ret = fr;
    } else if ((country == "Holland") || (country == "Niederlande")) {
        ret = nl;
    } else if (country == "Schweiz") {
        ret = ch;
    } else if (country == "Ungarn") {
        ret = hu;
    } else if ((country == "England") || (country == "Grossbritannien")) {
        ret = gb;
    } else if (country == "USA") {
        ret = us;
    } else if (country == "Schweden") {
        ret = se;
    } else if (country == "Italien") {
        ret = it;
    } else if (country == "Afghanistan") {
        ret = af;
    } else if (country == "Spanien") {
        ret = es;
    } else if (country == "Israel") {
        ret = il;
    } else {
        qDebug () << "Unknown country !";
        ret = nowhere;
    }
    return ret;
}

/*----------------------------------------------------------------------------------------------*/

void Controller::clearRadixData() {
    this->panel->dateEdit->setDate(QDate(1970, 1, 1));
    this->panel->timeEdit->setTime(QTime(12, 0, 0, 0));
    this->panel->lineEditLocation->setText("");
    this->panel->spinLatitudeDeg->setValue(0);
    this->panel->spinLatitudeMin->setValue(0);
    this->panel->spinLongitudeDeg->setValue(0);
    this->panel->spinLongitudeMin->setValue(0);
}

/*----------------------------------------------------------------------------------------------*/

void Controller::onWheelClicked(QMouseEvent * event) {

    if (panelInstantiated == FALSE) {
        this->panel = new SidePanel();
        this->panel->resize(QSize(this->wheel->width()/3, this->wheel->height()));
        this->panel->show();
        this->panel->move(wheel->x()+wheel->width() -panel->width(), wheel->y());
        this->panel->setWindowTitle("Daten");
        this->panelOut = FALSE;
        this->panelInstantiated = TRUE;
        this->wheel->activateWindow();      /* these two methods need to be called to put the main    */
        this->wheel->raise();               /* window on top again                                    */

        /* set wheels reference to panel                                    */
        wheel->panel = panel;

        /* hide some of the tabs, because we have no data yet           */
        panel->synastrieAct->setVisible(FALSE);
        panel->compositeAct->setVisible(FALSE);
        panel->combineAct->setVisible(FALSE);
        panel->solarAct->setVisible(FALSE);

        /* connections to/from radix form                                 */
        connect(this->panel->radixAct, SIGNAL(triggered()),
                this->wheel, SLOT(showRadix()));
        connect(this->panel->lineEditLocation, SIGNAL(textEdited(QString)), this,
                        SLOT (searchLocation (QString)) );
        connect(this->panel->lineEditName, SIGNAL(textEdited(QString)), this,
                        SLOT (searchRadix (QString)) );
        connect(this, SIGNAL(locationFound(QStringList)),
                        this, SLOT(updateLocations(QStringList)) );
        connect(this, SIGNAL(radixFound(QStringList)),
                        this, SLOT(updateRadices(QStringList)) );
        connect (this->panel->comboLocation, SIGNAL(activated(int)), this, SLOT(setLocation(int)));
        connect (this->panel->comboName, SIGNAL(activated(int)), this, SLOT(setRadix(int)));
        connect(this->panel->buttonOk, SIGNAL(clicked()), this, SLOT(calculateRadix()));
        connect(this->panel->buttonOk, SIGNAL(clicked()), this->wheel, SLOT(hidePartner()));        
        connect(this->panel->buttonStore, SIGNAL(clicked()), this, SLOT(saveRadix()));

        connect(this, SIGNAL(radixCalculated(Horoscope *)), this->table, SLOT (calculatePartnerProfiles(Horoscope *)));

        /* Connect transit date edit and transit time edit              */
        connect(this->panel->transitDateEdit, SIGNAL(dateChanged(const QDate)),
                this, SLOT(updateTransitDate(QDate)));
        connect(this->panel->transitTimeEdit, SIGNAL(timeChanged(const QTime)),
                this, SLOT(updateTransitTime(QTime)));

        /* connections to/from synastrie form                           */
        connect(this->panel->synastrieAct, SIGNAL(triggered()),
                this->wheel, SLOT(showPartner()));
        connect(this->panel->lineEditPartner, SIGNAL(textEdited(QString)), this,
                        SLOT (searchRadix (QString)) );
        connect(this, SIGNAL(partnerFound(QStringList)),
                        this, SLOT(updatePartners(QStringList)) );

        connect (this->panel->comboPartner, SIGNAL(activated(int)), this, SLOT(setPartner(int)));

        connect(this->panel->cbSynastriePlanetaryAspects, SIGNAL(stateChanged(int)),
                this->wheel, SLOT(showSynastriePlanetaryAspects(int)));
        connect(this->panel->cbSynastrieAxisAspects, SIGNAL(stateChanged(int)),
                this->wheel, SLOT(showSynastrieAxisAspects(int)));
        connect(this->panel->cbSynastrieAxisAspectsReverse, SIGNAL(stateChanged(int)),
                this->wheel, SLOT(showSynastrieAxisAspectsReverse(int)));
        connect(this->panel->bCalcSyn, SIGNAL(clicked()), this, SLOT(calculateSynastry()));

        /* connections to/from composite form                           */
        connect(this->panel->compositeAct, SIGNAL(triggered()),
                this->wheel, SLOT(showComposite()));
        connect(this->panel->lineEditComposite, SIGNAL(textEdited(QString)), this,
                        SLOT (searchRadix (QString)) );
        connect(this, SIGNAL(compositeFound(QStringList)),
                        this, SLOT(updateComposites(QStringList)) );
        connect (this->panel->comboComposite, SIGNAL(activated(int)), this, SLOT(setCompositePartner(int)));
        connect(this->panel->bCalcComp, SIGNAL(clicked()), this, SLOT(calculateComposite()));

        /* connections to/from combine form                             */
        connect(this->panel->combineAct, SIGNAL(triggered()),
                this->wheel, SLOT(showCombine()));
        connect(this->panel->lineEditCombine, SIGNAL(textEdited(QString)), this,
                        SLOT (searchRadix (QString)) );
        connect(this, SIGNAL(combineFound(QStringList)),
                        this, SLOT(updateCombines(QStringList)) );
        connect (this->panel->comboCombine, SIGNAL(activated(int)), this, SLOT(setCombinePartner(int)));
        connect(this->panel->bCalcComb, SIGNAL(clicked()), this, SLOT(calculateCombine()));

        /* connections to/from solar form                               */
        connect (this->panel->solarAct, SIGNAL(triggered()),
                 this->wheel, SLOT(showSolar()));
        connect(this->panel->comboSolar, SIGNAL(activated(int)),
                this->wheel, SLOT(setSolarYear(int)));
        connect (this->panel->bCalcSol, SIGNAL(clicked()), this, SLOT(calculateSolar()));

        /* connect print action directly to controlers print() slot     */
        connect(this->panel->printAct, SIGNAL(triggered()), this, SLOT(print()));

        /* connections from settings form                               */
        connect(this->panel->bCommitSettings, SIGNAL(clicked()), this, SLOT(setTheme()));

        /* adjust panel height to wheel hight if that changed           */
        connect(this->mainwin, SIGNAL(mainwinResized(QSize)), this, SLOT(setPanelSize(QSize)));

    }

    int x = event->x();
    int y = event->y();

    int d = qMin(this->wheel->width(), this->wheel->height());
    int r = d/2;
    int rEvent = sqrt( pow ( (x-r), 2) + pow ((y-r), 2));



    if (rEvent < 0.9*r) {
        if (event->button()== Qt::LeftButton) {
            /* Control Sidepanel movement                           */
            if (panelOut == FALSE) {
                animatePanelMovement (TRUE);
                panelOut = TRUE;
            } else {
                animatePanelMovement (FALSE);
                panelOut = FALSE;
            }
        } else if (event->button()== Qt::RightButton) {
            /* Replace Radix Wheel with Table                       */
            mainwin->setCurrentWidget(this->table);
        }
    } else {
        if (event->button()== Qt::RightButton) {
            this->panel->lineEditSolLocation->setText(h->location);
            if (!this->wheel->transitsOnly) {
                /* Switch Secondary Directions here                 */
                if (!this->wheel->showSecondaryDirections) {      
                    if (this->sd == NULL) {
                        if (this->wheel->solar) {
                            int sdDays = this->wheel->sol->h->getDateTime().date().year();
                                         - this->h->getDateTime().date().year();
                            sdDateTime = (this->h->getDateTime()).addDays(sdDays);
                            /* get location for secondary directions from radix data    */
                            QString sdLoc = this->wheel->sol->h->location;
                            float sdLat = this->wheel->sol->h->latitude;
                            float sdLon = this->wheel->sol->h->longitude;
                            this->sd = new Horoscope("Sekundärdirektionen", sdDateTime, sdLoc, sdLat, sdLon);
                            this->wheel->setSecondaryDirection(sd);
                            /* Show location from radix data as location for secondary directions        */
                            this->panel->lineEditPrgLocation->setText(sdLoc);
                        } else {
                            /* if secondary directions horoscope doesn't
                               exist yet, create it from years difference
                               between current date and birthdate       */
                            int sdDays = QDateTime::currentDateTime().date().year()
                                         - this->h->getDateTime().date().year();
                            sdDateTime = (this->h->getDateTime()).addDays(sdDays);
                            /* get location for secondary directions from radix data    */
                            QString sdLoc = h->location;
                            float sdLat = h->latitude;
                            float sdLon = h->longitude;
                            this->sd = new Horoscope("Sekundärdirektionen", sdDateTime, sdLoc, sdLat, sdLon);
                            this->wheel->setSecondaryDirection(sd);
                            /* Show location from radix data as location for secondary directions        */
                            this->panel->lineEditPrgLocation->setText(sdLoc);
                        }
                    } else {
                        this->wheel->showSecondaryDirections = true;
                    }
                    this->panel->showTransitEdits(true, false);
                    this->wheel->showTransits = false;
                } else {
                    this->wheel->showSecondaryDirections = false;
                    this->panel->showTransitEdits(false, false);   
                }
            }
        } else {
            if (!this->wheel->transitsOnly) {
                /* show transits on periphery of radix wheel        */
                if (!this->wheel->showTransits) {
                    if (this->t == 0) {
                        this->t = new Horoscope("Aktuelle Konstellation", QDateTime::currentDateTime(),
                                            "Hamburg", 52.77, 9.00);
                        this->wheel->setTransit(t);
                    } else {
                        this->t->setDateTime(QDateTime(this->panel->transitDateEdit->date(), this->panel->transitTimeEdit->time()));
                        this->wheel->showTransits = true;
                    }
                    this->panel->showTransitEdits(true, true);
                    this->wheel->showSecondaryDirections = false;
                } else {
                    this->wheel->showTransits = false;
                    this->panel->showTransitEdits(false, true);
                }
            }
        }
        this->wheel->update();
    }
    this->panelInstantiated = TRUE;
}

/*----------------------------------------------------------------------------------------------*/

void Controller::onMainWidgetClicked (QMouseEvent * event) {

    int x = event->x();
    int y = event->y();

    int d = qMin(this->table->width(), this->table->height());
    int r = d/2;
    int rEvent = sqrt( pow ( (x-r), 2) + pow ((y-r), 2));

    if (rEvent < 0.9*r) {
        if (event->button()== Qt::LeftButton) {
            /* Control Sidepanel movement                           */
            if (panelOut == FALSE) {
                animatePanelMovement (TRUE);
                panelOut = TRUE;
            } else {
                animatePanelMovement (FALSE);
                panelOut = FALSE;
            }
        } else if (event->button()== Qt::RightButton) {
            /* Replace Radix Wheel with Table                       */
            mainwin->setCurrentWidget(this->wheel);
        }
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateTransitDate(const QDate date) {
    if (this->wheel->showSecondaryDirections == true) {
        updateSecondaryDirectionsDate(date);
    } else {
        this->t->setDateTime(QDateTime(date, this->panel->transitTimeEdit->time()));
        this->wheel->update();
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateTransitTime(const QTime time) {
    if (!this->wheel->showSecondaryDirections) {
        this->t->setDateTime(QDateTime(this->panel->transitDateEdit->date(), time));
        this->wheel->update();
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateSecondaryDirectionsLoction(void) {

    /* get location for secondary directions                    */
    QString loco = this->panel->locations.at(0);
    QStringList l = loco.split("   ", QString::SkipEmptyParts);
    QStringList plz = l.at(1).trimmed().split(' ', QString::SkipEmptyParts);
    QString sdLoc = QString(plz.at(1) + " " + l.at(0));
    // qDebug() << "sdLoc = " << sdLoc;
    float sdLat = panel->spinLatitudeDeg->value() + (panel->spinLatitudeMin->value())/60.0F;
    float sdLon = panel->spinLongitudeDeg->value() + (panel->spinLongitudeMin->value())/60.0F;
    this->sd->setLocation(sdLoc, de, sdLat, sdLon); // TODO: use proper country code !
    this->wheel->update();

    // qDebug() << "updated sd location to: " << this->sd->location;
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateSolarLocation(void) {

    /* get location for solar                                   */
    QString loco = this->panel->locations.at(0);
    QStringList l = loco.split("   ", QString::SkipEmptyParts);
    QStringList plz = l.at(1).trimmed().split(' ', QString::SkipEmptyParts);
    QString solLoc = QString(plz.at(1) + " " + l.at(0));
    // qDebug() << "solLoc = " << solLoc;
    float solLat = panel->spinLatitudeDeg->value() + (panel->spinLatitudeMin->value())/60.0F;
    float solLon = panel->spinLongitudeDeg->value() + (panel->spinLongitudeMin->value())/60.0F;
    if (this->wheel->sol != NULL) {
        this->wheel->sol->setLocation (l.at(0), de, solLon, solLat);    // TODO: use proper country code
        this->wheel->update();
    }
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateSecondaryDirectionsDate(const QDate date) {

    /* calculate number of days for secondary directions        */
    QDateTime editDateTime = QDateTime(date, this->panel->transitTimeEdit->time());
    int sdDays = editDateTime.date().year() - this->h->getDateTime().date().year();
    sdDateTime = (this->h->getDateTime()).addDays(sdDays);

    /* calculate hours from months and days                     */
    int sdSecs = (editDateTime.date().dayOfYear() - this->h->getDateTime().date().dayOfYear())*24*60*60/365;
    // qDebug() << "sdSecs = " << sdSecs;
    sdDateTime = sdDateTime.addSecs(sdSecs);
    this->sd->setDateTime(sdDateTime);
    this->wheel->update();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::updateSecondaryDirectionsTime(const QTime time) {
    this->sd->setDateTime(QDateTime(this->panel->secondaryDirectionsDateEdit->date(), time));
    this->wheel->update();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setRadixOldFormat (int index) {
    QString radixLine = panel->radices.at(index);
    QStringList radixEntries = radixLine.split("   ", QString::SkipEmptyParts);
    QString name = radixEntries.at(0);
    QString dateString = radixEntries.at(1);
    QString timeString = radixEntries.at(2);
    QString locationString = radixEntries.at(3);
    QStringList locationEntries = locationString.split(" ", QString::SkipEmptyParts);
    QString plz = locationEntries.at(0);
    QString ort = locationEntries.at(1);
    QString latDegMin = radixEntries.at(4);
    QString lonDegMin = radixEntries.at(5);
    QStringList lat = latDegMin.split('N');
    QStringList lon = lonDegMin.split('E');
    this->panel->lineEditName->setText(name);
    this->panel->spinLatitudeDeg->setValue(lat.at(0).toInt());
    this->panel->spinLatitudeMin->setValue(lat.at(1).toInt());
    this->panel->spinLongitudeDeg->setValue(lon.at(0).toInt());
    this->panel->spinLongitudeMin->setValue(lon.at(1).toInt());
    QDate date = QDate::fromString(dateString, "dd.MM.yyyy");
    QTime time = QTime::fromString(timeString, "hh:mm");
    this->panel->dateEdit->setDate(date);
    this->panel->timeEdit->setTime(time);
    this->panel->lineEditLocation->setText(ort);
    this->searchLocation(ort);
}


/*----------------------------------------------------------------------------------------------*/

void Controller::print () {
    /* let radix wheel do the printing              */
    this->wheel->print();
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setTheme () {
    this->setTheme ("NONE");
}

/*----------------------------------------------------------------------------------------------*/

void Controller::setTheme (QString themeName) {

    QString styleSheet;

    if (themeName == "NONE") {
        // just reset theme to currently selected theme, e.g. after changing colors for printing
        currentTheme = this->panel->comboTheme->currentText();
    } else{
        currentTheme = themeName;
    }

    qDebug() << "Controller::setTheme, theme = " << themeName;

    if (currentTheme == "Saturnia") {
        styleSheet = "darkstyle.qss";
        this->wheel->setWheelColor (QColor(255, 240, 0));
        this->wheel->setItemColor (QColor(0, 127, 127, 191));
        this->wheel->setItemColor2 (QColor(160, 160, 160, 255));
        this->wheel->setMarkerColor (QColor(255, 240, 0));
        this->wheel->setEarthElementColor (QColor(0, 127, 127, 191));
        this->wheel->setFireElementColor (QColor(0, 127, 127, 191));
        this->wheel->setAirElementColor (QColor(0, 127, 127, 191));
        this->wheel->setWaterElementColor (QColor(0, 127, 127, 191));
        this->wheel->setTransitColor(QColor(160, 30, 30));
        this->wheel->setProgressionColor(QColor(20, 130, 30));
    } else if (currentTheme == "Virgo") {
        styleSheet = "virgo.qss";
        this->wheel->setWheelColor (QColor(0, 0, 0));
        this->wheel->setItemColor (QColor(0, 0, 0, 191));
        this->wheel->setItemColor2 (QColor(0, 0, 0, 191));
        this->wheel->setMarkerColor (QColor(0, 0, 0));
        this->wheel->setEarthElementColor (QColor (0, 200, 0));
        this->wheel->setFireElementColor (QColor (240, 0, 0));
        this->wheel->setAirElementColor (QColor (46, 198, 255));
        this->wheel->setWaterElementColor (QColor (0, 0, 170));
        this->wheel->setTransitColor(QColor(160, 30, 30));
        this->wheel->setProgressionColor(QColor(20, 130, 30));
    } else if (currentTheme == "Gemini") {
        styleSheet = "gemini.qss";
        this->wheel->setWheelColor (QColor(0, 0, 0));
        this->wheel->setItemColor (QColor(0, 0, 0, 191));
        this->wheel->setItemColor2 (QColor(0, 0, 0, 191));
        this->wheel->setMarkerColor (QColor(0, 0, 0));
        this->wheel->setEarthElementColor (QColor (0, 200, 0));
        this->wheel->setFireElementColor (QColor (240, 0, 0));
        this->wheel->setAirElementColor (QColor (255, 255, 150));
        this->wheel->setWaterElementColor (QColor (0, 0, 220));
        this->wheel->setTransitColor(QColor(160, 30, 30));
        this->wheel->setProgressionColor(QColor(20, 130, 30));
    }
    // load stylesheet from resource file
    QString styleSheetPath = ":/data/";
    styleSheetPath.append(styleSheet);
    QFile file(styleSheetPath);
    file.open(QFile::ReadOnly);

    /* We'll use qApp macro to get the QApplication pointer
    * and set the style sheet application wide. */
    qApp->setStyleSheet(QLatin1String(file.readAll()));

}
