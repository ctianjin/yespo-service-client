#ifndef QPID_DTMFPLATE_H
#define QPID_DTMFPLATE_H

#include <QScopedPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_DTMFPlate;

class DTMFPlate : public QWidget
{
	Q_OBJECT

public:
    explicit DTMFPlate(QWidget* parent = 0);
    virtual ~DTMFPlate();

	void setDTMFSize(const QSize& size);

Q_SIGNALS:
	void DTMFClicked(const QChar& c);
	
protected:
	virtual void changeEvent(QEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);

private Q_SLOTS:
	void slotDTMFClicked();

private:
    QScopedPointer<Ui_DTMFPlate> m_ui;

    Q_DISABLE_COPY(DTMFPlate)
};

} // namespace Qpid

#endif // QPID_DTMFPLATE_H
