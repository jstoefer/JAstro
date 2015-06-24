#include "sidepanel.h"
#include "controller.h"

SidePanel::SidePanel(bool mobileUI, QWidget *parent) : QFrame(parent) {
    this->mobileUI = mobileUI;
    this->initGui();
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::initGui() {

    /* create all forms                             */
    this->radixForm = createRadixForm();
    this->synastrieForm = createSynastrieForm();
    this->compositeForm = createCompositeForm();
    this->combineForm = createCombineForm();
    this->solarForm = createSolarForm();
    this->settingsForm = createSettingsForm();

    /* create main layout and stacked layout        */
    this->stackedLayout = new QStackedLayout;
    this->mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    /* crate toolbar and icons                      */
    upperToolBar   = new QToolBar ("Options", this );
    lowerToolBar   = new QToolBar ("More Options", this );

    // load images from resource file
    QString imgPath = ":/img/";

    iconRadix      = QIcon(imgPath+"person.png");
    iconSynastrie  = QIcon(imgPath+"synastrie.png");
    iconComposite  = QIcon(imgPath+"composite.png");
    iconCombine    = QIcon(imgPath+"combine.png");
    iconText       = QIcon(imgPath+"text.png");
    iconDB         = QIcon(imgPath+"db.png");
    iconSolar      = QIcon(imgPath+"sun.png");
    iconPrinter    = QIcon(imgPath+"printer.png");
    iconSettings   = QIcon(imgPath+"Settings-icon.png");

    /* set application icon                         */
    this->setWindowIcon(iconComposite);

    /* create actions                               */
    radixAct = new QAction(iconRadix, tr("&Radix"), this);
    radixAct->setStatusTip(tr("Radixdaten bearbeiten"));
    connect(radixAct, SIGNAL(triggered()), this, SLOT(showRadixForm()));

    synastrieAct = new QAction(iconSynastrie, tr("&Synastrie"), this);
    radixAct->setStatusTip(tr("Synastrieberechnungen"));
    connect(synastrieAct, SIGNAL(triggered()), this, SLOT(showSynastrieForm()));

    compositeAct = new QAction(iconComposite, tr("&Composit"), this);
    compositeAct->setStatusTip(tr("Composit"));
    connect(compositeAct, SIGNAL(triggered()), this, SLOT(showCompositeForm()));

    combineAct = new QAction(iconCombine, tr("&Combin"), this);
    combineAct->setStatusTip(tr("Combin"));
    connect(combineAct, SIGNAL(triggered()), this, SLOT(showCombineForm()));

    solarAct = new QAction(iconSolar, tr("&Solar"), this);
    solarAct->setStatusTip(tr("Solar"));
    connect(solarAct, SIGNAL(triggered()), this, SLOT(showSolarForm()));

    textAct = new QAction(iconText, tr("&Deutungstexte"), this);
    textAct->setStatusTip(tr("Deutung"));
    connect(textAct, SIGNAL(triggered()), this, SLOT(showTextForm()));

    dbAct = new QAction(iconDB, tr("&Datenbasis"), this);
    dbAct->setStatusTip(tr("Daten"));
    connect(dbAct, SIGNAL(triggered()), this, SLOT(showDBForm()));

    printAct = new QAction(iconPrinter, tr("&Drucken"), this);
    printAct->setStatusTip(tr("Drucken"));

    settingsAct = new QAction(iconSettings, tr("&Einstellungen"), this);
    settingsAct->setStatusTip(tr("Einstellungen"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(showSettingsForm()));

    /* add actions to upper toolbar                 */
    upperToolBar->addAction (radixAct);
    upperToolBar->addAction (synastrieAct);
    upperToolBar->addAction (compositeAct);
    upperToolBar->addAction (combineAct);
    upperToolBar->addAction (solarAct);

    /* add actions to lower toolbar                 */
    lowerToolBar->addAction (printAct);
    lowerToolBar->addAction (settingsAct);
    lowerToolBar->addAction (textAct);
    lowerToolBar->addAction (dbAct);

    /* add both toolbars                            */
    mainLayout->addWidget(upperToolBar);
    mainLayout->addWidget(lowerToolBar);

    /* add forms                                    */
    mainLayout->addLayout(this->stackedLayout);
    this->stackedLayout->addWidget(this->radixForm);
    this->stackedLayout->addWidget(this->synastrieForm);
    this->stackedLayout->addWidget(this->compositeForm);
    this->stackedLayout->addWidget(this->combineForm);
    this->stackedLayout->addWidget(this->solarForm);
    this->stackedLayout->addWidget(this->settingsForm);

}

/*----------------------------------------------------------------------------------------------*/

QWidget * SidePanel::createRadixForm (void) {

    QWidget * rf = new QWidget();
    QVBoxLayout *vbox;

    this->zones          = QStringList();
    this->zones << "UT" << "MEZ" << "UTC" << "GMT";

    this->countries     = QStringList();
    this->countries << "Deutschland" << "Österreich" << "DDR" << "Frankreich"<< "Niederlande"
            << "Schweiz" << "Ungarn" << "Grossbritannien" << "USA" << "Schweden" << "Italien"
            << "Afghanistan" << "Spanien" << "Israel" << "---";

    vbox                = new QVBoxLayout;
    rf->setLayout(vbox);

    QLabel * heading    = new QLabel();
    heading->setText("Radix Daten:");
    heading->setObjectName("heading");
    vbox->addWidget(heading);

    fLayout             = new QFormLayout;
    vbox->addLayout(fLayout);

    thisDate1           = new QDate(QDate::currentDate());
    thisDate2           = new QDate(QDate::currentDate());

    lineEditName        = new QLineEdit();
    lineEditLocation    = new QLineEdit();
    lineEditPrgLocation = new QLineEdit();
    lineEditSolLocation = new QLineEdit();
    dateEdit            = new CustomQDateEdit(*thisDate1);
    dateEdit->setDisplayFormat("dd-MMM-yyyy");
    timeEdit            = new QTimeEdit(QTime::currentTime());
    transitDateEdit     = new CustomQDateEdit(*thisDate2);
    transitDateEdit->setDisplayFormat("dd-MMM-yyyy");
    transitTimeEdit     = new QTimeEdit(QTime::currentTime());
    comboLocation       = new QComboBox();
    comboCountry        = new QComboBox();
    comboCountry->addItems(countries);
    comboCountry->setCurrentText("Deutschland");   // select "Deutschland"
    comboName           = new QComboBox();
    spinLongitudeDeg    = new QSpinBox();
    spinLongitudeMin    = new QSpinBox();
    spinLatitudeDeg     = new QSpinBox();
    spinLatitudeMin     = new QSpinBox();
    buttonOk            = new QPushButton("Berechnen");
    buttonStore         = new QPushButton("Abspeichern");

    hbox1               = new QHBoxLayout();
    hbox2               = new QHBoxLayout();
    hbox3               = new QHBoxLayout();
    hbox4               = new QHBoxLayout();
    hbox5               = new QHBoxLayout();

    hbox4->addWidget(this->lineEditName);
    fLayout->addRow(tr("Name:"), hbox4);
    fLayout->addRow(tr("DB:"), comboName);

    fLayout->addRow(tr("Datum:"), dateEdit);
    fLayout->addRow(tr("Zeit:"), timeEdit);

    hbox5->addWidget(this->lineEditLocation);
    fLayout->addRow(tr("Ort:"), hbox5);

    fLayout->addRow(tr("Land:"), comboCountry);
    fLayout->addRow(tr("Orte DB:"), comboLocation);

    hbox1->addWidget(spinLongitudeDeg);
    hbox1->addWidget(new QLabel("O"));
    hbox1->addWidget(spinLongitudeMin);
    fLayout->addRow(tr("Länge:"), hbox1);

    hbox2->addWidget(spinLatitudeDeg);
    hbox2->addWidget(new QLabel("N"));
    hbox2->addWidget(spinLatitudeMin);
    fLayout->addRow(tr("Breite:"), hbox2);
    fLayout->addItem(new QSpacerItem(10, 10));

    transitDateLbl      = new QLabel(tr("Transite:"));
    transitTimeLbl      = new QLabel(tr("Zeit:"));
    progressionLocLbl   = new QLabel(tr("Ort:"));

    fLayout->addRow(transitDateLbl, this->transitDateEdit);
    fLayout->addRow(transitTimeLbl, this->transitTimeEdit);
    fLayout->addRow(progressionLocLbl, this->lineEditPrgLocation);

    this->transitDateEdit->hide();
    this->transitTimeEdit->hide();
    this->transitDateLbl->hide();
    this->transitTimeLbl->hide();
    this->progressionLocLbl->hide();
    this->lineEditPrgLocation->hide();

    fLayout->addItem(new QSpacerItem(10, 10));

    hbox3->addWidget(buttonOk);
    hbox3->addWidget(buttonStore);
    buttonOk->setMaximumWidth(100);
    buttonStore->setMaximumWidth(100);
    vbox->addLayout(hbox3);

    comboLocation->setFixedWidth(210);
    comboName->setFixedWidth(210);

    spinLongitudeMin->setMaximum(59);
    spinLatitudeMin->setMaximum(59);

    return rf;

}

/*----------------------------------------------------------------------------------------------*/

QWidget * SidePanel::createSynastrieForm (void) {

    QWidget * sf                = new QWidget();
    QVBoxLayout *vbox           = new QVBoxLayout;
    sf->setLayout(vbox);

    vbox->addStretch(10);

    QLabel *heading     = new QLabel();
    heading->setObjectName("heading");
    heading->setText("Synastrie Daten:");
    vbox->addWidget(heading);

    vbox->addStretch(10);

    QFormLayout *fl             = new QFormLayout;
    vbox->addLayout(fl);

    lineEditPartner             = new QLineEdit();
    comboPartner                = new QComboBox();
    bCalcSyn                    = new QPushButton("Berechnen");

    QHBoxLayout *bl3            = new QHBoxLayout();
    QHBoxLayout *bl4            = new QHBoxLayout();

    bl4->addWidget(this->lineEditPartner);
    fl->addRow(tr("Name:"), bl4);
    fl->addRow(tr("DB:"), comboPartner);

    vbox->addStretch(4);

    vbox->addLayout(createAspectGroup());

    vbox->addStretch(10);

    bl3->addWidget(bCalcSyn);
    bCalcSyn->setMaximumWidth(100);
    vbox->addLayout(bl3);

    comboPartner->setFixedWidth(210);
    return sf;
}

/*----------------------------------------------------------------------------------------------*/

QVBoxLayout * SidePanel::createAspectGroup() {

    QVBoxLayout *vbox = new QVBoxLayout;

    QLabel *aspects     = new QLabel();
    aspects->setObjectName("aspects");
    aspects->setText("Zeige Aspekte:");
    vbox->addWidget(aspects);

     cbSynastriePlanetaryAspects = new QCheckBox(tr("&Planetare Aspekte"));
     cbSynastrieAxisAspects = new QCheckBox(tr("Achsenaspekte"));
     cbSynastrieAxisAspectsReverse = new QCheckBox(tr("Reverse Achsenaspekte"));

     vbox->addWidget(cbSynastriePlanetaryAspects);
     vbox->addWidget(cbSynastrieAxisAspects);
     vbox->addWidget(cbSynastrieAxisAspectsReverse);
     vbox->addStretch(1);

     return vbox;
}

/*----------------------------------------------------------------------------------------------*/

QWidget * SidePanel::createCompositeForm (void) {

    QWidget * cf                = new QWidget();
    QVBoxLayout *vbox           = new QVBoxLayout;
    cf->setLayout(vbox);

    QLabel *heading     = new QLabel();
    heading->setObjectName("heading");
    heading->setText("Composit-Daten:");
    vbox->addWidget(heading);

    QFormLayout *fl             = new QFormLayout;
    vbox->addLayout(fl);

    lineEditComposite           = new QLineEdit();
    comboComposite              = new QComboBox();
    bCalcComp                   = new QPushButton("Berechnen");

    QHBoxLayout *bl3            = new QHBoxLayout();
    QHBoxLayout *bl4            = new QHBoxLayout();

    bl4->addWidget(this->lineEditComposite);
    fl->addRow(tr("Name:"), bl4);
    fl->addRow(tr("DB:"), comboComposite);
    //vbox->addWidget(createAspectGroup());
    bl3->addWidget(bCalcComp);
    bCalcComp->setMaximumWidth(100);
    vbox->addLayout(bl3);

    comboComposite->setFixedWidth(210);
    return cf;
}

/*----------------------------------------------------------------------------------------------*/

QWidget * SidePanel::createCombineForm (void) {

    QWidget * cf                = new QWidget();
    QVBoxLayout *vbox           = new QVBoxLayout;
    cf->setLayout(vbox);
    QLabel *heading     = new QLabel();
    heading->setObjectName("heading");
    heading->setText("Combin Daten:");
    vbox->addWidget(heading);
    QFormLayout *fl             = new QFormLayout;
    vbox->addLayout(fl);
    lineEditCombine             = new QLineEdit();
    comboCombine                = new QComboBox();
    bCalcComb                   = new QPushButton("Berechnen");
    QHBoxLayout *bl3            = new QHBoxLayout();
    QHBoxLayout *bl4            = new QHBoxLayout();
    bl4->addWidget(this->lineEditCombine);
    fl->addRow(tr("Name:"), bl4);
    fl->addRow(tr("DB:"), comboCombine);
    bl3->addWidget(bCalcComb);
    bCalcComb->setMaximumWidth(100);
    vbox->addLayout(bl3);
    comboCombine->setFixedWidth(210);
    return cf;
}

/*----------------------------------------------------------------------------------------------*/

QWidget * SidePanel::createSolarForm (void) {
    QWidget * cf                = new QWidget();
    QVBoxLayout *vbox           = new QVBoxLayout;
    cf->setLayout(vbox);
    QLabel *heading             = new QLabel();
    heading->setObjectName("heading");
    heading->setText("Solar Daten:");
    vbox->addWidget(heading);
    QFormLayout *fl             = new QFormLayout;
    vbox->addLayout(fl);
    comboSolar                  = new QComboBox();
    bCalcSol                    = new QPushButton("Berechnen");
    QHBoxLayout *bl3            = new QHBoxLayout();
    fl->addRow(tr("Jahr:"), comboSolar);
    fl->addRow("Ort:", lineEditSolLocation);
    bl3->addWidget(bCalcSol);
    bCalcSol->setMaximumWidth(100);
    vbox->addLayout(bl3);
    comboSolar->setFixedWidth(210);
    lineEditSolLocation->setFixedWidth(210);
    return cf;
}

/*----------------------------------------------------------------------------------------------*/

QWidget * SidePanel::createSettingsForm (void) {
    QWidget * cf                = new QWidget();
    QVBoxLayout *vbox           = new QVBoxLayout;
    cf->setLayout(vbox);
    QLabel *heading             = new QLabel();
    heading->setObjectName("heading");
    heading->setText("Einstellungen:");
    vbox->addWidget(heading);
    QFormLayout *fl             = new QFormLayout;
    vbox->addLayout(fl);
    comboTheme                  = new QComboBox();
    themes << "Saturnia" << "Virgo" << "Gemini";
    comboTheme->addItems(themes);
    bCommitSettings             = new QPushButton("Übernehmen");
    QHBoxLayout *bl3            = new QHBoxLayout();
    fl->addRow(tr("Thema:"), comboTheme);
    bl3->addWidget(bCommitSettings);
    bCommitSettings->setMaximumWidth(100);
    vbox->addLayout(bl3);
    comboTheme->setFixedWidth(210);
    return cf;
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showRadixForm() {
    this->stackedLayout->setCurrentWidget(this->radixForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showSynastrieForm () {
    this->stackedLayout->setCurrentWidget(this->synastrieForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showCompositeForm () {
    this->stackedLayout->setCurrentWidget(this->compositeForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showCombineForm () {
    this->stackedLayout->setCurrentWidget(this->combineForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showSolarForm () {
    this->stackedLayout->setCurrentWidget(this->solarForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showTextForm () {
    this->stackedLayout->setCurrentWidget(this->textForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showDBForm () {
    this->stackedLayout->setCurrentWidget(this->dbForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showSettingsForm() {
    this->stackedLayout->setCurrentWidget(this->settingsForm);
}

/*----------------------------------------------------------------------------------------------*/

void SidePanel::showTransitEdits(bool show, bool transits) {
    if (show) {  
        this->transitDateEdit->show();
        this->transitDateLbl->show();
        if (transits) {
            this->lineEditPrgLocation->hide();
            this->progressionLocLbl->hide();
            this->transitTimeEdit->show();
            this->transitTimeLbl->show();
            this->transitDateLbl->setText(tr("Transite"));
        } else  {
            this->transitDateLbl->setText(tr("Progres."));
            this->transitTimeEdit->hide();
            this->transitTimeLbl->hide();
            this->lineEditPrgLocation->show();
            this->progressionLocLbl->show();
        }
    } else {
        this->transitDateEdit->hide();
        this->transitTimeEdit->hide();
        this->transitDateLbl->hide();
        this->transitTimeLbl->hide();
        this->lineEditPrgLocation->hide();
        this->progressionLocLbl->hide();
    }
}

