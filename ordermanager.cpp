#include "ordermanager.h"
#include "itemmanager.h"
#include "order.h"
#include "ui_ordermanager.h"

#include <QTime>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 450 << 630;
    ui->splitter->setSizes(sizes);
}

OrderManager::~OrderManager()
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("orderConnection");
    if (db.isOpen()) {
        qm->submitAll();
        delete qm;
        db.close();
    }
}

void OrderManager::loadData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "orderConnection");
    db.setDatabaseName("order.db");
    if (db.open()) {
        QSqlQuery query(db);
        query.exec("create table if not exists order_app(\
                   order_id            number(10),\
                   order_date          date,\
                   order_client_name   varchar2(20),\
                   order_item_name     varchar2(20),\
                   order_count         number(10),\
                   order_amount        number(20),\
                   primary key(order_id));");

        qm = new QSqlTableModel(this,db);
        qm->setTable("order_app");
        qm->select();
        qm->setHeaderData(0, Qt::Horizontal, QObject::tr("Order_ID"));
        qm->setHeaderData(1, Qt::Horizontal, QObject::tr("Order_Date"));
        qm->setHeaderData(2, Qt::Horizontal, QObject::tr("Client_Name"));
        qm->setHeaderData(3, Qt::Horizontal, QObject::tr("Item_Name"));
        qm->setHeaderData(4, Qt::Horizontal, QObject::tr("Order_Count"));
        qm->setHeaderData(5, Qt::Horizontal, QObject::tr("Order_Amount"));

        sqm = new QSqlTableModel(this,db);
        sqm->setTable("order_app");
        sqm->setHeaderData(0, Qt::Horizontal, QObject::tr("Order_ID"));
        sqm->setHeaderData(1, Qt::Horizontal, QObject::tr("Order_Date"));
        sqm->setHeaderData(2, Qt::Horizontal, QObject::tr("Client_Name"));
        sqm->setHeaderData(3, Qt::Horizontal, QObject::tr("Item_Name"));
        sqm->setHeaderData(4, Qt::Horizontal, QObject::tr("Order_Count"));
        sqm->setHeaderData(5, Qt::Horizontal, QObject::tr("Order_Amount"));

        ui->orderTreeView->setModel(qm);
        ui->searchTreeView->setModel(sqm);

//        itemIdNameMap[i_id] = i_name;
//        itemIdPriceMap[i_id] = ui->orderPricelineEdit->text().toInt();
    }
}

int OrderManager::makeId()
{
    if(qm->rowCount() == 0) {
        return 1;
    } else {
        auto id = qm->data(qm->index(qm->rowCount()-1,0)).toInt();
        return ++id;
    }
}

void OrderManager::getClientIdData(int id, QString name, QString addr, QString phonenum, QString type)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->clientTreeWidget);
    item->setText(0,QString::number(id));
    item->setText(1,name);
    item->setText(2,addr);
    item->setText(3,phonenum);
    item->setText(4,type);
    ui->clientTreeWidget->addTopLevelItem(item);
}

void OrderManager::on_clientLineEdit_returnPressed()
{
    ui->clientTreeWidget->clear();

    int i = ui->clientComboBox->currentIndex();

    if (i == 0) {
        int id = ui->clientLineEdit->text().toInt();
        emit clientDataSent(id);
    }
    else {
        QString name = ui->clientLineEdit->text();

        emit clientDataSent(i,name);
    }
}

void OrderManager::getItemIdData(int id, QString name, QString cat, QString color, int stock, int price)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->itemTreeWidget);
    item->setText(0,QString::number(id));
    item->setText(1,name);
    item->setText(2,cat);
    item->setText(3,color);
    item->setText(4,QString::number(stock));
    item->setText(5,QString::number(price));
    ui->itemTreeWidget->addTopLevelItem(item);
}

void OrderManager::on_itemLineEdit_returnPressed()
{
    ui->itemTreeWidget->clear();

    int i = ui->itemComboBox->currentIndex();

    if (i == 0) {
        int id = ui->itemLineEdit->text().toInt();
        emit itemDataSent(id);
    }
    else {
        QString str = ui->itemLineEdit->text();
        emit itemDataSent(i,str);
    }
}


void OrderManager::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    QTreeWidgetItem* c = ui->clientTreeWidget->currentItem();
    if(c != nullptr)
    {
        ui->orderClientIdlineEdit->setText(item->text(0));
    }
}


void OrderManager::on_itemTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    QTreeWidgetItem* c = ui->itemTreeWidget->currentItem();
    if(c != nullptr)
    {
        ui->orderItemIdlineEdit->setText(item->text(0));
        ui->orderPricelineEdit->setText(item->text(5));
    }
}


void OrderManager::on_orderQuantitylineEdit_returnPressed()
{
    int q = ui->orderQuantitylineEdit->text().toInt();

    int amount = ui->orderPricelineEdit->text().toInt() * q;

    ui->orderAmountlineEdit->setText(QString::number(amount));
}


