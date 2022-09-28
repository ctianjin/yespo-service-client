#include <QDebug>
#include <QMenu>
#include <QUrl>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QResizeEvent>
#include <QPainter>
#include <QBitmap>
#include <QMainWindow>
#include <QScrollArea>
#include <QMovie>
#include <QVBoxLayout>
#include <QDateTime>

#include "pageframewindow.h"
#include "ui_pageframewindow.h"
#include "phoneapplication.h"
#include "userservice.h"
#include "sipservice.h"
#include "audioservice.h"
#include "commandservice.h"
#include "photoservice.h"
#include "yespoproto.h"
#include "cameraservice.h"
#include "im.h"
#include "callsplash.h"
#include "pagecalling.h"
#include "recentlistitems_p.h"
#include "skillchannel.h"
#include "sipnotify.h"
#include "reportnotify.h"
#include "callmode.h"
#include "widgetphototranslation.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PageFrameWindow:"
#endif // DEBUG_PREFIX

namespace ns {

const QString PageFrameWindow::token = QLatin1String("token");
const QString PageFrameWindow::deviceId = QLatin1String("device_id");
const QString PageFrameWindow::language = QLatin1String("lang");

struct PageFrameWindowOptions
{
	static const QString recent_photo_;
	static const QString close_callin_;
	static const QString callin_prompt_;
};
const QString PageFrameWindowOptions::recent_photo_ = QLatin1String("ui.settings.general.recent_photo");
const QString PageFrameWindowOptions::close_callin_ = QLatin1String("ui.settings.callin.close_callin");
const QString PageFrameWindowOptions::callin_prompt_ = QLatin1String("ui.settings.callin.callin_prompt");

PageFrameWindow::PageFrameWindow(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_PageFrameWindow),
	m_cdrId(QString()),
	m_answeredCalls(0),
	m_succeedCalls(0),
	m_missedCalls(0),
	m_menuStatus(0),
	m_menuRecent(0),
	m_callSplash(0),
	m_recentType(RecentTypeAll),
	m_recentStatus(RecentStatusOnline),
	m_callId(-1),
	m_locate(-1),
	m_smtp(QLatin1String("smtp.163.com"), 465, SmtpClient::SslConnection),
	m_sender(QLatin1String("alks651843787@163.com")),
	m_recipient(QLatin1String("haris.chan@yespo.com"), QLatin1String("Whoever"))
	//m_recipient(QLatin1String("sysalarm@chnlove.com"), QLatin1String("Whoever"))
{
    m_ui->setupUi(this);

	m_smtp.setUser(QLatin1String("alks651843787@163.com"));
	m_smtp.setPassword(QLatin1String("nba8999761"));
	m_smtp.setCrash(false);
	connect(&m_smtp, SIGNAL(smtpSuccess()), this, SLOT(slotSmtpSendSucceed()));

	m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewIdle);

	m_menuStatus = new QMenu(this);
	if (m_menuStatus) {
		m_menuStatus->addAction(m_ui->actionOnline);
		m_menuStatus->addAction(m_ui->actionOffline);
		m_ui->pushButtonStatus->setMenu(m_menuStatus);
	}

	m_menuRecent = new QMenu(this);
	if (m_menuRecent) {
		m_menuRecent->addAction(m_ui->actionAll);
		m_menuRecent->addAction(m_ui->actionMissed);
		m_menuRecent->addAction(m_ui->actionAnswered);
//		m_ui->pushButtonFilter->setMenu(m_menuRecent);
	}

	m_callSplash = new CallSplash(QApplication::desktop());
	connect(m_callSplash, SIGNAL(answer()), this, SLOT(slotSplashAnswer()));
	connect(m_callSplash, SIGNAL(decline()), this, SLOT(slotSplashDecline()));

	m_movieCalling = new QMovie(QLatin1String(":/images/ui/calling.gif"));
	m_movieConfirming = new QMovie(QLatin1String("images/ui/confirming.gif"));

	connect(m_ui->labelUserPhoto, SIGNAL(clicked()),  this, SLOT(slotUserPhotoClicked()));
}

PageFrameWindow::~PageFrameWindow()
{
	removeSkillWidget();
}

UserService* PageFrameWindow::userService() const
{
	return m_userService;
}

void PageFrameWindow::setUserService(UserService* userService)
{
	if (m_userService) {
	}

	m_userService = userService;

	if (m_userService) {
	}
}

SipService* PageFrameWindow::sipService() const
{
	return m_sipService;
}

void PageFrameWindow::setSipService(SipService* sipService)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotSIPRegState(int, int, const QString&)));

		disconnect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSIPCallState(int, int)));

		disconnect(m_sipService, SIGNAL(incomingCallEx(const QStringList&)), 
			this, SLOT(slotSIPIncomingCallEx(const QStringList&)));
	}

	m_sipService = sipService;
	m_ui->viewIncomingCall->setSipService(m_sipService);

	if (m_sipService) {
		connect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotSIPRegState(int, int, const QString&)), Qt::QueuedConnection);

		connect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSIPCallState(int, int)), Qt::QueuedConnection);

		connect(m_sipService, SIGNAL(incomingCallEx(const QStringList&)), 
			this, SLOT(slotSIPIncomingCallEx(const QStringList&))/*, Qt::DirectConnection*/);
	}
}

AudioService* PageFrameWindow::audioService() const
{
	return m_audioService;
}

void PageFrameWindow::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
	}

	m_audioService = audioService;
	m_ui->viewIncomingCall->setAudioService(m_audioService);

	if (m_audioService) {
	}
}

CommandService* PageFrameWindow::commandService() const
{
	return m_commandService;
}

void PageFrameWindow::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(getcdrlistFinished(const Command::GetCDRListParam&, const Command::GetCDRListResult&)), 
			this, SLOT(slotCDRFinished(const Command::GetCDRListParam&, const Command::GetCDRListResult&)));
		disconnect(m_commandService, SIGNAL(getcdrinfoFinished(const Command::GetCDRInfoParam&, const Command::GetCDRInfoResult&)), 
			this, SLOT(slotCDRInfoFinished(const Command::GetCDRInfoParam&, const Command::GetCDRInfoResult&)));

		disconnect(m_commandService, SIGNAL(getuserinfoFinished(const Command::GetUserInfoParam&, const Command::GetUserInfoResult&)), 
			this, SLOT(slotUserInfoFinished(const Command::GetUserInfoParam&, const Command::GetUserInfoResult&)));

		disconnect(m_commandService, SIGNAL(getcalleruserFinished(const Command::GetCallerUserParam&, const Command::GetCallerUserResult&)), 
			this, SLOT(slotCallerUserFinished(const Command::GetCallerUserParam&, const Command::GetCallerUserResult&)));

		disconnect(m_commandService, SIGNAL(changestatusFinished(const Command::ChangeStatusParam&, const Command::ChangeStatusResult&)), 
			this, SLOT(slotChangeStatusFinished(const Command::ChangeStatusParam&, const Command::ChangeStatusResult&)));

		disconnect(m_commandService, SIGNAL(getmemberinfoFinished(const Command::GetMemberInfoParam&, const Command::GetMemberInfoResult&)),
			this, SLOT(slotMemberInfoFinished(const Command::GetMemberInfoParam&, const Command::GetMemberInfoResult&)));

		disconnect(m_commandService, SIGNAL(getblacklistFinished(const Command::GetBlacklistParam&, const Command::GetBlacklistResult&)),
			this, SLOT(slotGetBlacklistInfoFinished(const Command::GetBlacklistParam&, const Command::GetBlacklistResult&)));

		disconnect(m_commandService, SIGNAL(getuploadphotoFinished(const Command::GetUploadPhotoParam&, const Command::GetUploadPhotoResult&)),
			this, SLOT(slotGetUploadPhotoFinished(const Command::GetUploadPhotoParam&, const Command::GetUploadPhotoResult&)));

		disconnect(m_commandService, SIGNAL(todayStatFinished(const Command::TodayStatParam&, const Command::TodayStatResult&)),
			this, SLOT(slotTodayStatFinished(const Command::TodayStatParam&, const Command::TodayStatResult&)));

		disconnect(m_commandService, SIGNAL(getnoreportinfoFinished(const Command::GetNoReportParam&, const Command::GetNoReportResult&)),
			this, SLOT(slotGetNoReportInfoFinished(const Command::GetNoReportParam&, const Command::GetNoReportResult&)));
	}

	m_commandService = commandService;
	m_ui->viewContact->setCommandService(m_commandService);

	if (m_commandService) {
		connect(m_commandService, SIGNAL(getcdrlistFinished(const Command::GetCDRListParam&, const Command::GetCDRListResult&)), 
			this, SLOT(slotCDRFinished(const Command::GetCDRListParam&, const Command::GetCDRListResult&)));
		connect(m_commandService, SIGNAL(getcdrinfoFinished(const Command::GetCDRInfoParam&, const Command::GetCDRInfoResult&)), 
			this, SLOT(slotCDRInfoFinished(const Command::GetCDRInfoParam&, const Command::GetCDRInfoResult&)));

		connect(m_commandService, SIGNAL(getuserinfoFinished(const Command::GetUserInfoParam&, const Command::GetUserInfoResult&)), 
			this, SLOT(slotUserInfoFinished(const Command::GetUserInfoParam&, const Command::GetUserInfoResult&)));

		connect(m_commandService, SIGNAL(getcalleruserFinished(const Command::GetCallerUserParam&, const Command::GetCallerUserResult&)), 
			this, SLOT(slotCallerUserFinished(const Command::GetCallerUserParam&, const Command::GetCallerUserResult&)));

		connect(m_commandService, SIGNAL(changestatusFinished(const Command::ChangeStatusParam&, const Command::ChangeStatusResult&)), 
			this, SLOT(slotChangeStatusFinished(const Command::ChangeStatusParam&, const Command::ChangeStatusResult&)));

		connect(m_commandService, SIGNAL(getmemberinfoFinished(const Command::GetMemberInfoParam&, const Command::GetMemberInfoResult&)),
			this, SLOT(slotMemberInfoFinished(const Command::GetMemberInfoParam&, const Command::GetMemberInfoResult&)));

		connect(m_commandService, SIGNAL(getblacklistFinished(const Command::GetBlacklistParam&, const Command::GetBlacklistResult&)),
			this, SLOT(slotGetBlacklistInfoFinished(const Command::GetBlacklistParam&, const Command::GetBlacklistResult&)));

		connect(m_commandService, SIGNAL(getuploadphotoFinished(const Command::GetUploadPhotoParam&, const Command::GetUploadPhotoResult&)),
			this, SLOT(slotGetUploadPhotoFinished(const Command::GetUploadPhotoParam&, const Command::GetUploadPhotoResult&)));

		connect(m_commandService, SIGNAL(todayStatFinished(const Command::TodayStatParam&, const Command::TodayStatResult&)),
			this, SLOT(slotTodayStatFinished(const Command::TodayStatParam&, const Command::TodayStatResult&)));

		connect(m_commandService, SIGNAL(getnoreportinfoFinished(const Command::GetNoReportParam&, const Command::GetNoReportResult&)),
			this, SLOT(slotGetNoReportInfoFinished(const Command::GetNoReportParam&, const Command::GetNoReportResult&)));
	}
}

