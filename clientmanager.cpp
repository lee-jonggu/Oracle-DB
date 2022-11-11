#include "clientmanager.h"
#include "client.h"
#include "ui_clientmanager.h"

#include <QFile>
#include <QMenu>
#include <time.h>
#include <QTime>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

ClientManager::ClientManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManager)
{
    ui->setupUi(this);

//    if (!createConnection()) return;

    QList<int> sizes;                                                       // ui의 스플리터 사이즈 조절을 위한 사이즈
    sizes << 300 << 660;                                                    // 사이즈 설정
    ui->splitter->setSizes(sizes);                                          // ui 스플리터 사이즈 설정

    /* 등록된 클라이언트를 클릭하면 클라이언트 정보를 입력하는 창에 아이템 표시 */
//    connect(ui->clientTableView,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(showClient(QTreeWidgetItem*,int)));
}


void ClientManager::loadData()                                              // 미리 저장된 클라이언트 데이터를 가져온다
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","clientConnection");
    db.setDatabaseName("client.db");
    if (db.open()) {
        QSqlQuery query(db);
        query.exec("create table if not exists client(\
                   client_id           number(10),\
                   client_name         varchar2(100) not null,\
                   client_addr         varchar2(100),\
                   client_phoneNum     varchar2(100),\
                   client_type         varchar2(100),\
                   primary key(client_id));");

        qm = new QSqlTableModel(this,db);
        qm->setTable("client");
        qm->select();
        qm->setHeaderData(0, Qt::Horizontal, tr("Client_ID"));
        qm->setHeaderData(1, Qt::Horizontal, tr("Client_Name"));
        qm->setHeaderData(2, Qt::Horizontal, tr("Client_Addr"));
        qm->setHeaderData(3, Qt::Horizontal, tr("Client_PhoneNum"));
        qm->setHeaderData(4, Qt::Horizontal, tr("Client_Type"));

        sqm = new QSqlTableModel(this,db);
        sqm->setTable("client");
        sqm->setHeaderData(0, Qt::Horizontal, QObject::tr("Client_ID"));
        sqm->setHeaderData(1, Qt::Horizontal, QObject::tr("Client_Name"));
        sqm->setHeaderData(2, Qt::Horizontal, QObject::tr("Client_Addr"));
        sqm->setHeaderData(3, Qt::Horizontal, QObject::tr("Client_PhoneNum"));
        sqm->setHeaderData(4, Qt::Horizontal, QObject::tr("Client_Type"));

        ui->treeView->setModel(qm);
        ui->ClientSearchTreeView->setModel(sqm);

        for(int i=0; i < qm->rowCount(); i++) {
            int id = qm->data(qm->index(i,0)).toInt();
            QString name = qm->data(qm->index(i,1)).toString();
            emit sendClientIdName(id,name);
        }
    }
}

ClientManager::~ClientManager()                                             // 소멸자에서 데이터 저장
{
    delete ui;
    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    if (db.isOpen()) {
        qm->submitAll();
        delete qm;
        db.close();
//        QSqlDatabase::removeDatabase("clientConnection");
    }
}

int ClientManager::makeId()
{
    if(qm->rowCount() == 0) {
        return 1000;
    } else {
        auto id = qm->data(qm->index(qm->rowCount()-1,0)).toInt();
        return ++id;
    }
}


void ClientManager::on_ClientInfoAddPushButton_clicked()                    // add 버튼
{
    QString name, address, phoneNum, type;                                  // 이름, 주소, 전화번호, 타입
    int id = makeId();
    name = ui->ClientInfoInputNameLineEdit->text();                         // 라인에딧에 있는 이름 사용
    address = ui->ClientInfoInputAddressLineEdit->text();                   // 라인에딧에 있는 주소 사용
    phoneNum = ui->ClientInfoInputPhoneNumLineEdit->text();                 // 라인에딧에 있는 전화번호 사용
    type = ui->ClientInfoInputTypeLineEdit->text();                         // 라인에딧에 잇는 타입 사용

    ui->ClientInfoInputIdLineEdit->clear();                                  // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputNameLineEdit->clear();                               // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputAddressLineEdit->clear();                            // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputPhoneNumLineEdit->clear();                           // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputTypeLineEdit->clear();                               // 리스트를 저장하고 입력창 초기화

    ui->ClientInfoTimeListWidget->addItem("Added Time : "+QTime::currentTime().toString()); // 로그창에 출력

    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    QSqlQuery query(db);
    query.prepare("insert into client values (:id, :name, :addr, :phonenum, :type)");
    query.bindValue(":id",id);
    query.bindValue(":name", name);
    query.bindValue(":addr", address);
    query.bindValue(":phonenum", phoneNum);
    query.bindValue(":type", type);
    query.exec();
    qm->select();
}

