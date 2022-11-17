#include "itemmanager.h"
#include "qsqlerror.h"
#include "ui_itemmanager.h"
#include "item.h"

#include <QFile>
#include <QTime>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlDatabase>

ItemManager::ItemManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemManager)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 300 << 660;
    ui->splitter->setSizes(sizes);
}

ItemManager::~ItemManager()
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("itemConnection");
    if (db.isOpen()) {
        qm->submitAll();
        delete qm;
        db.close();
    }
}

void ItemManager::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","itemConnection");
    db.setDatabaseName("item.db");
    if (db.open()) {
        QSqlQuery query(db);
        query.exec("create table if not exists item(\
                   item_id         number(10),\
                   item_name       varchar2(100) not null,\
                   item_cat        varchar2(100),\
                   item_color      varchar2(100),\
                   item_stock      number(20),\
                   item_price      number(20),\
                   primary key(item_id));");

    qm = new QSqlTableModel(this,db);
    qm->setTable("item");
    qm->select();                                                                        // select * from item;
    qm->setHeaderData(0, Qt::Horizontal, QObject::tr("Item_ID"));
    qm->setHeaderData(1, Qt::Horizontal, QObject::tr("Item_Name"));
    qm->setHeaderData(2, Qt::Horizontal, QObject::tr("Item_Cat"));
    qm->setHeaderData(3, Qt::Horizontal, QObject::tr("Item_Color"));
    qm->setHeaderData(4, Qt::Horizontal, QObject::tr("Item_Stock"));
    qm->setHeaderData(5, Qt::Horizontal, QObject::tr("Item_Price"));

    sqm = new QSqlTableModel(this,db);
    sqm->setTable("item");
    sqm->setHeaderData(0, Qt::Horizontal, QObject::tr("Item_ID"));
    sqm->setHeaderData(1, Qt::Horizontal, QObject::tr("Item_Name"));
    sqm->setHeaderData(2, Qt::Horizontal, QObject::tr("Item_Cat"));
    sqm->setHeaderData(3, Qt::Horizontal, QObject::tr("Item_Color"));
    sqm->setHeaderData(4, Qt::Horizontal, QObject::tr("Item_Stock"));
    sqm->setHeaderData(5, Qt::Horizontal, QObject::tr("Item_Price"));

    ui->ItemTreeView->setModel(qm);
    ui->ItemSearchTreeView->setModel(sqm);

    for(int i=0; i < qm->rowCount(); i++) {
        int id = qm->data(qm->index(i,0)).toInt();
        QString name = qm->data(qm->index(i,1)).toString();
        int price = qm->data(qm->index(i,5)).toInt();
        emit sendItemIdName(id,name,price);
    }
    }
}

int ItemManager::makeId()
{
    if(qm->rowCount() == 0) {
        return 880000;
    } else {
        auto id = qm->data(qm->index(qm->rowCount()-1,0)).toInt();
        return ++id;
    }
}

void ItemManager::on_AddPushButton_clicked()
{
    QString name, categori, color, stock, price;
    int id = makeId();
    name = ui->InputNameLineEdit->text();
    categori = ui->InputCategoriLineEdit->text();
    color = ui->InputColorLineEdit->text();
    stock = ui->InputStockLineEdit->text();
    price = ui->priceLineEdit->text();
    emit itemAdded(name);

    addLogList.insert(id,QTime::currentTime().toString());

    ui->TimeListWidget->clear();

    ui->InputIdLineEdit->clear();
    ui->InputNameLineEdit->clear();
    ui->InputCategoriLineEdit->clear();
    ui->InputColorLineEdit->clear();
    ui->InputStockLineEdit->clear();
    ui->priceLineEdit->clear();

    ui->TimeListWidget->addItem("Added Time : "+QTime::currentTime().toString());

    QSqlDatabase db = QSqlDatabase::database("itemConnection");
    QSqlQuery query(db);
    query.prepare("insert into item values (:id, :name, :cat, :color, :stock, :price)");
    query.bindValue(":id",id);
    query.bindValue(":name",name);
    query.bindValue(":cat",categori);
    query.bindValue(":color",color);
    query.bindValue(":stock",stock);
    query.bindValue(":price",price);
    query.exec();
    qm->select();
}


void ItemManager::on_ModifyPushButton_clicked()
{
    QString name, categori, color, stock, price;
    int id = ui->InputIdLineEdit->text().toInt();
    name = ui->InputNameLineEdit->text();
    categori = ui->InputCategoriLineEdit->text();
    color = ui->InputColorLineEdit->text();
    stock = ui->InputStockLineEdit->text();
    price = ui->priceLineEdit->text();

    ui->InputIdLineEdit->clear();
    ui->InputNameLineEdit->clear();
    ui->InputCategoriLineEdit->clear();
    ui->InputColorLineEdit->clear();
    ui->InputStockLineEdit->clear();
    ui->priceLineEdit->clear();

    logTimeList.insert(id,QTime::currentTime().toString());

    QSqlDatabase db = QSqlDatabase::database("itemConnection");
    QSqlQuery query(db);
    query.prepare("update item set item_name = :name, item_cat = :cat,\
                  item_color = :color,\
                  item_stock = :stock,\
                  item_price = :price\
                  where item_id = :i_id");
    query.bindValue(":name", name);
    query.bindValue(":cat", categori);
    query.bindValue(":color", color);
    query.bindValue(":stock", stock);
    query.bindValue(":price", price);
    query.bindValue(":i_id", id);
    query.exec();
    qm->select();
}


