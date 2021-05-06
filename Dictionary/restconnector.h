#ifndef RESTCONNECTOR_H
#define RESTCONNECTOR_H

#include "connector.h"

class RESTConnector : public Connector
{
public:
    RESTConnector(QString path);

    bool isAvailable();
    QString search(int mode, QString word);
    bool addDefinition(int mode,QString word, QStringList meanings);

    QStringList getMatchingWords(const int mode, QString word);
};

#endif // RESTCONNECTOR_H
