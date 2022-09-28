#include <QtGui/QResizeEvent>
#include <QtGui/QPainter>
#include <QBitmap>
#include <QDebug>
#include "phoneapplication.h"
#include "interpretationselect.h"
#include "ui_interpretationselect.h"

namespace Qpid {

InterpretationSelect::InterpretationSelect(QWidget* parent) :
	QDialog(parent, Qt::FramelessWindowHint),
    m_ui(new Ui_InterpretationSelect)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
}

InterpretationSelect::~InterpretationSelect()
{
}

void InterpretationSelect::initialSourceLanguage()
{
	m_ui->comboBoxSource->clear();
	m_ui->comboBoxSource->addItem(tr("- Select -"), QVariant());

	QStringList sourceLanguages = qApp->supportedSource();
	foreach(const QString& l, sourceLanguages) {
		m_ui->comboBoxSource->addItem(qApp->languageFull(l), l);
	}

	m_ui->widgetHelp->setDisplayText(tr("How call interpretation works?"), 
		tr("Select the languages for interpretation"),
		tr("Call connect to a human-based interpreter"),
		tr("Call connect to the destination number"));
}

void InterpretationSelect::hideHelp()
{
	m_ui->widgetHelp->hide();
	adjustSize();
}

void InterpretationSelect::on_comboBoxSource_currentIndexChanged(int index)
{
	m_ui->comboBoxTarget->clear();
	m_ui->comboBoxTarget->addItem(tr("- Select -"), QVariant());

	QStringList targetLanguages = qApp->supportedTarget(m_ui->comboBoxSource->itemData(index).toString());
	foreach(const QString& l, targetLanguages) {
		m_ui->comboBoxTarget->addItem(qApp->languageFull(l), l);
	}
}

void InterpretationSelect::retranslateTr()
{
	m_ui->widgetHelp->setDisplayText(tr("How call interpretation works?"), 
		tr("Select the languages for interpretation"),
		tr("Call connect to a human-based interpreter"),
		tr("Call connect to the destination number"));

	m_ui->comboBoxSource->setItemText(0, tr("- Select -"));
	m_ui->comboBoxTarget->setItemText(0, tr("- Select -"));
}

void InterpretationSelect::on_pushButtonOK_clicked()
{
	QString source = m_ui->comboBoxSource->itemData(m_ui->comboBoxSource->currentIndex()).toString();
	QString target = m_ui->comboBoxTarget->itemData(m_ui->comboBoxTarget->currentIndex()).toString();
	if (source.isEmpty() || target.isEmpty()) {
		qWarning() << "InterpretationSelect:" << "No source and target language have been selected!";
		return;
	}

	Q_EMIT interpreterAdded(source, target);

	accept();
}

void InterpretationSelect::setVisible(bool visible)
{
    QDialog::setVisible(visible);
	
	if (visible) {
        QWidget* p = parentWidget();
        if (p) {
            QWidget* w = p->window();
            if (w) {
				move(w->x() + (w->width() - width()) / 2, w->y() + (w->height() - height()) / 2 + 50);
            }
        }
    }
}

void InterpretationSelect::changeEvent(QEvent* event)
{
	QDialog::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

void InterpretationSelect::resizeEvent(QResizeEvent* event)
{
	QDialog::resizeEvent(event);

// 	const QSize size = event->size();
// 	QPixmap pixmap(size);
// 	QPainter painter(&pixmap);
// 	painter.fillRect(pixmap.rect(), Qt::white);
// 	painter.setBrush(Qt::black);
// 	painter.drawRoundedRect(0, 0, size.width() - 1, size.height() - 1, 5, 5);
// 	setMask(pixmap.createMaskFromColor(Qt::white));
}

void InterpretationSelect::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
		return;

	QDialog::keyPressEvent(event);
}

} // namespace Qpid
