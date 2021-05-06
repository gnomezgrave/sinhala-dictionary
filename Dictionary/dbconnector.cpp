#include "dbconnector.h"

#include <stdio.h>
#include <cstdio>

using namespace std;

DBConnector::DBConnector(QString path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

}
bool DBConnector::isAvailable()
{
    return db.open();
}

QString DBConnector::search(int mode, QString word)
{
    QSqlQuery* query;
    if(mode == ENGLISH_TO_SINHALA){
        query = new QSqlQuery(QString("select * from english where word = '%1'").arg(word));
    }else{
        query = new QSqlQuery(QString("select * from sinhala where word = '%1'").arg(word));
    }
    if(query->next()){
        QString q = query->value(1).toString();
        return q;
    }else{
        qDebug() << "Error "<< query->lastError() ;
        qDebug() << "Error "<< query->lastQuery();
    }
    return NULL;
}

bool DBConnector::addDefinition(int mode, QString word, QStringList meanings)
{
    QSqlQuery* query;
    QString table = "english";
    if(mode == SINHALA_TO_ENGLISH){
        table = "sinhala";
    }
    query = new QSqlQuery(QString("select * from %1 where word = '%2'").arg(table).arg(word));

    if(query->next()){
        QMessageBox::StandardButton res;
        res = QMessageBox::question(NULL,"Duplicate Entry","The word that you're trying to add already exists.\nDo you want to update the definitions?",QMessageBox::Yes|QMessageBox::No);
        if(res == QMessageBox::Yes){
            QString m = query->value(1).toString();
            QStringList d = m.split("|");
            foreach (QString w,meanings) {
                w = w.trimmed();
                if(!d.contains(w)){
                    w =  QString("|").append(w);
                    m.append(w);
                }
            }
            bool ok = query->exec(QString("update %1 set meaning = '%2' where word = '%3'").arg(table).arg(m).arg(word));
            if(!ok){
                qDebug() << "Error Update "<< query->lastError() ;
                qDebug() << "Error Update "<< query->lastQuery();
                return false;
            }else{
                query = new QSqlQuery(QString("select * from user_%1 where word = '%2'").arg(table).arg(word));
                cout<<"Inmnnnn "<<endl;
                if(query->next()){
                    QString m = query->value(1).toString();
                    QStringList d = m.split("|");
                    foreach (QString w,meanings) {
                        w = w.trimmed();
                        if(!d.contains(w)){
                            w =  QString("|").append(w);
                            m.append(w);
                        }
                    }
                    bool ok = query->exec(QString("update user_%1 set meaning = '%2' where word = '%3'").arg(table).arg(m).arg(word));
                    if(!ok){
                        qDebug() << "Error Update "<< query->lastError() ;
                        qDebug() << "Error Update "<< query->lastQuery();
                        return false;
                    }
                }
                return true;
            }
        }else{
            QString m = "";
            for (int var = 0; var < meanings.size()-1; ++var) {
                m = m.append(meanings.at(var)).append("|");
            }
            m = m.append(meanings.at(meanings.size()-1));
            bool ok = query->exec(QString("insert into %1 values('%2','%3')").arg(table).arg(word).arg(m));

            if(!ok){
                qDebug() << "Error Insert "<< query->lastError() ;
                qDebug() << "Error Insert "<< query->lastQuery();
                return false;
            }else{
                ok = query->exec(QString("insert into user_%1 values('%2','%3')").arg(table).arg(word).arg(m));
            }
            return true;
        }
        return true;
    }
}

QStringList DBConnector::getMatchingWords(const int mode, QString word)
{
    QSqlQuery* query;
    int maxWordLen = word.length() + 2;
    if(mode == ENGLISH_TO_SINHALA){
        query = new QSqlQuery(QString("select * from english where length(word)<=%1").arg(maxWordLen));
    }else{
        query = new QSqlQuery(QString("select * from sinhala where length(word)<=%1").arg(maxWordLen));
    }
    QStringList list;
    while(query->next()){
        QString q = query->value(0).toString();
        list.append(q);
    }

    QStringList bloomF = Connector::bloomFilter(word);
    QStringList tmp;
    if(list.size()>0){
        QString regexp = Connector::generateRegEx(word);
        tmp = list.filter(QRegularExpression(regexp));
    }

    if(word.length()>=3){
        QString b = generateBloomFilterRegExp(bloomF);
        /*
        if(word.length()>5){
            b.append("|");
            b.append(complexFilter(word));
        }*/
        QStringList blm = list.filter(QRegularExpression(b));
        tmp.append(blm);
    }
    list = tmp;
    cout<<list.size()<<" Suggestions."<<endl;

    for (int var = 0; var < list.length(); ++var) {
        cout<<list.at(var).toStdString()<<endl;
    }
    fflush(stdout);
    return list;
}

QString DBConnector::generateBloomFilterRegExp(QStringList list)
{
    QString regexp;

    for (int var = 0; var < list.size()-1; ++var) {
        regexp.append("^");
        regexp.append(list.at(var));
        regexp.append("$|");
    }
    regexp.append("^");
    regexp.append(list.at(list.size()-1));
    regexp.append("$");
    cout<<regexp.toStdString()<<endl;
    return regexp;
}




