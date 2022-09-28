#ifndef CUSTOMLOADINGLABEL_H
#define CUSTOMLOADINGLABEL_H

#include <QtGui/QLabel>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QTimer;
class QPixmap;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL CustomLoadingLabel : public QLabel
{
    Q_OBJECT
	Q_PROPERTY(int delay READ animationDelay WRITE setAnimationDelay)
	Q_PROPERTY(bool displayedWhenStopped READ isDisplayedWhenStopped WRITE setDisplayedWhenStopped)
	Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit CustomLoadingLabel(QWidget* parent = 0);
    virtual ~CustomLoadingLabel();

	void uninit();

	QString largePhotolink() const;
	void setLargePhotolink(const QString& link);

	void setSmallPhotoLink(const QString& link);
	QString smallPhotoLink() const;

	QString localFile() const;
	void setLocalFile(const QString& localFile);

	void setPhotoId(const QString& id);
	QString photoId() const;

    int animationDelay() const { return m_delay; }

	void setLocalDir(const QString& localDir);

    bool isAnimated () const;

    bool isDisplayedWhenStopped() const;

    const QColor & color() const { return m_color; }

    virtual QSize sizeHint() const;
    int heightForWidth(int w) const;
	void setSmallPhoto(QPixmap pic);
	void setLoadedBytes(qint64 bytes);
	void setLoadTotalLength(qint64 length);

Q_SIGNALS:
	void signalClicked(const QString& path, const QString& fileName);

public Q_SLOTS:
    void startAnimation();

    void stopAnimation();

    void setAnimationDelay(int delay);

    void setDisplayedWhenStopped(bool state);

    void setColor(const QColor & color);

protected:
    virtual void timerEvent(QTimerEvent * event); 
    virtual void paintEvent(QPaintEvent * event);
	virtual void mousePressEvent(QMouseEvent *event);
private:
	QString m_localFile;
	QString m_largePhotoLink;
	QString m_smallPhotoLink;
	QString m_photoId;
	qint64 m_totalLength;
	qint64 m_bytes;
	QPixmap m_pic;
    int m_angle;
    int m_timerId;
    int m_delay;
	int m_progress;
    bool m_displayedWhenStopped;
    QColor m_color;
	QString m_localDir;

    Q_DISABLE_COPY(CustomLoadingLabel)
};

} // namespace ns

#endif // CUSTOMLOADINGLABEL_H
