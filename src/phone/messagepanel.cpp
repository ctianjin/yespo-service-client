#include <QtGui/QResizeEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtCore/QSize>
#include <QtGui/QBitmap>
#include <QMovie>
#include <QDebug>
#include "messagepanel.h"
#include "ui_messagepanel.h"

namespace Qpid {

MessagePanel::MessagePanel(QWidget* parent) :
    QDialog(parent, Qt::FramelessWindowHint),
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
    //adjustSize();
}

void MessagePanel::setMovie(QMovie* movie)
{
	m_movie = movie;
	m_ui->label->setMovie(movie);
}

void MessagePanel::setVisible(bool visible)
{
    QDialog::setVisible(visible);
	if (m_movie) {
		visible?m_movie->start():m_movie->stop();
	}

    if (visible) {
        QWidget* p = parentWidget();
        if (p) {
            QWidget* w = p->window();
            if (w) {
				qDebug() << w->x() << w->y() << w->geometry();
				move(w->x() + (w->width() - width()) / 2, w->y() + (w->height() - height()) / 2 + 50);
            }
        }
    }
}

void MessagePanel::resizeEvent(QResizeEvent* event)
{
	QDialog::resizeEvent(event);

	const QSize size = event->size();
	QPixmap pixmap(size);
	QPainter painter(&pixmap);
	painter.fillRect(pixmap.rect(), Qt::white);
	painter.setBrush(Qt::black);
	painter.drawRoundedRect(0, 0, size.width() - 1, size.height() - 1, 5, 5);
	setMask(pixmap.createMaskFromColor(Qt::white));
}

void MessagePanel::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
		return;

	QDialog::keyPressEvent(event);
}

} // namespace Qpid
