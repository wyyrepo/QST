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

#include "filter.h"

namespace Qst
{
namespace Gui
{

/*!
\class Filter
\brief
\lang_Russian
	Хранит данные об одном динамическом фильтре.
	Динамические фильтры - это виджеты на форме, сгенерированные
	по декларативному описанию, которые подключаются к классу-хэндлеру
	и влияют на генерацию секции WHERE SQL-зпроса.

	Во всех программах интерфейс фильтров выглядит примерно одинаково.
	Есть название фильтра, поле для ввода значения, иногда можно выбрать
	операцию сравнения из выпадающего списка или другим способом.
	Обычно фильтры можно включать и выключать с помощью checkBox.
	Динамические фильтры, включенные в QST, пока еще не слишком функциональы.

	В классе хранится следующая информация:
	- поле для секции WHERE (fieldName),
	- название фильтра (title),
	- тип поля редактирования (InputWidgetType),
	- функторы сравнения.

	На данный момент можно создавать фильтры нескольких типов.
	- Фильтр по строковому значению. Отображается на форме как QLineEdit,
	снабжается кнопкой очистки текстового поля. По умолчанию задается функтор Like,
	но можно указать функтор Equal, и для него будет создан checkable QToolButton
	"равно".
	- Фильтр по дате. Отображается как QDateEdit. Сравнение по функтору Equal.
	- Фильтр по значению выпадающего списка (QComboBox). В этом случае
	к списку нужно привязать класс-хэндлер, который предоставит список для
	QComboBox.

	Объекты Filter компонуются в списки FilterList, которые затем компонуются
	в стеки FilterStack. Визуально фильтры, включенные в один FilterList,
	помещаются на один QFormLayout. Стек фильтров может быть горизонтальным или
	вертикальным. Вертикальная компоновка позволяет расположить списки фильтров
	на одном QFormLayout, либо разделить их на разные QFormLayout.

	\attention Подсистема Qst::Gui еще находится в активной разработке.
	В будущих версиях она может измениться.
\endlang

\lang_English
	Stores dynamic filter's data. Dynamic filters is the widgets generated
	by the declarative description. Filters connected with a handler class
	and affect to WHERE-clause generation.

	In all programs filters look the same. Filter has name, input widget,
	sometimes you can choose comparison operator. Typically, filters can be
	turned on and off with checkBox. Dynamic filters of the QST not functional still.

	Class stores:
	- field name for WHERE clause;
	- filter title;
	- input widget type;
	- comparison functors.

	At this point, you can create several types of filters.
	- Filter on a string. Looks like QLineEdit with "clear" toolbutton.
	Functor "Like" is used by default, but you can specify a functor "Equal",
	and it will be created as checkable QToolButton "equals".
	- Filter by date. Appears as QDateEdit. Comparison of the functor Equal.
	- Filter by drop down list value (QComboBox). In this case,
	to the list you want to bind the class-handler, which will
	provide a list for QComboBox.

	Filter objects are arranged in lists FilterList, which are then arranged
	in stacks FilterStack. Filters included in a FilterList, placed on one QFormLayout.
	Filter stack can be horizontal or vertical. Vertical layout allows you to position
	filter lists on the same QFormLayout, or divide them into different QFormLayout.

	\attention Subsystem Qst:: Gui is still in active development.
	In future versions it may change.
\endlang

\sa FiltersManager
*/


/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
Filter::Filter()
	:
	  _title(QString()),
	  _inputWidgetType(LineEdit),
	  _fieldName(QString()),
	  _functor(Like),
	  _hasEqualButton(false)
{
}

/*!
\lang_Russian
	Главный конструктор.

	Если функторы не указаны, либо указан функтор FunctorNone,
	функтор будет присвоен автоматически в зависимости от типа виджета.
	Для строки это Like, для даты и ComboBox - Equal.
\endlang

\lang_English
	Main constructor.

	Takes two functor parameters: functors and functorList.
	At this point in the first parameter must be passed functors
	by default, while the second - all admissible functors.
	In the future we plan to do QComboBox, which would allow us to choose
	available comparison functor.

	If the functor is not specified or specified functor FunctorNone, then
	functor will be assigned automatically depending on the type of widget.
	For the string is Like, to date, and ComboBox - Equal.
\endlang
*/

Filter::Filter(const QString &fieldName,
			   const QString &title,
			   const InputWidgetType &inputWidget,
			   const Functor &functor)
	:
	  _title(title),
	  _inputWidgetType(inputWidget),
	  _fieldName(fieldName),
	  _functor(functor),
	  _hasEqualButton(functor == Equal)
{
	if (inputWidget == LineEdit)
		_functor = Like;
	if (inputWidget == ComboBox)
		_functor = Equal;
}

/*!
\lang_Russian
	Возвращает заголовок фильтра. Отображается на форме в виде QLabel.
\endlang

\lang_English
	Returns the title of the filter. Displayed on the form as a QLabel.
\endlang
*/
QString Filter::title() const
{
	return _title;
}

/*!
\lang_Russian
	Возвращает тип виджета, который необходимо создать под фильтр.
\endlang

\lang_English
	Returns the type of widget you want to create a filter.
\endlang
*/
Filter::InputWidgetType Filter::inputWidgetType() const
{
	return _inputWidgetType;
}


/*!
\lang_Russian
	Возвращает имя поля для связи фильтра и поля в таблице базы данных.
\endlang

\lang_English
	Returns the field name for the connection filter and a field in a database table.
\endlang
*/
QString	Filter::fieldName() const
{
	return _fieldName;
}

Functor Filter::functor() const
{
	return _functor;
}

void Filter::setFunctor(const Functor &functor)
{
	_functor = functor;
}

Qst::PercentPlaceholders Filter::percentPlaceholders() const
{
	if (_functor == Qst::Like)
		return Qst::LeftRightPercents;
	return Qst::NoPercentPlaceholders;
}

/*!
\lang_Russian
	Возвращает true, если фильтр является сложным GUI-объектом, то есть,
	кроме заголовка и виджета содержит еще какие-то элементы GUI.
	Для строкового фильтра такими элементами являются кнопка очистки
	поля и кнопка точного равенства.
\endlang

\lang_English
	Returns true, if the filter is a complex GUI object, ie,
	except the title and the widget also contains some elements of the GUI.

	For a string filter, such elements are the clear button and the button of field exact equality.
\endlang
*/
bool Filter::isComplex() const
{
	return _inputWidgetType == LineEdit;
}

/*!
\lang_Russian
	Возвращает true, если для текстового фильтра задана кнопка точного равенства.
\endlang

\lang_English
	Returns true, if for the text filter the exact equality button is seted.
\endlang
*/
bool Filter::hasEqualButton() const
{
	return _hasEqualButton;
}

} // End of namespace Gui
} // End of namespace Qst
