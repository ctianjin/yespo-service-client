#ifndef QPID_UISTYLE_P_H
#define QPID_UISTYLE_P_H

// Disable cleanlooks if use qtcurve
#ifdef QPID_UI_USE_QTCURVE
#  ifdef QPID_UI_USE_CLEANLOOKS
#    undef QPID_UI_USE_CLEANLOOKS
#  endif // QPID_UI_USE_CLEANLOOKS
#endif // QPID_UI_USE_QTCURVE

#ifdef QPID_UI_USE_QTCURVE
#include <qtcurve.h>
#endif // QPID_UI_USE_QTCURVE

#ifdef QPID_UI_USE_CLEANLOOKS
#include <QtGui/QCleanlooksStyle>
#endif // QPID_UI_USE_CLEANLOOKS

namespace ns {

#ifdef QPID_UI_USE_QTCURVE
class UiStyle : public QtCurve::Style
{
public:
    UiStyle();
    virtual ~UiStyle();

    virtual QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& contentsSize, const QWidget* widget = 0) const;

    virtual int styleHint(StyleHint hint, const QStyleOption* option = 0,
        const QWidget* widget = 0, QStyleHintReturn* returnData = 0) const;
};
#endif // QPID_UI_USE_QTCURVE

#ifdef QPID_UI_USE_CLEANLOOKS
class UiStyle : public QCleanlooksStyle
{
public:
    UiStyle();
    virtual ~UiStyle();

    virtual QSize sizeFromContents(ContentsType type, const QStyleOption* option,
        const QSize& contentsSize, const QWidget* widget = 0) const;

    virtual int styleHint(StyleHint hint, const QStyleOption* option = 0,
        const QWidget* widget = 0, QStyleHintReturn* returnData = 0) const;
};
#endif // QPID_UI_USE_CLEANLOOKS

} // namespace ns

#endif // QPID_UISTYLE_P_H