void OrderManager::on_orderPushButton_clicked()
{
    int id = makeId();
    qDebug() << id;
    date = QTime::currentTime().toString();

    // Client Name, Item Name
    int c_id = ui->orderClientIdlineEdit->text().toInt();
    int i_id = ui->orderItemIdlineEdit->text().toInt();
    QString c_name, i_name;

    foreach(auto v, ui->clientTreeWidget->findItems(QString::number(c_id),Qt::MatchCaseSensitive)){
        QTreeWidgetItem* item = v;
        c_name = item->text(1);
    }
    foreach(auto v, ui->itemTreeWidget->findItems(QString::number(i_id),Qt::MatchCaseSensitive)){
        QTreeWidgetItem* item = v;
        i_name = item->text(1);
    }
    // Qunatity
    int q = ui->orderQuantitylineEdit->text().toInt();
    // Amount
    int amount = ui->orderPricelineEdit->text().toInt() * q;

    ui->orderClientIdlineEdit->clear();
    ui->orderItemIdlineEdit->clear();
    ui->orderQuantitylineEdit->clear();
    ui->orderPricelineEdit->clear();
    ui->orderAmountlineEdit->clear();

    QSqlDatabase db = QSqlDatabase::database("orderConnection");
    QSqlQuery query(db);
    query.prepare("insert into order_app values (:id, :date, :c_name, :i_name, :q, :amount)");
    query.bindValue(":id",id);
    query.bindValue(":date",date);
    query.bindValue(":c_name",c_name);
    query.bindValue(":i_name",i_name);
    query.bindValue(":q",q);
    query.bindValue(":amount",amount);
    query.exec();
    qm->select();
}

void OrderManager::on_SearchPushButton_clicked()
{
//    ui->searchTreeWidget->clear();

//    int i = ui->SearchComboBox->currentIndex();
//    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
//                   : Qt::MatchCaseSensitive;
//    {
//        auto items = ui->orderTreeWidget->findItems(ui->searchLineEdit->text(),flag,i);

//        foreach(auto i, items)
//        {
//            Order* o = static_cast<Order*>(i);
//            int orderNum = o->orderNum();
//            ui->searchTreeWidget->addTopLevelItem(orderList[orderNum]);
//        }
//    }

//    foreach (auto v , ui->orderTreeWidget->findItems("",Qt::MatchContains)) {
//        QTreeWidgetItem* item = v;
//        qDebug() << item->text(0);
////        out << item->text(0) << ", " << item->text(1) << ", ";
////        out << item->text(2) << ", " << item->text(3) << ", ";
////        out << item->text(4) << ", " << item->text(5) << "\n";
//    }

}


void OrderManager::on_clearPushButton_clicked()
{
    ui->orderClientIdlineEdit->clear();
    ui->orderItemIdlineEdit->clear();
    ui->orderQuantitylineEdit->clear();
    ui->orderPricelineEdit->clear();
    ui->orderAmountlineEdit->clear();
}

void OrderManager::on_modifyPushButton_clicked()
{
    int o_id = qm->data(qm->index(ui->orderTreeView->currentIndex().row(),0)).toInt();
    int c_id = ui->orderClientIdlineEdit->text().toInt();
    int i_id = ui->orderItemIdlineEdit->text().toInt();
    int q = ui->orderQuantitylineEdit->text().toInt();
    int price = itemIdPriceHash[i_id];
    int amount = q * price;
    qDebug() << "price" << price;

    QString c_name = clientIdNameHash[c_id];
    QString i_name = itemIdNameHash[i_id];
    qDebug() << "c_name" << c_name << "i_name" << i_name;
    QSqlDatabase db = QSqlDatabase::database("orderConnection");
    QSqlQuery query(db);
    query.prepare("update order_app set order_client_name = :c_name, order_item_name = :i_name,\
                   order_count = :q, order_amount = :amount\
                   where order_id = :o_id");
    query.bindValue(":c_name",c_name);
    query.bindValue(":i_name",i_name);
    query.bindValue(":q",q);
    query.bindValue(":amount",amount);
    query.bindValue(":o_id",o_id);
    query.exec();
    qm->select();
}


void OrderManager::on_RemovePushButton_clicked()
{
//    QTreeWidgetItem* item = ui->orderTreeWidget->currentItem();
//    if(item != nullptr) {
//        orderList.remove(item->text(0).toInt());
//        ui->orderTreeWidget->takeTopLevelItem(ui->orderTreeWidget->indexOfTopLevelItem(item));
//        ui->orderTreeWidget->update();
//    }
}


void OrderManager::on_orderTreeView_clicked(const QModelIndex &index)
{
    QModelIndex idx = ui->orderTreeView->currentIndex();
    QString c_name = qm->data(qm->index(idx.row(),2)).toString();
    QString i_name = qm->data(qm->index(idx.row(),3)).toString();
    emit clickedClientName(c_name);
    emit clickedItemName(i_name);
    int c_id = client_id;
    int i_id = item_id;
    int q = qm->data(qm->index(idx.row(),4)).toInt();
    int amount = qm->data(qm->index(idx.row(),5)).toInt();
    int price = amount / q;

    ui->orderClientIdlineEdit->setText(QString::number(c_id));
    ui->orderItemIdlineEdit->setText(QString::number(i_id));
    ui->orderPricelineEdit->setText(QString::number(price));
    ui->orderQuantitylineEdit->setText(QString::number(q));
    ui->orderAmountlineEdit->setText(QString::number(amount));
}

void OrderManager::getClientId(int id)
{
    client_id =  id;
}

void OrderManager::getItemId(int id)
{
    item_id = id;
}


void OrderManager::on_orderItemIdlineEdit_returnPressed()
{
    QString arg1 = ui->orderItemIdlineEdit->text();
    ui->orderPricelineEdit->setText(QString::number(itemIdPriceHash[arg1.toInt()]));
}

void OrderManager::getClientIdName(int id, QString name)
{
    clientIdNameHash[id] = name;
    qDebug() << clientIdNameHash;
}

void OrderManager::getItemIdName(int id, QString name, int price)
{
    itemIdNameHash[id] = name;
    itemIdPriceHash[id] = price;
    qDebug() << itemIdNameHash;
}
