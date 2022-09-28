#ifndef PAGEFRAMEWINDOW_H
#define PAGEFRAMEWINDOW_H

#include <QtCore/QScopedPointer>
#include <QWidget>
#include <QPointer>
#include <QList>
#include <SmtpMime>

#include "command.h"
#include "request.h"
#include "imcommand.h"
#include "preferencesupport.h"
//#include "sipcommand.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QTreeWidgetItem;
class QMovie;
QT_END_NAMESPACE

namespace ns {

class UserService;
class SipService;
class AudioService;
class CommandService;
class PhotoService;
class CameraService;
class Ui_PageFrameWindow;
class CallSplash;
class PageCalling;
class CallRecordItem;
class SkillChannel;
class SipNotify;
class ReportNotify;
class CallMode;
class WidgetPhotoTranslation;
class YespoProto;

class PageFrameWindow : public QWidget, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(PhotoService* photoService READ photoService WRITE setPhotoService)
	Q_PROPERTY(CameraService* cameraService READ cameraService WRITE setCameraService)
	Q_PROPERTY(PageCalling* pageCalling READ pageCalling WRITE setPageCalling)
	Q_PROPERTY(WidgetPhotoTranslation* widgetPhotoTranslation READ widgetPhotoTranslation WRITE setWidgetPhotoTranslation)
	Q_PROPERTY(YespoProto* proto READ proto WRITE setProto)

public:
	static const QString token;
	static const QString deviceId;
	static const QString language;

    explicit PageFrameWindow(QWidget* parent = 0);
    virtual ~PageFrameWindow();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	PhotoService* photoService() const;
	void setPhotoService(PhotoService* photoService);

	YespoProto* proto() const;
	void setProto(YespoProto* proto);

	CameraService* cameraService() const;
	void setCameraService(CameraService* cameraService);

	PageCalling* pageCalling() const;
	void setPageCalling(PageCalling* pageCalling);

	WidgetPhotoTranslation* widgetPhotoTranslation() const;
	void setWidgetPhotoTranslation(WidgetPhotoTranslation* widgetPhotoTranslation);

	SipNotify* sipNotify() const;
	void setSipNotify(SipNotify* sipNotify);

	/*ReportNotify* reportNotify() const;*/
	void setReportNotify(ReportNotify* reportNotify);

	CallMode* callMode() const;
	void setCallMode(CallMode* callMode);

	virtual void setPreference(Preference* pref);

	void setHistoryLink(const QString& link);

	void refreshReportNum();

	void logout();

	void setSkill(const QStringList& skill);

	void setThreeWaySkill(const QStringList& skill);

	void setLocal(const QStringList& local);

	void setNetworkAddress(const QString& address);

	void callSplashNotify();

public Q_SLOTS:
	void showMainWindow();
	void showHome();

Q_SIGNALS:
	void callIn();
	void callConfirmed(bool callLocal);
	void callClosed();
	void callSucceed(const QString& cdrId);
	void prepareFinished();

protected:
	virtual void changeEvent(QEvent * event);
//	virtual void resizeEvent(QResizeEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);

public Q_SLOTS:
	void on_actionOffline_triggered();
	void on_actionOnline_triggered();
	void on_actionAll_triggered();
	void on_actionMissed_triggered();
	void on_actionAnswered_triggered();
	void on_pushButtonHistoryDetail_clicked();

	//void on_lineEditFilter_textChanged(const QString& text);

	void slotSIPRegState(int account, int code, const QString& reason);
	void slotSIPIncomingCallEx(const QStringList& callInContentList);
	void slotSIPCallState(int callID, int state);

	void slotSplashDecline();
	void slotSplashAnswer();

