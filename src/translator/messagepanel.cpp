#include <QKeyEvent>
#include <QMovie>
#include "messagepanel.h"
#include "ui_messagepanel.h"

namespace ns {

MessagePanel::MessagePanel(QWidget* parent) :
    QWidget(parent, Qt::FramelessWindowHint),
    m_ui(new Ui_MessagePanel),
	m_movie(0)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
}

MessagePanel::~MessagePanel()
{
}

QString MessagePanel::label() const
{
    return m_ui->label->text();
}

void MessagePanel::setLabel(const QString& label)
{
    m_ui->label->setText(label);
}

void MessagePanel::setMovie(QMovie* movie)
{
	m_movie = movie;
	m_ui->label->setMovie(movie);
}

void MessagePanel::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
		return;

	QWidget::keyPressEvent(event);
}

void MessagePanel::mouseMoveEvent(QMouseEvent * event)
{

}

} // namespace ns
