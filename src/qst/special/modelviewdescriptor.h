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

#ifndef MODELVIEWDESCRIPTOR_H
#define MODELVIEWDESCRIPTOR_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include <QAbstractItemView>
#include <QComboBox>
#include <QListView>
#include <QList>
#include <QMap>
#include <QVector>

#include <QTableView>
#include <QTreeView>
#include <QListView>

#include "qst/qstbatch.h"
#include "qst/qstglobal.h"
#include "qst/special/viewsettings.h"
#include "qst/special/comboboxsettings.h"

#include "qst/mvc/qstplainquerymodel.h"
#include "qst/mvc/qsttreequerymodel.h"

namespace Qst
{
namespace Special
{

class ModelViewDescriptor
{
private:
	QAbstractItemModel *_model;
	ModelType           _modelType;

	Qst::Special::ViewSettingsMap     _viewSettingsMap;
	Qst::Special::ComboBoxSettingsMap _comboBoxSettingsMap;

public:

	ModelViewDescriptor();
	ModelViewDescriptor(QAbstractItemModel *model,
						const ModelType &modelType);
	~ModelViewDescriptor();

	QAbstractItemModel *model();
	QAbstractItemModel *model() const;

	void setModel  (QAbstractItemModel *model, const ModelType &modelType);
	void insertView(QAbstractItemView  *view,  const ViewType &viewType);
	QAbstractItemView *defaultView() const;

	void setViewAppearance(QAbstractItemView *view, const ViewAppearance app);
	void setViewSorting   (QAbstractItemView *view, const bool &sortingEnabled);
	void setViewSetting   (QAbstractItemView *view,
						   const int &columnIndex,
						   const SettingType &settingType,
						   const QVariant &settingValue); // FIX ME: code removed

	void insertComboBox(QComboBox *comboBox);
	QComboBox *defaultComboBox() const;

	bool contains(QAbstractItemView *view) const;
	bool contains(QComboBox *comboBox) const;

	bool removeView(QAbstractItemView *view);
	bool removeComboBox(QComboBox *comboBox);

	ModelType modelType() const;
	void setModelType(const ModelType & modelType);

	ViewType viewType(QAbstractItemView *view) const;

	bool reactivateModel(const QString &query,
						 const FieldRoleIndexMap &fieldRoleIndexMap,
						 const QSqlDatabase &db);
	bool remodelize(const ViewAppearance &defaultAppSettings);

	bool isValid() const;
	void clear();

private:

	bool _setupViewModels();
	bool _setupViews(const ViewAppearance &defaultViewApp, // FIX ME : default view app
					 QAbstractItemModel *model);
	void _setupView(QAbstractItemView *view,
					const ViewSettings &viewSets,
					QAbstractItemModel *model);

	void _setupView(QAbstractItemView *view,
					const int &columnName,
					const ViewType &viewType,
					const ColumnSetting &columnSetting,
					const bool &sortingEnabled,
					QAbstractItemModel *model);

	void _setupTableView(QTableView * view,
						 const int &columnIndex,
						 const bool &visible,
						 const int &width);

	void _setupTreeView(QTreeView * view,
						const int &columnIndex,
						const bool &visible,
						const int &width);

	void _setupListView(QListView * view,
						const int &columnIndex,
						const bool &visible,
						const bool &isModelColumn);

	void _setupComboBoxes(const ViewAppearance &defaultViewApp,
						  QAbstractItemModel *model);

	void _connectSignal   (QAbstractItemModel *model, QAbstractItemView *view);
	void _disconnectSignal(QAbstractItemModel *model, QAbstractItemView *view);

	bool _setQuery(QstPlainQueryModel *model,
				   const QString &query,
				   const QSqlDatabase &db);
	bool _setQuery(QstTreeQueryModel *model,
				   const QString &query,
				   const QSqlDatabase &db);

};



}	// End of namespace Special
}	// End of namespace Qst

#endif // MODELVIEWDESCRIPTOR_H
