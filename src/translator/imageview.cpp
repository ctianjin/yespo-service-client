#include <QtGui/QPixmap>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDebug>
#include "imageview.h"
#include "ui_imageview.h"

#define MARGIN 20

namespace ns {

ImageView::ImageView(QWidget* parent) :
    QDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_ImageView)
{
    m_ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground); 
	setAttribute(Qt::WA_Hover);

	installEventFilter(this);

/*	m_ui->scrollArea->viewport()->setBackgroundRole(QPalette::Window);*/
	
	normalBackground();

	createImageControl();

	m_pushButtonNextExtract = new QPushButton(this);
	m_pushButtonNextExtract->resize(80,80);
	m_pushButtonNextExtract->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_next_extract.png);}QPushButton:hover{border-image: url(:/images/ui/iv_next_extract_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_next_extract_pressed.png);}"));
	connect(m_pushButtonNextExtract, SIGNAL(clicked()), this, SLOT(slotNextImage()));
	m_pushButtonNextExtract->setCursor(QCursor(Qt::PointingHandCursor));

	m_pushButtonPrevExtract = new QPushButton(this);
	m_pushButtonPrevExtract->resize(80,80);
	m_pushButtonPrevExtract->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_prev_extract.png);}QPushButton:hover{border-image: url(:/images/ui/iv_prev_extract_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_prev_extract_pressed.png);}"));
	connect(m_pushButtonPrevExtract, SIGNAL(clicked()), this, SLOT(slotPrevImage()));
	m_pushButtonPrevExtract->setCursor(QCursor(Qt::PointingHandCursor));

	connect(m_ui->labelImage, SIGNAL(requestFullScreen(bool)), this, SLOT(slotFullScreen(bool)));
	connect(m_ui->labelImage, SIGNAL(scaleFactorChanged(double)), this, SLOT(slotScaleFactorChanged(double)));

	m_pushButtonNextExtract->hide();
	m_pushButtonPrevExtract->hide();

	m_ui->pushButtonNormal->hide();
 
// 	m_ui->widgetImageControl->hide();

	hide();
}

ImageView::~ImageView()
{
	
}

void ImageView::setImageDir(const QString& path, const QString& fileName)
{
	m_ui->labelImage->setImageDir(path, fileName);
}

