#include <QDebug>
#include <QKeyEvent>
#include "phoneapplication.h"
#include "dtmfplate.h"
#include "ui_dtmfplate.h"

namespace Qpid {

DTMFPlate::DTMFPlate(QWidget* parent) :
	QWidget(parent, Qt::FramelessWindowHint),
    m_ui(new Ui_DTMFPlate)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);

	connect(m_ui->pushButtonDTMF1, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF2, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF3, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF4, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF5, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF6, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF7, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF8, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF9, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMFAst, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMF0, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
	connect(m_ui->pushButtonDTMFPound, SIGNAL(clicked()), this, SLOT(slotDTMFClicked()));
}

DTMFPlate::~DTMFPlate()
{
}

void DTMFPlate::setDTMFSize(const QSize& size)
{
	m_ui->pushButtonDTMF1->setMinimumSize(size);
	m_ui->pushButtonDTMF2->setMinimumSize(size);
	m_ui->pushButtonDTMF3->setMinimumSize(size);
	m_ui->pushButtonDTMF4->setMinimumSize(size);
	m_ui->pushButtonDTMF5->setMinimumSize(size);
	m_ui->pushButtonDTMF6->setMinimumSize(size);
	m_ui->pushButtonDTMF7->setMinimumSize(size);
	m_ui->pushButtonDTMF8->setMinimumSize(size);
	m_ui->pushButtonDTMF9->setMinimumSize(size);
	m_ui->pushButtonDTMFAst->setMinimumSize(size);
	m_ui->pushButtonDTMF0->setMinimumSize(size);
	m_ui->pushButtonDTMFPound->setMinimumSize(size);

	adjustSize();
}

void DTMFPlate::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void DTMFPlate::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
		return;

	QWidget::keyPressEvent(event);
}

void DTMFPlate::slotDTMFClicked()
{
	QChar DTMFDigit;
	QPushButton* buttonClicked = qobject_cast<QPushButton*>(sender());
	if (buttonClicked==m_ui->pushButtonDTMF1) {
		DTMFDigit = QLatin1Char('1');
	} else if(buttonClicked==m_ui->pushButtonDTMF2) {
		DTMFDigit = QLatin1Char('2');
	} else if(buttonClicked==m_ui->pushButtonDTMF3) {
		DTMFDigit = QLatin1Char('3');
	} else if(buttonClicked==m_ui->pushButtonDTMF4) {
		DTMFDigit = QLatin1Char('4');
	} else if(buttonClicked==m_ui->pushButtonDTMF5) {
		DTMFDigit = QLatin1Char('5');
	} else if(buttonClicked==m_ui->pushButtonDTMF6) {
		DTMFDigit = QLatin1Char('6');
	} else if(buttonClicked==m_ui->pushButtonDTMF7) {
		DTMFDigit = QLatin1Char('7');
	} else if(buttonClicked==m_ui->pushButtonDTMF8) {
		DTMFDigit = QLatin1Char('8');
	} else if(buttonClicked==m_ui->pushButtonDTMF9) {
		DTMFDigit = QLatin1Char('9');
	} else if(buttonClicked==m_ui->pushButtonDTMFAst) {
		DTMFDigit = QLatin1Char('*');
	} else if(buttonClicked==m_ui->pushButtonDTMF0) {
		DTMFDigit = QLatin1Char('0');
	} else if(buttonClicked==m_ui->pushButtonDTMFPound) {
		DTMFDigit = QLatin1Char('#');
	}

	Q_EMIT DTMFClicked(DTMFDigit);
}

} // namespace Qpid
