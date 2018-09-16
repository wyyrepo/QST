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
#include "viewappearance.h"

namespace Qst
{
namespace Special
{


/*!
\class ViewAppearance
\brief
\lang_Russian
	Содержит настройки представлений (QTreeView, QTableView, QListView).
\endlang

\lang_English
	Contains settings for views (QTreeView, QTableView, QListView).
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
ViewAppearance::ViewAppearance()
	:
	_currentIndex(0)
{
}

/*!
\lang_Russian
	Устанавливает настройки колонок.
\endlang

\lang_English
	Sets column settings.
\endlang
*/
void ViewAppearance::setColumnSettingsMap(const ColumnSettingsMap &columnSettingsMap)
{
	_columnsSettingMap = columnSettingsMap;
}

void ViewAppearance::mergeColumnSetting(const int &columnIndex,
										const ColumnSetting &set)
{
	if (!_columnsSettingMap.contains(columnIndex))
		 _columnsSettingMap[columnIndex] = set;
}

/*!
\lang_Russian
	Возвращает настройки колонок.
\endlang

\lang_English
	Returns column settings.
\endlang
*/
ColumnSettingsMap ViewAppearance::columnSettingsMap() const
{
	return _columnsSettingMap;
}

void ViewAppearance::merge(const ViewAppearance &viewApp)
{
	if (viewApp.currentIndex() != -1)
		_currentIndex = viewApp.currentIndex();

	ColumnSettingsMap map = viewApp.columnSettingsMap();
	ColumnSettingsMap::const_iterator iter = map.begin();
	while (iter != map.end())
	{
		if (!_columnsSettingMap.contains(iter.key()))
			_columnsSettingMap[iter.key()] = iter.value();
		iter++;
	}
}

/*!
\lang_Russian
	Сохраняет индекс текущей строки.
\endlang

\lang_English
	Saves view current inndex.
\endlang
*/
void ViewAppearance::setCurrentIndex(const int &index)
{
	_currentIndex = index;
}

/*!
\lang_Russian
	Возвращает индекс текущей строки.
\endlang

\lang_English
	Returns view current inndex.
\endlang
*/
int ViewAppearance::currentIndex() const
{
	return _currentIndex;
}

/*!
\lang_Russian
	Устанавливает настройки колонки.
\endlang

\lang_English
	Sets column settings.
\endlang
*/
void ViewAppearance::setColumnSetting(const int &columnIndex,
									   const SettingType &settingType,
									   const QVariant &settingValue)
{
	bool ok;
	int intval;

	switch (settingType)
	{
	case SettingTitle: _columnsSettingMap[columnIndex].setTitle(settingValue.toString()); break;
	case SettingWidth:
		intval = settingValue.toInt(&ok);
		if (ok) _columnsSettingMap[columnIndex].setWidth(intval); break;
	case SettingOrientation:
		intval = settingValue.toInt(&ok);
		if (ok) _columnsSettingMap[columnIndex].setOrientation((Qt::Orientation)intval); break;
	case SettingShown:         _columnsSettingMap[columnIndex].setVisible(true); break;
	case SettingHidden:        _columnsSettingMap[columnIndex].setVisible(false); break;
	case SettingVisibility:    _columnsSettingMap[columnIndex].setVisible(settingValue.toBool()); break;
	case SettingModelColumn:   _columnsSettingMap[columnIndex].setModelColumn(settingValue.toBool()); break;
	}
}

void ViewAppearance::setColumnSetting(const int &columnIndex,
									  const ColumnSetting &set)
{
	_columnsSettingMap[columnIndex] = set;
}

/*!
\lang_Russian
	Возвращает настройки колонки.
\endlang

\lang_English
	Returns column settings.
\endlang
*/
ColumnSetting ViewAppearance::columnSetting(const int &columnIndex) const
{
	return _columnsSettingMap.value(columnIndex, ColumnSetting());
}

int ViewAppearance::modelColumn() const
{
	ColumnSettingsMap::const_iterator iter = _columnsSettingMap.begin();
	int firstVis = -1;
	while (iter != _columnsSettingMap.end())
	{
		if (iter.value().isModelColumn())
			return iter.key();
		if (iter.value().isVisible() && firstVis == -1)
			firstVis = iter.key();
		iter++;
	}
	if (firstVis != -1)
		return firstVis;
	return 0;
}

bool ViewAppearance::isValid() const
{
	return !_columnsSettingMap.isEmpty();
}

}
}
