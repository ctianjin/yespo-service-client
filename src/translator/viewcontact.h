#ifndef QPID_VIEWCONTACT_H
#define QPID_VIEWCONTACT_H

#include <QtCore/QScopedPointer>
#include <QPointer>
#include <QWidget>

namespace ns {

class Ui_ViewContact;
class CommandService;

class ViewContact : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)

public:
    ViewContact(QWidget* parent = 0);
    ~ViewContact();

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	void setVESysID(const QString& Id);
	void setMemberInfo(const QString& displayName, const QString& language, const QString& nation, const QString& carrer);
	void setPhotoFile(const QString& phoFile);

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:
	void on_pushButtonBlacklist_clicked();

private:
	void retranslateTr();

	QString m_VESysId;
	QString m_displayName;
	QString m_language;
	QString m_nation;
	QString m_major;

	QPointer<CommandService> m_commandService;
	QScopedPointer<Ui_ViewContact> m_ui;
};

} // namespace ns

#endif // QPID_VIEWCONTACT_H