void ImageView::createImageControl()
{
	m_widgetImageControl = new QWidget(this);
	m_widgetImageControl->setMinimumSize(QSize(0, 40));
	m_widgetImageControl->setMaximumSize(QSize(16777215, 40));
	m_widgetImageControl->setGeometry(0, height() - m_widgetImageControl->height() - 1, width(), m_widgetImageControl->height() - 1);

	m_widgetImageControl->setObjectName(QLatin1String("widgetImageControl"));
	m_widgetImageControl->setStyleSheet(QLatin1String("QWidget#widgetImageControl{background: rgba(255, 255, 255, 80%);border-left: 1px solid #4E4945; border-right: 1px solid #4E4945;border-bottom-left-radius: 4px;border-bottom-right-radius: 4px;}"));

	m_widgetImageControl->setContentsMargins(1,0,1,1);

	m_opacityEffect = new QGraphicsOpacityEffect(m_widgetImageControl);
	m_widgetImageControl->setGraphicsEffect(m_opacityEffect);
	m_opacityEffect->setOpacity(0);

// 	m_animationShow = new QPropertyAnimation(m_opacityEffect, "opacity", this);
// 	m_animationShow->setDuration(1000);
// 	m_animationShow->setStartValue(0);
// 	m_animationShow->setEndValue(1);

	m_animationHide = new QPropertyAnimation(m_opacityEffect, "opacity");
	m_animationHide->setDuration(2000);
	m_animationHide->setStartValue(1);
	m_animationHide->setEndValue(0);

// 	connect(m_animationShow, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),
// 		this, SLOT(slotAnimationShowStateChanged(QAbstractAnimation::State,QAbstractAnimation::State))
	connect(m_animationHide, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),
		this, SLOT(slotAnimationHideStateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));

	QHBoxLayout* layout = new QHBoxLayout(m_widgetImageControl);
	QSpacerItem* spacerLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout->addItem(spacerLeft);

	m_pushButtonActual = new QPushButton(m_widgetImageControl);
	m_pushButtonActual->setMinimumSize(QSize(30, 30));
	m_pushButtonActual->setMaximumSize(QSize(30, 30));
	m_pushButtonActual->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_actual.png);}QPushButton:hover{border-image: url(:/images/ui/iv_actual_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_actual_pressed.png);}"));
	connect(m_pushButtonActual, SIGNAL(clicked()), this, SLOT(slotActual()));
	
	layout->addWidget(m_pushButtonActual);

	m_pushButtonFit = new QPushButton(m_widgetImageControl);
	m_pushButtonFit->setMinimumSize(QSize(30, 30));
	m_pushButtonFit->setMaximumSize(QSize(30, 30));
	m_pushButtonFit->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_fit.png);}QPushButton:hover{border-image: url(:/images/ui/iv_fit_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_fit_pressed.png);}"));
	connect(m_pushButtonFit, SIGNAL(clicked()), this, SLOT(slotFit()));
	
	layout->addWidget(m_pushButtonFit);

	m_pushButtonZoomin = new QPushButton(m_widgetImageControl);
	m_pushButtonZoomin->setMinimumSize(QSize(30, 30));
	m_pushButtonZoomin->setMaximumSize(QSize(30, 30));
	m_pushButtonZoomin->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_zoomin.png);}QPushButton:hover{border-image: url(:/images/ui/iv_zoomin_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_zoomin_pressed.png);}"));
	connect(m_pushButtonZoomin, SIGNAL(clicked()), this, SLOT(slotZoomin()));
	
	layout->addWidget(m_pushButtonZoomin);

	m_pushButtonZoomout = new QPushButton(m_widgetImageControl);
	m_pushButtonZoomout->setMinimumSize(QSize(30, 30));
	m_pushButtonZoomout->setMaximumSize(QSize(30, 30));
	m_pushButtonZoomout->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_zoomout.png);}QPushButton:hover{border-image: url(:/images/ui/iv_zoomout_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_zoomout_pressed.png);}"));
	connect(m_pushButtonZoomout, SIGNAL(clicked()), this, SLOT(slotZoomout()));
	
	layout->addWidget(m_pushButtonZoomout);

	m_pushButtonPrev = new QPushButton(m_widgetImageControl);
	m_pushButtonPrev->setMinimumSize(QSize(30, 30));
	m_pushButtonPrev->setMaximumSize(QSize(30, 30));
	m_pushButtonPrev->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_prev.png);}QPushButton:hover{border-image: url(:/images/ui/iv_prev_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_prev_pressed.png);}"));
	connect(m_pushButtonPrev, SIGNAL(clicked()), this, SLOT(slotPrevImage()));
	
	layout->addWidget(m_pushButtonPrev);

	m_indexLabel = new QLabel(m_widgetImageControl);
	m_indexLabel->setMinimumSize(QSize(50,30));
	m_indexLabel->setMaximumSize(QSize(50,30));
	m_indexLabel->setAlignment(Qt::AlignCenter);

	layout->addWidget(m_indexLabel);

	m_pushButtonNext = new QPushButton(m_widgetImageControl);
	m_pushButtonNext->setMinimumSize(QSize(30, 30));
	m_pushButtonNext->setMaximumSize(QSize(30, 30));
	m_pushButtonNext->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_next.png);}QPushButton:hover{border-image: url(:/images/ui/iv_next_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_next_pressed.png);}"));
	connect(m_pushButtonNext, SIGNAL(clicked()), this, SLOT(slotNextImage()));

	layout->addWidget(m_pushButtonNext);

	m_pushButtonCounterclockwiseRotate = new QPushButton(m_widgetImageControl);
	m_pushButtonCounterclockwiseRotate->setMinimumSize(QSize(30, 30));
	m_pushButtonCounterclockwiseRotate->setMaximumSize(QSize(30, 30));
	m_pushButtonCounterclockwiseRotate->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_leftangel.png);}QPushButton:hover{border-image: url(:/images/ui/iv_leftangel_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_leftangel_pressed.png);}"));
	connect(m_pushButtonCounterclockwiseRotate, SIGNAL(clicked()), this, SLOT(slotCounterClockRotate()));
	
	layout->addWidget(m_pushButtonCounterclockwiseRotate);

	m_pushButtonClockwiseRotate = new QPushButton(m_widgetImageControl);
	m_pushButtonClockwiseRotate->setMinimumSize(QSize(30, 30));
	m_pushButtonClockwiseRotate->setMaximumSize(QSize(30, 30));
	m_pushButtonClockwiseRotate->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/iv_rightangel.png);}QPushButton:hover{border-image: url(:/images/ui/iv_rightangel_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/iv_rightangel_pressed.png);}"));
	connect(m_pushButtonClockwiseRotate, SIGNAL(clicked()), this, SLOT(slotClockRotate()));
	
	layout->addWidget(m_pushButtonClockwiseRotate);

	m_labelResource = new QLabel(m_widgetImageControl);
	m_labelResource->setMinimumSize(100, 35);
	m_labelResource->setMaximumSize(100,35);

	layout->addWidget(m_labelResource);

	QSpacerItem* spacerRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	layout->addItem(spacerRight);
