#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QWidget>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include "configinfo.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "ConfigInfo:"
#endif // QPID_DEBUG_PREFIX

namespace ns {

ConfigItem::ConfigItem() : QObject(),
	m_accountid(0),
	m_callid(-1),
	m_callerid(),
	m_calleeid(),
	m_password(),
	m_autoAnswer(0),
	m_sourceLang(),
	m_targetLang(),
	m_absoluteTime(0),
	m_autocall(0),
	m_regStatus(UNREGISTER),
	m_duration(0),
	m_status(IDLE),
	m_idleDuration(0)
{

}

ConfigItem::ConfigItem(const ConfigItem& rhs) : 
	m_accountid(rhs.m_accountid),
	m_callid(rhs.m_callid),
	m_callerid(rhs.m_callerid),
	m_calleeid(rhs.m_calleeid),
	m_password(rhs.m_password),
	m_autoAnswer(rhs.m_autoAnswer),
	m_sourceLang(rhs.m_sourceLang),
	m_targetLang(rhs.m_targetLang),
	m_absoluteTime(rhs.m_absoluteTime),
	m_autocall(rhs.m_autocall),
	m_regStatus(rhs.m_regStatus),
	m_duration(rhs.m_duration),
	m_status(rhs.m_status),
	m_idleDuration(rhs.m_idleDuration)
{

}

ConfigItem::~ConfigItem(){}

ConfigItem& ConfigItem::operator=(const ConfigItem& rhs)
{
	m_accountid = rhs.m_accountid;
	m_callid = rhs.m_callid;
	m_callerid = rhs.m_callerid;
	m_calleeid = rhs.m_calleeid;
	m_password = rhs.m_password;
	m_autoAnswer = rhs.m_autoAnswer;
	m_sourceLang = rhs.m_sourceLang;
	m_targetLang = rhs.m_targetLang;
	m_absoluteTime = rhs.m_absoluteTime;
	m_autocall = rhs.m_autocall;
	m_regStatus = rhs.m_regStatus;
	m_duration = rhs.m_duration;
	m_status = rhs.m_status;
	m_idleDuration = rhs.m_idleDuration;

	return *this;
}

int ConfigItem::accountid() const
{
	return m_accountid;
}

void ConfigItem::setAccountid(int accountid)
{
	m_accountid = accountid;
}

int ConfigItem::callid() const
{
	return m_callid;
}

void ConfigItem::setCallid(int callid)
{
	m_callid = callid;
}

QString ConfigItem::callerid() const
{
	return m_callerid;
}

QString ConfigItem::calleeid() const
{
	return m_calleeid;
}

void ConfigItem::setConfigId(const QString& configId)
{
	QStringList sl = configId.split(QLatin1Char('/'));
	if(sl.count() >= 2) {
		m_callerid = sl.at(0);
		m_calleeid = sl.at(1);
	} else {
		m_callerid = configId;
	}
}

QString ConfigItem::password() const
{
	return m_password;
}

void ConfigItem::setPassword(const QString& password)
{
	m_password = password;
}

int ConfigItem::autoAnswer() const
{
	return m_autoAnswer;
}

void ConfigItem::setAutoAnswer(int autoAnswer)
{
	m_autoAnswer = autoAnswer;
}

QString ConfigItem::sourceLang() const
{
	return m_sourceLang;
}

void ConfigItem::setSourceLang(const QString& srcLang)
{
	m_sourceLang = srcLang;
}

QString ConfigItem::targetLang() const
{
	return m_targetLang;
}

void ConfigItem::setTargetLang(const QString& tarLang)
{
	m_targetLang = tarLang;
}

int ConfigItem::absoluteTime() const
{
	return m_absoluteTime;
}

void ConfigItem::setAbsoluteTime(int absoluteTime)
{
	m_absoluteTime = absoluteTime;
}

int ConfigItem::autoCall() const
{
	return m_autocall;
}

void ConfigItem::setAutoCall(int autoCall)
{
	m_autocall = autoCall;
}

QWidget* ConfigItem::widget() const
{
	return m_widget;
}

void ConfigItem::setWidget(QWidget* widget)
{
	m_widget = widget;
}

ConfigItem::RegStatus ConfigItem::regStatus() const
{
	return m_regStatus;
}

void ConfigItem::setRegStatus(ConfigItem::RegStatus status)
{
	m_regStatus = status;
}

int ConfigItem::callDuration() const
{
	return m_duration;
}

void ConfigItem::setCallDuration(int duration)
{
	m_duration = duration;
}

ConfigItem::CallStatus ConfigItem::callStatus() const
{
	return m_status;
}

void ConfigItem::setCallStatus(ConfigItem::CallStatus status)
{
	m_status = status;
}

int ConfigItem::idleDuration() const
{
	return m_idleDuration;
}

void ConfigItem::setIdleDuration(int duration)
{
	m_idleDuration = duration;
}


////////////////////////////////////////////////////////////////////////// ConfigInfo

ConfigInfo::ConfigInfo() : QObject(), 
	m_owner(Caller),
	m_server(),
	m_fileName()
{
    
}

ConfigInfo::~ConfigInfo()
{

}

bool ConfigInfo::reload(const QString& fileName)
{
	m_fileName = fileName;
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qCritical() << QPID_DEBUG_PREFIX << "Unable to open file" << fileName;
		return false;
	}

