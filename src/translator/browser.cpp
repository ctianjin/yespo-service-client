#include "browser.h"
#include "ui_browser.h"
#include <QDebug>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkRequest>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Browser:"
#endif // DEBUG_PREFIX

namespace ns {

Browser::Browser(QWidget* parent) :
	FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
	m_ui(new Ui_Browser),
	m_cookie(0)
{
	m_ui->setupUi(this);

	m_ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

	m_cookieJar = new QNetworkCookieJar();
	m_ui->webView->page()->networkAccessManager()->setCookieJar(m_cookieJar);

	connect(m_ui->webView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(slotLinkClicked(const QUrl&)));
	connect(m_ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(slotLoadFinished(bool)));
	connect(m_ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
		this, SLOT(populateJavaScriptWindowObject()));
}

Browser::~Browser()
{
}

QUrl Browser::url() const 
{
	return m_ui->webView->url();
}

void Browser::setUrl(const QUrl& url)
{
	m_url = url;
	m_ui->webView->setUrl(m_url);

	if (!isVisible())
		setVisible(true);
}

void Browser::loadRequest(const QNetworkRequest& request)
{
	qDebug() << request.url().toString();
	m_url = request.url();

	m_ui->webView->load(request);

	if (!isVisible())
		setVisible(true);
}

void Browser::reload()
{
	m_ui->webView->reload();
}

QByteArray Browser::requestCookies()
{
	return m_cookie;
}

void Browser::setRequestCookies(const QUrl url)
{
	m_cookieJar->setCookiesFromUrl(m_cookieList, url);
	QByteArray str;

	QNetworkCookie cookie;
	foreach (cookie , m_cookieList)
	{
		str += cookie.toRawForm() + "\n";
	}
	m_cookie = str;
	qDebug() << DEBUG_PREFIX << m_cookie;
}

void Browser::changeEvent(QEvent* event)
{
	QDialog::changeEvent(event);

	switch (event->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void Browser::closeEvent(QCloseEvent *event)
{
	/*Q_EMIT requestCloseBrowser();*/
}

void Browser::slotLinkClicked(const QUrl& url)
{
	if(url.isEmpty())
		return;
	QString urlString = url.toString();
	if (urlString.contains(QLatin1String("addReport?"))) {
		QString cdrId = url.queryItemValue(QLatin1String("meet_cdr_id"));
		Q_EMIT requestNewBrowser(cdrId);
	}
}

void Browser::submit()
{
	Q_EMIT requestRefreshBrowser();
	m_url = QUrl();
	close();
}

void Browser::later()
{
	m_url = QUrl();
	close();
}

void Browser::populateJavaScriptWindowObject()
{
	m_ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QLatin1String("browser"), this);
}

void Browser::slotLoadFinished(bool ok)
{
	if (ok && !m_url.isEmpty()) {
		if (m_cookieJar) {
			QByteArray str;
			QNetworkCookie cookie;
			m_cookieList = m_cookieJar->cookiesForUrl(m_url);
			int count = m_cookieList.count();
			foreach(cookie, m_cookieList) {
				str += cookie.toRawForm();
			}
			m_cookie = str;
		}
	}
}

} // namespace ns
