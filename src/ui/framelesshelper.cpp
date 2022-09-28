#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QRubberBand>
#include <QWidget>
#include "framelesshelper.h"
#include "framelesshelper_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "FramelessHelper:"
#endif // DEBUG_PREFIX

namespace ns {

// TODO Make it not static
int FramelessCursorPosCalculator::m_borderWidth = 2;

FramelessCursorPosCalculator::FramelessCursorPosCalculator()
{
    reset();
}

FramelessCursorPosCalculator::~FramelessCursorPosCalculator()
{
}

void FramelessCursorPosCalculator::reset()
{
    onEdges = false;
    onLeftEdge = false;
    onRightEdge = false;
    onTopEdge = false;
    onBottomEdge = false;
    onTopLeftEdge = false;
    onBottomLeftEdge = false;
    onTopRightEdge = false;
    onBottomRightEdge = false;
}

void FramelessCursorPosCalculator::recalculate(const QPoint& globalMousePos, const QRect& frameRect)
{
    const int globalMouseX = globalMousePos.x();
    const int globalMouseY = globalMousePos.y();
    const int frameX = frameRect.x();
    const int frameY = frameRect.y();
    const int frameWidth = frameRect.width();
    const int frameHeight = frameRect.height();

    onLeftEdge = globalMouseX >= frameX && globalMouseX <= frameX + m_borderWidth;

    onRightEdge = globalMouseX >= frameX + frameWidth - m_borderWidth
        && globalMouseX <= frameX + frameWidth;

    onTopEdge = globalMouseY >= frameY && globalMouseY <= frameY + m_borderWidth;

    onBottomEdge = globalMouseY >= frameY + frameHeight - m_borderWidth
        && globalMouseY <= frameY + frameHeight;

    onTopLeftEdge = onTopEdge && onLeftEdge;
    onBottomLeftEdge = onBottomEdge && onLeftEdge;
    onTopRightEdge = onTopEdge && onRightEdge;
    onBottomRightEdge = onBottomEdge && onRightEdge;

    // XXX Only these checks would be enough
    onEdges = onLeftEdge || onRightEdge || onTopEdge || onBottomEdge;
}

FramelessWidgetData::FramelessWidgetData(FramelessHelperPrivate* d, QWidget* widget) :
    m_d(d),
    m_widget(widget),
    m_rubberBand(0),
    m_leftButtonPressed(false),
    m_cursorShapeChanged(false),
    m_windowFlags(0)
{
    Q_ASSERT(m_d);
    Q_ASSERT(m_widget);

    m_windowFlags = m_widget->windowFlags();

    // XXX From Qt docs of setWindowFlags()
    // Note: This function calls setParent() when changing the flags for a window, causing the
    //  widget to be hidden. You must call show() to make the widget visible again.
    bool visible = m_widget->isVisible();

    m_widget->setMouseTracking(true);
    m_widget->setWindowFlags(Qt::FramelessWindowHint
#ifdef Q_WS_WIN
        | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint
#else // Q_WS_WIN
        | Qt::CustomizeWindowHint
#endif // Q_WS_WIN
        );
    // XXX Bug fix, mouse move events does not propagate from child widgets. So need the hover
    //  events.
    m_widget->setAttribute(Qt::WA_Hover);

    updateRubberBandStatus();

    m_widget->setVisible(visible);
}

FramelessWidgetData::~FramelessWidgetData()
{
    bool visible = m_widget->isVisible();

    m_widget->setMouseTracking(false);
    m_widget->setWindowFlags(m_windowFlags/* ^ Qt::CustomizeWindowHint ^ Qt::FramelessWindowHint*/);
    m_widget->setAttribute(Qt::WA_Hover, false);

    m_widget->setVisible(visible);

    delete m_rubberBand;
}

QWidget* FramelessWidgetData::widget() const
{
    return m_widget;
}

void FramelessWidgetData::updateRubberBandStatus()
{
    if (m_d->m_useRubberBandOnMove || m_d->m_useRubberBandOnResize) {
        if (!m_rubberBand)
            m_rubberBand = new QRubberBand(QRubberBand::Rectangle);
    } else {
        delete m_rubberBand;
        m_rubberBand = 0;
    }
}

void FramelessWidgetData::handleWidgetEvent(QEvent* event)
{
    switch (event->type()) {
        case QEvent::MouseButtonPress:
            handleMousePressEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            handleMouseReleaseEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            handleMouseMoveEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::Leave:
            handleLeaveEvent(event);
            break;
        case QEvent::HoverMove:
            handleHoverMoveEvent(static_cast<QHoverEvent*>(event));
            break;
        default:
            break;
    }
}

void FramelessWidgetData::updateCursorShape(const QPoint& globalMousePos)
{
    if (m_widget->isFullScreen() || m_widget->isMaximized()) {
        if (m_cursorShapeChanged)
            m_widget->unsetCursor();
        return;
    }

    m_moveMousePos.recalculate(globalMousePos, m_widget->frameGeometry());

    if (m_moveMousePos.onTopLeftEdge || m_moveMousePos.onBottomRightEdge) {
        m_widget->setCursor(Qt::SizeFDiagCursor);
        m_cursorShapeChanged = true;
    } else if (m_moveMousePos.onTopRightEdge || m_moveMousePos.onBottomLeftEdge) {
        m_widget->setCursor(Qt::SizeBDiagCursor);
        m_cursorShapeChanged = true;
    } else if (m_moveMousePos.onLeftEdge || m_moveMousePos.onRightEdge) {
        m_widget->setCursor(Qt::SizeHorCursor);
        m_cursorShapeChanged = true;
    } else if (m_moveMousePos.onTopEdge || m_moveMousePos.onBottomEdge) {
        m_widget->setCursor(Qt::SizeVerCursor);
        m_cursorShapeChanged = true;
    } else {
        if (m_cursorShapeChanged) {
            m_widget->unsetCursor();
            m_cursorShapeChanged = false;
        }
    }
}

void FramelessWidgetData::moveWidget(const QPoint& globalMousePos)
{
    if (m_d->m_useRubberBandOnMove) {
        m_rubberBand->move(globalMousePos - m_dragPos);
    } else {
        m_widget->move(globalMousePos - m_dragPos);
    }
}

void FramelessWidgetData::resizeWidget(const QPoint& globalMousePos)
{
    QRect origRect;

    if (m_d->m_useRubberBandOnResize)
        origRect = m_rubberBand->frameGeometry();
    else
        origRect = m_widget->frameGeometry();

    int left = origRect.left();
    int top = origRect.top();
    int right = origRect.right();
    int bottom = origRect.bottom();

    const int minWidth = m_widget->minimumWidth();
    const int minHeight = m_widget->minimumHeight();

    if (m_pressedMousePos.onTopLeftEdge) {
        left = globalMousePos.x();
        top = globalMousePos.y();
    } else if (m_pressedMousePos.onBottomLeftEdge) {
        left = globalMousePos.x();
        bottom = globalMousePos.y();
    } else if (m_pressedMousePos.onTopRightEdge) {
        right = globalMousePos.x();
        top = globalMousePos.y();
    } else if (m_pressedMousePos.onBottomRightEdge) {
        right = globalMousePos.x();
        bottom = globalMousePos.y();
    } else if (m_pressedMousePos.onLeftEdge) {
        left = globalMousePos.x();
    } else if (m_pressedMousePos.onRightEdge) {
        right = globalMousePos.x();
    } else if (m_pressedMousePos.onTopEdge) {
        top = globalMousePos.y();
    } else if (m_pressedMousePos.onBottomEdge) {
        bottom = globalMousePos.y();
    }

    QRect newRect(QPoint(left, top), QPoint(right, bottom));
    if (newRect.isValid()) {
        if (minWidth > newRect.width()) {
            // Determine what has caused the width change.
            if (left != origRect.left())
                newRect.setLeft(origRect.left());
            else
                newRect.setRight(origRect.right());
        }
        if (minHeight > newRect.height()) {
            // Determine what has caused the height change.
            if (top != origRect.top())
                newRect.setTop(origRect.top());
            else
                newRect.setBottom(origRect.bottom());
        }

        if (m_d->m_useRubberBandOnResize) {
            m_rubberBand->setGeometry(newRect);
        } else {
            m_widget->setGeometry(newRect);
        }
    } else {
        qDebug() << DEBUG_PREFIX << "Calculated rect is not valid" << newRect;
    }
}

void FramelessWidgetData::handleMousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_leftButtonPressed = true;

