/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst::Gui module of the QsT SQL Tools.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: http://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/licenses/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** author (graninas@gmail.com).
**
****************************************************************************/

#ifndef FILTERGUIITEM_H
#define FILTERGUIITEM_H

#include <QObject>

#include <QVariant>
#include <QList>

#include <QLayout>
#include <QWidget>
#include <QToolButton>
#include <QCheckBox>

#include "filter.h"
#include "qst/qstabstractmodelhandler.h"

namespace Qst
{
namespace Gui
{

typedef QList<QWidget*> ItemsList;

class FilterGuiItem : public QObject
{

	Q_OBJECT

private:

	Filter _filter;

	QWidget   *_inputWidget;
	QCheckBox *_checkBox;
	QLayout   *_filterItemsLayout;

	QstAbstractModelHandler *_bindedHandler;

	ItemsList _itemsList;

	bool    _enabled;

public:
	FilterGuiItem();
	FilterGuiItem(const Filter &filter);
	~FilterGuiItem();

	void setInputWidget(QWidget *inputWidget);

	void		setCheckBox(QCheckBox *checkBox);
	QCheckBox	*checkBox();

	void	setFilterItemsLayout(QLayout *layout);
	QLayout	*filterItemsLayout();

	QVariant value() const;
	QueryField queryField() const;

	Filter filter() const;

	void bindHandler(Qst::QstAbstractModelHandler *handler);

	void clear();

public slots:

	void equalButtonStateChanged(const bool &checked);
	void setEnabled(const bool &enabled);

private:

	void _detachHandlerView();
};

} // End of namespace Gui
} // End of namespace Qst

#endif // FILTERGUIITEM_H
