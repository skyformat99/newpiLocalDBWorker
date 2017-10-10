#include "pilocaldbworkerbasis.h"

piLocalDBWorkerBasis::piLocalDBWorkerBasis(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<piLocalDBWorkerBasis::PVIData>("piLocalDBWorkerBasis::PVIData");
    qRegisterMetaType<piLocalDBWorkerBasis::RFIDData>("piLocalDBWorkerBasis::RFIDData");
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("piLocalDBWorkerBasis Constructed"));
}

void piLocalDBWorkerBasis::initialize()
{
    dispose();

}

void piLocalDBWorkerBasis::dispose()
{

}

bool piLocalDBWorkerBasis::connectLocalDatabase()
{

}

void piLocalDBWorkerBasis::closeLocalDatabaseConnection()
{

}

void piLocalDBWorkerBasis::setError(const piLocalDBWorkerBasis::Error &anErrorType, const QString &anErrorInfo)
{

}

void piLocalDBWorkerBasis::clearError()
{

}

void piLocalDBWorkerBasis::clearCache()
{

}