PhotoService* PageFrameWindow::photoService() const
{
	return m_photoService;
}

void PageFrameWindow::setPhotoService(PhotoService* photoService)
{
	if (m_photoService) {
		disconnect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoDownloaded(const QString&, const QString&)));
		disconnect(m_photoService, SIGNAL(photoDownloadFailed(const QString&)), 
			this, SLOT(slotPhotoDownloadFailed(const QString&)));
	}

	m_photoService = photoService;

	if (m_photoService) {
		connect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoDownloaded(const QString&, const QString&)));
		connect(m_photoService, SIGNAL(photoDownloadFailed(const QString&)), 
			this, SLOT(slotPhotoDownloadFailed(const QString&)));
	}
}

YespoProto* PageFrameWindow::proto() const 
{
	if (m_proto)
		return m_proto;
	return NULL;
}

void PageFrameWindow::setProto(YespoProto* proto)
{
	if (m_proto) {
		disconnect(m_proto, SIGNAL(calleeNotify(const CalleeNotify&)), this, SLOT(slotCalleeNotify(const CalleeNotify&)));
		disconnect(m_proto, SIGNAL(unregNotify()), this, SLOT(slotUnregNotify()));
		disconnect(m_proto, SIGNAL(photoNotify(const PhotoNotify&)), this, SLOT(slotPhotoNotify(const PhotoNotify&)));
	}

	m_proto = proto;

	if (m_proto) {
		connect(m_proto, SIGNAL(calleeNotify(const CalleeNotify&)), this, SLOT(slotCalleeNotify(const CalleeNotify&)));
		connect(m_proto, SIGNAL(unregNotify()), this, SLOT(slotUnregNotify()));
		connect(m_proto, SIGNAL(photoNotify(const PhotoNotify&)), this, SLOT(slotPhotoNotify(const PhotoNotify&)));
	}
}

CameraService* PageFrameWindow::cameraService() const
{
	return m_cameraService;
}

void PageFrameWindow::setCameraService(CameraService* cameraService)
{
	if (m_cameraService) {
		disconnect(m_cameraService, SIGNAL(captureFinished(const QString&)), this, SLOT(slotCaptureFinished(const QString&)));
	}

	m_cameraService = cameraService;
//	m_ui->viewIncomingCall->setCameraService(m_cameraService);

	if (m_cameraService) {
		connect(m_cameraService, SIGNAL(captureFinished(const QString&)), this, SLOT(slotCaptureFinished(const QString&)));
	}
}

void PageFrameWindow::setPageCalling(PageCalling* pageCalling)
{
	if (m_pageCalling) {
		disconnect(m_pageCalling, SIGNAL(captureFinished(const QString&)), this, SLOT(slotCaptureFinished(const QString&)));
	}

	m_pageCalling = pageCalling;

	if (m_pageCalling) {
		connect(m_pageCalling, SIGNAL(captureFinished(const QString&)), this, SLOT(slotCaptureFinished(const QString&)));
	}
}

PageCalling* PageFrameWindow::pageCalling() const
{
	return m_pageCalling;
}

WidgetPhotoTranslation* PageFrameWindow::widgetPhotoTranslation() const 
{
	if (m_widgetPhotoTranslation)
		return m_widgetPhotoTranslation;
	return NULL;
}

void PageFrameWindow::setWidgetPhotoTranslation(WidgetPhotoTranslation* widgetPhotoTranslation)
{
	if (m_widgetPhotoTranslation) {

	}

	m_widgetPhotoTranslation = widgetPhotoTranslation;

	if (m_widgetPhotoTranslation) {

	}
}

SipNotify* PageFrameWindow::sipNotify() const 
{
	if (m_sipNotify)
		return m_sipNotify;
	return NULL;
}

void PageFrameWindow::setSipNotify(SipNotify* sipNotify)
{
	if (sipNotify) {
	}

	m_sipNotify = sipNotify;
	m_sipNotify->setParent(this);

	if (m_sipNotify) {
	}
}

// ReportNotify* PageFrameWindow::reportNotify() const 
// {
// 	if (m_reportNotify) 
// 		return m_reportNotify;
// 	return NULL;
// }

void PageFrameWindow::setReportNotify(ReportNotify* reportNotify)
{
// 	if (m_reportNotify) {
// 	}

	//m_reportNotify = reportNotify;
	m_ui->viewIdle->setReportNotify(reportNotify);
// 	m_reportNotify->setParent(this);

// 	if (m_reportNotify) {
// 
// 	}
}

CallMode* PageFrameWindow::callMode() const 
{
	if (m_widgetCallMode)
		return m_widgetCallMode;
	return NULL;
}

void PageFrameWindow::setCallMode(CallMode* callMode)
{
	if (m_widgetCallMode) {
	}

	m_widgetCallMode = callMode;
	m_ui->viewIncomingCall->setCallMode(m_widgetCallMode);

	if (m_widgetCallMode) {
	}
}

void PageFrameWindow::setPreference(Preference* pref)
{
	PreferenceSupport::setPreference(pref);

	m_ui->viewIncomingCall->setPreference(new Preference(pref, QLatin1String("ui.settings.hint")));
}

void PageFrameWindow::setHistoryLink(const QString& link)
{
	m_historyLink = link;
}

void PageFrameWindow::refreshReportNum()
{
	if (m_commandService) {
		Command::GetNoReportParam param;
		m_commandService->getnoreportinfo(param);
	}
}

void PageFrameWindow::logout()
{
	if (m_sipService) {
		if (m_callId >= 0) {
			m_sipService->hangupCall(m_callId);
		}
		m_sipService->unregisterServer();
	}

	m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewIdle);
	m_skillWidgetList.clear();
	m_threeWaySkillWidgetList.clear();
	removeSkillWidget();
}

void PageFrameWindow::setSkill(const QStringList& skill)
{
	if (skill.size() > 0) {
		QString skillInfo;
		QStringList skillDetail;
		for (int i = 0; i < skill.size(); i++) {
			skillInfo = skill.at(i);
			skillDetail = skillInfo.split(QLatin1String("|"));
			if(skillDetail.size() >= 4) {
				SkillChannel* skill = new SkillChannel(m_ui->widgetTranslation);
				skill->setTrasnlationLanguage(skillDetail.at(0), skillDetail.at(1));
				m_ui->verticalLayout_6->addWidget(skill);
				m_skillWidgetList.append(skill);
			}		
		}
	} else {
		SkillChannel* skill = new SkillChannel(m_ui->widgetTranslation, true);
		m_ui->verticalLayout_6->addWidget(skill);
		m_skillWidgetList.append(skill);
	}
}

