#ifndef QPID_SKILLCHANNEL_H
#define QPID_SKILLCHANNEL_H

#include <QtGui/QWidget>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QMovie;
QT_END_NAMESPACE

namespace ns {

class Ui_SkillChannel;

class SkillChannel : public QWidget
{
    Q_OBJECT

	static const QString m_idleStyle;
	static const QString m_waitingStyle;
	static const QString m_ringingStyle;
	static const QString m_confirmingStyle;

public:
    explicit SkillChannel(QWidget* parent = 0);
	explicit SkillChannel(QWidget* parent, bool empty);
    virtual ~SkillChannel();

	void changeStatus(bool online);
	void setTrasnlationLanguage(const QString& source, const QString& target);
	void setLocalLanguage(const QString& country, const QString& language);
	void setStyleCalling();
	void setStyleConfirming();
	void restroeState();
	QString targetLanguage() {return m_targetLang;}
	QString localLanguage() {return m_localLanguage;}

protected:
	void changeEvent(QEvent *event);

private:
	enum RecentCallStatus {
		RecentStatusCalling = 0,
		RecentStatusDisconnect,
		RecentStatusConfirmed
	};
	RecentCallStatus m_recentCallStatus;

	bool m_empty;
	bool m_online;
	bool m_local;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_country;
	QString m_localLanguage;
	QMovie* m_movieCalling;
	QMovie* m_movieConfirming;

	QScopedPointer<Ui_SkillChannel> m_ui;

	void retranslateTr();

protected:
    Q_DISABLE_COPY(SkillChannel)
};

} // namespace ns

#endif // QPID_SKILLCHANNEL_H
