#include "locationdb.h"

LocationDB::LocationDB (QStringList dbFiles) {
    this->dbFiles = dbFiles;
}

/*----------------------------------------------------------------------------------------------*/

QStringList LocationDB::search (QString searchString) {

    int nResults = 0;
    int maxResults = 50;
    QStringList locations;              /* result list                  */

    for (int k=0; k < dbFiles.size(); k++) {
        QFile inputFile(dbFiles.at(k));
        inputFile.open(QIODevice::ReadOnly);
        QTextStream in(&inputFile);
        // in.setCodec("ISO-8859-15");   /* für deutsche Umlaute         */
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (processLine(line, searchString)){
                if (nResults++ < maxResults) {
                    locations << line;
                }
            }
        }
        inputFile.close();
    }
    return locations;
}

/*----------------------------------------------------------------------------------------------*/

bool LocationDB::processLine (QString line, QString searchString) {
    if (line.startsWith(searchString, Qt::CaseInsensitive)) {
        return true;
    } else {
        return false;
    }
}

/*----------------------------------------------------------------------------------------------*/