void PageFrameWindow::setThreeWaySkill(const QStringList& skill)
{
	if (skill.size() > 0) {
		QString skillInfo;
		QStringList skillDetail;
		for (int i = 0; i < skill.size(); i++) {
			skillInfo = skill.at(i);
			skillDetail = skillInfo.split(QLatin1String("|"));
			if(skillDetail.size() >= 4) {
				SkillChannel* skill = new SkillChannel(m_ui->widgetLocal);
				skill->setTrasnlationLanguage(skillDetail.at(0), skillDetail.at(1));
				m_ui->verticalLayout_7->insertWidget(i + 1,skill);
				m_threeWaySkillWidgetList.append(skill);
			}		
		}
	} else {
		SkillChannel* skill = new SkillChannel(m_ui->widgetLocal, true);
		m_ui->verticalLayout_6->addWidget(skill);
		m_threeWaySkillWidgetList.append(skill);
	}
}

void PageFrameWindow::setLocal(const QStringList& local)
{
	if(local.size() > 0) {
		QString localInfo;
		QStringList localDetail;
		for(int i = 0; i < local.size(); i++) {
			localInfo = local.at(i);
			localDetail = localInfo.split(QLatin1String("|"));
			if(localDetail.size() >= 2) {	
				SkillChannel* skill = new SkillChannel(m_ui->widgetLocal);
				skill->setLocalLanguage(localDetail.at(0), localDetail.at(1));
				m_ui->verticalLayout_7->insertWidget(i + 1,skill);
				m_skillWidgetList.append(skill);
			}	
		}
	} else {
		SkillChannel* skill = new SkillChannel(m_ui->widgetLocal, true);
		m_ui->verticalLayout_7->insertWidget(1,skill);
		m_skillWidgetList.append(skill);
	}
}

void PageFrameWindow::setNetworkAddress(const QString& address)
{
	m_networkAddress = address;
}

void PageFrameWindow::callSplashNotify()
{
 	typedef PageFrameWindowOptions Opt;
 	Preference* pref = preference();
 	if (pref) {
 
 		bool closeCallin = pref->getBool(Opt::close_callin_, false);
 		int callinPrompt = pref->getInt(Opt::callin_prompt_, 0);
 
 		if (!closeCallin && callinPrompt!=2) {
 			if (m_callSplash) {
 				m_callSplash->slowShow(callinPrompt==0);
 			}
 		}
 	}
}

void PageFrameWindow::showMainWindow()
{
	m_ui->actionOnline->setChecked(true);
	m_ui->actionOffline->setChecked(false);
	m_ui->pushButtonStatus->setText(m_ui->actionOnline->text());

	m_recentType = RecentTypeAll;
	updateRecentTypeAction();

	if (m_userService) {
		m_ui->labelDisplayName->setText(m_userService->displayName());
		m_ui->labelDisplayID->setText(m_userService->loginId());
		m_ui->viewIdle->setDisplayInfo(m_userService->displayName());
		//m_ui->viewIdle->setLocal(m_userService->local());
		setSkill(m_userService->skill());
		setThreeWaySkill(m_userService->skill());
		//setLocal(m_userService->local());

		if (m_photoService) {
			if (m_photoService->isReady(m_userService->photo())) {
				QString fileName = m_photoService->photoFile(m_userService->photo());
				QPixmap pic(fileName);
				m_ui->labelUserPhoto->setPixmap(pic.scaled(m_ui->labelUserPhoto->size()));
				m_ui->viewIdle->setPhoto(fileName);

				typedef PageFrameWindowOptions Opt;
				Preference* pref = preference();
				if (pref) {
					pref->putString(Opt::recent_photo_, fileName);
				}

			} else {
				if (!m_photoService->isDownloading(m_userService->photo()))
					m_photoService->downloadPhoto(m_userService->photo());
			}
		}

// 		m_userService->todayCallRecord(m_answeredCalls, m_succeedCalls, m_missedCalls);
// 		updateStatistics();

	}
#if 0
	User::CallRecordList cdrList = m_userService->findCallRecord();
	foreach(const User::CallRecord& cdr, cdrList) {
		CallRecordItem* newItem = new CallRecordItem(m_ui->treeWidgetRecent, cdr);
		if (newItem) {
			newItem->setPhotoService(m_photoService);
			m_cdrItems.insert(cdr.meetCdrId(), newItem);
		}
	}
#endif
	if (m_commandService) {
		Command::TodayStatParam paramStat;
		m_commandService->todayStat(paramStat);

		Command::GetNoReportParam paramReport;
		m_commandService->getnoreportinfo(paramReport);

		QString recentBlacklistID = m_userService->recentBlacklistID();
		Command::GetBlacklistParam param;
		param.setId(recentBlacklistID);
		m_commandService->getblacklist(param);
	}
}

void PageFrameWindow::showHome()
{
	if (m_ui->stackedWidgetView->currentWidget() == m_ui->viewIncomingCall)
		return;

	m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewIdle);
}

void PageFrameWindow::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

// void PageFrameWindow::resizeEvent(QResizeEvent* event)
// {
// 	QWidget::resizeEvent(event);
// 
// 	const QSize size = event->size();
// 	QPixmap pixmap(size);
// 	QPainter painter(&pixmap);
// 	painter.fillRect(pixmap.rect(), Qt::white);
// 	painter.setBrush(Qt::black);
// 
// 	QPainterPath path;
// 	path.setFillRule(Qt::WindingFill);
// 	path.addRoundedRect(0, 0, size.width(), size.height(), 4, 4);
// 	path.addRect(QRect(0,0,size.width(),50));
// 	path.addRect(QRect(size.width()-50,size.height()-50,50,50));
// 	painter.drawPath( path.simplified() );
// 
// 	setMask(pixmap.createMaskFromColor(Qt::white));
// }

void PageFrameWindow::applyPreference(const QSet<QString>& paths, bool force)
{
	typedef PageFrameWindowOptions Opt;
	Preference* pref = preference();
	if (pref) {
	}
}

void PageFrameWindow::on_actionOffline_triggered()
{
	if (m_commandService) {
		Command::ChangeStatusParam param;
		param.setOnlineVisibleStatus(QLatin1String("2"));
		m_commandService->changestatus(param);
	}
}

void PageFrameWindow::on_actionOnline_triggered()
{
	if (m_commandService) {
		Command::ChangeStatusParam param;
		param.setOnlineVisibleStatus(QLatin1String("1"));
		m_commandService->changestatus(param);
	}
}

void PageFrameWindow::on_actionAll_triggered()
{
	m_recentType = RecentTypeAll;
	updateRecentTypeAction();
	doRecentFilter(QString());	
}

void PageFrameWindow::on_actionMissed_triggered()
{
	m_recentType = RecentTypeMissed;
	updateRecentTypeAction();
	doRecentFilter(QString());	
}

void PageFrameWindow::on_actionAnswered_triggered()
{
	m_recentType = RecentTypeAnswered;
	updateRecentTypeAction();
	doRecentFilter(QString());	
}

