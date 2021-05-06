#include "addword.h"
#include "ui_addword.h"

AddWord::AddWord(QWidget *parent,Dictionary* dictionary) :
    QDialog(parent,Qt::FramelessWindowHint),
    ui(new Ui::AddWord)
{
    ui->setupUi(this);

    this->dictionary = dictionary;

    isEnToSi = true;

    stdModel = new QStandardItemModel(this);

    ui->lstMeanings->setModel(stdModel);
}

AddWord::~AddWord()
{
    delete ui;
}

void AddWord::setWord(QString word,bool isEnToSi)
{
    this->word = word;
    this->isEnToSi = isEnToSi;
    changeUpDownIcon();
    ui->txtWord->setText(word);
}

void AddWord::on_btnAdd_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Definition"),tr("Definition:"), QLineEdit::Normal,"", &ok);
    if(ok){
        QStandardItem* i = new QStandardItem(text.trimmed());
        stdModel->appendRow(i);
    }
}

void AddWord::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void AddWord::mouseMoveEvent(QMouseEvent *event) {
    int x = event->globalX()-m_nMouseClick_X_Coordinate;
    int y = event->globalY()-m_nMouseClick_Y_Coordinate;

    move(x,y);
}

void AddWord::changeUpDownIcon()
{
    if(!isEnToSi){
        ui->btnEnSi->setIcon(QIcon("left.png"));
    }else{
        ui->btnEnSi->setIcon(QIcon("right.png"));
    }
}

void AddWord::on_btnEnSi_clicked()
{
    isEnToSi = !isEnToSi;
    changeUpDownIcon();
}

void AddWord::on_btnClose_clicked()
{
    this->~AddWord();
}

void AddWord::on_btnRemove_clicked()
{
    QModelIndexList lst = ui->lstMeanings->selectionModel()->selectedRows();
    if(lst.size()>0){
        QModelIndex i = lst.at(0);

        stdModel->removeRow(i.row());
    }
}


void AddWord::on_btnAddDef_clicked()
{
    QStringList list;
    for (int var = 0; var < stdModel->rowCount(); ++var) {
        QStandardItem* i = stdModel->item(var);
        list.append(i->text());
        //cout<<i->text().toStdString()<<endl;
    }

    if(dictionary->addDefinitions(isEnToSi?0:1,ui->txtWord->text(),list)){
        cout<<"Done"<<endl;
        ui->txtWord->setText("");
        stdModel->clear();
    }else{
        cout<<"Error"<<endl;
    }
}
