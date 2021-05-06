#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include "connector.h"
#include "process.h"

class DBConnector : public Connector{
public:
    DBConnector(QString path);

    bool isAvailable();
    QString search(int mode, QString word);
    bool addDefinition(int mode, QString word, QStringList meanings);

    QStringList getMatchingWords(const int mode, QString word);


private:
    QSqlDatabase db;

    QString generateBloomFilterRegExp(QStringList list);

};

#endif // DBCONNECTOR_H
