/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Special module of the QsT SQL Tools.
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

#ifndef VIEWSETTINGS_H
#define VIEWSETTINGS_H

#include "viewappearance.h"

#include <QMap>
#include <QAbstractItemView>
#include <QSortFilterProxyModel>


#include "qst/qstglobal.h"

namespace Qst
{
namespace Special
{

class ViewSettings
{
private:

	ViewType _viewType;
	QAbstractItemView *_view;

	bool _sortingEnabled;
	QSortFilterProxyModel *_sortFilterProxyModel;

	ViewAppearance _viewAppearance;

public:
    ViewSettings();
	~ViewSettings();

	void setView(QAbstractItemView *view, const ViewType &viewType);
	void setViewSorting(const bool &sortingEnabled);
	void setViewAppearance(const ViewAppearance &app);
	void setColumnSetting(const int &columnIndex,
						  const SettingType &settingType,
						  const QVariant &settingValue);

	ViewType       viewType() const;
	ViewAppearance viewAppearance() const;
	ColumnSetting  columnSetting(const int &columnIndex) const;

	void mergeViewAppearance(const ViewAppearance &viewApp);

	void setupSortFilterProxyModel(QAbstractItemModel *baseModel);
	QAbstractItemModel *viewModel (QAbstractItemModel *baseModel);

	bool sortingEnabled() const;

private:

	void _createSortFilterModel(QAbstractItemModel *baseModel);
	void _deleteSortFilterModel();
};

////////////////////////////////////////////////////////////////////////////////

typedef QMap<QAbstractItemView *, ViewSettings> ViewSettingsMap;

////////////////////////////////////////////////////////////////////////////////


} // End of namespace Special
} // End of namespace Qst

#endif // VIEWSETTINGS_H
