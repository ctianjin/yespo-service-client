#ifndef ELIDEDLABEL_H
#define ELIDEDLABEL_H

#include <QLabel>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL ElidedLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool isElided READ isElided)

public:
	explicit ElidedLabel(QWidget *parent);
	virtual ~ElidedLabel();

    bool isElided() const { return elided; }
	void setRadius(qreal xRadius, qreal yRadius);
	void setMaskPixmap(const QPixmap& pix, const QPixmap& mask);

public Q_SLOTS:
	void setText(const QString& text);
	
protected:
	virtual void resizeEvent(QResizeEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

Q_SIGNALS:
    void elisionChanged(bool elided);
	void clicked();

private:
    bool elided;
	qreal m_xRadius;
	qreal m_yRadius;
};

} // namespace ns

#endif // ELIDEDLABEL_H
