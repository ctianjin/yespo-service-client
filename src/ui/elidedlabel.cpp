#include "elidedlabel.h"
#include <QPainter>
#include <QTextLayout>
#include <QRect>
#include <QResizeEvent>
#include <QString>
#include <QDebug>
#include <QBitmap>

namespace ns {
 
ElidedLabel::ElidedLabel(QWidget *parent) 
	: QLabel(parent), 
	elided(false)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

ElidedLabel::~ElidedLabel()
{

}

void ElidedLabel::setText(const QString& text)
{
	if (!wordWrap()) {
		QFontMetricsF ff(font());
		QString elidedText = ff.elidedText(text, Qt::ElideRight, geometry().width());
		QLabel::setText(elidedText);
	} else {
		QLabel::setText(text);
	}
}

void ElidedLabel::setRadius(qreal xRadius, qreal yRadius)
{
	m_xRadius = xRadius;
	m_yRadius = yRadius;
	update();
}

void ElidedLabel::setMaskPixmap(const QPixmap& pix, const QPixmap& mask)
{
	QImage resultImage(size(),QImage::Format_ARGB32_Premultiplied);
	QImage maskImg = mask.toImage();
	maskImg.scaled(size());
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, maskImg);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
	painter.drawImage(0, 0, pix.toImage().scaled(size()));
	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	painter.end();
	QLabel::setPixmap(QPixmap::fromImage(resultImage));
}

void ElidedLabel::resizeEvent(QResizeEvent* event)
{
	QLabel::resizeEvent(event);
}

void ElidedLabel::mousePressEvent(QMouseEvent* event)
{
	Q_EMIT clicked();
}

}