void PageFrameWindow::on_pushButtonHistoryDetail_clicked()
{
	if (m_historyLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_historyLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	QDesktopServices::openUrl(url);
}

// void PageFrameWindow::on_treeWidgetRecent_itemClicked(QTreeWidgetItem * item, int column)
// {
// 	if (m_ui->stackedWidgetView->currentWidget() == m_ui->viewIncomingCall)
// 		return;
// 
// 	if (item && item->type() == ItemBase::CallRecordItemType) {
// 		CallRecordItem* cdrItem = static_cast<CallRecordItem*>(item);
// 		
// 		if (cdrItem) {
// 			if (cdrItem->cdr().isCallerVeLoginIdNull()) {
// 				if (m_commandService) {
// 					Command::GetCDRInfoParam param;
// 					param.setMeetCDRId(cdrItem->cdr().meetCdrId());
// 					m_commandService->getcdrinfo(param);
// 				}
// 			} else {
// 
// 				User::CallRecord cdr = cdrItem->cdr();
// 				m_ui->viewRecentDetail->setCallerInfo(cdr.callerDisplayName(), cdr.callerMobile(), cdr.callerLargePhoto());
// 				m_ui->viewRecentDetail->setCalleeInfo(cdr.calleeDisplayName(), cdr.calleeMobile(), cdr.calleeLargePhoto());
// 				m_ui->viewRecentDetail->setPartnerInfo(cdr.partnerLargePhoto());
// 				m_ui->viewRecentDetail->setCallDetail(cdr.startCallTime(), cdr.meetCdrId(), cdr.callTime(), cdr.callMode());
// 				m_ui->viewRecentDetail->setLanguage(cdr.language1(), cdr.language2());
// 				if (m_photoService) {
// 					m_ui->viewRecentDetail->setCallerPhoto(cdr.callerLargePhoto(), m_photoService->photoFile(cdr.callerLargePhoto()));
// 					m_ui->viewRecentDetail->setPartnerPhoto(cdr.partnerLargePhoto(), m_photoService->photoFile(cdr.partnerLargePhoto()));
// 					m_ui->viewRecentDetail->setCalleePhoto(cdr.calleeLargePhoto(), m_photoService->photoFile(cdr.calleeLargePhoto()));
// 				}
// 			}
// 			m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewRecentDetail);
// 		}
// 		
// 		if (column == ItemBase::PhotoColumn) {
// 			if(m_commandService) {
// 				Command::GetMemberInfoParam param;
// 				param.setVESysId(cdrItem->cdr().callerVeSysId());
// 				m_commandService->getmemberinfo(param);				
// 			}
// 			m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewContact);
// 		}
// 	}
// }

// void PageFrameWindow::on_lineEditFilter_textChanged(const QString& text)
// {
// 	const QString filter = m_ui->lineEditFilter->text();
// 	doRecentFilter(filter);
// }

void PageFrameWindow::slotSIPRegState(int account, int code, const QString& reason)
{
 	qWarning() << DEBUG_PREFIX << account << code;
// 	if (code == 200) {
// 		if (m_sipNotify && m_sipNotify->isVisible()) {
// 			m_sipNotify->registedSucceed();
// 		}
// 		if (m_lastCode != 200) {
// 			sendEmil(QDateTime::currentDateTime().toString(Qt::SystemLocaleDate), QString::number(code), reason);
// 
// 			QFile file(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + QLatin1String("/") + m_userService->displayName());
// 			if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
// 				QString time = QDateTime::currentDateTime().toString(Qt::SystemLocaleDate);
// 				QTextStream stream(&file);
// 				stream << time << QLatin1Char(' ') << code << QLatin1Char(' ')<< reason << QLatin1String("\n");
// 				file.close();
// 			}
// 			m_lastCode = 200;
// 		}
// 	} else if (code == 401) {
// 
// 	} else {	
// 		sendEmil(QDateTime::currentDateTime().toString(Qt::SystemLocaleDate), QString::number(code), reason);
// 
// 		if (m_userService) {
// 			QFile file(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + QLatin1String("/") + m_userService->displayName());
// 			if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
// 				QString errortime = QDateTime::currentDateTime().toString(Qt::SystemLocaleDate);
// 				QString errorreason = reason.trimmed();
// 				QTextStream errorStream(&file);
// 				errorStream << errortime << QLatin1Char(' ') << code << QLatin1Char(' ') << errorreason << QLatin1Char(' ') << m_userService->sipServiceHost() << QLatin1String("\n") ;
// 				m_lastCode = 408;
// 				file.close();
// 			}
// 		}	
// 		if (m_sipNotify) {
// 			int locationH = 0;
// 			if (m_ui->viewIdle->reportNotify()->isVisible()) {
// 				locationH = m_ui->viewIdle->reportNotify()->height();
// 			}
// 			m_sipNotify->setGeometry(m_ui->widgetRightPanel->x(), m_ui->widgetRightPanel->y() + locationH, m_ui->widgetRightPanel->width(), 32);
// 			m_sipNotify->show();
// 			m_sipNotify->setError(QString::number(code), reason);
// 		}
// 	}
}

void PageFrameWindow::slotSIPIncomingCallEx(const QStringList& callInContentList)
{
	if (m_callMode == QLatin1String("a1")) {
		for(int i = 0; i < m_skillWidgetList.size(); i++) {
			if (m_callTarLang == m_skillWidgetList.at(i)->targetLanguage()) {
				m_locate = i;
				m_skillWidgetList.at(i)->setStyleCalling();
				break;
			}
		}
	} else {
		for(int i = 0; i < m_skillWidgetList.size(); i++) {
			if (m_callTarLang == m_threeWaySkillWidgetList.at(i)->targetLanguage()) {
				m_locate = i;
				m_threeWaySkillWidgetList.at(i)->setStyleCalling();
				break;
			}
		}
	}

	m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewIncomingCall);

	m_callId = callInContentList.at(SipService::CallID).toInt();
	m_ui->viewIncomingCall->callIncoming(m_callId);

	if (m_pageCalling) {
		m_pageCalling->setCallId(m_callId);
		if (m_widgetCallMode)
			m_pageCalling->setCallMode(m_widgetCallMode);
	}
	if (m_callSplash)
		m_callSplash->setCallId(m_callId);

	Q_EMIT callIn();

#if 0
	if (callInContentList.size() <= 2) {
		qWarning() << DEBUG_PREFIX << "argument inadequate";
		return;
	}
	if (m_reportNotify)
		m_reportNotify->setVisible(false);

	m_ui->pushButtonStatus->setEnabled(false);
	//Reject the second call when calling
	if (m_sipService && m_callId!=-1) {
		m_sipService->rejectCall(callInContentList.at(SipService::CallID).toInt());
		return;
	} 

/*	m_sipService->setCallVolume(m_callId, 128);*/

	for (int i = 0; i < callInContentList.size(); i++) {
		qDebug () << DEBUG_PREFIX << callInContentList.at(i);
	}

	m_cdrId = callInContentList.at(SipService::CallRecordId);

	QStringList photoId;
	if (!callInContentList.at(SipService::CallPhotoes).isEmpty()) {
		photoId = callInContentList.at(SipService::CallPhotoes).split(QLatin1String("|"));
		if (m_widgetPhotoTranslation)
			m_widgetPhotoTranslation->init(photoId);
		m_ringLater = true;
	}
	
	m_callId = callInContentList.at(SipService::CallID).toInt();
	m_callMode = callInContentList.at(SipService::CallMode);
	bool local = (bool)callInContentList.at(SipService::LocalPartner).toInt();
	m_callLocal = local;
	if (m_callMode == QLatin1String("two_way")) {
		for(int i = 0; i < m_skillWidgetList.size(); i++) {
			if (callInContentList.at(SipService::TargetLanguage) == m_skillWidgetList.at(i)->targetLanguage()) {
				m_locate = i;
				m_skillWidgetList.at(i)->setStyleCalling();
				break;
			}
		}
	} else {
		for(int i = 0; i < m_skillWidgetList.size(); i++) {
			if (callInContentList.at(SipService::TargetLanguage) == m_threeWaySkillWidgetList.at(i)->targetLanguage()) {
				m_locate = i;
				m_threeWaySkillWidgetList.at(i)->setStyleCalling();
				break;
			}
		}
	}
	
	m_ui->viewIncomingCall->setLanguage(callInContentList.at(SipService::SourceLanguage), callInContentList.at(SipService::TargetLanguage));
	m_ui->viewIncomingCall->setCallMode(callInContentList.at(SipService::CallMode));

	if (m_widgetPhotoTranslation) {
		if (m_widgetPhotoTranslation->photoIdList().size() > 0) {
			m_ui->viewIncomingCall->setWidgetPhotoTranslation(m_widgetPhotoTranslation);
		}
	}

	if(m_pageCalling) {
		m_pageCalling->setCallId(callInContentList.at(SipService::CallID).toInt());
		m_pageCalling->setLanguage(callInContentList.at(SipService::SourceLanguage), callInContentList.at(SipService::TargetLanguage));
		m_pageCalling->setCallMode(callInContentList.at(SipService::CallMode));
		m_pageCalling->setServiceMode(callInContentList.at(SipService::ServiceMode));
		m_pageCalling->setRemoteContact(callInContentList.at(SipService::RemoteContect));
	}

	if(m_callSplash) {
		m_callSplash->setLanguage(callInContentList.at(SipService::SourceLanguage), callInContentList.at(SipService::TargetLanguage));
		m_callSplash->setServiceMode(callInContentList.at(SipService::ServiceMode));
		m_callSplash->setRemoteContact(callInContentList.at(SipService::RemoteContect));
	}

	m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewIncomingCall);

	m_ui->viewIncomingCall->setRemoteContact(callInContentList.at(SipService::RemoteContect));
	m_ui->viewIncomingCall->callIncoming((bool)callInContentList.at(SipService::LocalPartner).toInt(), callInContentList.at(SipService::LocalCountry), callInContentList.at(SipService::LocalLanguage),
		callInContentList.at(SipService::AcconutID).toInt(), callInContentList.at(SipService::CallID).toInt());
	
	if (m_cameraService && m_userService) {
		m_cameraService->capture(m_userService->createScreenshotName());
	}

	/* remote eg: "addison" <sip:5450@58.64.141.118> */	
	QString remote = callInContentList.at(SipService::RemoteContect);
	QRegExp exp(QLatin1String("sip:(\\d+)[^@]"));
	exp.indexIn(remote, 0);
	QString extensionNumber = exp.cap(0).remove(QLatin1String("sip:"));
	QString callerName = remote.section(QRegExp(QLatin1String("[\"]")), 1, 1);

	if (m_commandService) {
		Command::GetCallerUserParam paramUser;
		paramUser.setCallerId(extensionNumber);
		m_commandService->getcalleruser(paramUser);

		if (!callInContentList.at(SipService::CalleeNumber).isEmpty()) {
			paramUser.setCallerId(callInContentList.at(SipService::CalleeNumber));
			m_commandService->getcalleruser(paramUser);
		}

		if (!photoId.isEmpty()) {
			Command::GetUploadPhotoParam paramPhoto;
			paramPhoto.setCallerId(extensionNumber);
			paramPhoto.setPhotoId(callInContentList.at(SipService::CallPhotoes));
			m_commandService->getuploadphoto(paramPhoto);
		}
	}

	Q_EMIT callIn();
