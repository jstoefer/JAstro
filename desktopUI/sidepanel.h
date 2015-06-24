#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include <QDebug>
#include <QWidget>
#include <QStackedLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
// #include <QDateEdit>
#include "customqdateedit.h"
#include <QTimeEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QSlider>

class Controller;

class SidePanel : public QFrame {

     Q_OBJECT

    public:
       SidePanel(bool mobileUI = false, QWidget *parent = 0);
       void initGui (void);
       QWidget * createRadixForm (void);
       QWidget * createSynastrieForm (void);
       QWidget * createCompositeForm (void);
       QWidget * createCombineForm (void);
       QWidget * createSolarForm(void);
       QWidget * createSettingsForm (void);
       QVBoxLayout * createAspectGroup(void);

       QStackedLayout *stackedLayout;

       QWidget *radixForm, *synastrieForm, *transitForm, *compositeForm, *combineForm,
                *solarForm, *settingsForm, *textForm, *dbForm;

       QDate * thisDate1, * thisDate2;

       QToolBar * upperToolBar, * lowerToolBar;

       QLineEdit *lineEditName, *lineEditLocation, *lineEditPrgLocation, *lineEditSolLocation, *lineEditPartner, *lineEditComposite, *lineEditCombine;
       QComboBox *comboLocation, *comboName, *comboCountry, *comboPartner, *comboComposite,
                 *comboCombine, *comboSolar, *comboTheme;
       CustomQDateEdit *dateEdit, *transitDateEdit, *secondaryDirectionsDateEdit;
       QTimeEdit *timeEdit, *transitTimeEdit, *secondaryDirectionsTimeEdit;
       QSpinBox *spinLongitudeDeg, *spinLongitudeMin, *spinLatitudeDeg, *spinLatitudeMin;
       QPushButton *buttonOk, *buttonStore, *buttonDB1, *buttonDB2, *bCalcSyn, *bCalcComp,
                   *bCalcComb, *bCalcSol, *bCommitSettings;
       QCheckBox *cbSynastriePlanetaryAspects, *cbSynastrieAxisAspects, * cbSynastrieAxisAspectsReverse;
       QIcon iconDB, iconSynastrie, iconComposite, iconCombine, iconSolar, iconRadix, iconText,
                iconSettings, iconPrinter;
       QLabel *transitDateLbl, *transitTimeLbl, *progressionLocLbl;
       QAction *radixAct, * synastrieAct, *compositeAct, *combineAct, *solarAct, *settingsAct, *textAct,
                *dbAct, *printAct;
       QStringList locations, radices, partners, composites, combines, themes, countries, zones;


    public slots:

       void showRadixForm();
       void showSynastrieForm();
       void showCompositeForm();
       void showCombineForm();
       void showSolarForm();
       void showTextForm();
       void showDBForm();
       void showSettingsForm();
       void showTransitEdits(bool show, bool transits);

    private:
       QVBoxLayout * mainLayout;
       QFormLayout *fLayout;
       QHBoxLayout *hbox1, *hbox2, *hbox3, *hbox4, *hbox5, *hbox6;
       bool mobileUI;

};

#endif // SIDEPANEL_H
