#ifndef QPID_TRANSLATIONLANGUAGE_H
#define QPID_TRANSLATIONLANGUAGE_H

#include <QtCore/QScopedPointer>
#include <QWidget>

namespace ns {

class Ui_TranslationLanguage;

class TranslationLanguage : public QWidget
{
    Q_OBJECT

public:
    TranslationLanguage::TranslationLanguage(QWidget* parent = 0);
    ~TranslationLanguage();

	void setLanguage(const QString& source, const QString& target);
	void setServiceMode(const QString& mode);
	void setLanguageStyle(const QString& style);

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent* event);

private	Q_SLOTS:

private:
	void retranslateTr();

	QString m_source;
	QString m_target;
	QString m_mode;

	QScopedPointer<Ui_TranslationLanguage> m_ui;
};

} // namespace ns

#endif // QPID_TRANSLATIONLANGUAGE_H
