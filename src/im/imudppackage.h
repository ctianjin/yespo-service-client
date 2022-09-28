#ifndef UDPPACKAGE_H
#define UDPPACKAGE_H

#include <QTime>
#include "imglobal.h"
#include "improtocol.h"

namespace ns {

class ImUdpPackage : public QSharedData
{
	enum ParseStatus {
		IMParseACKMiss = -2,
		IMParseResultMiss,
		IMParseACK,
		IMParseResult,
		IMParseMsg
	};

public:
	ImUdpPackage();
	ImUdpPackage(const ImUdpPackage& rhs);
	~ImUdpPackage();

	void parse();
	int parsePack(QByteArray& data);
	void packPack(QByteArray& data);

	void setPackageName(const QString& name);
	QString name() const;

	void setServerSeq(qint32 serverSeq);
	qint32 serverSeq() const;

	void setBlockNum(qint8 blockNum);
	qint8 blockNum() const;

	void setBlockSeq(qint8 blockSeq);
	qint8 blockSeq() const;

	void setSessionId(qint32 sessionId);
	qint32 sessionId() const;

	void setAck(qint8 ack);
	qint8 ack() const;

	void setProVersion(qint8 proVersion);
	qint8 proVersion() const;

	void setClientSeq(qint16 clientSeq);
	qint16 clientSeq() const;

	void setCheckSum(qint16 checkSum);
	qint16 checkSum() const;

	void setDataLen(qint16 dataLen);
	qint16 dataLen() const;

	void setReserved(qint32 reserved);
	qint32 reserved() const;

	void setPackType(qint16 packType);
	qint16 packType() const;

	void setResult(qint16 result);
	qint16 result() const;

	void setData(QByteArray& data);
	QByteArray data()  const;

	void setPackageData(QByteArray& data);
	QByteArray packageData() const; 

	void setTimeStamp(QTime timeStamp);
	QTime timeStamp();

	void setRetransCount(qint8 retransCount);
	qint8 retransCount();

Q_SIGNALS:
	void reLogin();
	void error(qint16& result);
private:
	QString m_packageName;
	qint32 m_serverSeq;
	qint8 m_blockNum;
	qint8 m_blockSeq;
	qint32 m_sessionId;
	qint8 m_ack;
	qint8 m_proVersion;
	qint16 m_clientSeq;
	qint16 m_checkSum;
	qint16 m_dataLen;
	qint32 m_reserved;
	qint16 m_packType;
	qint16 m_result;

	QTime m_timeStamp;
	qint8 m_retransCount;

	QByteArray m_data;
	QByteArray m_packageData;
	
};

} // namespace ns

#endif // UDPPACKAGE_H
