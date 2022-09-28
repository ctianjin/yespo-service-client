#include <QDialog>
#include "abstractphoneview.h"

namespace Qpid {

AbstractPhoneView::AbstractPhoneView(QWidget* parent) :
    QWidget(parent)
{
}

AbstractPhoneView::~AbstractPhoneView()
{
}

void AbstractPhoneView::signIn()
{
	
}

void AbstractPhoneView::signOut()
{
	QList<QDialog*> allDialogs = findChildren<QDialog*>();
	for(int i = 0; i < allDialogs.size(); ++i) {
		if (allDialogs.at(i)->isModal())
			allDialogs.at(i)->close();
	}
}

} // namespace Qpid
