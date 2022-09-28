#ifndef FRAMELESSHELPER_H
#define FRAMELESSHELPER_H

#include <QtCore/QObject>
#include "uiglobal.h"

namespace ns {

class FramelessHelperPrivate;

class BUILDSYS_UI_DECL FramelessHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(bool movable READ isMovable WRITE setMovable)
    Q_PROPERTY(bool resizable READ isResizable WRITE setResizable)
    Q_PROPERTY(bool useRubberBandOnMove READ useRubberBandOnMove WRITE setUseRubberBandOnMove)
    Q_PROPERTY(bool useRubberBandOnResisze READ useRubberBandOnResisze WRITE setUseRubberBandOnResize)

public:
    explicit FramelessHelper(QObject* parent = 0);
    virtual ~FramelessHelper();

    int borderWidth() const;
    void setBorderWidth(int borderWidth);

    bool isMovable() const;
    void setMovable(bool movable);

    bool isResizable() const;
    void setResizable(bool resizable);

    bool useRubberBandOnMove() const;
    void setUseRubberBandOnMove(bool useRubberBandOnMove);

    bool useRubberBandOnResisze() const;
    void setUseRubberBandOnResize(bool useRubberBandOnResisze);

public Q_SLOTS:
    void activate(QWidget* widget);
    void deactivate(QWidget* widget);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);

private:
    FramelessHelperPrivate* const d_ptr;

    Q_DECLARE_PRIVATE(FramelessHelper)
    Q_DISABLE_COPY(FramelessHelper)
};

} // namespace ns

#endif // FRAMELESSHELPER_H
