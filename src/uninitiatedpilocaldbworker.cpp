#include "uninitiatedpilocaldbworker.h"

uninitiatedPiLocalDBWorker::uninitiatedPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("uninitiatedPiLocalDBWorker Constructed"));
}

void uninitiatedPiLocalDBWorker::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("uninitiatedPiLocalDBWorker Entered"));
    qApp->processEvents();
    if (basisptr->previousStateName.isEmpty() && !(basisptr))
    {

    }
}

void uninitiatedPiLocalDBWorker::onExit(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("Leave uninitiatedPiLocalDBWorker"));
    basisptr->previousStateName = objectName();
}
