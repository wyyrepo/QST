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

#include "filterassembly.h"

#include <QLayout>

/*!
\class FilterAssembly
\brief
\lang_Russian
	"Сборка фильтров". Содержит родительский виджет, ориентацию фильтров,
	флаг деления на секции при вертикальной компоновке виджетов,
	а так же toolTips и Icons для кнопок сокрытия/отображения виджета.
\endlang

\lang_English
	Has a parent widget, the orientation of the filters,
	flag division into sections in vertical layout,
	as well as toolTips and Icons for the hide/show widget buttons.
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
FilterAssembly::FilterAssembly()
	:
	_assemblyWidget(NULL)
{
}

/*!
\lang_Russian
	Главный конструктор. Если для родительского виджета установлен
	layout, то он будет удален.
\endlang

\lang_English
	Main constructor. If parent widget has layout, it will be removed.
\endlang
*/
FilterAssembly::FilterAssembly(QWidget *assemblyWidget,
							   const Qt::Orientation &orientation,
							   const bool &splitVerticalSections,
							   const QString &showToolTip,
							   const QString &hideToolTip,
							   const QIcon &showIcon,
							   const QIcon &hideIcon)
	:
	_assemblyWidget(assemblyWidget),
	_orientation(orientation),
	_splitVerticalSections(splitVerticalSections),
	_showToolTip(showToolTip),
	_hideToolTip(hideToolTip),
	_showIcon(showIcon),
	_hideIcon(hideIcon)
{
	Q_ASSERT(assemblyWidget != NULL);

	if(assemblyWidget->layout() != NULL);
		delete assemblyWidget->layout();
}

/*!
\lang_Russian
	Возвращает родительский виджет, на котором компонуются виджеты фильтров.
\endlang

\lang_English
	Returns the parent widget, on which widgets are linked filters.
\endlang
*/
QWidget *FilterAssembly::assemblyWidget()
{
	return _assemblyWidget;
}

/*!
\lang_Russian
	Возвращает ориентацию фильтров.
\endlang

\lang_English
	Returns the orientation of the filters.
\endlang
*/
Qt::Orientation FilterAssembly::orientation() const
{
	return _orientation;
}

/*!
\lang_Russian
	Возвращает флаг деления на секции при вертикальной компоновке фильтров.
\endlang

\lang_English
	Returns the flag division into sections in vertical arrangement of filters.
\endlang
*/
bool FilterAssembly::splitVerticalSections() const
{
	return _splitVerticalSections;
}

/*!
\lang_Russian
	Возвращает toolTip для кнопки отображения.
\endlang

\lang_English
	Returns the toolTip for the show button.
\endlang
*/
QString FilterAssembly::showToolTip() const
{
	return _showToolTip;
}

/*!
\lang_Russian
	Возвращает toolTip для кнопки скрытия.
\endlang

\lang_English
	Returns the toolTip for the hide button.
\endlang
*/
QString FilterAssembly::hideToolTip() const
{
	return _hideToolTip;
}

/*!
\lang_Russian
	Возвращает true, если установлены непустые toolTips для кнопок скрытия / отображения.
\endlang

\lang_English
	Returns true, if hide/show buttons have non-empty toolTips.
\endlang
*/
bool FilterAssembly::isValidToolTips() const
{
	return !_hideToolTip.isEmpty() && !_showToolTip.isEmpty();
}

/*!
\lang_Russian
	Возвращает icon для кнопки отображения.
\endlang

\lang_English
	Returns show button icon.
\endlang
*/
QIcon FilterAssembly::showIcon() const
{
	return _showIcon;
}

/*!
\lang_Russian
	Возвращает icon для кнопки сокрытия.
\endlang

\lang_English
	Returns hide button icon.
\endlang
*/
QIcon FilterAssembly::hideIcon() const
{
	return _hideIcon;
}

/*!
\lang_Russian
	Возвращает true, если установлены ненулевые иконки для кнопок скрытия / отображения.
\endlang

\lang_English
	Returns true, if hide/show buttons have valid icons.
\endlang
*/
bool FilterAssembly::isValidIcons() const
{
	return !_hideIcon.isNull() && !_showIcon.isNull();
}

/*!
\lang_Russian
	Возвращает true, если родительский виджет установлен.
\endlang

\lang_English
	Returns true, if a parent widget is seted.
\endlang
*/
bool FilterAssembly::isValid() const
{
	return _assemblyWidget != NULL;
}
