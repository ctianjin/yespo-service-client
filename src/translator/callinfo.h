#ifndef QPID_CALLINFO_H
#define QPID_CALLINFO_H

#include <QString>

namespace Qpid {
namespace ToneGo {

enum CallMode {
	cmTwoWay = 0,
	cmThreeWay = 1,
};

CallMode callMode(const QString& serviceMode);

} // namespace Qpid::ToneGo
} // namespace Qpid

#endif // QPID_CALLINFO_H
