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
#ifndef VIEWAPPEARANCE_H
#define VIEWAPPEARANCE_H

#include "columnsetting.h"
#include <QAbstractItemView>
#include <QMap>

namespace Qst
{
namespace Special
{

class ViewAppearance	// FIX ME: documentation // FULL REWRITE
{
private:

	ColumnSettingsMap _columnsSettingMap;
	int               _currentIndex;

public:
	ViewAppearance();

	void setColumnSettingsMap(const ColumnSettingsMap &columnSettingsMap);
	void mergeColumnSetting(const int &columnIndex, const ColumnSetting &set);
	ColumnSettingsMap columnSettingsMap() const;

	void merge(const ViewAppearance &viewApp);

	void setCurrentIndex(const int &index);
	int  currentIndex() const;

	void setColumnSetting(const int &columnIndex,
						  const SettingType &settingType,
						  const QVariant &settingValue);
	void setColumnSetting(const int &columnIndex,
						  const ColumnSetting &set);
	ColumnSetting columnSetting(const int &columnIndex) const;

	int modelColumn() const;

	bool isValid() const;

	friend bool operator==(const ViewAppearance &ap1, const ViewAppearance &ap2)
	{
		return  ap1._currentIndex == ap2._currentIndex
				&& ap1._columnsSettingMap == ap2._columnsSettingMap
				;
	}
};

// --------------------------------------------------------------------------------- //

typedef QMap<QAbstractItemView *, ViewAppearance> ViewAppearanceMap;

// --------------------------------------------------------------------------------- //

}
}

#endif // VIEWAPPEARANCE_H