	void slotDelayCDR();
	void slotCDRFinished(const Command::GetCDRListParam& param, const Command::GetCDRListResult& result);
	void slotCDRInfoFinished(const Command::GetCDRInfoParam& param, const Command::GetCDRInfoResult& result);
	void slotUserInfoFinished(const Command::GetUserInfoParam& param, const Command::GetUserInfoResult& result);
	void slotCallerUserFinished(const Command::GetCallerUserParam& param, const Command::GetCallerUserResult& result);
	void slotChangeStatusFinished(const Command::ChangeStatusParam& param, const Command::ChangeStatusResult& result);
	void slotMemberInfoFinished(const Command::GetMemberInfoParam& param, const Command::GetMemberInfoResult& result);
	void slotGetBlacklistInfoFinished(const Command::GetBlacklistParam& param, const Command::GetBlacklistResult& result);
	void slotGetUploadPhotoFinished(const Command::GetUploadPhotoParam& param, const Command::GetUploadPhotoResult& result);
	void slotTodayStatFinished(const Command::TodayStatParam& param, const Command::TodayStatResult& result);
	void slotGetNoReportInfoFinished(const Command::GetNoReportParam& param, const Command::GetNoReportResult& result);

	void slotPhotoDownloaded(const QString& url, const QString& fileName);
	void slotPhotoDownloadFailed(const QString& url);

	void slotCaptureFinished(const QString& fileName);

	void slotUserPhotoClicked();

	void slotSmtpSendSucceed();

	void slotImessageFinished(const ImCommand::GetUploadPhotoMsgResult& result);
	void slotImessageError();

	void slotCalleeNotify(const CalleeNotify& notify);
	void slotUnregNotify();
	void slotPhotoNotify(const PhotoNotify& notify);
private:
	void removeSkillWidget();
	void retranslateTr();
	void doRecentFilter(const QString& text);
	void updateRecentTypeAction();
	void updateStatistics();
	void updateRecentStatusAction();
	void restoreState();
	void setStateConfirmed();
	void setStateCalling();
	bool isTodayRecord(const QString& startCallTime);
	void sendEmil(const QString& dateTime, const QString& code, const QString& reason);

	SmtpClient m_smtp;
	MimeMessage m_message;
	EmailAddress m_sender;
	EmailAddress m_recipient;
	MimeText m_text;
	int m_answeredCalls;
	int m_succeedCalls;
	int m_missedCalls;

	QString m_sourceLanguage;
	QString m_targetLanguage;

	QString m_historyLink;

	int m_locate;
	QMovie *m_movieCalling;
	QMovie* m_movieConfirming;

	QMenu* m_menuStatus;
	QMenu* m_menuRecent;

	int m_callId;
	QString m_callMode;
	QString m_callSrcLang;
	QString m_callTarLang;
	QString m_caller;
	QString m_cdrId;
	QString m_callee;
	bool m_callLocal;
	CallSplash* m_callSplash;
	QString m_networkAddress;

	QHash<QString, CallRecordItem*> m_cdrItems;
	enum RecentType
	{
		RecentTypeAll = 0,
		RecentTypeMissed = 1,
		RecentTypeAnswered = 2
	};
	RecentType m_recentType;

	enum RecentStatus {
		RecentStatusOnline = 0,
		RecentStatusOffline
	};
	RecentStatus m_recentStatus;

	QList<SkillChannel*> m_skillWidgetList;
	QList<SkillChannel*> m_threeWaySkillWidgetList;

	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
	QPointer<AudioService> m_audioService;
	QPointer<CommandService> m_commandService;
	QPointer<PhotoService> m_photoService;
	QPointer<CameraService> m_cameraService;
	QPointer<YespoProto> m_proto;
	QPointer<PageCalling> m_pageCalling;
	QPointer<WidgetPhotoTranslation> m_widgetPhotoTranslation;
	QPointer<SipNotify> m_sipNotify;
	QPointer<ReportNotify> m_reportNotify;
	QPointer<CallMode> m_widgetCallMode;

	QScopedPointer<Ui_PageFrameWindow> m_ui;
};

} // namespace ns

#endif // PAGEFRAMEWINDOW_H
