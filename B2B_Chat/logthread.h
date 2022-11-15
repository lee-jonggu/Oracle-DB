#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QThread>
#include <QList>
#include <QQueue>

class QTreeWidgetItem;

class LogThread : public QThread
{
    Q_OBJECT
public:
    explicit LogThread(QObject *parent = nullptr);

private:
    void run();

    QQueue<QTreeWidgetItem*> itemList;
    QString filename;

signals:
    void send(int data);

public slots:
    void appendData(QTreeWidgetItem*);
    void saveData();
};

#endif // LOGTHREAD_H
