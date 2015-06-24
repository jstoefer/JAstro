#include <QtCore/QCoreApplication>
#include "horoscope.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    swe_set_ephe_path("../data");
    /**
      * simple constructor for class horoscope
      */
    // Horoscope (QString name, QDateTime dTime, QString location, float latitude, float longitude);
    Horoscope * h = new Horoscope("Hier und jetzt", QDateTime::currentDateTime(), "Hamburg", 52.77, 9.00);
    return a.exec();
}
