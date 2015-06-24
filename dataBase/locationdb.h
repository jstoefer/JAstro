#ifndef LOCATIONDB_H
#define LOCATIONDB_H

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

class LocationDB {

public:

    /**
      * constructs a location database using the supplied list of files with
      * location data.
      * Location data is kept in a text file format, one location per line.
      *
      */
    LocationDB(QStringList ldbLists);

    /**
      * search for a string in the location database
      *
      * @param  QString     string to search for
      * @return QStringList Each line within a textfile that contains the search string
      *                     will be returned as a string in the stringlist.
      */
    QStringList search (QString searchString);



private:

    /**
      * list of database file names
      */
    QStringList dbFiles;

    /**
      * check for occurence of a search string in a given line from the text file
      *
      * @return bool    true, if line contains search string
      */
    bool processLine (QString line, QString searchString);

};

#endif // LOCATIONDB_H
