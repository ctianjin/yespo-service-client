#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QScopedPointer>
#include <QString>
#include <QIcon>
#include <QAbstractAnimation>
#include "framelessdialog.h"

QT_BEGIN_MOC_NAMESPACE
class QPropertyAnimation;
class QGraphicsOpacityEffect;
class QLabel;
QT_END_NAMESPACE;

namespace ns {

class Ui_ImageView;

class ImageView : public QDialog
{
    Q_OBJECT

public:
    explicit ImageView(QWidget* parent = 0);
    virtual ~ImageView();

	void setImageDir(const QString& path, const QString& fileName);
	void createImageControl();
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

public Q_SLOTS:
	void slotSetImageDir(const QString& path, const QString& fileName);

protected Q_SLOTS:
	void slotZoomin();
	void slotZoomout();
	void slotPrev();
	void slotNext();
	void slotCounterClockRotate();
	void slotClockRotate();
	void slotActual();
	void slotFit();
	void slotFullScreen(bool full);
	void slotScaleFactorChanged(double scaleFactor);
	void on_pushButtonMaxmize_clicked();
	void on_pushButtonNormal_clicked();
	void slotNextImage();
	void slotPrevImage();
	void slotAnimationShowStateChanged(QAbstractAnimation::State newState,
		QAbstractAnimation::State oldState);
	void slotAnimationHideStateChanged(QAbstractAnimation::State newState,
		QAbstractAnimation::State oldState);
	

protected:

private:
	void normalBackground();
	void fullScreenBackground();
	QWidget* m_widgetImageControl;
	QGraphicsOpacityEffect* m_opacityEffect;
/*	QPropertyAnimation* m_animationShow;*/
	QPropertyAnimation* m_animationHide;
	bool leftButtonPressed;
	QPoint dragPos;

	QPushButton* m_pushButtonPrevExtract;
	QPushButton* m_pushButtonNextExtract;
	
	QPushButton* m_pushButtonZoomin;
	QPushButton* m_pushButtonZoomout;
	QPushButton* m_pushButtonPrev;
	QPushButton* m_pushButtonNext;
	QPushButton* m_pushButtonCounterclockwiseRotate;
	QPushButton* m_pushButtonClockwiseRotate;
	QPushButton* m_pushButtonActual;
	QPushButton* m_pushButtonFit;
	QLabel* m_labelResource;
	QLabel* m_indexLabel;

    QScopedPointer<Ui_ImageView> m_ui;

    Q_DISABLE_COPY(ImageView)
};

} // namespace ns

#endif // IMAGEVIEW_H
