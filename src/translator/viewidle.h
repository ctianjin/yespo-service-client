#ifndef VIEWIDLE_H
#define VIEWIDLE_H

#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include <QWidget>
#include "reportnotify.h"

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

class ReportNotify;

namespace ns {

class Ui_ViewIdle;

class ViewIdle : public QWidget
{
    Q_OBJECT

public:
    ViewIdle(QWidget* parent = 0);
    ~ViewIdle();

	void setDisplayInfo(const QString& displayName);
	void setPhoto(const QString& photoFile);

	void showNotify(int height);

	ReportNotify* reportNotify() const;
	void setReportNotify(ReportNotify* reportNotify);

Q_SIGNALS:

public Q_SLOTS:

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:
	void slotUpdateTime();
private:

	enum TimeInterval{
		Unknown = 0,
		MorningEve,
		Morning,
		Inmorning,
		EveningEve,
		Evening,
		Inevening
	};

	TimeInterval timeToNameSpace(int& hour);
	QString nameToString(ViewIdle::TimeInterval tInterval);
	void nameToWelcomeCode(ViewIdle::TimeInterval tInterval);

	TimeInterval m_timeInterval;
	void retranslateTr();
	QString m_displayName;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_country;
	QString m_language;
	QString m_welcomeCode;
	QString m_timeCode;

	int m_duration;
	QTimer *m_timer;
	QPointer<ReportNotify> m_reportNotify;
	QScopedPointer<Ui_ViewIdle> m_ui;
};

} // namespace ns

#endif // VIEWIDLE_H
