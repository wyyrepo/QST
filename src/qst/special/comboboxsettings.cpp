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

#include "comboboxsettings.h"

namespace Qst
{
namespace Special
{

/*!
\class QstComboBoxSettings
\brief
\lang_Russian
	Инкапсулирует модельные настройки для выпадающего списка (QComboBox).
\endlang

\lang_English
	Incapsulates model column settings for QComboBox.
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
ComboBoxSettings::ComboBoxSettings()
	:
	_currentIndex(0),
	_modelColumn(-1)
{
}

/*!
\lang_Russian
	Сохраняет текущий индекс QComboBox.
\endlang

\lang_English
	Saves current index of QComboBox.
\endlang
*/
void ComboBoxSettings::setCurrentIndex(const int &index)
{
	_currentIndex = index;
}

/*!
\lang_Russian
	Возвращает сохраненный ранее текущий индекс.
\endlang

\lang_English
	Returns previously saved current index.
\endlang
*/
int ComboBoxSettings::currentIndex() const
{
	return _currentIndex;
}

/*!
\lang_Russian
	Устанавливает индекс колонки для модели, которая будет отображаться в QComboBox.
\endlang

\lang_English
	Sets data model column index for column will be displayed in QComboBox.
\endlang
*/
void ComboBoxSettings::setModelColumn(const int &column)
{
	_modelColumn = column;
}

/*!
\lang_Russian
	Возвращает индекс колонки для модели, которая будет отображаться в QComboBox.
\endlang

\lang_English
	Returns data model column index for column will be displayed in QComboBox.
\endlang
*/
int ComboBoxSettings::modelColumn() const
{
	return _modelColumn;
}

bool ComboBoxSettings::isValid() const
{
	return _modelColumn != -1;
}

} // End of namespace Special
} // End of namespace Qst