#endif

// 	typedef PageFrameWindowOptions Opt;
// 	Preference* pref = preference();
// 	if (pref) {
// 
// 		bool closeCallin = pref->getBool(Opt::close_callin_, false);
// 		int callinPrompt = pref->getInt(Opt::callin_prompt_, 0);
// 
// 		if (!closeCallin && callinPrompt!=2) {
// 			if (m_callSplash) {
// 				m_callSplash->setServiceMode(callInContentList.at(SipService::ServiceMode));
// 				m_callSplash->setRemoteContact(callInContentList.at(SipService::RemoteContect));
// 				m_callSplash->slowShow(callinPrompt==0);
// 			}
// 		}
// 	}

	//Q_EMIT callIn();
}

void PageFrameWindow::slotSIPCallState(int callID, int state)
{
	qDebug() << DEBUG_PREFIX << callID << state;
	switch(state) {
		case 1: /*PJSIP_INV_STATE_CALLING*/
			break;
		case 4: /* PJSIP_INV_STATE_CONNECTING */
			break;
		case 5: /* PJSIP_INV_STATE_CONFIRMED */

			m_ui->viewIncomingCall->callAnswered();

			if (m_callSplash) {
				m_callSplash->hide();
			}
			setStateConfirmed();

			Q_EMIT callConfirmed(m_callLocal);
			break;
		case 6: /* PJSIP_INV_STATE_DISCONNECTED */
			if (m_callSplash) {
				m_callSplash->hide();
			}
			m_callId = -1;

			restoreState();

			m_ui->viewIncomingCall->setCallId(m_callId);

			if (m_pageCalling) {
				m_pageCalling->cleanReceiveText();
			}

			if (m_widgetPhotoTranslation) {
				m_widgetPhotoTranslation->uninit();
			}

			if (m_cameraService && m_userService) {
				m_cameraService->capture(m_userService->createScreenshotName());
			}

			m_ui->pushButtonStatus->setEnabled(true);

			QTimer::singleShot(1000, this, SLOT(slotDelayCDR()));
			break;
	}
}

void PageFrameWindow::slotSplashDecline()
{
	if (m_sipService && m_callId!=-1) {
		m_sipService->rejectCall(m_callId);
	}
}

void PageFrameWindow::slotSplashAnswer()
{
	if (m_sipService && m_callId!=-1) {
		m_sipService->answerCall(m_callId);
	}
}

void PageFrameWindow::slotDelayCDR()
{
	if (m_commandService) {
		Command::TodayStatParam paramStat;
		m_commandService->todayStat(paramStat);
		Command::GetNoReportParam paramReport;
		m_commandService->getnoreportinfo(paramReport);
	}

	m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewIdle);
	m_ui->viewIncomingCall->callDisconnected();
	Q_EMIT callClosed();
}

void PageFrameWindow::slotCDRFinished(const Command::GetCDRListParam& param, const Command::GetCDRListResult& result)
{
	if (result.common().error().isEmpty()) {

		User::CallRecord cdr;
		cdr.setCdrId(result.Id());
		cdr.setCallerVeSysId(result.VESysId());
		cdr.setMeetCdrId(result.meetCDRId());
		cdr.setSmallPhoto(result.smallPhoto());
		cdr.setDisplayName(result.displayName());
		cdr.setStartCallTime(result.startCallTime());
		cdr.setCallTime(result.callTime());
		cdr.setIsAnswered(result.isAnswered());
		cdr.setCallMode(result.callMode());
		cdr.setIsSuccess(result.isSuccess());

		if (isTodayRecord(cdr.startCallTime())) {
			if (cdr.isAnswered() == QLatin1String("1")) {
				m_answeredCalls += 1;
			} else {
				m_missedCalls += 1;
			}

			if (cdr.isSuccess() == QLatin1String("1"))
				m_succeedCalls += 1;
		}
	
		updateStatistics();
		
		if (m_userService)
			m_userService->addCallRecord(cdr);
#if 0
		CallRecordItem* newItem = new CallRecordItem(m_ui->treeWidgetRecent, cdr);
		if (newItem) {
			if (m_userService)
				m_userService->addCallRecord(cdr);
			newItem->setPhotoService(m_photoService);
			m_cdrItems.insert(cdr.meetCdrId(), newItem);
			if (m_photoService) {
				if (!m_photoService->isReady(cdr.smallPhoto())) {
					if( !m_photoService->isDownloading(cdr.smallPhoto()))
						m_photoService->downloadPhoto(cdr.smallPhoto());
				}
			}
		}
#endif

		if (m_userService) {
			m_userService->updateScreenshotMeetCDRId(result.meetCDRId());
			m_userService->uploadScreenshot();
		}

	} else {
		qWarning() << DEBUG_PREFIX << "Get CDR record failed!";
	}
}

void PageFrameWindow::slotCDRInfoFinished(const Command::GetCDRInfoParam& param, const Command::GetCDRInfoResult& result)
{
#if 0
	if (result.common().error().isEmpty()) {
		
		if (m_userService) {
			User::CallRecord cdr = m_userService->findCallRecord(param.meetCDRId());
			if (cdr.isValid()) {

				qDebug() << result.callerVESysId() << result.callerDisplayName() << result.calleeVESysId() << result.partnerLevel()
					<< result.startCallTime();

				cdr.setCdrType(result.CDRType());
				cdr.setCallerVeSysId(result.callerVESysId());
				cdr.setCallerId(result.callerId());
				cdr.setCallerVeLoginId(result.callerVELoginId());
				cdr.setCallerDisplayName(result.callerDisplayName());
				cdr.setCallerMobile(result.callerMobile());
				cdr.setCallerSmallPhoto(result.callerSmallPhoto());
				cdr.setCallerLargePhoto(result.callerLargePhoto());
				cdr.setIsPstnCall(result.isPSTNCall());
				cdr.setCallCountry(result.callCountry());
				cdr.setCalleeVeSysId(result.calleeVESysId());
				cdr.setCalleeId(result.calleeId());
				cdr.setCalleeVeLoginId(result.calleeVELoginId());
				cdr.setCalleeDisplayName(result.calleeDisplayName());
				cdr.setCalleeMobile(result.calleeMobile());
				cdr.setCalleeSmallPhoto(result.calleeSmallPhoto());
				cdr.setCalleeLargePhoto(result.calleeLargePhoto());
				cdr.setPartnerSysId(result.partnerVESysId());
				cdr.setPartnerId(result.partnerId());
				cdr.setPartnerLoginId(result.partnerLoginId());
				cdr.setPartnerDisplayName(result.partnerDisplayName());
				cdr.setPartnerSmallPhoto(result.partnerSmallPhoto());
				cdr.setPartnerLargePhoto(result.partnerLargePhoto());
				cdr.setIsFree(result.isFree());
				cdr.setPartnerLevel(result.partnerLevel());
				cdr.setLanguage1(result.language1());
				cdr.setLanguage2(result.language2());
				cdr.setCallTime(result.callTime());
				cdr.setStartCallTime(result.startCallTime());
				m_userService->updateCallRecord(cdr);

				CallRecordItem* item = m_cdrItems.value(param.meetCDRId());
				if (item) {
					item->setCDR(cdr);
				}

				m_ui->viewRecentDetail->setCallerInfo(cdr.callerDisplayName(), cdr.callerMobile(), cdr.callerLargePhoto());
				m_ui->viewRecentDetail->setCalleeInfo(cdr.calleeDisplayName(), cdr.calleeMobile(), cdr.calleeLargePhoto());
				m_ui->viewRecentDetail->setPartnerInfo(cdr.partnerLargePhoto());
				m_ui->viewRecentDetail->setCallDetail(cdr.startCallTime(), cdr.meetCdrId(), cdr.callTime(), cdr.callMode());
				m_ui->viewRecentDetail->setLanguage(cdr.language1(), cdr.language2());
				if (m_photoService) {
					m_ui->viewRecentDetail->setCallerPhoto(cdr.callerLargePhoto(), m_photoService->photoFile(cdr.callerLargePhoto()));
					m_ui->viewRecentDetail->setPartnerPhoto(cdr.partnerLargePhoto(), m_photoService->photoFile(cdr.partnerLargePhoto()));
					m_ui->viewRecentDetail->setCalleePhoto(cdr.calleeLargePhoto(), m_photoService->photoFile(cdr.calleeLargePhoto()));
				}

				if (m_photoService) {
					if (!m_photoService->isReady(cdr.callerSmallPhoto())) {
						if (!m_photoService->isDownloading(cdr.callerSmallPhoto()))
							m_photoService->downloadPhoto(cdr.callerSmallPhoto());
					}
					if (!m_photoService->isReady(cdr.callerLargePhoto())) {
						if (!m_photoService->isDownloading(cdr.callerLargePhoto()))
							m_photoService->downloadPhoto(cdr.callerLargePhoto());
					}
					if (!m_photoService->isReady(cdr.partnerSmallPhoto())) {
						if (!m_photoService->isDownloading(cdr.partnerSmallPhoto()))
							m_photoService->downloadPhoto(cdr.partnerSmallPhoto());
					}
					if (!m_photoService->isReady(cdr.partnerLargePhoto())) {
						if (!m_photoService->isDownloading(cdr.partnerLargePhoto()))
							m_photoService->downloadPhoto(cdr.partnerLargePhoto());
					}
					if (!m_photoService->isReady(cdr.calleeSmallPhoto())) {
						if (!m_photoService->isDownloading(cdr.calleeSmallPhoto()))
							m_photoService->downloadPhoto(cdr.calleeSmallPhoto());
					}
					if (!m_photoService->isReady(cdr.calleeLargePhoto())) {
						if (!m_photoService->isDownloading(cdr.calleeLargePhoto()))
							m_photoService->downloadPhoto(cdr.calleeLargePhoto());
					}
					
				}
			}
		}
	} else {
		qWarning() << DEBUG_PREFIX << "Get CDR Information failed!";
	}
#endif
}

