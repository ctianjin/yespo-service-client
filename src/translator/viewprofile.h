#ifndef VIEWPROFILE_H
#define VIEWPROFILE_H

#include <QtCore/QScopedPointer>
#include <QWidget>

namespace ns {

class Ui_ViewProfile;

class ViewProfile : public QWidget
{
    Q_OBJECT

public:
    ViewProfile(QWidget* parent = 0);
    ~ViewProfile();

	void setDisplayName(const QString& name, const QString& Id);
	void setPhotoUrl(const QString& url);
	void setPhoto(const QString& url, const QString& photo);
	void setIntroduction(const QString& intro);
	void setLanguage(const QString& source, const QString& target);
	void setServiceMode(const QString& mode);

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:

private:
	void retranslateTr();

	QString m_photoUrl;
	QString m_displayName;
	QString m_loginId;

	QScopedPointer<Ui_ViewProfile> m_ui;
};

} // namespace ns

#endif // VIEWPROFILE_H
