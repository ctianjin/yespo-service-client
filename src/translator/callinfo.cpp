#include "callinfo.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "CallInfo:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {
namespace ToneGo {

CallMode callMode(const QString& serviceMode)
{
	if (serviceMode == QLatin1String("a0")
		|| serviceMode == QLatin1String("a1")) {
		return cmTwoWay;
	} else {
		return cmThreeWay;
	}
}

}
} // namespace Qpid
