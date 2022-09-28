#ifndef QPID_FRAMELESSHELPER_P_H
#define QPID_FRAMELESSHELPER_P_H

#include <QtCore/QHash>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include "framelesshelper.h"

QT_BEGIN_NAMESPACE
class QHoverEvent;
class QMouseEvent;
class QRubberBand;
QT_END_NAMESPACE

namespace ns {

class FramelessCursorPosCalculator
{
public:
    static int m_borderWidth;

    bool onEdges;
    bool onLeftEdge;
    bool onRightEdge;
    bool onTopEdge;
    bool onBottomEdge;
    bool onTopLeftEdge;
    bool onBottomLeftEdge;
    bool onTopRightEdge;
    bool onBottomRightEdge;

    FramelessCursorPosCalculator();
    ~FramelessCursorPosCalculator();

    void reset();
    void recalculate(const QPoint& globalMousePos, const QRect& frameRect);
};

class FramelessWidgetData
{
public:
    FramelessWidgetData(FramelessHelperPrivate* d, QWidget* widget);
    ~FramelessWidgetData();

    QWidget* widget() const;

    void updateRubberBandStatus();
    void handleWidgetEvent(QEvent* event);

private:
    void updateCursorShape(const QPoint& globalMousePos);
    void moveWidget(const QPoint& globalMousePos);
    void resizeWidget(const QPoint& globalMousePos);

    void handleMousePressEvent(QMouseEvent* event);
    void handleMouseReleaseEvent(QMouseEvent* event);
    void handleMouseMoveEvent(QMouseEvent* event);
    void handleLeaveEvent(QEvent* event);
    void handleHoverMoveEvent(QHoverEvent* event);

private:
    FramelessHelperPrivate* m_d;
    QWidget* m_widget;
    QRubberBand* m_rubberBand;
    bool m_leftButtonPressed;
    QPoint m_dragPos;
    FramelessCursorPosCalculator m_pressedMousePos;
    FramelessCursorPosCalculator m_moveMousePos;
    bool m_cursorShapeChanged;
    Qt::WindowFlags m_windowFlags;
};

class FramelessHelperPrivate
{
    Q_DECLARE_PUBLIC(FramelessHelper)

public:
    FramelessHelper* q_ptr;

    QHash<QWidget*, FramelessWidgetData*> m_widgetDatas;
    bool m_movable;
    bool m_resizable;
    bool m_useRubberBandOnResize;
    bool m_useRubberBandOnMove;

    FramelessHelperPrivate();
    virtual ~FramelessHelperPrivate();

    void updateRubberBandStatus();
};

} // namespace ns

#endif // QPID_FRAMELESSHELPER_P_H