	return import(&file);
}

QString ConfigInfo::fileName() const
{
	return m_fileName;
}

ConfigInfo::ConfigOwner ConfigInfo::owner() const
{
	return m_owner;
}

QString ConfigInfo::server() const
{
	return m_server;
}

bool ConfigInfo::isEmpty() const
{
	return m_listitem.isEmpty();
}

QList<ConfigItem>& ConfigInfo::items()
{
	return m_listitem;
}

bool ConfigInfo::import(QIODevice* device)
{
	if (!device) {
		qDebug() << QPID_DEBUG_PREFIX << "IO device not available";
		return false;
	}

	m_listitem.clear();

	QDomDocument doc;
	doc.setContent(device);

	QDomElement root = doc.documentElement();

	//application 
	QDomNode appConfig = root.firstChild();
	if (!appConfig.isNull())
	{
		QDomElement eleOwner = appConfig.firstChildElement(QLatin1String("owner"));
		QDomElement eleServer = appConfig.firstChildElement(QLatin1String("server"));
		m_owner = (ConfigOwner)eleOwner.text().toInt();
		m_server = eleServer.text();
	}

	//information
	QDomNode infoConfig = appConfig.nextSibling();
	if (!infoConfig.isNull())
	{
		QDomNodeList items = infoConfig.toElement().elementsByTagName(QLatin1String("item"));
		if (items.isEmpty())
			return true;
		
		int index = 0;
		for(; index < items.count(); ++index) {
			QDomNode item = items.at(index);
			if (item.isNull())
				continue;
			
			QDomElement eleId = item.firstChildElement(QLatin1String("id"));
			QDomElement elePwd = item.firstChildElement(QLatin1String("pwd"));
			QDomElement eleAbsolutetime = item.firstChildElement(QLatin1String("absolutetime"));
			QDomElement eleAutocall = item.firstChildElement(QLatin1String("autocall"));
			QDomElement eleSourceLang = item.firstChildElement(QLatin1String("sourcelang"));
			QDomElement eleTargetLang = item.firstChildElement(QLatin1String("targetlang"));
			QDomElement eleAutoAnswer = item.firstChildElement(QLatin1String("autoanswer"));
			ConfigItem ci;
			if (!eleId.isNull())
				ci.setConfigId(eleId.text());
			if(!elePwd.isNull())
				ci.setPassword(elePwd.text());
			if(!eleAbsolutetime.isNull())
				ci.setAbsoluteTime(eleAbsolutetime.text().toInt());
			if(!eleAutocall.isNull())
				ci.setAutoCall(eleAutocall.text().toInt());
			if (!eleSourceLang.isNull())
				ci.setSourceLang(eleSourceLang.text());
			if (!eleTargetLang.isNull())
				ci.setTargetLang(eleTargetLang.text());
			if (!eleAutoAnswer.isNull())
				ci.setAutoAnswer(eleAutoAnswer.text().toInt());

			qDebug() << "Add item : " << ci.callerid() << ci.calleeid() << ci.absoluteTime() << ci.autoCall() << ci.autoAnswer() << ci.sourceLang() << ci.targetLang();

			m_listitem.push_back(ci);
		}

	}

	return true;

}

} // namespace Qpid
