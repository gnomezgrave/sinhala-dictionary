#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "dbconnector.h"
#include "restconnector.h"

class Dictionary
{
public:
    Dictionary(int mode, QString path);
    QString search(const int mode, QString word);

    QStringList getMatchingWords(const int mode, QString word);

    bool isAvailable();

    QStringList decodeMeanings(QString meaning);

    bool addDefinitions(int mode, QString word,QStringList defs);

    static const int DATABSE = 0;
    static const int ONLINE = 0;


private:
    Connector* con;
};

#endif // DICTIONARY_H
