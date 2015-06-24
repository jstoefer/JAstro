#include "persondb.h"

PersonDB::PersonDB(QStringList dbFiles){
    this->dbFiles = dbFiles;    /* store list of database files */
}

/*----------------------------------------------------------------------------------------------*/

bool PersonDB::save(QString name, QString line) {

    bool newEntry = true;

    QFile outputFile(dbFiles.at(0));        /* save radix data to first db file in list     */
    outputFile.open(QIODevice::ReadWrite);
    QTextStream out(&outputFile);
    out.setCodec("ISO-8859-1");

    while (!out.atEnd()) {
        QString line = out.readLine();
        if (line.startsWith (name)) {
            newEntry = false;
        }
    }

    if (newEntry) {
        out << line << "\n";
    }
    outputFile.close();

    return newEntry;
}

/*----------------------------------------------------------------------------------------------*/

QStringList PersonDB::search (QString name) {

    QStringList radices;        /* result list                  */
    int nResults = 0;           /* number of search results     */
    int maxResults = 50;        /* maximum number of results    */

    for (int k=0; k < dbFiles.size(); k++) {
        QFile inputFile(dbFiles.at(k));
        inputFile.open(QIODevice::ReadOnly);
        QTextStream in(&inputFile);
        in.setCodec("ISO-8859-1");  /* für deutsche Umlaute     */
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (processEntry(line, name)){
                if (nResults++ < maxResults) {
                    radices << line;
                }
            }
        }
        inputFile.close();
    }
    return radices;
}

/*----------------------------------------------------------------------------------------------*/

bool PersonDB::processEntry (QString entry, QString name) {
    if ((name.length() >0) && (entry.startsWith(name, Qt::CaseInsensitive))) {
        return true;
    } else {
        return false;
    }
}
