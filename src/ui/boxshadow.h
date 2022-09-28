#ifndef AROUNDSHADOW_H
#define AROUNDSHADOW_H

#include <QGraphicsEffect>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL BoxShadow : public QGraphicsEffect
{
    Q_OBJECT

public:
    explicit BoxShadow(QObject *parent = 0);
	virtual ~BoxShadow();

    void draw(QPainter* painter);
    QRectF boundingRectFor(const QRectF& rect) const;

    inline void setDistance(qreal distance) { _distance = distance; updateBoundingRect(); }
    inline qreal distance() const { return _distance; }

    inline void setBlurRadius(qreal blurRadius) { _blurRadius = blurRadius; updateBoundingRect(); }
    inline qreal blurRadius() const { return _blurRadius; }

    inline void setColor(const QColor& color) { _color = color; }
    inline QColor color() const { return _color; }
	
private:
	qreal  _distance;
    qreal  _blurRadius;
    QColor _color;

    Q_DISABLE_COPY(BoxShadow)
};

} // namespace ns

#endif // AROUNDSHADOW_H