void PageFrameWindow::slotUserInfoFinished(const Command::GetUserInfoParam& param, const Command::GetUserInfoResult& result)
{
	if (result.common().error().isEmpty()) {
		
		m_ui->viewProfile->setDisplayName(result.displayName(), result.loginId());
		m_ui->viewProfile->setIntroduction(result.introduction());
		m_ui->viewProfile->setPhotoUrl(result.largePhoto());
		if (m_photoService) {
			if (m_photoService->isReady(result.largePhoto())) {
				m_ui->viewProfile->setPhoto(result.largePhoto(), m_photoService->photoFile(result.largePhoto()));
			} else {
				if (!m_photoService->isDownloading(result.largePhoto()))
					m_photoService->downloadPhoto(result.largePhoto());
			}
		}

		QStringList skill = result.skill();
		if (skill.size()>0) {
			QString skillInfo = skill.at(0);
			QStringList skillDetail = skillInfo.split(QLatin1String("|"));
			if (skillDetail.size()>=4) {
				m_ui->viewProfile->setLanguage(skillDetail.at(0), skillDetail.at(1));
				m_ui->viewProfile->setServiceMode(skillDetail.at(2)==QLatin1String("1")?QLatin1String("a0"):QLatin1String("a1"));
			}
		}

	} else {
		qWarning() << DEBUG_PREFIX << "Get User Information failed!";
	}
}

void PageFrameWindow::slotCallerUserFinished(const Command::GetCallerUserParam& param, const Command::GetCallerUserResult& result)
{
	if (result.common().error().isEmpty()) {
		m_ui->viewIncomingCall->setCallerUser(param.callerId(), result.largePhoto(), result.displayName(), result.mobile());

		if (m_pageCalling)
			m_pageCalling->setCallerUser(param.callerId(), result.largePhoto(), result.displayName(), result.mobile());

		if (m_callSplash)
			m_callSplash->setCallerUser(param.callerId(), result.largePhoto(), result.displayName(), result.mobile());

		if (m_photoService) {
			if (m_photoService->isReady(result.largePhoto())) {
				m_ui->viewIncomingCall->setPhotoPicture(result.largePhoto(), m_photoService->photoFile(result.largePhoto()));
				
				if (m_pageCalling)
					m_pageCalling->setPhotoPicture(result.largePhoto(), m_photoService->photoFile(result.largePhoto()));

				if (m_callSplash)
					m_callSplash->setPhotoPicture(result.largePhoto(), m_photoService->photoFile(result.largePhoto()));
			} else {
				if (!m_photoService->isDownloading(result.largePhoto())) {
//					m_ringLater = true;
					m_photoService->downloadPhoto(result.largePhoto());
				}
			}
		}
// 		if (!m_ringLater) {
// 			m_ui->viewIncomingCall->startRinging();
// 		}
	} else {
		qWarning() << DEBUG_PREFIX << "Get Caller User Information Failed!";
	}
}

void PageFrameWindow::slotChangeStatusFinished(const Command::ChangeStatusParam& param, const Command::ChangeStatusResult& result)
{
	if (result.common().error().isEmpty()) {
		
		if (param.onlineVisibleStatus() == QLatin1String("1")) { 
			//online
			m_recentStatus = RecentStatusOnline;
			m_ui->actionOffline->setChecked(false);
			m_ui->actionOnline->setChecked(true);
			m_ui->pushButtonStatus->setText(m_ui->actionOnline->text());
			m_ui->pushButtonStatusIcon->setStyleSheet(QLatin1String("background-image: url(:/images/ui/status_ready.png);"));
		} else { 
			//offline
			m_recentStatus = RecentStatusOffline;
			m_ui->actionOffline->setChecked(true);
			m_ui->actionOnline->setChecked(false);
			m_ui->pushButtonStatus->setText(m_ui->actionOffline->text());
			m_ui->pushButtonStatusIcon->setStyleSheet(QLatin1String("background-image: url(:/images/ui/status_offline.png);"));
		}

		for (int i = 0; i < m_skillWidgetList.size(); i++) {
			m_skillWidgetList.at(i)->changeStatus(param.onlineVisibleStatus() == QLatin1String("1"));
		}

		for (int i = 0; i < m_threeWaySkillWidgetList.size(); i++) {
			m_threeWaySkillWidgetList.at(i)->changeStatus(param.onlineVisibleStatus() == QLatin1String("1"));
		}

		updateRecentStatusAction();
	} else {
		qWarning() << DEBUG_PREFIX << "Change Status Failed!";
	}
}

void PageFrameWindow::slotMemberInfoFinished(const Command::GetMemberInfoParam& param, const Command::GetMemberInfoResult& result)
{
	if(result.common().error().isEmpty()) {
		if (m_userService) {
			qDebug() << result.displayName() << result.nativeLanguage() << result.country() << result.major();
			m_ui->viewContact->setVESysID(result.VESysId());
			m_ui->viewContact->setMemberInfo(result.displayName(),result.nativeLanguage(),result.country(),result.major());
		}

		if (m_photoService) {
			if (m_photoService->isReady(result.smallPhoto())) {
				m_ui->viewContact->setPhotoFile(m_photoService->photoFile(result.smallPhoto()));
			} else {
				if (!m_photoService->isDownloading(result.smallPhoto()))
					m_photoService->downloadPhoto(result.smallPhoto());
			}
		}
	}
	else {
		qWarning() << DEBUG_PREFIX << "Get Member Information Failed!";
	}
}

void PageFrameWindow::slotGetBlacklistInfoFinished(const Command::GetBlacklistParam& param, const Command::GetBlacklistResult& result)
{
	if (result.common().error().isEmpty()) {

		if (m_userService) {
			qDebug() << result.blacklistId() << result.displayName() << result.VESysId() << result.smallPhoto() << result.id() << result.parterSysId();
			User::Blacklist blist;
			blist.setUniqueid(result.id());
			blist.setBlacklistId(result.blacklistId());
			blist.setDisplayName(result.displayName());
			blist.setSmallPhoto(result.smallPhoto());
			blist.setVeSysId(result.VESysId());
			blist.setPartnerSysId(result.parterSysId());
			m_userService->addBlacklistRecord(blist);

			if (m_photoService) {
				if (!m_photoService->isReady(result.smallPhoto())) {
					if (!m_photoService->isDownloading(result.smallPhoto())) {
						m_photoService->downloadPhoto(result.smallPhoto());
					}
				}
			}
		}
	}
}

