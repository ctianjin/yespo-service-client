#ifndef BROWSER_H
#define BROWSER_H

#include <QScopedPointer>
#include <QString>
#include <QIcon>
#include <QUrl>
#include "framelessdialog.h"

QT_BEGIN_NAMESPACE
class QUrl;
class QNetworkRequest;
class QNetworkCookie;
class QNetworkCookieJar;
QT_END_NAMESPACE
namespace ns {

class Ui_Browser;

class Browser : public FramelessDialog
{
	Q_OBJECT

public:
	explicit Browser(QWidget* parent = 0);
	virtual ~Browser();

	QUrl url() const;
	void setUrl(const QUrl& url);
	void loadRequest(const QNetworkRequest& request);
	void reload();
	QByteArray requestCookies();
	void setRequestCookies(const QUrl url);
Q_SIGNALS:
	void requestNewBrowser(const QString& cdrId);
	void requestCloseBrowser();
	void requestRefreshBrowser();
protected:
	virtual void changeEvent(QEvent* event);
	virtual void closeEvent(QCloseEvent *event);
public slots:
	void slotLinkClicked(const QUrl& url);
	void submit();
	void later();
	void populateJavaScriptWindowObject();
	void slotLoadFinished(bool ok);

private:
	QScopedPointer<Ui_Browser> m_ui;
	QUrl m_url;
	QNetworkCookieJar *m_cookieJar;
	QByteArray m_cookie;
	QList<QNetworkCookie> m_cookieList;
	Q_DISABLE_COPY(Browser)
};

} // namespace ns

#endif // BROWSER_H
