#ifndef VIEWRECENTDETAIL_H
#define VIEWRECENTDETAIL_H

#include <QtCore/QScopedPointer>
#include <QWidget>

namespace ns {

class Ui_ViewRecentDetail;

class ViewRecentDetail : public QWidget
{
    Q_OBJECT

public:
    ViewRecentDetail(QWidget* parent = 0);
    ~ViewRecentDetail();

	void setCallerInfo(const QString& displayName, const QString& mobile, const QString& photoUrl);
	void setCallerPhoto(const QString& url, const QString& photo);
	void setPartnerInfo(const QString& photoUrl);
	void setPartnerPhoto(const QString& url, const QString& photo);
	void setCalleeInfo(const QString& displayName, const QString& mobile, const QString& photoUrl);
	void setCalleePhoto(const QString& url, const QString& photo);
	void setCallDetail(const QString& dateTime, const QString& Id, const QString& duration, const QString& mode);
	void setLanguage(const QString& srcLang, const QString& targetLang);

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:

private:
	void retranslateTr();

	QString m_mode;
	QString m_callerPhotoUrl;
	QString m_calleePhotoUrl;
	QString m_partnerPhotoUrl;

	QString m_callerDisplayName;
	QString m_callerMobile;
	QString m_calleeDisplayName;
	QString m_calleeMobile;

	QString m_datetime;
	QString m_id;
	QString m_duration;

	QScopedPointer<Ui_ViewRecentDetail> m_ui;
};

} // namespace ns

#endif // VIEWRECENTDETAIL_H
