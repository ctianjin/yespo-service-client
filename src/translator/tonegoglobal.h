#ifndef CALLINFO_H
#define CALLINFO_H

#include <QString>

namespace ns {
namespace YesPo {

enum CallMode {
	cmTwoWay = 0,
	cmThreeWay = 1,
};

CallMode callMode(const QString& serviceMode);
QString majorText(const QString& major);

} // namespace ns::YesPo
} // namespace ns

#endif // CALLINFO_H
