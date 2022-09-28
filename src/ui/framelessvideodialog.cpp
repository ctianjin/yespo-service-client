#include <QtCore/QDebug>
#include <QtGui/QPixmap>
#include <QtGui/QResizeEvent>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include <QtGui/QStyleOption>
#include <QApplication>
#include <QMouseEvent>
#include <QEvent>
#include "framelessvideodialog.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "FramelessVideoDialog:"
#endif // DEBUG_PREFIX

namespace ns {

FramelessVideoDialog::FramelessVideoDialog(QWidget* parent, Qt::WindowFlags flags) :
    QDialog(parent, flags)	
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
//	setAttribute(Qt::WA_TranslucentBackground);
}

FramelessVideoDialog::~FramelessVideoDialog()
{
}

void FramelessVideoDialog::paintEvent(QPaintEvent *event)
{
	updateSkin();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawPixmap(0,0,m_pixMask);
}

void FramelessVideoDialog::updateSkin()
{
	m_pixMask = QPixmap(width(), height());
	m_pixMask.fill(Qt::transparent);
	QPainter p(&m_pixMask);
	QStyleOption opt;
	opt.init(this);

	p.setBrush(QColor(175,175,175));
	p.setRenderHint(QPainter::Antialiasing);
	p.drawRoundedRect(0,0,width(),height(),4,4);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	p.end();
	setMask(m_pixMask.mask());
}

#if 0
bool FramelessVideoDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *e = static_cast<QMouseEvent *> (event);
		if (e->buttons() && Qt::LeftButton) {
			m_isLeftPressed = true;
			m_pLast = e->globalPos();
			m_curPos = countFlag(e->pos(), countRow(e->pos()));
		}
	}
	if (event->type() == QEvent::MouseMove) {
		QMouseEvent *e = static_cast<QMouseEvent *> (event);
		int poss=countFlag(e->pos(),countRow(e->pos()));
		setCursorType(poss);
		if(m_isLeftPressed) {
			QPoint ptemp = e->globalPos();
			ptemp = ptemp- m_pLast;
			if(m_curPos == 22 || m_curPos == 12) {
				ptemp=ptemp+pos();
				move(ptemp);
			} else {
				QRect wid=geometry();

				switch(m_curPos)
				{
					case 11:wid.setTopLeft(wid.topLeft()+ptemp);break;
					case 13:wid.setTopRight(wid.topRight()+ptemp);break;
					case 31:wid.setBottomLeft(wid.bottomLeft()+ptemp);break;
					case 33:wid.setBottomRight(wid.bottomRight()+ptemp);break;
					//case 12:wid.setTop(wid.top()+ptemp.y());break;
					case 21:
						wid.setLeft(wid.left()+ptemp.x());
						wid.setBottom(wid.bottom()-ptemp.x());
						break;
					case 23:
						wid.setRight(wid.right()+ptemp.x());
						wid.setBottom(wid.bottom()+ptemp.x());
						break;
					case 32:
						wid.setBottom(wid.bottom()+ptemp.y());
						wid.setRight(wid.right()+ptemp.y());
						break;
				}
				setGeometry(wid);
			}

			m_pLast = e->globalPos();
		}
	}
	if (event->type() == QEvent::MouseButtonRelease) {
		m_isLeftPressed = false;
		setCursor(Qt::ArrowCursor);
	}
	return QDialog::eventFilter(obj, event);
}

void FramelessVideoDialog::resizeEvent(QResizeEvent* event)
{
	QDialog::resizeEvent(event);

	const QSize size = event->size();
	QPixmap pixmap(size);
	QPainter painter(&pixmap);
	painter.fillRect(pixmap.rect(), Qt::white);
	painter.setBrush(Qt::black);
	painter.drawRoundedRect(0, 0, size.width() - 1, size.height() - 1, 3, 3);
	setMask(pixmap.createMaskFromColor(Qt::white));

	if (width() != height() * 3 / 2)
		setGeometry(geometry().x(), geometry().y(), height() * 3 / 2, height());
	if (height() != width() * 2 / 3)
		setGeometry(geometry().x(), geometry().y(), width(), width() * 2 / 3);
}
#endif

} // namespace ns
