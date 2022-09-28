#include <QDebug>
#include <QDesktopServices>
#include <QRegExpValidator>
#include <QKeyEvent>
#include "phoneaddcontactview.h"
#include "phoneapplication.h"
#include "ui_phoneaddcontactview.h"
#include "userservice.h"
#include "sipservice.h"
#include "commandservice.h"
#include "mediaservice.h"
#include "messagepanel.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneAddContactView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

class SearchContactItem : public QTreeWidgetItem, public QObject
{
public:
	enum ItemType
	{
		SearchContactItemType = QTreeWidgetItem::UserType + 101,
	};

	enum Column
	{
		MinColumn = 0,
		PhotoColumn = MinColumn,
		StatusColumn,
		LabelColumn,
		OperatorColumn,

		MaxColumn
	};

	explicit SearchContactItem(QTreeWidget* view, ItemType type = SearchContactItemType);
	SearchContactItem(const SearchContactItem& rhs);
	virtual ~SearchContactItem();

	SearchContactItem& operator=(const SearchContactItem& rhs);

	virtual bool operator<(const QTreeWidgetItem& rhs) const;

	virtual QVariant data(int column, int role) const;

	Command::PhoneGetUserInfoResult userInfo() const;
	void setUserInfo(const Command::PhoneGetUserInfoResult& info);

	QString photoFile() const;
	void setPhotoFile(const QString& fileName);

protected:
	// Compare returns negative, 0, positive, rhs is always the same type of this
	// Returns 0 by default
	virtual int compare(const QTreeWidgetItem& rhs) const;

	QVariant photoData(int role) const;
	QVariant statusData(int role) const;
	QVariant labelData(int role) const;
	QVariant operatorData(int role) const;

private:
	Command::PhoneGetUserInfoResult m_userInfo;
	QString m_photoFile;

};

SearchContactItem::SearchContactItem(QTreeWidget* view, ItemType type) :
	QTreeWidgetItem(view, type)
{
}

SearchContactItem::SearchContactItem(const SearchContactItem& rhs) :
	QTreeWidgetItem(rhs)
{
}

SearchContactItem::~SearchContactItem()
{
}

SearchContactItem& SearchContactItem::operator=(const SearchContactItem& rhs)
{
	QTreeWidgetItem::operator=(rhs);

	return *this;
}

bool SearchContactItem::operator<(const QTreeWidgetItem& rhs) const
{
	// Compare the item type first
	if (type() != rhs.type())
		return type() < rhs.type();

	// Now for the same type
	int n = compare(rhs);
	if (n == 0) {
		return QTreeWidgetItem::operator<(rhs);
	} else {
		return n < 0;
	}
}

int SearchContactItem::compare(const QTreeWidgetItem& rhs) const
{
	return 0;
}

QVariant SearchContactItem::data(int column, int role) const
{
	QVariant ret;
	switch (column) {
		case PhotoColumn:
			ret = photoData(role);
			break;
		case StatusColumn:
			ret = statusData(role);
			break;
		case LabelColumn:
			ret = labelData(role);
			break;
		case OperatorColumn:
			ret = operatorData(role);
			break;
		default:
			break;
	}

	return ret;
}

Command::PhoneGetUserInfoResult SearchContactItem::userInfo() const
{
	return m_userInfo;
}

void SearchContactItem::setUserInfo(const Command::PhoneGetUserInfoResult& info)
{
	m_userInfo = info;
}

QString SearchContactItem::photoFile() const
{
	return m_photoFile;
}

void SearchContactItem::setPhotoFile(const QString& fileName)
{
	m_photoFile = fileName;
	emitDataChanged();
}

QVariant SearchContactItem::photoData(int role) const
{
	if (role == Qt::DecorationRole) {	
		QIcon statusIcon;
		if (!m_photoFile.isEmpty())
			statusIcon.addFile(m_photoFile);
		else
			statusIcon.addFile(QLatin1String(":/images/ui/default_photo.jpg"));
		return statusIcon;
	} 

	return QVariant();
}