void PageFrameWindow::slotGetUploadPhotoFinished(const Command::GetUploadPhotoParam& param, const Command::GetUploadPhotoResult& result)
{
	qDebug() << result.photoId() << result.expire() << result.smallPhoto() << result.largePhoto();
	if (result.common().error().isEmpty()) {
		if (m_widgetPhotoTranslation) {
			m_widgetPhotoTranslation->setSmallPhotoUrl(result.photoId(), result.smallPhoto());
			m_widgetPhotoTranslation->downloadPhoto(result.largePhoto(), result.photoId());
		}
		if (m_photoService) {
			if (m_photoService->isReady(result.smallPhoto())) {
				if (m_widgetPhotoTranslation) {
					m_widgetPhotoTranslation->setPhotoPicture(result.smallPhoto(), m_photoService->photoFile(result.smallPhoto()));
				}
			} else {
				if (!m_photoService->isDownloading(result.smallPhoto()))
					m_photoService->downloadPhoto(result.smallPhoto());
			}
		}
	} else {
		qWarning() << DEBUG_PREFIX << "Get Upload Photo Information Failed!";
	}
}

void PageFrameWindow::slotTodayStatFinished(const Command::TodayStatParam& param, const Command::TodayStatResult& result)
{
	qDebug() << DEBUG_PREFIX << result.answer() << result.success() << result.noanswer();
	if (result.common().error().isEmpty()) {
		if (result.success().toInt() == m_succeedCalls + 1 && !m_cdrId.isEmpty()) {
			Q_EMIT callSucceed(m_cdrId);
			m_cdrId = QString();
		}
		m_answeredCalls = result.answer().toInt();
		m_succeedCalls = result.success().toInt();
		m_missedCalls = result.noanswer().toInt();
		updateStatistics();
	}
}

void PageFrameWindow::slotGetNoReportInfoFinished(const Command::GetNoReportParam& param, const Command::GetNoReportResult& result)
{
	if (result.common().error().isEmpty()) {
		qDebug() << DEBUG_PREFIX << result.num();
		if (result.num().toInt() >= 1 && m_ui->viewIdle->reportNotify()) {
			m_ui->viewIdle->reportNotify()->setNoReportNum(result.num());
			int locationH = 0;
			m_ui->viewIdle->showNotify(locationH);
		}
	}
}

void PageFrameWindow::slotPhotoDownloaded(const QString& url, const QString& fileName)
{
	qDebug() << DEBUG_PREFIX << "Download photo finished !" << url << fileName;

	if (m_userService) {
		if (m_userService->photo() == url) {
			QPixmap pic(fileName);
			m_ui->labelUserPhoto->setPixmap(pic.scaled(m_ui->labelUserPhoto->size()));
			m_ui->viewIdle->setPhoto(fileName);

			typedef PageFrameWindowOptions Opt;
			Preference* pref = preference();
			if (pref) {
				pref->putString(Opt::recent_photo_, fileName);
			}

		} else {
			m_ui->viewIncomingCall->setPhotoPicture(url, fileName);

			if (m_widgetPhotoTranslation)
				m_widgetPhotoTranslation->setPhotoPicture(url, fileName);
			if (m_pageCalling)
				m_pageCalling->setPhotoPicture(url, fileName);
			if (m_callSplash)
				m_callSplash->setPhotoPicture(url, fileName);
// 			if (m_ui->viewIncomingCall->callId() != -1 && m_ringLater) 
// 				m_ui->viewIncomingCall->startRinging();
		}
	}

// 	if (m_ui->stackedWidgetView->currentWidget() == m_ui->viewRecentDetail) {
// 		m_ui->viewRecentDetail->setCallerPhoto(url, fileName);
// 		m_ui->viewRecentDetail->setPartnerPhoto(url, fileName);
// 		m_ui->viewRecentDetail->setCalleePhoto(url, fileName);
// 	}

// 	if(m_ui->stackedWidgetView->currentWidget() == m_ui->viewContact) {
// 		m_ui->viewContact->setPhotoFile(fileName);
// 	}

	m_ui->viewProfile->setPhoto(url, fileName);
}

void PageFrameWindow::slotPhotoDownloadFailed(const QString& url)
{

}

void PageFrameWindow::slotCaptureFinished(const QString& fileName)
{
	if (m_userService) {
		User::Screenshot screenshot;
		screenshot.setPhoto(fileName);
		m_userService->addScreenshot(screenshot);
	}
}

void PageFrameWindow::slotUserPhotoClicked()
{
	if (m_ui->stackedWidgetView->currentWidget() == m_ui->viewIncomingCall)
		return;

	if (m_commandService) {
		Command::GetUserInfoParam param;
		m_commandService->getuserinfo(param);

		m_ui->stackedWidgetView->setCurrentWidget(m_ui->viewProfile);
	}
}

void PageFrameWindow::slotSmtpSendSucceed()
{
	qWarning() << DEBUG_PREFIX << "Send email succeed";
}

void PageFrameWindow::slotImessageFinished(const ImCommand::GetUploadPhotoMsgResult& result)
{
	qWarning() << DEBUG_PREFIX << result.photoId() << result.smallPhoto() << result.largePhoto();

	if (m_callId != -1) {
		if (m_widgetPhotoTranslation) {
			if (m_widgetPhotoTranslation->photoIdList().contains(result.photoId()))
				return;
			m_widgetPhotoTranslation->init(result.photoId());
			m_widgetPhotoTranslation->setSmallPhotoUrl(result.photoId(), result.smallPhoto());
			m_widgetPhotoTranslation->downloadPhoto(result.largePhoto(), result.photoId());
			if (m_pageCalling)
				m_pageCalling->setWidgetPhotoTranslation(m_widgetPhotoTranslation);
		}
		if (m_photoService) {
			if (m_photoService->isReady(result.smallPhoto())) {
				if (m_widgetPhotoTranslation) {
					//m_widgetPhotoTranslation->setPhotoPicture(result.smallPhoto(), m_photoService->photoFile(result.smallPhoto()));
					m_widgetPhotoTranslation->setPhotoPicture(result.photoId(), result.smallPhoto(), m_photoService->photoFile(result.smallPhoto()));
				}
			} else {
				if (!m_photoService->isDownloading(result.smallPhoto()))
					m_photoService->downloadPhoto(result.smallPhoto());
			}
		}
	}	
}

void PageFrameWindow::slotImessageError()
{

}

void PageFrameWindow::slotCalleeNotify(const CalleeNotify& notify)
{
	qWarning() << notify.userId() << notify.sourceLang() << notify.targetLang() << notify.callMode() << notify.cdrId() << notify.calleeId();
	
	if (m_sipService && m_userService)
		m_sipService->registerServer(m_userService->callerId(), m_userService->secret(), notify.roomAddress());

	m_callMode = notify.callMode();
	m_callSrcLang = notify.sourceLang();
	m_callTarLang = notify.targetLang();
	m_caller = QString::number(notify.userId());
	m_cdrId = notify.cdrId();
	m_callee = QString::number(notify.calleeId());

	m_ui->viewIncomingCall->setLanguage(m_callSrcLang, m_callTarLang);
	m_ui->viewIncomingCall->setCallMode(m_callMode);

	if(m_pageCalling) {
		m_pageCalling->setLanguage(m_callSrcLang, m_callTarLang);
		m_pageCalling->setCallMode(m_callMode);
		m_pageCalling->setRemoteContact(m_caller);
	}

	if(m_callSplash) {
		m_callSplash->setLanguage(m_callSrcLang, m_callTarLang);
		m_callSplash->setRemoteContact(m_caller);
	}

	m_ui->viewIncomingCall->setRemoteContact(m_caller);
	//m_ui->viewIncomingCall->callIncoming();

	if (m_cameraService && m_userService) {
		m_cameraService->capture(m_userService->createScreenshotName());
	}

	if (m_commandService) {
		Command::GetCallerUserParam paramUser;
		paramUser.setCallerId(m_caller);
		m_commandService->getcalleruser(paramUser);

		if (notify.calleeId() > 0) {
			paramUser.setCallerId(m_callee);
			m_commandService->getcalleruser(paramUser);
		}
	}
}

void PageFrameWindow::slotUnregNotify()
{
	if (m_sipService) {
		m_sipService->unregisterServer();
	}
}

void PageFrameWindow::slotPhotoNotify(const PhotoNotify& notify)
{
	qWarning() << DEBUG_PREFIX << notify.photoId() << notify.smallPhoto() << notify.largePhoto();

	if (m_callId != -1) {
		if (m_widgetPhotoTranslation) {
			if (m_widgetPhotoTranslation->photoIdList().contains(notify.photoId()))
				return;
			m_widgetPhotoTranslation->init(notify.photoId());
			m_widgetPhotoTranslation->setSmallPhotoUrl(notify.photoId(), notify.smallPhoto());
			m_widgetPhotoTranslation->downloadPhoto(notify.largePhoto(), notify.photoId());
			if (m_pageCalling)
				m_pageCalling->setWidgetPhotoTranslation(m_widgetPhotoTranslation);
		}
		if (m_photoService) {
			if (m_photoService->isReady(notify.smallPhoto())) {
				if (m_widgetPhotoTranslation) {
					m_widgetPhotoTranslation->setPhotoPicture(notify.photoId(), notify.smallPhoto(), m_photoService->photoFile(notify.smallPhoto()));
				}
			} else {
				if (!m_photoService->isDownloading(notify.smallPhoto()))
					m_photoService->downloadPhoto(notify.smallPhoto());
			}
		}
	}	
}

