#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include <QWidget>
#include <QHash>

class Item;
class QTreeWidgetItem;
class QSqlTableModel;

namespace Ui {
class ItemManager;
}

class ItemManager : public QWidget
{
    Q_OBJECT

public:
    explicit ItemManager(QWidget *parent = nullptr);
    ~ItemManager();

    void loadData();
    int makeId();
public slots:
    void itemNameListData(int, QString);
    void itemIdListData(int);

    void getItemName(QString);

signals:
    void itemAdded(QString);

    void sendItemData(int,QString,QString,QString,int,int);
    void itemNameDataSent(Item*,QTreeWidgetItem*);

    void sendItemId(int);
    void sendItemIdName(int,QString,int);

private slots:
    void on_AddPushButton_clicked();
    void on_ModifyPushButton_clicked();
    void on_RemovePushButton_clicked();
    void on_SearchPushButton_clicked();

//    void on_ItemTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_ItemTreeView_clicked(const QModelIndex &index);

private:
    Ui::ItemManager *ui;

    QMap<int, Item*> itemList;

    QMap<int, QString> addLogList;
    QMultiMap<int, QString> logTimeList;
    QSqlTableModel *qm;
    QSqlTableModel *sqm;
};

#endif // ITEMMANAGER_H
