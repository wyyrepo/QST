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

#include "viewsettings.h"

#include <QDebug>

namespace Qst
{
namespace Special
{

ViewSettings::ViewSettings()
	:
	  _view(NULL),
	  _sortingEnabled(false),
	  _sortFilterProxyModel(NULL)
{
}

ViewSettings::~ViewSettings()
{
	_deleteSortFilterModel();
}

void ViewSettings::setView(QAbstractItemView *view, const ViewType &viewType)
{
	_view = view;
	_viewType = viewType;
}

void ViewSettings::setViewSorting(const bool &sortingEnabled)
{
	_sortingEnabled = sortingEnabled;
}

void ViewSettings::setViewAppearance(const ViewAppearance &app)
{
	_viewAppearance = app;
}

void ViewSettings::setColumnSetting(const int &columnIndex,
									const SettingType &settingType,
									const QVariant &settingValue)
{
	_viewAppearance.setColumnSetting(columnIndex, settingType, settingValue);
}

ViewType ViewSettings::viewType() const
{
	return _viewType;
}

ViewAppearance ViewSettings::viewAppearance() const
{
	return _viewAppearance;
}

ColumnSetting ViewSettings::columnSetting(const int &columnIndex) const
{
	return _viewAppearance.columnSetting(columnIndex);
}

void ViewSettings::mergeViewAppearance(const ViewAppearance &viewApp)
{
	_viewAppearance.merge(viewApp);
}

void ViewSettings::setupSortFilterProxyModel(QAbstractItemModel *baseModel)
{
	if (_sortingEnabled)
		_createSortFilterModel(baseModel);
	else
		_deleteSortFilterModel();
}

QAbstractItemModel * ViewSettings::viewModel(QAbstractItemModel *baseModel)
{
	Q_ASSERT(baseModel != NULL);
	if (_sortingEnabled)
		return _sortFilterProxyModel;

	return baseModel;
}

bool ViewSettings::sortingEnabled() const
{
	return _sortingEnabled;
}

void ViewSettings::_createSortFilterModel(QAbstractItemModel *baseModel)
{
	if (_sortFilterProxyModel == NULL)
		_sortFilterProxyModel = new QSortFilterProxyModel();

	Q_ASSERT(_sortFilterProxyModel != NULL);
	_sortFilterProxyModel->setSourceModel(baseModel);
}

void ViewSettings::_deleteSortFilterModel()
{
	if (_sortFilterProxyModel != NULL)
		delete _sortFilterProxyModel;
}


} // End of namespace Special
} // End of namespace Qst