/*	m_widgetImageControl->hide();*/
}

void ImageView::slotSetImageDir(const QString& path, const QString& fileName)
{
	setImageDir(path, fileName);
	//setImageDir(QLatin1String("D:\\123"), QString());
	if (isMinimized()) {
		showNormal();
	}
	if (isHidden()) {
		show();
	}
// 	raise();
// 	activateWindow();
	slotFit();
	m_indexLabel->setText(tr("%1/%2").arg(m_ui->labelImage->currentImageIndex() + 1).arg(m_ui->labelImage->getImageCount()));
}

void ImageView::slotNextImage()
{
	m_ui->labelImage->resetScaleValue();
	m_ui->labelImage->nextImage();
	slotFit();
	m_indexLabel->setText(tr("%1/%2").arg(m_ui->labelImage->currentImageIndex() + 1).arg(m_ui->labelImage->getImageCount()));
}

void ImageView::slotPrevImage()
{
	m_ui->labelImage->resetScaleValue();
	m_ui->labelImage->prevImage();
	slotFit();
	m_indexLabel->setText(tr("%1/%2").arg(m_ui->labelImage->currentImageIndex() + 1).arg(m_ui->labelImage->getImageCount()));
}

void ImageView::slotAnimationShowStateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
	Q_UNUSED(oldState);
	if (newState != QAbstractAnimation::Running) {
		m_widgetImageControl->show();
	}
}

void ImageView::slotAnimationHideStateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
	Q_UNUSED(oldState);
	if (newState != QAbstractAnimation::Running) {
		m_widgetImageControl->hide();
	}
}

void ImageView::normalBackground()
{
	QPalette pal;
	const QColor color(240, 240, 240);
	pal.setColor(QPalette::Window, color);
	m_ui->scrollArea->setPalette(pal);
	m_ui->scrollAreaWidgetContents->setPalette(pal);
}

void ImageView::fullScreenBackground()
{
	QPalette pal;
	const QColor color(52, 51, 50);
	pal.setColor(QPalette::Window, color);
	m_ui->scrollArea->setPalette(pal);
	m_ui->scrollAreaWidgetContents->setPalette(pal);
}

