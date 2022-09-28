#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QList>
#include <QDir>
#include <QStringList>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QPixmap;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL ImageLabel : public QLabel
{
    Q_OBJECT

public:
	explicit ImageLabel(QWidget *parent = 0);
	virtual ~ImageLabel();

	void scaleImage(bool zoomin);
	void rotateImage(bool isClockwise);
	void actualSize();
	void fitSize();
	void setImageDir(const QString& dir, const QString& fileName = QLatin1String(""));
	void nextImage();
	void prevImage();
	double scaleValue() const;
	QPixmap getPixmap() const;
	void setFitWindow(bool fit);
	void resetScaleValue();
	void setFullScreen(bool full);
	int getImageCount(){return m_imageList.size();}
	int currentImageIndex(){return m_imageIndex;}
public Q_SLOTS:

protected:
	virtual void paintEvent(QPaintEvent *event);
//	virtual void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
Q_SIGNALS:
	void requestFullScreen(bool full);
	void scaleFactorChanged(double scaleFactor);
private:
	QPixmap m_pixmap;
	bool m_fitWindow;
	qreal m_angel;
	double m_scaleFactor;
	QString m_imagePath;

	bool leftButtonPressed;
	QPoint dragPos;

	QStringList m_imageList;
	int m_imageIndex;
	QDir m_imageDir;

	bool m_fullScreen;
};

} // namespace ns

#endif // IMAGELABEL_H
