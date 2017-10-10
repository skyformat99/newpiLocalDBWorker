#include"binaryprotocol.h"


BinaryProtocol::BinaryProtocol()
{
    anIf(BinaryProtocolDbgEn, anTrk("Object Constructed"));
}

///
/// \brief BinaryProtocol::BinaryProtocol
/// \param BPNum
///
BinaryProtocol::BinaryProtocol(const quint8 BPNum)
    :mBPNo(BPNum)
    ,mHdrCmd(quint8(BPNum+_BaseHdrCmd))
    ,mHdrRsp(quint8(BPNum+_BaseHdrRsp))
    ,mHdr(0)
    ,mDataLen(_StdDatLen)
    ,mCmd(0)
    ,mCh(0)
    ,mData()
    ,mChkSum(0)
    ,mMsg()
{
    anIf(BinaryProtocolDbgEn, anTrk("Object Constructed With BPNo="<<BPNum));
}

BinaryProtocol &BinaryProtocol::fromQByteArray(const QByteArray &QBArr)
{
    TypHdr IntTmp1 = 0;
    TypHdr IntTmp2 = QBArr.left(_SzHdr).toHex().toInt(NULL,16);
    if (IntTmp2 < _BaseHdrCmd)
        IntTmp1 = IntTmp2 - _BaseHdrRsp;
    else
        IntTmp1 = IntTmp2 - _BaseHdrCmd;
    BinaryProtocol * BPTmp = new BinaryProtocol(0);
    BPTmp->SetBPNo(IntTmp1);
    IntTmp1 = 0;
    BPTmp->mHdr = IntTmp2;
    IntTmp1 += _SzHdr;
    BPTmp->mDataLen = QBArr.mid(IntTmp1,_SzDatLen).toHex().toInt(NULL,16);
    IntTmp1 += _SzDatLen;
    BPTmp->mCmd = QBArr.mid(IntTmp1,_SzCmd).toHex().toInt(NULL,16);
    IntTmp1 += _SzCmd;
    BPTmp->mCh = QBArr.mid(IntTmp1,_SzCh).toHex().toInt(NULL,16);
    IntTmp1 += _SzCh;
    IntTmp2 = QString::fromLocal8Bit(* new QByteArray() << BPTmp->mDataLen).toInt()-_SzCmd-_SzCh;
    BPTmp->mData = QBArr.mid(IntTmp1,IntTmp2);
    IntTmp1 += IntTmp2;
    BPTmp->mChkSum = QBArr.mid(IntTmp1,_SzChkSum).toHex().toInt(NULL,16);
    //Vadility Of Response Message Should Be Added Here
    BPTmp->mMsg.clear();
    BPTmp->mMsg<<BPTmp->mHdr<<BPTmp->mDataLen<<BPTmp->mCmd<<BPTmp->mCh<<BPTmp->mData<<BPTmp->mChkSum;
    return *BPTmp;
}

quint8 BinaryProtocol::GetBPNo() const
{
    return mBPNo;
}

BinaryProtocol &BinaryProtocol::SetBPNo(const quint8 BPNum)
{
    mBPNo = BPNum;
    mHdrCmd = quint8(BPNum+_BaseHdrCmd);
    mHdrRsp = quint8(BPNum+_BaseHdrRsp);
    return *this;
}

BinaryProtocol &BinaryProtocol::HdrCmd()
{
    mHdr = mHdrCmd;
    return *this;
}

BinaryProtocol &BinaryProtocol::HdrRsp()
{
    mHdr = mHdrRsp;
    return *this;
}

BinaryProtocol::TypHdr BinaryProtocol::GetHdrCmd() const
{
    return mHdrCmd;
}

BinaryProtocol::TypHdr BinaryProtocol::GetHdrRsp() const
{
    return mHdrRsp;
}

BinaryProtocol::TypHdr BinaryProtocol::GetHdr() const
{
    return mHdr;
}

BinaryProtocol::TypCh BinaryProtocol::GetCh() const
{
    return mCh;
}

BinaryProtocol::TypCmd BinaryProtocol::GetCmd() const
{
    return mCmd;
}

const BinaryProtocol::TypDat BinaryProtocol::GetData() const
{
    return mData;
}

BinaryProtocol::TypChkSum BinaryProtocol::GetChkSum() const
{
    return mChkSum;
}

const QByteArray BinaryProtocol::GetMsg() const
{
    return mMsg;
}

///
/// \brief BinaryProtocol::GenMsg
/// \return New Message Generated From
///             Current Values Of Protocol Parts
///
const QByteArray BinaryProtocol::GenMsg()
{
    mMsg.clear();
    quint8 int8tmp = _SzCmd + _SzCh + mData.size();
    if (int8tmp == 4)
        mDataLen = _StdDatLen;
    else
        mDataLen = IntStr2QBArr0Pad(int8tmp, _SzDatLen).toHex().toInt(NULL,16);
    mMsg<<mHdr<<mDataLen<<mCmd<<mCh<<mData;
    mChkSum=XORofAllBytesInQByteArr(mMsg)&_MskChkSum;
    mMsg<<mChkSum;
    return mMsg;
}