bool ImageView::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* ev = static_cast<QKeyEvent*> (event);
		if (ev->key() == Qt::Key_Escape) {
			slotFullScreen(false);
			return true;
		}
	}
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* ev = static_cast<QMouseEvent*> (event);
		dragPos = ev->globalPos();
		if (m_ui->frameCaption->rect().contains(ev->pos()))
			leftButtonPressed = true;
	}
	if (event->type() == QEvent::MouseMove) {
		QMouseEvent* ev = static_cast<QMouseEvent*> (event);
		if( ev->buttons().testFlag(Qt::LeftButton) && leftButtonPressed ) {
			move(pos() + (ev->globalPos() - dragPos));
			dragPos = ev->globalPos();
		}
	}
	if (event->type() == QEvent::MouseButtonRelease) {
		leftButtonPressed = false;
	}
	if (event->type() == QEvent::HoverMove) {
		QHoverEvent* ev = static_cast<QHoverEvent*> (event);
		if (ev->pos().x() >= width() - 80) {
			m_pushButtonNextExtract->setGeometry(m_ui->widgetInner->width() - 85, m_ui->scrollArea->rect().height() / 2, 80, 80);
			m_pushButtonNextExtract->show();
		} else {
			m_pushButtonNextExtract->hide();
		}
		
		if (ev->pos().x() <= 80) {
			m_pushButtonPrevExtract->setGeometry(5, m_ui->scrollArea->rect().height() / 2, 80, 80);
			m_pushButtonPrevExtract->show();
		} else {
			m_pushButtonPrevExtract->hide();
		}

		if (m_widgetImageControl) {
			if (ev->pos().y() >= height() - m_widgetImageControl->height() * 2) {
				if (!m_widgetImageControl->isVisible()) {
					m_widgetImageControl->show();
				} else {
					if (m_opacityEffect && m_opacityEffect->opacity() < 1) {
						if(m_animationHide && m_animationHide->state() != QAbstractAnimation::Stopped) {
 							m_animationHide->pause();
						}
						m_opacityEffect->setOpacity(1);
					}
				}
			} else {
				if (m_widgetImageControl->isVisible()) {
					if (m_animationHide) {
						m_animationHide->start();
					}
				}
			}
		}
	}

	if (event->type() == QEvent::HoverLeave) {
		if (m_pushButtonPrevExtract)
			m_pushButtonPrevExtract->hide();
		if (m_pushButtonNextExtract)
			m_pushButtonNextExtract->hide();
	}

	return QDialog::eventFilter(obj, event);
}

void ImageView::resizeEvent(QResizeEvent *event)
{
	QDialog::resizeEvent(event);

	m_pushButtonNextExtract->hide();
	m_pushButtonPrevExtract->hide();
	int left, top,w,h;

	if (!m_ui->labelImage->getPixmap().isNull()) {
		m_ui->labelImage->setFitWindow(true);
		QRect childRect = m_ui->scrollArea->rect();
		h = m_ui->labelImage->getPixmap().height() > childRect.height()?childRect.height():m_ui->labelImage->getPixmap().height();
		w = h * m_ui->labelImage->getPixmap().width() / m_ui->labelImage->getPixmap().height();
		m_ui->labelImage->resize(w, h);
		top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, w, h);
	}

	m_ui->labelImage->resetScaleValue();

	if (m_widgetImageControl)
		m_widgetImageControl->setGeometry(0, height() - m_widgetImageControl->height() - 1, width(), m_widgetImageControl->height() - 1);

	m_pushButtonFit->hide();
	m_pushButtonActual->show();
}

void ImageView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if (event->orientation() == Qt::Vertical) {
		m_ui->labelImage->setFitWindow(true);
		if (event->delta() > 0) {
			if (m_ui->labelImage->scaleValue() > 3)
				return;
			m_ui->labelImage->scaleImage(true);
			m_ui->labelImage->resize( m_ui->labelImage->size() * 1.2);
		} else {
			if (m_ui->labelImage->scaleValue() < 0.05)
				return;
			m_ui->labelImage->scaleImage(false);
			m_ui->labelImage->resize( m_ui->labelImage->size() / 1.2);
		}
		QRect childRect = m_ui->scrollArea->rect();
		int top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		int left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, m_ui->labelImage->width(), m_ui->labelImage->height());
	} 
	event->accept();
}

void ImageView::slotZoomin()
{
	if (m_ui->labelImage->scaleValue() > 3)
		return;
	m_ui->labelImage->setFitWindow(true);
	m_ui->labelImage->scaleImage(true);
	m_ui->labelImage->resize( m_ui->labelImage->size() * 1.2);

	QRect childRect = m_ui->scrollArea->rect();
	int top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
	int left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
	m_ui->labelImage->setGeometry(left, top, m_ui->labelImage->width(), m_ui->labelImage->height());
}