void ClientManager::on_ClientInfoModifyPushButton_clicked()                 // modify 버튼 클릭
{
    QModelIndex idx = ui->treeView->currentIndex();
    QString name, address, phoneNum, type;                              // 이름, 주소, 전화번호, 타입
    int id = ui->ClientInfoInputIdLineEdit->text().toInt();
    name = ui->ClientInfoInputNameLineEdit->text();                     // 수정할 이름 쓰기
    address = ui->ClientInfoInputAddressLineEdit->text();               // 수정할 주소 쓰기
    phoneNum = ui->ClientInfoInputPhoneNumLineEdit->text();             // 수정할 전화번호 쓰기
    type = ui->ClientInfoInputTypeLineEdit->text();                     // 수정할 타입 쓰기

    ui->ClientInfoInputIdLineEdit->clear();                                  // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputNameLineEdit->clear();                               // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputAddressLineEdit->clear();                            // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputPhoneNumLineEdit->clear();                           // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputTypeLineEdit->clear();                               // 리스트를 저장하고 입력창 초기화

    logTimeList.insert(id,QTime::currentTime().toString());            // 바뀐 정보를 로그에 찍기

    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    QSqlQuery query(db);
    query.prepare("update client set client_id = :id, client_name = :name,\
                   client_addr = :addr, client_phonenum = :phonenum, client_type = :type\
                   where client_id = :c_id");
    query.bindValue(":id",id);
    query.bindValue(":name", name);
    query.bindValue(":addr", address);
    query.bindValue(":phonenum", phoneNum);
    query.bindValue(":type", type);
    query.bindValue(":c_id", id);
    query.exec();
    qm->select();
}

void ClientManager::on_ClientInfoRemovePushButton_clicked()                 // remove 버튼 클릭
{
    QModelIndex idx = ui->treeView->currentIndex();

    int id = ui->ClientInfoInputIdLineEdit->text().toInt();

    ui->ClientInfoInputIdLineEdit->clear();                                  // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputNameLineEdit->clear();                               // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputAddressLineEdit->clear();                            // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputPhoneNumLineEdit->clear();                           // 리스트를 저장하고 입력창 초기화
    ui->ClientInfoInputTypeLineEdit->clear();                               // 리스트를 저장하고 입력창 초기화

    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    QSqlQuery query(db);
    query.prepare("delete from client where client_id = :id");
    query.bindValue(":id",id);
    query.exec();
    qm->select();


//        emit clientRemove(item->text(0).toInt(),itemidx);                   // 클라이언트 삭제 할 id,인덱스 시그널 보내기
}

void ClientManager::on_ClientSearchPushButton_clicked()                     // search 버튼 클릭
{

    int i = ui->ClientSearchComboBox->currentIndex();                       // 검색할 id,이름,전화번호, 타입을 설정

    QString check = ui->ClientSearchInputLineEdit->text();
    QSqlDatabase db = QSqlDatabase::database("clientConnection");
    switch (i) {
    case 0:
        sqm->QSqlQueryModel::setQuery(QString("select * from client where client_id = %1").arg(check),db);
        break;
    case 1:
        sqm->QSqlQueryModel::setQuery(QString("select * from client where client_name like '%%1%'").arg(check),db);
        break;
    case 2:
        sqm->QSqlQueryModel::setQuery(QString("select * from client where client_addr like '%%1%'").arg(check),db);
        break;
    case 3:
        sqm->QSqlQueryModel::setQuery(QString("select * from client where client_phonenum like '%%1%'").arg(check),db);
        break;
    case 4:
        sqm->QSqlQueryModel::setQuery(QString("select * from client where client_type like '%%1%'").arg(check),db);
        break;
    default:
        break;
    }
}

void ClientManager::clientIdListData(int id)                                // 클라이언트 id를 가지고 클라이언트 정보 추출
{
    QString check = ui->ClientSearchInputLineEdit->text();
    QSqlDatabase db = QSqlDatabase::database("clientConnection");

    QModelIndexList idx = qm->match(qm->index(0,0),Qt::EditRole, id, -1, Qt::MatchFlag(Qt::MatchCaseSensitive));

    foreach(auto index, idx) {
        int id = qm->data(index.siblingAtColumn(0)).toInt();
        QString name = qm->data(index.siblingAtColumn(1)).toString();
        QString addr = qm->data(index.siblingAtColumn(2)).toString();
        QString phonenum = qm->data(index.siblingAtColumn(3)).toString();
        QString type = qm->data(index.siblingAtColumn(4)).toString();
        emit sendClientIdData(id,name,addr,phonenum,type);
    }
}