QVariant SearchContactItem::statusData(int role) const
{
	if (role == Qt::DecorationRole) {
		QIcon statusIcon;
		if (m_userInfo.onlineStatus()==QLatin1String("1"))
			statusIcon.addFile(QLatin1String(":/images/ui/online_status_online.png"));
		else
			statusIcon.addFile(QLatin1String(":/images/ui/online_status_offline.png"));
		return statusIcon;
	}  else if (role == Qt::SizeHintRole) {
		return QSize(13,13);
	} 

	return QVariant();
}

QVariant SearchContactItem::labelData(int role) const
{
	if (role == Qt::DisplayRole) {
		if (m_userInfo.nickname().isEmpty()) {
			return m_userInfo.voiceraID();
		} else {
			return (m_userInfo.nickname() + QLatin1String("\n") + m_userInfo.voiceraID());
		}
		
	} 

	return QVariant();
}

QVariant SearchContactItem::operatorData(int role) const
{
	if (role == Qt::DisplayRole) {
		return tr("Add to contact");
	} else if (role == Qt::TextAlignmentRole) {
		return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
	} else if(role == Qt::TextColorRole) {
		return QColor(0,0,255);
	} 
	return QVariant();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PhoneAddContactViewOptions
{
	static const int max_name_length_;
	static const int max_number_length_;
};

const int PhoneAddContactViewOptions::max_name_length_ = 32;
const int PhoneAddContactViewOptions::max_number_length_ = 26;

PhoneAddContactView::PhoneAddContactView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneAddContactView),
    m_opt(new PhoneAddContactViewOptions),
	m_messagePanel(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->stackedWidgetResult->hide();
	m_ui->frameEmptyContactNamePanel->hide();
	m_ui->frameEmptyCountryCodePanel->hide();
	m_ui->frameEmptyNumberPanel->hide();
	m_ui->frameExistPanel->hide();
	m_ui->frameNoResultPanel->hide();

	m_ui->comboBoxCountryCode->setIconSize(QSize(36, 19));

	QHeaderView* header = m_ui->treeWidgetResult->header();
	m_ui->treeWidgetResult->setColumnCount(SearchContactItem::MaxColumn);
	m_ui->treeWidgetResult->setFocusPolicy(Qt::NoFocus);
	m_ui->treeWidgetResult->setAttribute(Qt::WA_MacShowFocusRect, false);
	header->setResizeMode(SearchContactItem::PhotoColumn, QHeaderView::ResizeToContents);
	header->setResizeMode(SearchContactItem::StatusColumn, QHeaderView::ResizeToContents);
	header->setResizeMode(SearchContactItem::LabelColumn, QHeaderView::Stretch);
	header->setResizeMode(SearchContactItem::OperatorColumn, QHeaderView::ResizeToContents);

	m_ui->textBrowserAddNumber->insertHtml(tr("If you want to save a phone number, please <a href=\"#\">click here</a>."));

	m_ui->comboBoxCountryCode->setIconSize(QSize(36,19));

	m_ui->lineEditContactName->setDefaultText(tr("Name"));
	m_ui->lineEditContactName->setMaxLength(PhoneAddContactViewOptions::max_name_length_);
	m_ui->lineEditPhoneNumber->setDefaultText(tr("Phone Number"));
	m_ui->lineEditPhoneNumber->setMaxLength(PhoneAddContactViewOptions::max_number_length_);
	m_ui->lineEditPhoneNumber->setValidator(new QRegExpValidator(QRegExp(QLatin1String("[0-9]+$"))));

	m_messagePanel = new MessagePanel(this);
	m_messagePanel->setLabel(tr("Searching contacts..."));

	m_ui->stackedWidgetAddContact->setCurrentWidget(m_ui->pageAddContact);
}

PhoneAddContactView::~PhoneAddContactView()
{

}

UserService* PhoneAddContactView::userService() const
{
	return m_userService;
}

void PhoneAddContactView::setUserService(UserService* userService)
{
	if (m_userService) {
	}

	m_userService = userService;
	
	if (m_userService) {
	}
}

SipService* PhoneAddContactView::sipService() const
{
	return m_sipService;
}

void PhoneAddContactView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;

	if (m_sipService) {}
}

CommandService* PhoneAddContactView::commandService() const
{
	return m_commandService;
}

void PhoneAddContactView::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
	}

	m_commandService = commandService;

	if (m_commandService) {
	}
}