void ImageView::slotZoomout()
{
	if (m_ui->labelImage->scaleValue() < 0.05)
		return;
	m_ui->labelImage->setFitWindow(true);
	m_ui->labelImage->scaleImage(false);
	m_ui->labelImage->resize( m_ui->labelImage->size() / 1.2);

	QRect childRect = m_ui->scrollArea->rect();
	int top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
	int left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
	m_ui->labelImage->setGeometry(left, top, m_ui->labelImage->width(), m_ui->labelImage->height());
}

void ImageView::slotPrev()
{
	m_ui->labelImage->resetScaleValue();
	m_ui->labelImage->prevImage();

	int left, top,w,h;

	if (!m_ui->labelImage->getPixmap().isNull()) {
		m_ui->labelImage->setFitWindow(true);
		QRect childRect = m_ui->scrollArea->rect();
		h = m_ui->labelImage->getPixmap().height() > childRect.height()?childRect.height():m_ui->labelImage->getPixmap().height();
		w = h * m_ui->labelImage->getPixmap().width() / m_ui->labelImage->getPixmap().height();
		m_ui->labelImage->resize(w, h);
		top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, w, h);
	}
	m_indexLabel->setText(tr("%1/%2").arg(m_ui->labelImage->currentImageIndex() + 1).arg(m_ui->labelImage->getImageCount()));
	m_ui->labelImage->fitSize();
}

void ImageView::slotNext()
{
	m_ui->labelImage->resetScaleValue();
	m_ui->labelImage->nextImage();

	int left, top,w,h;

	if (!m_ui->labelImage->getPixmap().isNull()) {
		m_ui->labelImage->setFitWindow(true);
		QRect childRect = m_ui->scrollArea->rect();
		h = m_ui->labelImage->getPixmap().height() > childRect.height()?childRect.height():m_ui->labelImage->getPixmap().height();
		w = h * m_ui->labelImage->getPixmap().width() / m_ui->labelImage->getPixmap().height();
		m_ui->labelImage->resize(w, h);
		top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, w, h);
	}
	m_indexLabel->setText(tr("%1/%2").arg(m_ui->labelImage->currentImageIndex() + 1).arg(m_ui->labelImage->getImageCount()));
	m_ui->labelImage->fitSize();
}

void ImageView::slotCounterClockRotate()
{
	m_ui->labelImage->setFitWindow(true);

	QRect childRect = m_ui->scrollArea->rect();
	if (m_ui->labelImage->getPixmap().width() < childRect.width()) {
		if (m_ui->labelImage->width() > m_ui->labelImage->height()) {
			m_ui->labelImage->resize(m_ui->labelImage->getPixmap().width(),m_ui->labelImage->getPixmap().height());
			m_ui->labelImage->setGeometry((childRect.width() - m_ui->labelImage->height()) / 2, (childRect.height() - m_ui->labelImage->width()) / 2,
				m_ui->labelImage->height(), m_ui->labelImage->width());
		} else {
			m_ui->labelImage->resize(m_ui->labelImage->getPixmap().height(),m_ui->labelImage->getPixmap().width());
			m_ui->labelImage->setGeometry((childRect.width() - m_ui->labelImage->width()) / 2, (childRect.height() - m_ui->labelImage->height()) / 2,
				m_ui->labelImage->width(), m_ui->labelImage->height());
		}
	} else {
		m_ui->labelImage->resize(childRect.size());
	}

	m_ui->labelImage->rotateImage(false);
	int left, top,w,h;

	if (!m_ui->labelImage->getPixmap().isNull()) {
		m_ui->labelImage->setFitWindow(true);
		QRect childRect = m_ui->scrollArea->rect();
		h = m_ui->labelImage->getPixmap().height() > childRect.height()?childRect.height():m_ui->labelImage->getPixmap().height();
		w = h * m_ui->labelImage->getPixmap().width() / m_ui->labelImage->getPixmap().height();
		m_ui->labelImage->resize(w, h);
		top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, w, h);
	}
/*	slotFit();*/
}