        const QRect frameRect = m_widget->frameGeometry();
        m_pressedMousePos.recalculate(event->globalPos(), frameRect);

        m_dragPos = event->globalPos() - frameRect.topLeft();

        if (m_pressedMousePos.onEdges) {
            if (m_d->m_useRubberBandOnResize) {
                m_rubberBand->setGeometry(frameRect);
                m_rubberBand->show();
            }
        } else if (m_d->m_useRubberBandOnMove) {
            m_rubberBand->setGeometry(frameRect);
            m_rubberBand->show();
        }
    }
}

void FramelessWidgetData::handleMouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_leftButtonPressed = false;
        m_pressedMousePos.reset();
        if (m_rubberBand && m_rubberBand->isVisible()) {
            m_rubberBand->hide();
            m_widget->setGeometry(m_rubberBand->geometry());
        }
    }
}

void FramelessWidgetData::handleMouseMoveEvent(QMouseEvent* event)
{
    if (m_widget->isMaximized())
        return;

    if (m_leftButtonPressed) {
        if (m_d->m_resizable && m_pressedMousePos.onEdges) {
            resizeWidget(event->globalPos());
        } else if (m_d->m_movable) {
            moveWidget(event->globalPos());
        }
    } else if (m_d->m_resizable) {
        updateCursorShape(event->globalPos());
    }
}

