#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientmanager.h"
#include "itemmanager.h"
#include "ordermanager.h"
#include "chatserver.h"
#include "chatwindow.h"
#include "serverclientchat.h"
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!createConnection( )) return;

    clientManager = new ClientManager(this);
    ui->tabWidget->addTab(clientManager,"&Client");

    itemManager = new ItemManager(this);
    ui->tabWidget->addTab(itemManager,"&Item");


    orderManager = new OrderManager(this);
    ui->tabWidget->addTab(orderManager, "&Order");

    chatManager = new ChatServer(this);
    ui->tabWidget->addTab(chatManager,"&Chat Server");

    connect(clientManager,SIGNAL(clientAdded(int,QString)),chatManager,SLOT(showIdName(int,QString)));
    connect(clientManager,SIGNAL(clientRemove(int,int)),chatManager,SLOT(removeIdName(int,int)));

    //clientManager 에서 ID 검색 리스트를 OrderManager에 전달
    connect(clientManager, SIGNAL(sendClientIdData(int,QString,QString,QString,QString)),
            orderManager, SLOT(getClientIdData(int,QString,QString,QString,QString)));
    // Qstring 받아와서 clientManager에 전해줘서 리스트를 검색
    connect(orderManager,SIGNAL(clientDataSent(int)),clientManager,SLOT(clientIdListData(int))); // id
    connect(orderManager,SIGNAL(clientDataSent(int,QString)),clientManager,SLOT(clientNameListData(int,QString)));  // id 제외

    //itemManager 에서 검색한 리스트를 OrderManager에 전달
    connect(itemManager, SIGNAL(sendItemData(int,QString,QString,QString,int,int)),
            orderManager, SLOT(getItemIdData(int,QString,QString,QString,int,int)));
    // OrderManager에서 받아와서 itemManager에 전달
    connect(orderManager,SIGNAL(itemDataSent(int)),itemManager,SLOT(itemIdListData(int)));  // id
    connect(orderManager,SIGNAL(itemDataSent(int,QString)),itemManager,SLOT(itemNameListData(int,QString)));    // id 제외

    // 주문창에서 고객ID, 아이템ID를 이용해서 등록하면 오더리스트에 고객 이름, 아이템 이름으로 나오게 하는 커넥트
    connect(orderManager,SIGNAL(clientNameDataSent(int,QTreeWidgetItem*)),clientManager,SLOT(clientIdNameListData(int,QTreeWidgetItem*)));
    connect(clientManager,SIGNAL(clientNameDataSent(Client*,QTreeWidgetItem*)),orderManager,SLOT(showClientNameData(Client*,QTreeWidgetItem*)));
    connect(orderManager,SIGNAL(itemNameDataSent(int,QTreeWidgetItem*)),itemManager,SLOT(itemIdNameListData(int,QTreeWidgetItem*)));
    connect(itemManager,SIGNAL(itemNameDataSent(Item*,QTreeWidgetItem*)),orderManager,SLOT(showItemNameData(Item*,QTreeWidgetItem*)));

//    connect(chatManager,SIGNAL(serverToChat(int,QString)),serverClientChat,SLOT(clientIdToName(int,QString)));

    // 주문 리스트에서 아이템 클릭 시 회원이름, 상품이름을 가지고 clientManager,ItemManager에서 ID를 가져오는 커넥트
    connect(orderManager,SIGNAL(clickedClientName(QString)),clientManager,SLOT(getClientName(QString)));
    connect(clientManager,SIGNAL(sendClientId(int)),orderManager,SLOT(getClientId(int)));
    connect(orderManager,SIGNAL(clickedItemName(QString)),itemManager,SLOT(getItemName(QString)));
    connect(itemManager,SIGNAL(sendItemId(int)),orderManager,SLOT(getItemId(int)));

    // OrderManager 에서 ClientManager로부터 ID,NAME HASH 반환
    connect(clientManager,SIGNAL(sendClientIdName(int,QString)),orderManager,SLOT(getClientIdName(int,QString)));
    // OrderManager 에서 ItemManager로부터 ID,PRICE 반환
    connect(itemManager,SIGNAL(sendItemIdName(int,QString,int)),orderManager,SLOT(getItemIdName(int,QString,int)));

    itemManager->loadData();
    orderManager->loadData();
    clientManager->loadData();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete clientManager;
    delete itemManager;
    delete chatManager;
    delete orderManager;

    QStringList list = QSqlDatabase::connectionNames();
    for(int i=0; i<list.count(); ++i) {
        QSqlDatabase::removeDatabase(list[i]);
    }
}


void MainWindow::on_pushButton_clicked()
{
    chatWindow = new ChatWindow(nullptr);
    chatWindow->show();
}

bool MainWindow::createConnection()
{
//#if 0
//    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
//    db.setDatabaseName("Oraclex64");
//    db.setUserName("db_project");
//    db.setPassword("1234");
//#else
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    QSqlDatabase db2 = QSqlDatabase::addDatabase("QSQLITE");
//#endif
//    if (!db.open()) {
//        qDebug() << db.lastError().text();
//    } else {
//        qDebug("success");
//    }

    return true;
}
