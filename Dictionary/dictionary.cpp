#include "dictionary.h"

Dictionary::Dictionary(int mode,QString path)
{
    if(mode == DATABSE){
        con = new DBConnector(path);
    }else if (mode == ONLINE){
        con = new RESTConnector(path);
    }
}

QString Dictionary::search(const int mode, QString word){
    return con->search(mode,word);
}

QStringList Dictionary::getMatchingWords(const int mode, QString word)
{
    return con->getMatchingWords(mode, word);
}

bool Dictionary::isAvailable(){
    return con->isAvailable();
}

QStringList Dictionary::decodeMeanings(QString meaning){
    return meaning.split("|",QString::SkipEmptyParts);
}

bool Dictionary::addDefinitions(int mode, QString word, QStringList defs)
{
    if(isAvailable())
        return con->addDefinition(mode,word,defs);
}