void ImageView::slotClockRotate()
{
	m_ui->labelImage->setFitWindow(true);

	QRect childRect = m_ui->scrollArea->rect();
	if (m_ui->labelImage->getPixmap().width() < childRect.width()) {
		if (m_ui->labelImage->width() > m_ui->labelImage->height()) {
			m_ui->labelImage->resize(m_ui->labelImage->getPixmap().width(),m_ui->labelImage->getPixmap().height());
			m_ui->labelImage->setGeometry((childRect.width() - m_ui->labelImage->height()) / 2, (childRect.height() - m_ui->labelImage->width()) / 2,
				m_ui->labelImage->height(), m_ui->labelImage->width());
		} else {
			m_ui->labelImage->resize(m_ui->labelImage->getPixmap().height(),m_ui->labelImage->getPixmap().width());
			m_ui->labelImage->setGeometry((childRect.width() - m_ui->labelImage->width()) / 2, (childRect.height() - m_ui->labelImage->height()) / 2,
				m_ui->labelImage->width(), m_ui->labelImage->height());
		}
	} else {
		m_ui->labelImage->resize(childRect.size());
	}

	m_ui->labelImage->rotateImage(true);

	int left, top,w,h;

	if (!m_ui->labelImage->getPixmap().isNull()) {
		m_ui->labelImage->setFitWindow(true);
		QRect childRect = m_ui->scrollArea->rect();
		h = m_ui->labelImage->getPixmap().height() > childRect.height()?childRect.height():m_ui->labelImage->getPixmap().height();
		w = h * m_ui->labelImage->getPixmap().width() / m_ui->labelImage->getPixmap().height();
		m_ui->labelImage->resize(w, h);
		top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, w, h);
	}
/*	slotFit();*/
}

void ImageView::slotActual()
{
	m_pushButtonActual->hide();
	m_pushButtonFit->show();

	m_ui->labelImage->resize(m_ui->labelImage->getPixmap().size());
	m_ui->labelImage->actualSize();

	QRect childRect = m_ui->scrollArea->rect();
	int top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
	int left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
	m_ui->labelImage->setGeometry(left, top, m_ui->labelImage->width(), m_ui->labelImage->height());
}

void ImageView::slotFit()
{
	m_pushButtonFit->hide();
	m_pushButtonActual->show();

	int left, top,w,h;

	if (!m_ui->labelImage->getPixmap().isNull()) {
		m_ui->labelImage->setFitWindow(true);
		QRect childRect = m_ui->scrollArea->rect();
		h = m_ui->labelImage->getPixmap().height() > childRect.height()?childRect.height():m_ui->labelImage->getPixmap().height();
		w = h * m_ui->labelImage->getPixmap().width() / m_ui->labelImage->getPixmap().height();
		m_ui->labelImage->resize(w, h);
		top = m_ui->labelImage->height() > childRect.height()?0:(childRect.height() - m_ui->labelImage->height())/2;
		left = m_ui->labelImage->width() > childRect.width()?0:(childRect.width() - m_ui->labelImage->width()) / 2;
		m_ui->labelImage->setGeometry(left, top, w, h);
	}
	m_ui->labelImage->fitSize();
}

void ImageView::slotFullScreen(bool full)
{
	if (full) {
		m_ui->labelImage->setFullScreen(true);
		m_ui->frameCaption->hide();
		showFullScreen();
		m_ui->pushButtonNormal->show();
		m_ui->pushButtonMaxmize->hide();
		fullScreenBackground();
	} else {
		m_ui->labelImage->setFullScreen(false);
		m_ui->frameCaption->show();
		showNormal();
		m_ui->pushButtonNormal->hide();
		m_ui->pushButtonMaxmize->show();
		normalBackground();
	}
}

void ImageView::slotScaleFactorChanged(double scaleFactor)
{
	if (m_labelResource) {
		m_labelResource->setText(QString(QLatin1String("%1%")).arg((int)(scaleFactor * 100)));
	}
}

void ImageView::on_pushButtonMaxmize_clicked()
{
	showMaximized();
	m_ui->pushButtonMaxmize->setVisible(!isMaximized());
	m_ui->pushButtonNormal->setVisible(isMaximized());
}

void ImageView::on_pushButtonNormal_clicked()
{
	showNormal();
	m_ui->pushButtonMaxmize->setVisible(!isMaximized());
	m_ui->pushButtonNormal->setVisible(isMaximized());
}

} // namespace ns
