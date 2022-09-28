#ifndef QPID_INTERPRETATIONSELECT_H
#define QPID_INTERPRETATIONSELECT_H

#include <QScopedPointer>
#include <QDialog>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_InterpretationSelect;

class InterpretationSelect : public QDialog
{
	Q_OBJECT

public:
    explicit InterpretationSelect(QWidget* parent = 0);
    virtual ~InterpretationSelect();

    // FIXME For bug that not centering dialog to parent window
    virtual void setVisible(bool visible);

	void initialSourceLanguage();
	void hideHelp();

Q_SIGNALS:
	void interpreterAdded(const QString& source, const QString& target);

protected:
	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void keyPressEvent(QKeyEvent * event);

private Q_SLOTS:
	void on_pushButtonOK_clicked();
	void on_comboBoxSource_currentIndexChanged(int index);

private:
	void retranslateTr();

    QScopedPointer<Ui_InterpretationSelect> m_ui;

    Q_DISABLE_COPY(InterpretationSelect)
};

} // namespace Qpid

#endif // QPID_INTERPRETATIONSELECT_H
