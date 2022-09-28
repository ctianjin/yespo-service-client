#include "customfontlabel.h"
#include <QPainter>
#include <QTextLayout>
#include <QRect>
#include <QResizeEvent>
#include <QString>
#include <QDebug>
#include <QBitmap>

namespace ns {
 
CustomFontLabel::CustomFontLabel(QWidget *parent) 
	: QLabel(parent),
	m_fontFamily(QLatin1String("Verdana")),
	m_pointSize(70),
	m_fontWeight(100),
	m_fontAlpha(210),
	m_fontBorderWidth(4),
	m_fontSpacing(0.0)
{
}

CustomFontLabel::~CustomFontLabel()
{

}

void CustomFontLabel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QPainterPath path;
	QPen pen;
	QFont font(m_fontFamily, m_pointSize, m_fontWeight);

	font.setLetterSpacing(QFont::AbsoluteSpacing, m_fontSpacing);
	int size = font.pointSize();

	QFontMetrics fm(font);
	int w = fm.width(text());
	int h = fm.height();
	int left = (width() - w) / 2;
	pen.setWidth(m_fontBorderWidth);
	pen.setColor(QColor(0,0,0,110));
	painter.setFont(font);

	font.setStyleStrategy(QFont::ForceOutline);
	path.addText(left,h,font, text()); 
	//painter.drawText(Qt::AlignCenter, Qt::AlignCenter, text());
	painter.strokePath(path, pen);

	path.closeSubpath();

	painter.fillPath(path, QColor(255, 255, 255,  230));

	painter.setRenderHint(QPainter::Antialiasing, false);
}

}

