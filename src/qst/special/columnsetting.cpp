/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst::Mvc module of the QsT SQL Tools.
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

#include "columnsetting.h"

namespace Qst
{
namespace Special
{


/*!
\class ColumnSetting
\brief
\lang_Russian
	Инкапсулирует настройки колонки для представлений (QTableView, QTreeView).
\endlang

\lang_English
	Incapsulates column settings for views (QTableView, QTreeView).
\endlang
*/

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
ColumnSetting::ColumnSetting()
	:
	  _visible(true),
	  _title(""),
	  _width(100),
	  _orientation(Qt::Horizontal),
	  _modelColumn(false)
{
}

/*!
\lang_Russian
	Главный конструктор.
\endlang

\lang_English
	Main constructor.
\endlang
*/
ColumnSetting::ColumnSetting(const bool &visible,
							 const QString &title,
							 const int &width,
							 const Qt::Orientation &orientation)
	:
	  _visible(visible),
	  _title(title),
	  _width(width),
	  _orientation(orientation),
	  _modelColumn(false)
{
}


/*!
\lang_Russian
	Возвращает true, если колонка отображается во view.
\endlang

\lang_English
	Returns true if view column is visible.
\endlang
*/
bool ColumnSetting::isVisible() const
{
	return _visible;
}

/*!
\lang_Russian
	Устанавливает отображаемость колонки во view.
\endlang

\lang_English
	Sets view column visibility.
\endlang
*/
void ColumnSetting::setVisible(const bool &visible)
{
	_visible = visible;
}

/*!
\lang_Russian
	Устанавливает отображаемость колонки во view.
\endlang

\lang_English
	Sets view column visibility.
\endlang
*/
void ColumnSetting::setHidden(const bool &hidden)
{
	_visible = !hidden;
}

/*!
\lang_Russian
	Возвращает заголовок колонки.
\endlang

\lang_English
	Returns column title.
\endlang
*/
QString ColumnSetting::title() const
{
	return _title;
}

/*!
\lang_Russian
	Устанавливает заголовок колонки.
\endlang

\lang_English
	Sets column title.
\endlang
*/
void ColumnSetting::setTitle(const QString &title)
{
	_title = title;
}

/*!
\lang_Russian
	Возвращает ширину колонки.
\endlang

\lang_English
	Returns column width.
\endlang
*/
int ColumnSetting::width() const
{
	return _width;
}

/*!
\lang_Russian
	Устанавливает ширину колонки.
\endlang

\lang_English
	Sets column width.
\endlang
*/
void ColumnSetting::setWidth(const int &width)
{
	_width = width;
}

/*!
\lang_Russian
	Возвращает ориентацию колонки.
\endlang

\lang_English
	Returns column orientation.
\endlang
*/
Qt::Orientation ColumnSetting::orientation() const
{
	return _orientation;
}

/*!
\lang_Russian
	Устанавливает ориентацию колонки.
\endlang

\lang_English
	Sets column orientation.
\endlang
*/
void ColumnSetting::setOrientation(const Qt::Orientation &orientation)
{
	_orientation = orientation;
}

bool ColumnSetting::isModelColumn() const
{
	return _modelColumn;
}

void ColumnSetting::setModelColumn(const bool &modelColumn)
{
	_modelColumn = modelColumn;
}

} // End of namespace Special;
} // End of namespace Qst;
