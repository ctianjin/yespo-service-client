#include <QToolButton>
#include <QRect>
#include <QSize>
#include "searchedit.h"

namespace Qpid {

SearchEdit::SearchEdit(QWidget* parent) :
    SmartEdit(parent),
	m_searchButton(0)
{
	QRect rc = geometry();

	m_iconSize = new QSize(16, 16);
	int top = (rc.height()-m_iconSize->height()) / 2;
	int paddingLeft = top;

	m_searchButton = new QToolButton(this);
	if (m_searchButton) {
		m_searchButton->setStyleSheet(QLatin1String("border-image: url(:/images/input_search.png)"));
		m_searchButton->setCursor(Qt::ArrowCursor);
		m_searchButton->setFocusPolicy(Qt::NoFocus);
		m_searchButton->setGeometry(paddingLeft, top, m_iconSize->width(), m_iconSize->height());
		connect(m_searchButton, SIGNAL(clicked()), this, SLOT(slotSearchClicked()));
	}	
}

SearchEdit::~SearchEdit()
{
	
}

void SearchEdit::slotSearchClicked()
{
	setFocus(Qt::MouseFocusReason);
}


} // namespace Qpid
