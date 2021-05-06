#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    animationSpeed = 300;

    ui->setupUi(this);

    isExpanded = false;
    isEnToSi = true;
    foundMeanings = true;

    meaningHeight = 180;

    initialHeight = this->height();
    initialWidth = this->width();

    QDesktopWidget desktop;

    QRect scr = desktop.availableGeometry(desktop.primaryScreen());

    int desktopHeight = scr.height();
    int desktopWidth  = scr.width();

    initialX = (desktopWidth - initialWidth)/2;
    initialY = (desktopHeight - initialHeight)/2;

    initialSearchX = ui->searchFrame->x();
    initialSearchY = ui->searchFrame->y();

    initialMeaningX = ui->meaning->x();
    initialMeaningY = ui->meaning->y();

    dictionary = new Dictionary(Dictionary::DATABSE, "d:/db.db");

    ui->searchText->setFocus();

    ui->lstSuggestions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->btnWarn->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSearch_clicked()
{
    initialX = this->x();
    initialY = this->y();

    if (dictionary->isAvailable()){
        QString word = ui->searchText->text();
        if(!word.isEmpty()){
            search(word);
        }else{

        }
    }
}

void MainWindow::on_searchText_returnPressed()
{
    on_btnSearch_clicked();
}

void MainWindow::resize(){

    initialX = this->x();
    initialY = this->y();

    animation = new QPropertyAnimation(ui->searchFrame, "geometry");
    animation->setDuration(animationSpeed);
    animation->setEndValue(QRect(initialSearchX, initialSearchY , ui->searchFrame->width(), ui->searchFrame->height()));
    animation->start();

    ui->meaning->hide();
    ui->meaning->setGeometry(ui->meaning->x(),ui->meaning->y(),ui->meaning->width(),0);
    ui->suggestionFrame->hide();
    ui->suggestionFrame->setGeometry(ui->suggestionFrame->x(),ui->suggestionFrame->y(),ui->suggestionFrame->width(),0);

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(animationSpeed);
    animation->setEndValue(QRect(initialX, initialY + meaningHeight, this->width(), initialHeight));
    animation->start();

    ui->btnResize->setIcon(QIcon("up.png"));

}

void MainWindow::expandSuggestions()
{
    initialX = this->x();
    initialY = this->y();

    animation = new QPropertyAnimation(ui->searchFrame, "geometry");
    animation->setDuration(2*animationSpeed);
    animation->setEndValue(QRect(initialSearchX, initialSearchY + meaningHeight + 5, ui->searchFrame->width(), ui->searchFrame->height()));
    animation->start();

    ui->meaning->hide();
    ui->suggestionFrame->setGeometry(ui->suggestionFrame->x(),ui->suggestionFrame->y(),ui->suggestionFrame->width(),meaningHeight);
    ui->suggestionFrame->show();

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(animationSpeed);
    int targetHeight = ui->topFrame->height() + ui->searchFrame->height() + meaningHeight;
    animation->setEndValue(QRect(initialX, initialY - meaningHeight, this->width(), targetHeight));

    animation->start();

    ui->btnResize->setIcon(QIcon("down.png"));
}

void MainWindow::search(QString word)
{
    QString m = "";
    bool haveResults = true;
    if(isEnToSi){
        m = dictionary->search(Connector::ENGLISH_TO_SINHALA, word);
    }else{
        m = dictionary->search(Connector::SINHALA_TO_ENGLISH, word);
    }
    if(m != NULL){
        this->fillMeanings(dictionary->decodeMeanings(m));
        ui->btnWarn->hide();
        foundMeanings = true;
        if(!isExpanded){
            expandMeanings();
            isExpanded = true;
        }else{
            ui->suggestionFrame->hide();
            ui->meaning->show();
            ui->meaning->setGeometry(ui->meaning->x(),ui->meaning->y(),ui->meaning->width(),meaningHeight);
        }
    }else{
        ui->meaning->setPlainText("<< No Meanings Found..>>");
        foundMeanings = false;
        qDebug()<<"No meanings.";
        ui->btnWarn->show();
        QStringList list;
        if(isEnToSi){
            list = dictionary->getMatchingWords(Connector::ENGLISH_TO_SINHALA, word);
        }else{
            list = dictionary->getMatchingWords(Connector::SINHALA_TO_ENGLISH, word);
        }
        fillSuggestions(list);
        if(list.size()<1){
            haveResults = false;
        }else{
            if(!isExpanded){
                expandSuggestions();
                isExpanded = true;
            }else{
                ui->suggestionFrame->show();
                ui->meaning->hide();
                ui->suggestionFrame->setGeometry(ui->suggestionFrame->x(),ui->suggestionFrame->y(),ui->suggestionFrame->width(),meaningHeight);
            }
        }
    }
    QPalette *palette = new QPalette();


    if(haveResults){
         palette->setColor(QPalette::Text,Qt::black);
    }else{
         palette->setColor(QPalette::Text,Qt::red);
    }
    ui->searchText->setPalette(*palette);
}

