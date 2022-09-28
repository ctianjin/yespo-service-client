#ifndef CUSTOMFONTLABEL_H
#define CUSTOMFONTLABEL_H

#include <QLabel>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QColor;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL CustomFontLabel : public QLabel
{
    Q_OBJECT

public:
	explicit CustomFontLabel(QWidget *parent);
	virtual ~CustomFontLabel();

	void setFontWeight(int weight) {m_fontWeight = weight;}

	void setFontAlpha(int alpha) {m_fontAlpha = alpha;}

	
	void setFontBorder(int width)
	{
		m_fontBorderWidth = width; 
	}

	void setPointSize(int pointSize) {m_pointSize = pointSize;}
	
	void setFontFamily(const QString& family) {m_fontFamily = family;}

	void setFontSpacing(qreal spacing) {m_fontSpacing = spacing;}

public Q_SLOTS:
	
protected:
	virtual void paintEvent(QPaintEvent* event);

Q_SIGNALS:

private:
	qreal m_fontSpacing;
	int m_fontAlpha;
	QString m_fontFamily;
	int m_pointSize;
	int m_fontWeight;
	int m_fontBorderWidth;
	QColor m_fontBorderColor;
};

} // namespace ns

#endif // CUSTOMFONTLABEL_H
