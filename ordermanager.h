#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "client.h"
#include "item.h"
#include "ui_ordermanager.h"
#include <QWidget>
#include <QMap>

class Order;
class QTreeWidgetItem;
class ClientManager;
class ItemManager;
class QTreeWidgetItem;
class QSqlTableModel;

namespace Ui {
class OrderManager;
}

class OrderManager : public QWidget
{
    Q_OBJECT

public:
    explicit OrderManager(QWidget *parent = nullptr);
    ~OrderManager();

    void loadData();

private slots:
    void on_clientLineEdit_returnPressed();
    void getClientIdData(int,QString,QString,QString,QString);
    void getItemIdData(int,QString,QString,QString,int,int);

    void on_itemLineEdit_returnPressed();
    void on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_itemTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_orderQuantitylineEdit_returnPressed();
    void on_orderPushButton_clicked();

    void on_SearchPushButton_clicked();

    void on_clearPushButton_clicked();

//    void on_orderTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_modifyPushButton_clicked();

    void on_RemovePushButton_clicked();

    void on_orderTreeView_clicked(const QModelIndex &index);

    void getClientId(int);
    void getItemId(int);

    void on_orderItemIdlineEdit_returnPressed();

    void getClientIdName(int,QString);
    void getItemIdName(int,QString,int);

signals:
    void clientDataSent(int,QString);
    void clientDataSent(int);

    void itemDataSent(int,QString);
    void itemDataSent(int);

    void clickedClientName(QString);
    void clickedItemName(QString);
private:
    Ui::OrderManager *ui;

    int makeId();
    int client_id;
    int item_id;
    QString date;
    QMap<int, QTreeWidget*> saveOrderList;
    QMap<int, Order*> orderList;
    QHash<int, QString> clientIdNameHash;
    QHash<int, QString> itemIdNameHash;
    QHash<int, int> itemIdPriceHash;
    QSqlTableModel *qm;
    QSqlTableModel *sqm;
};

#endif // ORDERMANAGER_H