void MainWindow::changeWindowSize()
{
    if(isExpanded){
        resize();
        isExpanded = false;

    }else{
        if(foundMeanings)
            expandMeanings();
        else
            expandSuggestions();
        isExpanded = true;
    }
}

void MainWindow::expandMeanings()
{
    initialX = this->x();
    initialY = this->y();

    animation = new QPropertyAnimation(ui->searchFrame, "geometry");
    animation->setDuration(2*animationSpeed);
    animation->setEndValue(QRect(initialSearchX, initialSearchY + meaningHeight + 5, ui->searchFrame->width(), ui->searchFrame->height()));
    animation->start();

    ui->suggestionFrame->hide();
    ui->meaning->setGeometry(ui->meaning->x(),ui->meaning->y(),ui->meaning->width(),meaningHeight);
    ui->meaning->show();

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(animationSpeed);
    int targetHeight = ui->topFrame->height() + ui->searchFrame->height() + meaningHeight;
    animation->setEndValue(QRect(initialX, initialY - meaningHeight, this->width(), targetHeight));

    animation->start();

    ui->btnResize->setIcon(QIcon("down.png"));

}

void MainWindow::fillMeanings(QStringList list)
{
    QString text = "";
    for (int var = 0; var < list.size()-1; ++var) {
        QString str = list.at(var).trimmed();
        if(str.length()>0)
            text+=str.append("\n");
    }
    QString str = list.at(list.size()-1).trimmed();
    if(str.length()>0)
        text+=str;
    ui->meaning->setPlainText(text);
}

void MainWindow::fillSuggestions(QStringList list)
{
    QStandardItemModel *stdModel = new QStandardItemModel(this);
    foreach (QString w , list) {
        QStandardItem* i = new QStandardItem(w);
        stdModel->appendRow(i);
    }
    ui->lstSuggestions->setModel(stdModel);
    QString sug = "No meanings were found. Showing ";
    sug.append(QString::number(list.size()));
    sug.append(" suggestion");
    if(list.size()>1){
        sug.append("s");
    }
    sug.append(".");
    ui->lblSuggestions->setText(sug);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    int x = event->globalX()-m_nMouseClick_X_Coordinate;
    int y = event->globalY()-m_nMouseClick_Y_Coordinate;

    move(x,y);
    initialX = x;
    initialY = y;
}


void MainWindow::on_btnResize_clicked()
{
    changeWindowSize();
}

void MainWindow::on_btnClose_clicked()
{
    exit(0);
}


void MainWindow::on_btnEnSi_clicked()
{
    if(isEnToSi){
        ui->btnEnSi->setIcon(QIcon("left.png"));
        isEnToSi = false;
    }else{
        ui->btnEnSi->setIcon(QIcon("right.png"));
        isEnToSi = true;
    }
}

void MainWindow::on_btnWarn_clicked()
{
    AddWord* add = new AddWord(this,dictionary);
    add->setWord(ui->searchText->text(),isEnToSi);
    add->show();
}

void MainWindow::on_lstSuggestions_doubleClicked(const QModelIndex &index)
{
    QString word = index.data(Qt::DisplayRole).toString();
    search(word);
}

void MainWindow::on_btnAddWord_clicked()
{
    AddWord* add = new AddWord(this,dictionary);
    add->setWord(QString(""),isEnToSi);
    add->show();
}
