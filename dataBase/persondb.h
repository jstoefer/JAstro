#ifndef PERSONDB_H
#define PERSONDB_H

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

class PersonDB {

    public:
    PersonDB(QStringList dbFiles);

    /**
      * search radix data for a given horoscope owner
      *
      * @param  QString     name of horoscope owner
      * @return QStringList list of result strings
      *
      */
    QStringList search(const QString name);

    /**
      * save one set of radix data
      *
      * @param  QString name of radix / person
      * @param  QString tab separated radix data
      * @return bool    wether data could be saved successfullys
      */
    bool save (QString name, QString line);

    private:

    /**
      * list of database files where to search for persons or events
      *
      * note: New persons will be stored in the first file in the list !
      *
      */
    QStringList dbFiles;

    /**
      * check wether db entry contains a given name string
      */
    bool processEntry (QString entry, QString name);

};

#endif // PERSONDB_H
