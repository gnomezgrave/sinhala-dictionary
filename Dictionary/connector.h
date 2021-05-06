#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "headers.h"

class Connector
{
public:

    // Abstract Methods = To be overridden
    virtual bool isAvailable() = 0;
    virtual QString search(const int mode, QString word) = 0;
    virtual bool addDefinition(int mode,QString word, QStringList meanings) = 0;
    virtual QStringList getMatchingWords(const int mode, QString word) = 0;

    // Local Variables =
    static QString generateRegEx(QString word);
    static QStringList bloomFilter(QString word);
    static QString complexFilter(QString word);

    static const int ENGLISH_TO_SINHALA = 0;
    static const int SINHALA_TO_ENGLISH = 1;

};

#endif // CONNECTOR_H