MediaService* PhoneAddContactView::mediaService() const
{
	return m_mediaService;
}

void PhoneAddContactView::setMediaService(MediaService* mediaService)
{
	if (m_mediaService) {
		disconnect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotContactPhotoDownloaded(const QString&, const QString&)));
		disconnect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
			this, SLOT(slotContactPhotoDownloadFailed(const QString&, const QString&)));
	}

	m_mediaService = mediaService;

	if (m_mediaService) {
		connect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotContactPhotoDownloaded(const QString&, const QString&)));
		connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)),
			this, SLOT(slotContactPhotoDownloadFailed(const QString&, const QString&)));
	}
}

void PhoneAddContactView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneAddContactView::signOut()
{
	disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)),
		this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));

	m_ui->frameExistPanel->hide();
	m_ui->lineEditContactName->clear();
	m_ui->comboBoxCountryCode->clear();
	m_ui->lineEditPhoneNumber->clear();
	m_ui->frameEmptyContactNamePanel->hide();
	m_ui->frameEmptyCountryCodePanel->hide();
	m_ui->frameEmptyNumberPanel->hide();
	m_ui->lineEditContactName->setDefaultText(tr("Name"));
	m_ui->lineEditPhoneNumber->setDefaultText(tr("Phone Number"));

	m_ui->lineEditSearch->clear();
	resetSearchedContact();
	m_ui->stackedWidgetResult->setCurrentWidget(m_ui->pageResult);

	m_ui->stackedWidgetAddContact->setCurrentWidget(m_ui->pageAddContact);

	AbstractPhoneView::signOut();
}

void PhoneAddContactView::setPSTNSupported(const QStringList& supported)
{
	foreach(const QString& s, supported) {
		QStringList country = s.split(QLatin1Char('='));
		if (country.size() != 2)
			continue;
		m_ui->comboBoxCountryCode->addItem(QIcon(QString(QLatin1String(":/images/ui/flag/%1.png")).arg(country[0].trimmed())), 
			QString(QLatin1String("+%1")).arg(country[1].trimmed()), 
			country[0].trimmed());
	}
}

void PhoneAddContactView::getContactDetail(User::Contact& contact)
{
	if (m_ui->stackedWidgetAddContact->currentWidget() == m_ui->pageAddContact) {	

		QTreeWidgetItem* item = m_ui->treeWidgetResult->currentItem();
		if (item && item->type() == SearchContactItem::SearchContactItemType) {
			SearchContactItem* contactItem = static_cast<SearchContactItem*>(item);		
			contact.setContactName(contactItem->userInfo().nickname());
			contact.setSmallPhoto(contactItem->userInfo().smallPhoto());
			contact.setLargePhoto(contactItem->userInfo().largePhoto());
//			contact.setPhone(contactItem->userInfo().ph);
			contact.setVoiceraId(contactItem->userInfo().voiceraID());
			contact.setSipCallerId(contactItem->userInfo().SIPCallerID());
//			contact.setSourceLanguage(contactItem->userInfo());
//			contact.setTargetLanguage(contactItem->userInfo());
			contact.setOnlineStatus(contactItem->userInfo().onlineStatus());
//			contact.setSystemUser(contactItem->userInfo());			
		}
		resetSearchedContact();
		m_ui->stackedWidgetResult->setCurrentWidget(m_ui->pageResult);
		m_ui->lineEditSearch->clear();
	} else {
		contact.setContactName(m_ui->lineEditContactName->text().trimmed());
		QString countryCode = m_ui->comboBoxCountryCode->currentText();
		countryCode.replace(QLatin1String("+"), QLatin1String("00"));
		QString number = m_ui->comboBoxCountryCode->itemData(m_ui->comboBoxCountryCode->currentIndex()).toString();
		number.append(QLatin1Char('|'));
		number.append(countryCode);
		number.append(m_ui->lineEditPhoneNumber->text().trimmed());
		contact.setPhone(number);

		m_ui->lineEditContactName->clear();
		m_ui->lineEditPhoneNumber->clear();
	}
}

void PhoneAddContactView::resetShow()
{
	m_ui->stackedWidgetAddContact->setCurrentWidget(m_ui->pageAddContact);
}

