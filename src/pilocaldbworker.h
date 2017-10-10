#ifndef PILOCALDBWORKER_H
#define PILOCALDBWORKER_H

#define piLocalDBWorkerDbgEn 1

#include <QStateMachine>
#include "pilocaldbworkerbasis.h"
#include "shared/directtransition.h"

class piLocalDBWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit piLocalDBWorker(QObject *parent = nullptr);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    piLocalDBWorkerVarSet * currentVarSet = Q_NULLPTR;

};

#endif // PILOCALDBWORKER_H
