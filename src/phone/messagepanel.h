#ifndef QPID_MESSAGEPANEL_H
#define QPID_MESSAGEPANEL_H

#include <QScopedPointer>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace Qpid {

class Ui_MessagePanel;

class MessagePanel : public QDialog
{
public:
    explicit MessagePanel(QWidget* parent = 0);
    virtual ~MessagePanel();

    QString label() const;
    void setLabel(const QString& label);

	void setMovie(QMovie* movie);

    // FIXME For bug that not centering dialog to parent window
    virtual void setVisible(bool visible);

protected:
	virtual void resizeEvent(QResizeEvent* event);
	virtual void keyPressEvent(QKeyEvent * event);

private:
    QScopedPointer<Ui_MessagePanel> m_ui;
	QMovie* m_movie;

    Q_DISABLE_COPY(MessagePanel)
};

} // namespace Qpid

#endif // QPID_MESSAGEPANEL_H