void PhoneAddContactView::changeEvent(QEvent* event)
{
	AbstractPhoneView::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

void PhoneAddContactView::keyPressEvent(QKeyEvent *event)
{
	AbstractPhoneView::keyPressEvent(event);

	if (event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter) {

		if (m_ui->stackedWidgetAddContact->currentWidget() == m_ui->pageAddContact) {
			on_pushButtonSearch_clicked();
		} else {
			on_pushButtonSaveNumber_clicked();
		}
	}
}

void  PhoneAddContactView::slotPhoneGetUserInfoFinished(const Command::PhoneGetUserInfoParam& param, const Command::PhoneGetUserInfoResult& result)
{
	disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)),
		this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));

	if (!result.voiceraID().isEmpty()) {

		m_ui->stackedWidgetResult->show();
		SearchContactItem* newItem = new SearchContactItem(m_ui->treeWidgetResult);
		newItem->setUserInfo(result);
		m_contacts.insert(result.voiceraID(), newItem);
		
		if (!result.smallPhoto().isEmpty() && m_mediaService) {
			QString fileName = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
			QString photoLink = result.smallPhoto();
			int index = photoLink.lastIndexOf(QLatin1Char('/'));
			fileName.append(QString(QLatin1String("/%1")).arg(photoLink.mid(index+1)));
			m_mediaService->downloadFile(result.smallPhoto(), fileName);
		}

	} else {
		m_ui->stackedWidgetResult->hide();
		m_ui->frameNoResultPanel->show();
		m_ui->labelQueryItem->setText(m_ui->lineEditSearch->text());
	}
}

void PhoneAddContactView::slotContactPhotoDownloadFailed(const QString& link, const QString& reason)
{
}

void PhoneAddContactView::slotContactPhotoDownloaded(const QString& photoLink, const QString& fileName)
{
	QList<SearchContactItem*> searchItemList = m_contacts.values();
	
	foreach (SearchContactItem* searchItem, searchItemList) {
		if (searchItem->userInfo().smallPhoto() == photoLink)
			searchItem->setPhotoFile(fileName);
	}
}

void PhoneAddContactView::on_pushButtonSearch_clicked()
{
	resetSearchedContact();
	m_ui->stackedWidgetResult->setCurrentWidget(m_ui->pageResult);

	if (m_ui->lineEditSearch->text().isEmpty()) {
		qDebug() << QPID_DEBUG_PREFIX << "You must input a text when begin clicking search";
		return;
	}

	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)),
			this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));
		connect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)),
			this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));
		Command::PhoneGetUserInfoParam param;
		param.setVoiceraID(m_ui->lineEditSearch->text());
		m_commandService->phonegetuserinfo(param);
	}
}

void PhoneAddContactView::on_pushButtonAdd_clicked()
{
	if (m_commandService) {
		Command::PhoneAddContactParam param;
		param.setVoiceraID(m_ui->lineEditVoiceID->text());
		m_commandService->phoneaddcontact(param);
	}
}

void PhoneAddContactView::on_pushButtonReturn_clicked()
{
	m_ui->stackedWidgetResult->setCurrentWidget(m_ui->pageResult);
}

void PhoneAddContactView::on_pushButtonSaveNumber_clicked()
{
	m_ui->frameEmptyContactNamePanel->hide();
	m_ui->frameEmptyCountryCodePanel->hide();
	m_ui->frameEmptyNumberPanel->hide();

	if (m_ui->lineEditContactName->text().trimmed().isEmpty()) {
		m_ui->frameEmptyContactNamePanel->show();
		return;
	}
	
	if (m_ui->lineEditPhoneNumber->text().trimmed().isEmpty()) {
		m_ui->frameEmptyNumberPanel->show();
		return;
	}
	
	if (m_commandService) {
		Command::PhoneAddContactParam param;
		QString phone = m_ui->comboBoxCountryCode->currentText();
		phone.replace(QLatin1String("+"), QLatin1String("00"));
		phone.append(m_ui->lineEditPhoneNumber->text().trimmed());
		param.setName(m_ui->lineEditContactName->text().trimmed());
		param.setPhone(phone);
		param.setPhoneCountry(m_ui->comboBoxCountryCode->itemData(m_ui->comboBoxCountryCode->currentIndex()).toString());
		m_commandService->phoneaddcontact(param);
	}
	
}

