#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QRect>
#include <QDebug>

#include "dictionary.h"
#include "addword.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSearch_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void fillMeanings(QStringList list);
    void fillSuggestions(QStringList list);

    void expandMeanings();
    void resize();
    void expandSuggestions();

    void search(QString word);

    void changeWindowSize();

    void on_btnResize_clicked();

    void on_btnClose_clicked();

    void on_searchText_returnPressed();

    void on_btnEnSi_clicked();

    void on_btnWarn_clicked();

    void on_lstSuggestions_doubleClicked(const QModelIndex &index);

    void on_btnAddWord_clicked();

private:
    Ui::MainWindow *ui;
    QPropertyAnimation* animation;

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    bool isExpanded;
    bool isEnToSi;
    bool foundMeanings;

    int animationSpeed;

    int meaningHeight;
    int initialX;
    int initialY;

    int initialHeight;
    int initialWidth;

    int initialSearchX;
    int initialSearchY;

    int initialMeaningX;
    int initialMeaningY;

    Dictionary* dictionary;

};

#endif // MAINWINDOW_H
