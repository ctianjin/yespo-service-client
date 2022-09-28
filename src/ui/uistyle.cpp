#ifdef BUILDSYS_UI_USE_QTCURVE
#include <QtGui/QStyleOptionButton>
#endif // BUILDSYS_UI_USE_QTCURVE
#ifdef BUILDSYS_UI_USE_CLEANLOOKS
#include <QtGui/QStyleOptionButton>
#endif // BUILDSYS_UI_USE_CLEANLOOKS
#include "uistyle_p.h"

namespace ns {

#ifdef BUILDSYS_UI_USE_QTCURVE
UiStyle::UiStyle()
{
    Options& opts = options();
    opts.tooltipAppearance = APPEARANCE_FLAT;
}

UiStyle::~UiStyle()
{
}

QSize UiStyle::sizeFromContents(ContentsType type, const QStyleOption* option,
    const QSize& contentsSize, const QWidget* widget) const
{
    // Add borders to itemview item
    if (type == CT_ItemViewItem) {
        QSize newSize = QtCurve::Style::sizeFromContents(type, option, contentsSize, widget);
        newSize += QSize(0, 2);
        return newSize;
    }

    return QtCurve::Style::sizeFromContents(type, option, contentsSize, widget);
}

int UiStyle::styleHint(StyleHint hint, const QStyleOption* option,
    const QWidget* widget, QStyleHintReturn* returnData) const
{
    // Disable button box's icon
    if (hint == SH_DialogButtonBox_ButtonsHaveIcons)
        return false;

    return QtCurve::Style::styleHint(hint, option, widget, returnData);
}
#endif // BUILDSYS_UI_USE_QTCURVE

#ifdef BUILDSYS_UI_USE_CLEANLOOKS
UiStyle::UiStyle()
{
}

UiStyle::~UiStyle()
{
}

QSize UiStyle::sizeFromContents(ContentsType type, const QStyleOption* option,
    const QSize& contentsSize, const QWidget* widget) const
{
    // Do not make button in button box too wide and too high
    if (type == CT_PushButton) {
        QSize newSize = QWindowsStyle::sizeFromContents(type, option, contentsSize, widget);
        if (const QStyleOptionButton* btn = qstyleoption_cast<const QStyleOptionButton*>(option)) {
            if (!btn->icon.isNull() && btn->iconSize.height() > 16)
                newSize -= QSize(0, 2);
            newSize += QSize(0, 1);
        }
        return newSize;
    } else {
        return QCleanlooksStyle::sizeFromContents(type, option, contentsSize, widget);
    }
}

int UiStyle::styleHint(StyleHint hint, const QStyleOption* option,
    const QWidget* widget, QStyleHintReturn* returnData) const
{
    // Disable button box's icon
    if (hint == SH_DialogButtonBox_ButtonsHaveIcons)
        return false;

    return QCleanlooksStyle::styleHint(hint, option, widget, returnData);
}
#endif // BUILDSYS_UI_USE_CLEANLOOKS

} // namespace ns