void ItemManager::on_RemovePushButton_clicked()
{
    int id = ui->InputIdLineEdit->text().toInt();

    ui->InputIdLineEdit->clear();
    ui->InputNameLineEdit->clear();
    ui->InputCategoriLineEdit->clear();
    ui->InputColorLineEdit->clear();
    ui->InputStockLineEdit->clear();
    ui->priceLineEdit->clear();

    QSqlDatabase db = QSqlDatabase::database("itemConnection");
    QSqlQuery query(db);
    query.prepare("delete from item where item_id = :id;");
    query.bindValue(":id",id);
    query.exec();
    qm->select();
}


void ItemManager::on_SearchPushButton_clicked()
{
    int i = ui->SearchComboBox->currentIndex();
    QString check = ui->InputLineEdit->text();
    QSqlDatabase db = QSqlDatabase::database("itemConnection");

    switch (i) {
    case 0:
        sqm->QSqlQueryModel::setQuery(QString("select * from item where item_id = %1").arg(check),db);
        break;
    case 1:
        sqm->QSqlQueryModel::setQuery(QString("select * from item where item_name like '%%1%'").arg(check),db);
        break;
    case 2:
        sqm->QSqlQueryModel::setQuery(QString("select * from item where item_cat like '%%1%'").arg(check),db);
        break;
    case 3:
        sqm->QSqlQueryModel::setQuery(QString("select * from item where item_color like '%%1%'").arg(check),db);
        break;
    default:
        break;
    }
}


void ItemManager::itemIdListData(int id)
{
    QModelIndexList idx = qm->match(qm->index(0,0),Qt::EditRole, id, -1, Qt::MatchFlag(Qt::MatchCaseSensitive));

    foreach(auto index, idx) {
        int id = qm->data(index.siblingAtColumn(0)).toInt();
        QString name = qm->data(index.siblingAtColumn(1)).toString();
        QString cat = qm->data(index.siblingAtColumn(2)).toString();
        QString color = qm->data(index.siblingAtColumn(3)).toString();
        int stock = qm->data(index.siblingAtColumn(4)).toInt();
        int price = qm->data(index.siblingAtColumn(5)).toInt();
        emit sendItemData(id,name,cat,color,stock,price);
    }
}

void ItemManager::itemNameListData(int i,QString cstr)                        // 클라이언트 이름을 가지고 클라이언트 정보 추출
{
    if (i==1) {
        QModelIndexList idx = qm->match(qm->index(0,1),Qt::EditRole,cstr,-1,Qt::MatchFlag(Qt::MatchContains));

        foreach(auto index, idx) {
            int id = qm->data(index.siblingAtColumn(0)).toInt();
            QString name = qm->data(index.siblingAtColumn(1)).toString();
            QString cat = qm->data(index.siblingAtColumn(2)).toString();
            QString color = qm->data(index.siblingAtColumn(3)).toString();
            int stock = qm->data(index.siblingAtColumn(4)).toInt();
            int price = qm->data(index.siblingAtColumn(5)).toInt();
            emit sendItemData(id,name,cat,color,stock,price);
        }
    }
    else if (i==2) {
        QModelIndexList idx = qm->match(qm->index(0,2),Qt::EditRole,cstr,-1,Qt::MatchFlag(Qt::MatchContains));

        foreach(auto index, idx) {
            int id = qm->data(index.siblingAtColumn(0)).toInt();
            QString name = qm->data(index.siblingAtColumn(1)).toString();
            QString cat = qm->data(index.siblingAtColumn(2)).toString();
            QString color = qm->data(index.siblingAtColumn(3)).toString();
            int stock = qm->data(index.siblingAtColumn(4)).toInt();
            int price = qm->data(index.siblingAtColumn(5)).toInt();
            emit sendItemData(id,name,cat,color,stock,price);
        }
    }
    else if (i==3) {
        QModelIndexList idx = qm->match(qm->index(0,3),Qt::EditRole,cstr,-1,Qt::MatchFlag(Qt::MatchContains));

        foreach(auto index, idx) {
            int id = qm->data(index.siblingAtColumn(0)).toInt();
            QString name = qm->data(index.siblingAtColumn(1)).toString();
            QString cat = qm->data(index.siblingAtColumn(2)).toString();
            QString color = qm->data(index.siblingAtColumn(3)).toString();
            int stock = qm->data(index.siblingAtColumn(4)).toInt();
            int price = qm->data(index.siblingAtColumn(5)).toInt();
            emit sendItemData(id,name,cat,color,stock,price);
        }
    }
}

void ItemManager::on_ItemTreeView_clicked(const QModelIndex &index)
{
    QModelIndex idx = ui->ItemTreeView->currentIndex();
    ui->InputIdLineEdit->setText(QString::number(idx.sibling(idx.row(),0).data().toInt()));
    ui->InputNameLineEdit->setText(idx.sibling(idx.row(),1).data().toString());
    ui->InputCategoriLineEdit->setText(idx.sibling(idx.row(),2).data().toString());
    ui->InputColorLineEdit->setText(idx.sibling(idx.row(),3).data().toString());
    ui->InputStockLineEdit->setText(idx.sibling(idx.row(),4).data().toString());
    ui->priceLineEdit->setText(idx.sibling(idx.row(),5).data().toString());
}

void ItemManager::getItemName(QString i_name)
{
    QModelIndexList idx = qm->match(qm->index(0,1),Qt::EditRole, i_name, -1, Qt::MatchFlag(Qt::MatchCaseSensitive));

    foreach(auto index,idx) {
        int id = qm->data(index.siblingAtColumn(0)).toInt();
        emit sendItemId(id);
    }
}