void PhoneAddContactView::on_pushButtonCancel_clicked()
{
	m_ui->lineEditContactName->clear();
	m_ui->lineEditPhoneNumber->clear();
	m_ui->lineEditContactName->setDefaultText(tr("Name"));
	m_ui->lineEditPhoneNumber->setDefaultText(tr("Phone Number"));
	m_ui->frameEmptyContactNamePanel->hide();
	m_ui->frameEmptyCountryCodePanel->hide();
	m_ui->frameEmptyNumberPanel->hide();
	m_ui->stackedWidgetAddContact->setCurrentWidget(m_ui->pageAddContact);
}

void PhoneAddContactView::on_treeWidgetResult_itemClicked(QTreeWidgetItem * item, int column)
{
	if (item && item->type() == SearchContactItem::SearchContactItemType) {
		SearchContactItem* contactItem = static_cast<SearchContactItem*>(item);

		//Check already exist
		bool exist = true;
		if (m_userService && contactItem)
			exist = m_userService->phoneContactExist(contactItem->userInfo().voiceraID());

		if (column == SearchContactItem::OperatorColumn) {
			if (exist) {
				showDetails(contactItem, exist);
			} else {
				Command::PhoneAddContactParam param;
				param.setVoiceraID(contactItem->userInfo().voiceraID());
				m_commandService->phoneaddcontact(param);
			}

		} else {
			showDetails(contactItem, exist);
		}
	}	
}

void PhoneAddContactView::on_textBrowserAddNumber_anchorClicked(const QUrl & link)
{
	m_ui->stackedWidgetAddContact->setCurrentWidget(m_ui->pageAddNumber);
}

void PhoneAddContactView::resetSearchedContact()
{
	if (!m_contacts.isEmpty()) {
		QHashIterator<QString, SearchContactItem*> i(m_contacts);
		while (i.hasNext()) {
			i.next();
			delete i.value();
		}
	}

	m_contacts.clear();

	m_ui->stackedWidgetResult->hide();
	m_ui->frameNoResultPanel->hide();
}

void PhoneAddContactView::showDetails(SearchContactItem* contactItem, bool alreadyExist)
{
	if (contactItem->photoFile().isEmpty()) {
		m_ui->labelPhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));
	}
	else {
		m_ui->labelPhoto->setPixmap(QPixmap(contactItem->photoFile()));
	}

	if (contactItem->userInfo().nickname().isEmpty()) {
		m_ui->lineEditDisplayName->setText(contactItem->userInfo().voiceraID());
	} else {
		m_ui->lineEditDisplayName->setText(contactItem->userInfo().nickname());
	}

	if (contactItem->userInfo().onlineStatus() == QLatin1String("1")) {
		m_ui->lineEditStatus->setText(tr("Online"));
	} else {
		m_ui->lineEditStatus->setText(tr("Offline"));
	}

	m_ui->lineEditVoiceID->setText(contactItem->userInfo().voiceraID());
	m_ui->lineEditLanguage->setText(qApp->languageFull(contactItem->userInfo().nativeLanguage()));
	m_ui->lineEditNationality->setText(qApp->countryFull(contactItem->userInfo().country()));
	m_ui->lineEditProfession->setText(qApp->majorName(contactItem->userInfo().major()));

	m_ui->stackedWidgetResult->setCurrentWidget(m_ui->pageDetails);

	m_ui->frameCallPanel->setVisible(!alreadyExist);
	m_ui->frameExistPanel->setVisible(alreadyExist);
}

void PhoneAddContactView::retranslateTr()
{
	m_ui->textBrowserAddNumber->clear();
	m_ui->textBrowserAddNumber->insertHtml(tr("If you want to save a phone number, please <a href=\"#\">click here</a>."));
	m_ui->lineEditContactName->setDefaultText(tr("Name"));
	m_ui->lineEditPhoneNumber->setDefaultText(tr("Phone Number"));
	m_messagePanel->setLabel(tr("Searching contacts..."));
}

} // namespace Qpid