void FramelessWidgetData::handleLeaveEvent(QEvent* /*event*/)
{
    if (!m_leftButtonPressed) {
        m_widget->unsetCursor();
    }
}

void FramelessWidgetData::handleHoverMoveEvent(QHoverEvent* event)
{
    if (m_widget->isMaximized())
        return;

    if (m_d->m_resizable) {
        updateCursorShape(m_widget->mapToGlobal(event->pos()));
    }
}

FramelessHelperPrivate::FramelessHelperPrivate() :
    q_ptr(0),
    m_movable(true),
    m_resizable(true),
    m_useRubberBandOnResize(false),
    m_useRubberBandOnMove(false)
{
}

FramelessHelperPrivate::~FramelessHelperPrivate()
{
    qDeleteAll(m_widgetDatas.values());
}

void FramelessHelperPrivate::updateRubberBandStatus()
{
    QHashIterator<QWidget*, FramelessWidgetData*> iter(m_widgetDatas);
    while (iter.hasNext()) {
        iter.next();
        iter.value()->updateRubberBandStatus();
    }
}

FramelessHelper::FramelessHelper(QObject* parent) :
    QObject(parent),
    d_ptr(new FramelessHelperPrivate)
{
    d_ptr->q_ptr = this;
}

FramelessHelper::~FramelessHelper()
{
    delete d_ptr;
}

bool FramelessHelper::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(FramelessHelper);

    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::Leave:
        case QEvent::HoverMove: {
            FramelessWidgetData* data = d->m_widgetDatas.value(static_cast<QWidget*>(watched));
            if (data) {
                data->handleWidgetEvent(event);
            }
            break;
        }

        default:
            break;
    }

    return false;
}

int FramelessHelper::borderWidth() const
{
    return FramelessCursorPosCalculator::m_borderWidth;
}

void FramelessHelper::setBorderWidth(int borderWidth)
{
    if (borderWidth >= 0) {
        FramelessCursorPosCalculator::m_borderWidth = borderWidth;
    }
}

bool FramelessHelper::isMovable() const
{
    Q_D(const FramelessHelper);
    return d->m_movable;
}

void FramelessHelper::setMovable(bool movable)
{
    Q_D(FramelessHelper);
    d->m_movable = movable;
}

bool FramelessHelper::isResizable() const
{
    Q_D(const FramelessHelper);
    return d->m_resizable;
}

void FramelessHelper::setResizable(bool resizable)
{
    Q_D(FramelessHelper);
    d->m_resizable = resizable;
}

bool FramelessHelper::useRubberBandOnMove() const
{
    Q_D(const FramelessHelper);
    return d->m_useRubberBandOnMove;
}

void FramelessHelper::setUseRubberBandOnMove(bool useRubberBandOnMove)
{
    Q_D(FramelessHelper);

    d->m_useRubberBandOnMove = useRubberBandOnMove;
    d->updateRubberBandStatus();
}

bool FramelessHelper::useRubberBandOnResisze() const
{
    Q_D(const FramelessHelper);
    return d->m_useRubberBandOnResize;
}

void FramelessHelper::setUseRubberBandOnResize(bool useRubberBandOnResize)
{
    Q_D(FramelessHelper);

    d->m_useRubberBandOnResize = useRubberBandOnResize;
    d->updateRubberBandStatus();
}

void FramelessHelper::activate(QWidget* widget)
{
    Q_D(FramelessHelper);

    if (d->m_widgetDatas.contains(widget))
        return;

    FramelessWidgetData* data = new FramelessWidgetData(d, widget);
    d->m_widgetDatas.insert(widget, data);

    widget->installEventFilter(this);
}

void FramelessHelper::deactivate(QWidget* widget)
{
    Q_D(FramelessHelper);

    FramelessWidgetData* data = d->m_widgetDatas.take(widget);
    if (data) {
        widget->removeEventFilter(this);
        delete data;
    }
}

} // namespace ns
