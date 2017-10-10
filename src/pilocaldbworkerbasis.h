#ifndef PILOCALDBWORKERBASIS_H
#define PILOCALDBWORKERBASIS_H

#define piLocalDBWorkerBasisDbgEn 1

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtNetwork>
#include <QNetworkReply>
#include <QMetaType>
#include <QMetaEnum>
#include "shared/commonthings.h"
#include "shared/abstractstatemachinebasis.h"
#include "BinaryProtocol/src/binaryprotocol.h"
#include "WindowProtocol/src/windowprotocol.h"
#include "CanProtocol/src/canprotocol.h"
#include "anLogger/src/anlogger.h"

class piLocalDBWorkerBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit piLocalDBWorkerBasis(QObject *parent = nullptr);

    enum Data
    {
        NoData = 0,
        updatePVIData,
        updateRFIDData,
        checkControlTable,
        syncOfflineData
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        QSqlError
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0,
        readyToWork
    };
    Q_ENUM(Notification)

    typedef struct {
        quint32 GlobalID;
        QString Pressure;
        QString Voltage;
        QString Current;
    } PVIData;

    typedef struct {
        quint32 GlobalID;
        QString srcRFID;
    } RFIDData;

    void initialize();
    void dispose();
    bool connectLocalDatabase();
    void closeLocalDatabaseConnection();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();
    void clearCache();

    bool isOnline = true;
    QSqlDatabase localDb;
    Error ErrorType = NoError;
    QString ErrorInfo;

    QNetworkReply * networkReply = nullptr;

    bool isPendingJsonDataNotPING();

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const QMetaEnum QSqlErrorMetaEnum;

    void sendJsonDataPackage(const QByteArray &data);
    bool sendPendingJsonDataPackage();
    const QJsonObject createAJsonDataPackage(const QString &GlobalID, const QString &HV, const QString &Valve, const QString &Pressure, const QString &Voltage, const QString &Current, const QString &dstRFID, const QString &Time);
    const QJsonArray getPendingJsonDataPackage();
signals:
    void DatabaseConnected();
    void DatabaseClosed();
    void jsonPackageTransmitted();
    void controlUHV2(BinaryProtocol);
    void controlUHV4(WindowProtocol);
    void controlValve(CanProtocol);
public slots:
    void requestControlTableCheck();
    void updateLocalDatabase();
    void setIsSentColumnOnLocalDatabase();
    void emitErrorGlobalSignal();
private:
    static const QByteArray constPINGJsonPackage;
    static const QNetworkRequest onlineDb;
    QNetworkAccessManager NAManager;

    QSqlQuery instantQuery;

    //temporary variables
    GlobalSignal currentGlobalSignal;
    QSqlQuery sourceQuery;
    QSqlQuery tmpQuery;
    QSqlQuery tmpQuery2;
    QString tmpStr;
    QString tmpStr1;
    QString tmpStr2;
    QString tmpStr3;
    QString tmpStr4;
    QString tmpStr5;
    QString reservedStr1;
    QString reservedStr2;
    QByteArray pendingJSonData;
    BinaryProtocol tmpBP;
    WindowProtocol tmpWP;
    CanProtocol tmpCP;
};

#endif // PILOCALDBWORKERBASIS_H
