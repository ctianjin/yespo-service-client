#include "imudppackage.h"
#include <QIODevice>
#include <QDataStream>
#include <QDebug>
#include <QtEndian>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "ImUpdPackage:"
#endif // DEBUG_PREFIX

namespace ns {

ImUdpPackage::ImUdpPackage()
	: m_packageName(QString()),
	m_serverSeq(-1),
	m_blockNum(-1),
	m_blockSeq(-1),
	m_sessionId(-1),
	m_ack(-1),
	m_proVersion(-1),
	m_clientSeq(-1),
	m_checkSum(-1),
	m_dataLen(-1),
	m_reserved(-1),
	m_packType(-1),
	m_result(-1),
	m_data(QByteArray()),
	m_packageData(QByteArray()),
	m_timeStamp(QTime()),
	m_retransCount(0)
{

}

ImUdpPackage::ImUdpPackage(const ImUdpPackage& rhs) :
	QSharedData(rhs),
	m_packageName(rhs.m_packageName),
	m_serverSeq(rhs.m_serverSeq),
	m_blockNum(rhs.m_blockNum),
	m_blockSeq(rhs.m_blockSeq),
	m_sessionId(rhs.m_sessionId),
	m_ack(rhs.m_ack),
	m_proVersion(rhs.m_proVersion),
	m_clientSeq(rhs.m_clientSeq),
	m_checkSum(rhs.m_checkSum),
	m_dataLen(rhs.m_dataLen),
	m_reserved(rhs.m_reserved),
	m_packType(rhs.m_packType),
	m_result(rhs.m_result),
	m_data(rhs.m_data),
	m_packageData(rhs.m_packageData),
	m_timeStamp(rhs.m_timeStamp),
	m_retransCount(rhs.m_retransCount)
{

}

ImUdpPackage::~ImUdpPackage()
{

}

int ImUdpPackage::parsePack(QByteArray& data)
{
	int size = data.size();
	qDebug() << "data.size:" << size;
	if (size < YP_ACK_PACK_SIZE) {
		return IMParseACKMiss;
	} 
	if ( size > YP_ACK_PACK_SIZE && size < YP_RESULT_PACK_SIZE) {
		return IMParseResultMiss;
	}

	QDataStream package(&data, QIODevice::ReadOnly);
	package.setByteOrder(QDataStream::BigEndian);
	qint8 head1, head2;
	qint32 serverSeq, sessionId, reserved;
	qint16 clientSeq, checkSum, dataLen, packType, result;
	package.readRawData((char*)&head1, 1);
	package.readRawData((char*)&head2, 1);

	int rs = package.readRawData((char*)&serverSeq, sizeof(qint32));

	package.readRawData((char*)&m_blockNum, sizeof(qint8));
	package.readRawData((char*)&m_blockSeq, sizeof(qint8));
	package.readRawData((char*)&sessionId, sizeof(qint32));
	package.readRawData((char*)&m_ack, sizeof(qint8));
	package.readRawData((char*)&m_proVersion, sizeof(qint8));
	package.readRawData((char*)&clientSeq, sizeof(qint16));
	package.readRawData((char*)&checkSum, sizeof(qint16));
	package.readRawData((char*)&dataLen, sizeof(qint16));
	package.readRawData((char*)&reserved, sizeof(qint32));

	m_clientSeq = qFromBigEndian(clientSeq);
	m_checkSum = qFromBigEndian(checkSum);
	m_dataLen = qFromBigEndian(dataLen);
	m_reserved = qFromBigEndian(reserved);

	if (data.size() == 24) {
		//ack pack
		return IMParseACK;
	} else  {
		package.readRawData((char*)&packType, sizeof(qint16));
		package.readRawData((char*)&result, sizeof(qint16));
		//sessionId && serverSeq from the result pack should be sent back
		m_serverSeq = qFromBigEndian(serverSeq);
		m_sessionId = qFromBigEndian(sessionId);
		m_packType = qFromBigEndian(packType);
		m_result = qFromBigEndian(result);
		if (data.size() == 28) {
			//result pack
			return IMParseResult;
		} else {
			// msg pack
			char* buffer = new char[m_dataLen + 1];
			int sizeRead = package.readRawData(buffer, m_dataLen);
			m_data.setRawData(buffer, m_dataLen);
			//delete buffer;
			return IMParseMsg;
		}
	}

	return -1;
}

void ImUdpPackage::packPack(QByteArray& data)
{
	qDebug() << m_serverSeq << m_dataLen;
	QDataStream package(&data, QIODevice::WriteOnly);
	package.setByteOrder(QDataStream::LittleEndian);

	qint32 serverSeq, sessionId, reserved;
	qint16 clientSeq, checkSum, dataLen, packType, result;

	serverSeq = qToBigEndian(m_serverSeq);	sessionId = qToBigEndian(m_sessionId);	clientSeq = qToBigEndian(m_clientSeq);	checkSum = qToBigEndian(m_checkSum);	dataLen = qToBigEndian(m_dataLen);	reserved = qToBigEndian(m_reserved);

	package.writeRawData("YP", 2);
	package.writeRawData((char*)&serverSeq, sizeof(qint32));
	package.writeRawData((char*)&m_blockNum, sizeof(qint8));
	package.writeRawData((char*)&m_blockSeq, sizeof(qint8));
	package.writeRawData((char*)&sessionId, sizeof(qint32));
	package.writeRawData((char*)&m_ack, sizeof(qint8));
	package.writeRawData((char*)&m_proVersion, sizeof(qint8));
	package.writeRawData((char*)&clientSeq, sizeof(qint16));
	package.writeRawData((char*)&checkSum, sizeof(qint16));
	package.writeRawData((char*)&dataLen, sizeof(qint16));
	package.writeRawData((char*)&reserved, sizeof(qint32));	if (m_packType != -1 && m_result != -1) {		packType = qToBigEndian(m_packType);		result = qToBigEndian(m_result);		package.writeRawData((char*)&packType, sizeof(qint16));		package.writeRawData((char*)&result, sizeof(qint16));	}	if (m_dataLen > 0 && m_data.length() > 0)		package.writeRawData(m_data.data(), m_data.length());	m_packageData = data;}

void ImUdpPackage::setPackageName(const QString& name)
{
	m_packageName = name;
}

QString ImUdpPackage::name() const 
{
	return m_packageName;
}

void ImUdpPackage::setServerSeq(qint32 serverSeq)
{
	m_serverSeq = serverSeq;
}

qint32 ImUdpPackage::serverSeq() const
{
	return m_serverSeq;
}

void ImUdpPackage::setBlockNum(qint8 blockNum)
{
	m_blockNum = blockNum;
}

qint8 ImUdpPackage::blockNum() const
{
	return m_blockNum;
}

void ImUdpPackage::setBlockSeq(qint8 blockSeq)
{
	m_blockSeq = blockSeq;
}

qint8 ImUdpPackage::blockSeq() const
{
	return m_blockSeq;
}

void ImUdpPackage::setSessionId(qint32 sessionId)
{
	m_sessionId = sessionId;
}

qint32 ImUdpPackage::sessionId() const
{
	return m_sessionId;
}

void ImUdpPackage::setAck(qint8 ack)
{
	m_ack = ack;
}

qint8 ImUdpPackage::ack() const
{
	return m_ack;
}

void ImUdpPackage::setProVersion(qint8 proVersion)
{
	m_proVersion = proVersion;
}

qint8 ImUdpPackage::proVersion() const
{
	return m_proVersion;
}

void ImUdpPackage::setClientSeq(qint16 clientSeq)
{
	m_clientSeq = clientSeq;
}

qint16 ImUdpPackage::clientSeq() const 
{
	return m_clientSeq;
}

void ImUdpPackage::setCheckSum(qint16 checkSum)
{
	m_checkSum = checkSum;
}

qint16 ImUdpPackage::checkSum() const 
{
	return m_checkSum;
}

void ImUdpPackage::setDataLen(qint16 dataLen)
{
	m_dataLen = dataLen;
}

qint16 ImUdpPackage::dataLen() const 
{
	return m_dataLen;
}

void ImUdpPackage::setReserved(qint32 reserved)
{
	m_reserved = reserved;
}

qint32 ImUdpPackage::reserved() const 
{
	return m_reserved;
}

void ImUdpPackage::setPackType(qint16 packType)
{
	m_packType = packType;
}

qint16 ImUdpPackage::packType() const 
{
	return m_packType;
}

void ImUdpPackage::setResult(qint16 result)
{
	m_result = result;
}

qint16 ImUdpPackage::result() const 
{
	return m_result;
}

void ImUdpPackage::setData(QByteArray& data)
{
	m_data = data;
}

QByteArray ImUdpPackage::data() const 
{
	return m_data;
}

void ImUdpPackage::setPackageData(QByteArray& data)
{
	m_packageData = data;
}

QByteArray ImUdpPackage::packageData() const
{
	return m_packageData;
}

void ImUdpPackage::setTimeStamp(QTime timeStamp)
{
	m_timeStamp = timeStamp;
}

QTime ImUdpPackage::timeStamp()
{
	return m_timeStamp;
}

void ImUdpPackage::setRetransCount(qint8 retransCount)
{
	m_retransCount = retransCount;
}

qint8 ImUdpPackage::retransCount()
{
	return m_retransCount;
}

} // namespace ns