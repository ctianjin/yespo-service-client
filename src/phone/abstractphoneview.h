#ifndef QPID_ABSTRACTPHONEVIEW_H
#define QPID_ABSTRACTPHONEVIEW_H

#include <QWidget>

namespace Qpid {

class AbstractPhoneView : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractPhoneView(QWidget* parent = 0);
    virtual ~AbstractPhoneView();
	
	virtual void signIn();
	virtual void signOut();

public Q_SLOTS:

Q_SIGNALS:

private:
    Q_DISABLE_COPY(AbstractPhoneView)
};

} // namespace Qpid

#endif // QPID_ABSTRACTPHONEVIEW_H