const QString BinaryProtocol::GetMessageDirection() const
{
    if (mHdr<_BaseHdrCmd)
        return QString("From");
    else
        return QString("To");
}

quint8 BinaryProtocol::GetHeader() const
{
    return mBPNo;
}

quint8 BinaryProtocol::GetDataLength() const
{
    QByteArray QBTmp = "";
    QBTmp << mDataLen;
    return QBTmp.toInt();
}

const QString BinaryProtocol::GetCommand() const
{
    return BinaryProtocol::CmdCode2CmdMean.value(mCmd);
}

quint8 BinaryProtocol::GetChannel() const
{
    return (mCh-_BaseCh);
}

const QString BinaryProtocol::GetDataTranslation() const
{
    QString StrTmp = "";
    switch (mCmd) {
    case 0x5A30:
        switch (mData.toHex().toInt(NULL,16)) {
        case 0x30:
            StrTmp = "Local";
            break;
        case 0x31:
            StrTmp = "Remote I/O";
            break;
        case 0x32:
            StrTmp = "Serial";
            break;
        }
        break;
    case 0x4130:
        switch (mData.toHex().toInt(NULL,16)) {
        case 0x30:
            StrTmp = "Off";
            break;
        case 0x31:
            StrTmp = "On With RunMode:Start/Step";
            break;
        case 0x32:
            StrTmp = "On With RunMode:Start/Fixed";
            break;
        case 0x33:
            StrTmp = "On With RunMode:Protect/Step";
            break;
        case 0x34:
            StrTmp = "On With RunMode:Protect/Fixed";
            break;
        case 0x2D33:
            StrTmp = "Off Due To Interlock Cable/Panel";
            break;
        case 0x2D34:
            StrTmp = "Off Due To Remote I/O Interlock";
            break;
        case 0x2D36:
            StrTmp = "Off Due To HV Protect";
            break;
        case 0x2D37:
            StrTmp = "Off Due To HV Short Circuit/ Remote I/O Failure";
            break;
        case 0x2D38:
            StrTmp = "Off Due To HV Overtemperature";
            break;
        }
        break;
    case 0x4430:
        switch (mData.toHex().toInt(NULL,16)) {
        case 0x30:
            StrTmp = "Torr";
            break;
        case 0x31:
            StrTmp = "mBar";
            break;
        case 0x32:
            StrTmp = "Pascal";
            break;
        }
        break;
    case 0x5D30:
        switch (mData.toHex().toInt(NULL,16)) {
        case 0x01:
        case 0x10:
            StrTmp = "Reserved (always 0)";
            break;
        case 0x02:
        case 0x20:
            StrTmp = "Front Panel Interlock (equal to bit 20h)";
            break;
        case 0x04:
            StrTmp = "HV1 Remote I/O Interlock";
            break;
        case 0x08:
            StrTmp = "HV1 Cable interlock";
            break;
        case 0x40:
            StrTmp = "HV2 Remote I/O Interlock";
            break;
        case 0x80:
            StrTmp = "HV2 Cable interlock";
            break;
        }
        break;
    case 0x4630:
        StrTmp = QString::number(mData.toHex().toInt(NULL,16));
//*Uncomment Following Lines If Need**************************/
//        switch (mCh) {
//        case 0x31:
//        case 0x32:
//            switch (mData.toHex().toInt(NULL,16)) {
//            case 0x30:
//                StrTmp = "Spare";
//                break;
//            case 0x31:
//                StrTmp = "500 SC/Tr";
//                break;
//            case 0x32:
//                StrTmp = "300 SC/Tr";
//                break;
//            case 0x33:
//                StrTmp = "150 SC/Tr";
//                break;
//            case 0x34:
//                StrTmp = "75-55-40SC/T";
//                break;
//            case 0x35:
//                StrTmp = "20 SC/Tr";
//                break;
//            case 0x36:
//                StrTmp = "500 Diode/ND";
//                break;
//            case 0x37:
//                StrTmp = "300 Diode/ND";
//                break;
//            case 0x38:
//                StrTmp = "150 Diode/ND";
//                break;
//            case 0x39:
//                StrTmp = "75-55-40 D/ND";
//                break;
//            case 0x3A:
//                StrTmp = "20 -25 Diode/ND";
//                break;
//            }
//            break;
//        case 0x33:
//        case 0x34:
//            switch (mData.toHex().toInt(NULL,16)) {
//            case 0x30:
//                StrTmp = "Convectorr";
//                break;
//            case 0x31:
//                StrTmp = "Mini-B/A";
//                break;
//            case 0x32:
//                StrTmp = "Cold Cathode";
//                break;
//            }
//            break;
//        case 0x35:
//            switch (mData.toHex().toInt(NULL,16)) {
//            case 0x30:
//                StrTmp = "RS232/422";
//                break;
//            case 0x31:
//                StrTmp = "RS485";
//                break;
//            }
//            break;
//        }
        break;
    case 0x5430:
    case 0x5530:
        StrTmp = QString(mData);
        StrTmp =  QString::number((StrTmp.section('E',0,0,QString::SectionSkipEmpty).toDouble())
                                  *qPow(10,StrTmp.section('E',1,1,QString::SectionSkipEmpty).toDouble())).toUpper();
        break;
    default:
        StrTmp = QString(mData);
        break;
    }
    if (mData.toHex().toInt(NULL,16) == 0x3F)
        StrTmp = "To Be Collected !";
    return StrTmp;
}

