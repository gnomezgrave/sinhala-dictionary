#include "connector.h"
#include <bitset>


QString Connector::generateRegEx(QString word)
{
    QString reg = "^";
    reg.append(word);
    reg.append("|");

    //reg.append(word);
    //reg.append("$|");

    for (int var = 0; var < word.length(); ++var) {
        reg.append(word.at(var)).append("[a-z]?");
    }

    cout<<reg.toStdString()<<endl;
    fflush(stdout);
    return reg;
}

QStringList Connector::bloomFilter(QString word)
{
    QStringList output;

    int len = word.length();
    int bits = (-1 << len) ^ (0xFFFFFFFF);

    int v1=1;
    int v2=1;

    set<int> vals;
    for (int i = 0; i < len; ++i) {

        for (int j = 0; j < len; ++j) {
            int val = bits & (v1 | v2);
            vals.insert(val);
            v2 = v2 << 1;
        }
        v2 = 1;
        v1 = v1 << 1;
    }

    int c = 0;

    foreach (int i, vals) {
        QString final;
        for (int j = 0; j < len; ++j) {
            int v = ((1<<j) & i)>>j;
            v = v==1?0:1;
            if(v==1){
                final.append(word.at(j));
            }
        }
        //wcout<<"Bloom "<<final.toStdWString()<<endl;
        output.append(final);
        c++;
    }
    return output;
}

QString Connector::complexFilter(QString word)
{
    QStringList bloom = bloomFilter(word);
    QString regexp;
    for (int var = 0; var < bloom.size()-1; ++var) {
        regexp.append(generateRegEx(bloom.at(var)));
        regexp.append("|");
    }
    regexp.append(generateRegEx(bloom.at(bloom.size()-1)));
    cout<<regexp.toStdString()<<endl;

    return regexp;
}