void ClientManager::clientNameListData(int i,QString cstr)                        // 클라이언트 이름을 가지고 클라이언트 정보 추출
{
    if (i==1) {
        QModelIndexList idx = qm->match(qm->index(0,1),Qt::EditRole, cstr, -1, Qt::MatchFlag(Qt::MatchContains));

        foreach(auto index, idx) {
            int id = qm->data(index.siblingAtColumn(0)).toInt();
            QString name = qm->data(index.siblingAtColumn(1)).toString();
            QString addr = qm->data(index.siblingAtColumn(2)).toString();
            QString phonenum = qm->data(index.siblingAtColumn(3)).toString();
            QString type = qm->data(index.siblingAtColumn(4)).toString();
            emit sendClientIdData(id,name,addr,phonenum,type);
        }
    }
    else if (i==2) {
        QModelIndexList idx = qm->match(qm->index(0,2),Qt::EditRole, cstr, -1, Qt::MatchFlag(Qt::MatchContains));

        foreach(auto index, idx) {
            int id = qm->data(index.siblingAtColumn(0)).toInt();
            QString name = qm->data(index.siblingAtColumn(1)).toString();
            QString addr = qm->data(index.siblingAtColumn(2)).toString();
            QString phonenum = qm->data(index.siblingAtColumn(3)).toString();
            QString type = qm->data(index.siblingAtColumn(4)).toString();
            emit sendClientIdData(id,name,addr,phonenum,type);
        }
    }
    else if (i==3) {
        QModelIndexList idx = qm->match(qm->index(0,4),Qt::EditRole, cstr, -1, Qt::MatchFlag(Qt::MatchContains));

        foreach(auto index, idx) {
            int id = qm->data(index.siblingAtColumn(0)).toInt();
            QString name = qm->data(index.siblingAtColumn(1)).toString();
            QString addr = qm->data(index.siblingAtColumn(2)).toString();
            QString phonenum = qm->data(index.siblingAtColumn(3)).toString();
            QString type = qm->data(index.siblingAtColumn(4)).toString();
            emit sendClientIdData(id,name,addr,phonenum,type);
        }
    }
}

void ClientManager::serverClientList()                                      // 서버로 클라이언트 정보 전송
{
    foreach(auto i, clientList)                                             // 저장된 클라이언트 리스트를 돌면서
    {
        Client* c = static_cast<Client*>(i);                                // 클라이언트 형태로 저장
        int id = c->id();                                                   // 클라이언트 id 추출
        QString name = c->getName();                                        // 클라이언트 이름 추출

        QTreeWidgetItem *item = new QTreeWidgetItem;                        // 트리위젯아이템으로
        item->setText(0,QString::number(id));                               // 클라이언트 id 설정
        item->setText(1,name);                                              // 클라이언트 이름 설정
        serverClient.insert(id,name);                                       // 서버에 접속한 리스트 관리
        emit clientToServer(item);                                          // 저장된 객체를 시그널로 서버에 전송
    }
}


void ClientManager::on_treeView_clicked(const QModelIndex &index)
{
    QModelIndex idx = ui->treeView->currentIndex();
    ui->ClientInfoInputIdLineEdit->setText(QString::number(idx.sibling(idx.row(),0).data().toInt()));
    ui->ClientInfoInputNameLineEdit->setText(idx.sibling(idx.row(),1).data().toString());
    ui->ClientInfoInputAddressLineEdit->setText(idx.sibling(idx.row(),2).data().toString());
    ui->ClientInfoInputPhoneNumLineEdit->setText(idx.sibling(idx.row(),3).data().toString());
    ui->ClientInfoInputTypeLineEdit->setText(idx.sibling(idx.row(),4).data().toString());
}

void ClientManager::getClientName(QString c_name)
{
    QModelIndexList idx = qm->match(qm->index(0,1),Qt::EditRole, c_name, -1, Qt::MatchFlag(Qt::MatchCaseSensitive));

    foreach(auto index,idx) {
        int id = qm->data(index.siblingAtColumn(0)).toInt();
        emit sendClientId(id);
    }
}