const QString BinaryProtocol::GetMessageTranslation() const
{
    QString StrTmp = "";
    if (GetChannel() == 0)
        StrTmp = "No Channel";
    else
        StrTmp = QString("Channel #").append(QString::number(GetChannel()));
    StrTmp = GetMessageDirection() + " - Pump #" + QString::number(GetHeader())
        + " - " + StrTmp + " - " + GetCommand() + " - Data " + GetDataTranslation();
    return StrTmp;
}

///
/// \brief BinaryProtocol::Cmd
/// \param Code
/// \return A Reference To Itself
///             After Update Its Protocol Command Part With Given Command Code
///
BinaryProtocol &BinaryProtocol::Cmd(const quint16 CmdCode)
{
    mCmd = CmdCode;
    return *this;
}

BinaryProtocol &BinaryProtocol::ModeLRS()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::HVSwitch()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::ProtectSwitch()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::UnitPres()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::ReaduCFWVer()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::ReadDSPFWVer()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::DevID()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::ReadDevType()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::ReadV()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::ReadI()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::ReadP()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::ReadErr()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::WriteSerialReset()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::ReadInterlock()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__)).Read();
}

BinaryProtocol &BinaryProtocol::SerialConfigMode()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::SerialProperty()
{
    return this->Cmd(CmdMean2CmdCode.value(__func__));
}

BinaryProtocol &BinaryProtocol::Ch(const quint8 Channel)
{
    mCh=Channel;
    return *this;
}

BinaryProtocol &BinaryProtocol::ChannelNo(const quint8 ChNo)
{
    mCh=ChNo+_BaseCh;
    return *this;
}

BinaryProtocol &BinaryProtocol::Ch1()
{
    return this->Ch(1+_BaseCh);
}

BinaryProtocol &BinaryProtocol::Ch2()
{
    return this->Ch(2+_BaseCh);
}

BinaryProtocol &BinaryProtocol::ChSerial()
{
    return this->Ch(5+_BaseCh);
}

BinaryProtocol &BinaryProtocol::noCh()
{
    return this->Ch(_BaseCh);
}

BinaryProtocol &BinaryProtocol::Data(const QByteArray &Data)
{
    mData.clear();
    mData<<Data;
    return *this;
}

BinaryProtocol &BinaryProtocol::Read()
{
    return this->Data(_Read);
}

BinaryProtocol &BinaryProtocol::On()
{
    return this->Data(_On);
}

BinaryProtocol &BinaryProtocol::Off()
{
    return this->Data(_Off);
}

///
/// \brief BinaryProtocol::Data
/// \param Data
/// \return A Reference To Itself
///             After Update Its Protocol Data Part With Given Data
///
BinaryProtocol &BinaryProtocol::Data(const quint8 Data)
{
    mData.clear();
    mData<<Data;
    return *this;
}

BinaryProtocol::TypDatLen BinaryProtocol::GetDataLen() const
{
    return mDataLen;
}

///
/// \brief UHV Command Set
/// \abstract Fast Search For Code Indexed By Mean
///
const QHash<QString , BinaryProtocol::TypCmd> &BinaryProtocol::CmdMean2CmdCode = * new QHash<QString, BinaryProtocol::TypCmd>
({
     {"ModeLRS", 0x5A30},
     {"HVSwitch", 0x4130},
     {"UnitPres", 0x4430},
     {"ReaduCFWVer", 0x4530},
     {"ReadDSPFWVer", 0x4531},
     {"DevID", 0x4630},
     {"ReadDevType", 0x4631},
     {"ReadV", 0x5330},
     {"ReadI", 0x5430},
     {"ReadP", 0x5530},
     {"ReadErr", 0x7A30},
     {"WriteSerialReset", 0x5B30},
     {"ReadInterlock", 0x5D30},
     {"SerialConfigMode", 0x7861},
     {"SerialProperty", 0x7862},
     {"ProtectSwitch", 0x4330}
});
///
/// \brief UHV Command Set
/// \abstract Fast Search For Mean Indexed By Code
///
const QHash<BinaryProtocol::TypCmd, QString > &BinaryProtocol::CmdCode2CmdMean = SwapKeyValOnOneToOneQHash(BinaryProtocol::CmdMean2CmdCode);
