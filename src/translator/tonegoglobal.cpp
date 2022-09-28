#include <QHash>
#include "tonegoglobal.h"

namespace ns {
namespace YesPo {

CallMode callMode(const QString& serviceMode)
{
	if (serviceMode == QLatin1String("a0")
		|| serviceMode == QLatin1String("a1")) {
		return cmTwoWay;
	} else {
		return cmThreeWay;
	}
}

QString majorText(const QString& major)
{
	static QHash<QString, QString> majorMap;
	majorMap[QLatin1String("01")] = QObject::tr("Arts/Design");
	majorMap[QLatin1String("02")] = QObject::tr("Admin/Human Resources");
	majorMap[QLatin1String("03")] = QObject::tr("Accounting/Finance");
	majorMap[QLatin1String("04")] = QObject::tr("Building/Construction");
	majorMap[QLatin1String("05")] = QObject::tr("Computing/Software");
	majorMap[QLatin1String("06")] = QObject::tr("Doctor/Medicine");
	majorMap[QLatin1String("07")] = QObject::tr("Engineering");
	majorMap[QLatin1String("08")] = QObject::tr("Education/Training");
	majorMap[QLatin1String("09")] = QObject::tr("Energy/Environment");
	majorMap[QLatin1String("10")] = QObject::tr("Government");
	majorMap[QLatin1String("11")] = QObject::tr("Hotel/Tourism/Catering");
	majorMap[QLatin1String("12")] = QObject::tr("Insurance");
	majorMap[QLatin1String("13")] = QObject::tr("Information Technology");
	majorMap[QLatin1String("14")] = QObject::tr("Legal/Consulting");
	majorMap[QLatin1String("15")] = QObject::tr("Media/Editor");
	majorMap[QLatin1String("16")] = QObject::tr("Manufacturing/Operations");
	majorMap[QLatin1String("17")] = QObject::tr("Property/Real Estate");
	majorMap[QLatin1String("18")] = QObject::tr("Public Relations/Advertising");
	majorMap[QLatin1String("19")] = QObject::tr("Retired");
	majorMap[QLatin1String("20")] = QObject::tr("Sport");
	majorMap[QLatin1String("21")] = QObject::tr("Soldier");
	majorMap[QLatin1String("22")] = QObject::tr("Service");
	majorMap[QLatin1String("23")] = QObject::tr("Student");
	majorMap[QLatin1String("24")] = QObject::tr("Sales/Marketing");
	majorMap[QLatin1String("25")] = QObject::tr("Senior Management");
	majorMap[QLatin1String("26")] = QObject::tr("Translator");
	majorMap[QLatin1String("27")] = QObject::tr("Transportantion/Logistics");
	majorMap[QLatin1String("28")] = QObject::tr("Other");

	return majorMap.value(major, major);
}

}
} // namespace ns
