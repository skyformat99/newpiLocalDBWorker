#include <QCoreApplication>
#include "src/pilocaldbworker.h"
#include <QThread>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    registerGlobalSignal;
    connectLocalQSqlDatabase;



    anWarn("START TEST");

    piLocalDBWorker * test = new piLocalDBWorker();
    QThread * newThread = new QThread();
    test->moveToThread(newThread);
    QObject::connect(newThread, &QThread::started, test, &piLocalDBWorker::start);
    newThread->start();



    anWarn("MACHINE STARTED");
    return a.exec();
}
