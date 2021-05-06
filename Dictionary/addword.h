#ifndef ADDWORD_H
#define ADDWORD_H

using namespace std;

#include <stdio.h>
#include <iostream>

#include <QDialog>
#include <QInputDialog>
#include <QString>
#include <QMouseEvent>
#include <QStandardItem>
#include <QStandardItemModel>

#include "dictionary.h"

namespace Ui {
class AddWord;
}

class AddWord : public QDialog
{
    Q_OBJECT

public:
    explicit AddWord(QWidget *parent = 0,Dictionary* dictionary=NULL);
    ~AddWord();

    void setWord(QString word, bool isEnToSi);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void changeUpDownIcon();

    void on_btnAdd_clicked();

    void on_btnEnSi_clicked();

    void on_btnClose_clicked();

    void on_btnRemove_clicked();


    void on_btnAddDef_clicked();

private:
    Ui::AddWord *ui;
    bool isEnToSi;

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    QStandardItemModel *stdModel;

    QString word;
    Dictionary* dictionary;
};

#endif // ADDWORD_H
