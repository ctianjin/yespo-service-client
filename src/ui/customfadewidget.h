#ifndef CUSTOMFADEWIDGET_H
#define CUSTOMFADEWIDGET_H

#include <QtGui/QWidget>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QTimer;
class QPixmap;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL CustomFadeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomFadeWidget(QWidget* parent = 0);
    virtual ~CustomFadeWidget();

	void startTimer(const QString& timeString);
	int alpha();
	bool isTimerActive();
	void setFadePixmap(const QPixmap& pic);
	void setUserPixmap(const QPixmap& pic);
	void setTimeString(const QString& timeString);
	QString timeString() const;

private Q_SLOTS:
		void slotTimer();

protected:
	void paintEvent(QPaintEvent *e);

private:
	QString m_timeString;
	bool m_userFade;
	int m_alpha;
	int m_alphaShow;
	QPixmap m_userBackground;
	QPixmap m_background;
	QTimer* m_timer;

    Q_DISABLE_COPY(CustomFadeWidget)
};

} // namespace ns

#endif // CUSTOMFADEWIDGET_H
