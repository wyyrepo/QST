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

#include "filterguiitem.h"

#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>

namespace Qst
{
namespace Gui
{

/*!
\class FilterGuiItem
\brief
\lang_Russian
	Служебный класс. В классе хранится список виджетов, участвующих в построении
	GUI для фильтров.
\endlang

\lang_English
	Service class. The class keeps a list of widgets, involved in the construction
	GUI for filters.
\endlang

\sa FiltersManager, Filter
*/

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
FilterGuiItem::FilterGuiItem()
	:
	_inputWidget(NULL),
	_checkBox(NULL),
	_filterItemsLayout(NULL),
	_bindedHandler(NULL),
	_enabled(false)
{
}

/*!
\lang_Russian
	Деструктор по умолчанию.
\endlang

\lang_English
	Default destructor.
\endlang
*/
FilterGuiItem::~FilterGuiItem()
{
	clear();
}

/*!
\lang_Russian
	Главный конструктор.
\endlang

\lang_English
	Main constructor.
\endlang
*/
FilterGuiItem::FilterGuiItem(const Filter &filter)
	:
	_filter(filter),
	_inputWidget(NULL),
	_checkBox(NULL),
	_filterItemsLayout(NULL),
	_bindedHandler(NULL),
	_enabled(false)
{
}

/*!
\lang_Russian
	Устанавливает InputWidget фильтра.
\endlang

\lang_English
	Sets filter's InputWidget.
\endlang
*/
void FilterGuiItem::setInputWidget(QWidget* inputWidget)
{
	Q_ASSERT(inputWidget != NULL);
	_inputWidget = inputWidget;
}

/*!
\lang_Russian
	Устанавливае checkBox фильтра.
\endlang

\lang_English
	Sets checkBox of the filter.
\endlang
*/
void FilterGuiItem::setCheckBox(QCheckBox *checkBox)
{
	Q_ASSERT(checkBox != NULL);
	_checkBox = checkBox;
}

/*!
\lang_Russian
	Возвращает checkBox фильтра.
\endlang

\lang_English
	Returns checkBox of the filter.
\endlang
*/
QCheckBox *FilterGuiItem::checkBox()
{
	return _checkBox;
}

/*!
\lang_Russian
	Устанавливает layout элементов фильтра.
\endlang

\lang_English
	Sets local layout for GUI items of single filter.
\endlang
*/
void FilterGuiItem::setFilterItemsLayout(QLayout *layout)
{
	Q_ASSERT(layout != NULL);
	_filterItemsLayout = layout;
}

/*!
\lang_Russian
	Возвращает layout элементов фильтра.
\endlang

\lang_English
	Returns local layout for GUI items of single filter.
\endlang
*/
QLayout *FilterGuiItem::filterItemsLayout()
{
	return _filterItemsLayout;
}

/*!
\lang_Russian
	Возвращает значение, введенное в виджет.
\endlang

\lang_English
	Returns the value entered in the widget.
\endlang
*/
QVariant FilterGuiItem::value() const
{
	Q_ASSERT(_inputWidget != NULL);

	if (_inputWidget == NULL)
		return QVariant();

	if (!_enabled)
		return QVariant();

	switch (_filter.inputWidgetType())
	{
	case Filter::LineEdit:
		{
			QLineEdit *lineEdit = (QLineEdit*) _inputWidget;
			if (lineEdit->text().isEmpty())
				return QVariant();
			return lineEdit->text();
		}
	case Filter::DateEdit:
		{
			QDateEdit *dateEdit = (QDateEdit*) _inputWidget;
			return dateEdit->date();
		}
	case Filter::ComboBox:
			QComboBox *comboBox = (QComboBox*) _inputWidget;
			return comboBox->currentText();
	}

	Q_ASSERT(false);
return QVariant();
}


QueryField FilterGuiItem::queryField() const
{
	Q_ASSERT(_inputWidget != NULL);
	if (_inputWidget == NULL)
		return QueryField();

	if (!_enabled)
		return QueryField();

	switch (_filter.inputWidgetType())
	{
	case Filter::LineEdit:
	case Filter::DateEdit: return QueryField(_filter.fieldName(),
											 QueryValue(value(), _filter.percentPlaceholders()),
											 _filter.functor(),
											 QString(),
											 ClauseWhere);
	case Filter::ComboBox:

		Q_ASSERT(_bindedHandler != NULL);

		if (_bindedHandler == NULL)
			return QueryField();
		else
			return QueryField(_filter.fieldName(),
							  QueryValue(_bindedHandler->comboBoxKeyValue(), NoPercentPlaceholders),
							  _filter.functor(),
							  QString(),
							  ClauseWhere);
	}
return QueryField();
}

/*!
\lang_Russian
	Возвращает объект класса Filter, на основе которого построен GUI фильтра.
\endlang

\lang_English
	Returns an object of class Filter, based on which is built GUI filter.
\endlang
*/
Filter FilterGuiItem::filter() const
{
	return _filter;
}

/*!
\lang_Russian
	Привязывает хэндлер к фильтру типа QComboBox.
\endlang

\lang_English
	Binds handler with ComboBox filter.
\endlang
*/
void FilterGuiItem::bindHandler(Qst::QstAbstractModelHandler *handler)
{
	Q_ASSERT(handler != NULL && (_filter.inputWidgetType() & Filter::ComboBox));
	_bindedHandler = handler;

	QComboBox *comboBox = (QComboBox*) _inputWidget;
	_bindedHandler->setComboBox(comboBox);
}

/*!
\lang_Russian
	Уничтожает все виджеты.
\endlang

\lang_English
	Deletes all widgets.
\endlang
*/
void FilterGuiItem::clear()
{
	_detachHandlerView();

	if (_inputWidget != NULL)
		delete _inputWidget;

	if (_checkBox != NULL)
		delete _checkBox;

	if (_filterItemsLayout != NULL)
		delete _filterItemsLayout;

	for (int i = 0; i < _itemsList.count(); ++i)
	{
		Q_ASSERT(_itemsList[i] != NULL);
		delete _itemsList[i];
	}
	_itemsList.clear();

	_inputWidget = NULL;
	_checkBox    = NULL;
	_filterItemsLayout = NULL;
}

/*!
\lang_Russian
	Слот, который вызывается, если изменилось состояние кнопки точного равенства.
\endlang

\lang_English
	Calls on exact quality button state change.
\endlang
*/
void FilterGuiItem::equalButtonStateChanged(const bool &checked)
{
	if (checked) _filter.setFunctor(Equal);
	else		 _filter.setFunctor(Like);
}

/*!
\lang_Russian
	Вызывается, если изменилось состояние QCheckBox.
	Устанавливает свойство Enabled для виджетов. В неактивном состоянии
	фильтром возвращаются инвалидные значения QVariant() и QueryValue(),
	что эквивалентно отсутствию фильтра при генерации SQL.
\endlang

\lang_English
	Called when the QCheckBox state changed.
	Sets the Enabled property for widgets. In the inactive state
	filter returns invalid values QVariant () and QueryValue (),
	which is equivalent to no filter when generating SQL.
\endlang
*/
void FilterGuiItem::setEnabled(const bool &enabled)
{
	_enabled = enabled;
}

void FilterGuiItem::_detachHandlerView()
{
	switch (_filter.inputWidgetType())
	{
	case Filter::LineEdit:
	case Filter::DateEdit:	return;
	case Filter::ComboBox:

		Q_ASSERT(_bindedHandler != NULL);
		Q_ASSERT(_inputWidget != NULL);

		QComboBox *comboBox = (QComboBox *) _inputWidget;
		_bindedHandler->unsetComboBox(comboBox);
	}
}


} // End of namespace Gui
} // End of namespace Qst
