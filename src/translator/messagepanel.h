#ifndef MESSAGEPANEL_H
#define MESSAGEPANEL_H

#include <QScopedPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace ns {

class Ui_MessagePanel;

class MessagePanel : public QWidget
{
public:
    explicit MessagePanel(QWidget* parent = 0);
    virtual ~MessagePanel();

    QString label() const;
    void setLabel(const QString& label);

	void setMovie(QMovie* movie);

protected:
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);

private:
    QScopedPointer<Ui_MessagePanel> m_ui;
	QMovie* m_movie;

    Q_DISABLE_COPY(MessagePanel)
};

} // namespace ns

#endif // MESSAGEPANEL_H