void PageFrameWindow::removeSkillWidget()
{
	if (!m_ui->widgetTranslation->children().isEmpty()) {
		QList<SkillChannel *> widgetTranslations = m_ui->widgetTranslation->findChildren<SkillChannel *>();
		foreach(SkillChannel * widget, widgetTranslations) {
			delete widget;
		}
	}

	if (!m_ui->widgetLocal->children().isEmpty()) {
		QList<SkillChannel *> widgetLocals = m_ui->widgetLocal->findChildren<SkillChannel *>();
		foreach(SkillChannel * widget, widgetLocals) {
			delete widget;
		}
	}
}

void PageFrameWindow::retranslateTr()
{
	m_ui->labelDisplayName->setText(m_userService->displayName());
	m_ui->labelDisplayID->setText(m_userService->loginId());
	m_ui->labelAnsweredValue->setText(QString::number(m_answeredCalls));
	m_ui->labelSucceedValue->setText(QString::number(m_succeedCalls));
	m_ui->labelMissedValue->setText(QString::number(m_missedCalls));

	if(m_recentStatus == RecentStatusOnline) {
		m_ui->pushButtonStatus->setText(m_ui->actionOnline->text());
		
	} else if(m_recentStatus == RecentStatusOffline) {
		m_ui->pushButtonStatus->setText(m_ui->actionOffline->text());
		
	} 

	QFont font = m_ui->pushButtonStatus->font();
	QFontMetrics fm(font);
	int width = fm.width(m_ui->pushButtonStatus->text());

// 	QString styleSheet = QString(QLatin1String("#widgetLeftPanel #widgetProfile #pushButtonStatus::menu-indicator {"
// 																			"left: %1px;"
// 																			"}"
// 
// 																			"#widgetLeftPanel #widgetProfile #pushButtonStatus::menu-indicator:hover {"
// 																			"left: %2px;"
// 																			"}"
// 
// 																			"#widgetLeftPanel #widgetProfile #pushButtonStatus::menu-indicator:pressed {"
// 																			"left: %3px;"
// 																			"}")).arg(width - 135).arg(width - 135).arg(width - 135);
// 	m_ui->pushButtonStatus->setStyleSheet(styleSheet);
}

void PageFrameWindow::doRecentFilter(const QString& text)
{
#if 0
	QList<CallRecordItem*> cdrItemList = m_cdrItems.values();
	m_ui->treeWidgetRecent->setUpdatesEnabled(false);
	
	foreach (CallRecordItem* cdrItem, cdrItemList) {
		Q_ASSERT(cdrItem);
		bool hidden = false;
		QString label = cdrItem->data(ItemBase::LabelColumn, Qt::DisplayRole).toString();
		label = label.split(QLatin1String("\n")).at(0);
		
		hidden = !label.contains(text, Qt::CaseInsensitive);
		if (m_recentType==RecentTypeMissed && cdrItem->isAnswered())
			hidden = true;
		if (m_recentType==RecentTypeAnswered && !cdrItem->isAnswered())
			hidden = true;
		cdrItem->setHidden(hidden);
	}
	
	m_ui->treeWidgetRecent->setUpdatesEnabled(true);
#endif
}

void PageFrameWindow::updateRecentTypeAction()
{
#if 0
	m_ui->actionAll->setChecked(m_recentType==RecentTypeAll);
	m_ui->actionMissed->setChecked(m_recentType==RecentTypeMissed);
	m_ui->actionAnswered->setChecked(m_recentType==RecentTypeAnswered);

	if (m_recentType==RecentTypeAll) {
		m_ui->pushButtonFilter->setText(m_ui->actionAll->text());
	} else if (m_recentType==RecentTypeMissed) {
		m_ui->pushButtonFilter->setText(m_ui->actionMissed->text());
	} else {
		m_ui->pushButtonFilter->setText(m_ui->actionAnswered->text());
	}

	m_ui->widgetRecentFilter->update();
#endif
}

void PageFrameWindow::updateStatistics()
{
	m_ui->labelAnsweredValue->setText(tr("%1").arg(m_answeredCalls));
	m_ui->labelMissedValue->setText(tr("%1").arg(m_missedCalls));
	m_ui->labelSucceedValue->setText(tr("%1").arg(m_succeedCalls));
}

void PageFrameWindow::updateRecentStatusAction()
{
	m_ui->actionOnline->setChecked(m_recentStatus == RecentStatusOnline);
	m_ui->actionOffline->setChecked(m_recentStatus == RecentStatusOffline);

	if(m_recentStatus == RecentStatusOnline) {
		m_ui->pushButtonStatus->setText(m_ui->actionOnline->text());
	} else if(m_recentStatus == RecentStatusOffline) {
		m_ui->pushButtonStatus->setText(m_ui->actionOffline->text());
	}
}

void PageFrameWindow::restoreState()
{
	//Unknown error
	if (m_locate == -1) {
		return;
	}

	if (m_callMode == QLatin1String("a1")) {
		m_skillWidgetList.at(m_locate)->restroeState();
	} else {
		m_threeWaySkillWidgetList.at(m_locate)->restroeState();
	}

// 	if (m_skillWidgetList.size() <= 0)	{
// 		return;
// 	}
	
	m_locate = -1;
}

void PageFrameWindow::setStateCalling()
{
	//Unknown error
	if (m_locate == -1) {
		return;
	}

	if (m_callMode == QLatin1String("a1")) {
		m_skillWidgetList.at(m_locate)->setStyleCalling();
	} else {
		m_threeWaySkillWidgetList.at(m_locate)->setStyleCalling();
	}

// 	if (m_skillWidgetList.size() <= 0)	{
// 		return;
// 	}
// 	m_skillWidgetList.at(m_locate)->setStyleCalling();
}

void PageFrameWindow::setStateConfirmed()
{
	//Unknown error
	if (m_locate == -1) {
		return;
	}

	if (m_callMode == QLatin1String("a1")) {
		m_skillWidgetList.at(m_locate)->setStyleConfirming();
	} else {
		m_threeWaySkillWidgetList.at(m_locate)->setStyleConfirming();
	}

// 	if (m_skillWidgetList.size() <= 0)	{
// 		return;
// 	}
// 	m_skillWidgetList.at(m_locate)->setStyleConfirming();
}

bool PageFrameWindow::isTodayRecord(const QString& startCallTime)
{
	if (startCallTime.isEmpty())
		return false;

	QString time_t = startCallTime;
	QDateTime today = QDateTime::currentDateTime();
	today.setTime(QTime(0,0,0));
	QDateTime todayBegin = today;
	today.setTime(QTime(23,59,59));
	QDateTime todayEnd = today;

	QDateTime record = QDateTime::fromTime_t(time_t.toUInt());

	if (record.date().day() == today.date().day()) {
		if (record.time().hour() > todayBegin.time().hour() && record.time().hour() <= todayEnd.time().hour()) {
			return true;
		}
	}

	return false;
}

void PageFrameWindow::sendEmil(const QString& dateTime, const QString& code, const QString& reason)
{
	qWarning() << DEBUG_PREFIX << "register sip server failed, send email";
	QString senderName = QString();
	QString serviceHost = QString();

	if (m_userService) {
		serviceHost = m_userService->sipServiceHost();
		senderName = m_userService->displayName();
		m_sender.setName(senderName);
	}

	m_message.setSender(&m_sender);

	/*EmailAddress to(QLatin1String("sysalarm@chnlove.com"), QLatin1String("Recipient's Name"));*/
	m_message.addRecipient(&m_recipient);

	m_message.setSubject(QLatin1String("CrashReport for YesPoPartner"));

	m_text.setText(QString(QLatin1String("This is an email with crash report files. %1 %2 %3 client IP:%4 server IP: %5")).arg(dateTime).arg(code).arg(reason).arg(m_networkAddress).arg(serviceHost));
	m_message.addPart(&m_text);

	m_smtp.setMail(m_message);

	if (!m_smtp.connectToHost()) {
		qWarning() << DEBUG_PREFIX << "SMTP Failed to connect to host!";		
		return;
	}

// 	if (!m_smtp.login()) {
// 		qWarning() << DEBUG_PREFIX << "Failed to login!";		
// 		return;
// 	}
// 
// 	if (!m_smtp.sendMail(message)) {
// 		qWarning() << DEBUG_PREFIX << "Failed to send mail!";		
// 		return;
// 	}
// 
// 	m_smtp.quit();
}

} // namespace ns
