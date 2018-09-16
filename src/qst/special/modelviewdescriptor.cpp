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

#include "modelviewdescriptor.h"

#include <QDebug>

namespace Qst
{
namespace Special
{

/*!
\class ModelViewDescriptor
\brief
\lang_Russian
	Служебный класс для хранения модели данных, представлений (QTableView,
	QTreeView, QListView, QComboBox) и настроек этих представлений.

	Класс используется в QstAbstractModelHandler.
\endlang

\lang_English
	Service class. Stores data model, Qt's views (QTableView,
	QTreeView, QListView, QComboBox) and them settings.

	Used in QstAbstractModelHandler.
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
ModelViewDescriptor::ModelViewDescriptor()
	:
	_model(NULL),
	_modelType(PlainModel)
{
}

/*!
\lang_Russian
	Конструктор, принимающий модель данных и ее вид (плоская, иерархическая).
\endlang

\lang_English
	Constructor takes data model and model type (plain, tree).
\endlang
*/
ModelViewDescriptor::ModelViewDescriptor(QAbstractItemModel* model,
											   const ModelType & modelType)
	:
	_model(model),
	_modelType(modelType)
{
	Q_ASSERT(model != NULL);
}

/*!
\lang_Russian
	Деструктор.
\endlang

\lang_English
	Destructor.
\endlang
*/
ModelViewDescriptor::~ModelViewDescriptor()
{
}

/*!
\lang_Russian
	Возвращает указатель на модель данных.
\endlang

\lang_English
	Returns data model.
\endlang
*/
QAbstractItemModel *ModelViewDescriptor::model()
{
	return _model;
}

/*!
\lang_Russian
	Возвращает константный указатель на модель данных.
\endlang

\lang_English
	Returns const pointer to data model.
\endlang
*/
QAbstractItemModel *ModelViewDescriptor::model() const
{
	return _model;
}

/*!
\lang_Russian
	Устанавливает модель данных и ее вид (плоская, древовидная).
\endlang

\lang_English
	Sets data model and model type (plain, tree).
\endlang
*/
void ModelViewDescriptor::setModel(QAbstractItemModel *model,
								   const ModelType &modelType)
{
	Q_ASSERT(model != NULL);
	if (model == NULL)
		return;

	_model = model;
	_modelType = modelType;
}

/*!
\lang_Russian
	Добавляет представление и его настройки в массив представлений и возвращает его итератор.
\endlang

\lang_English
	Adds view and it's settings to the view map. Returns it's iterator.
\endlang
*/
void ModelViewDescriptor::insertView(QAbstractItemView *view,
									 const ViewType &viewType)
{
	Q_ASSERT(view != NULL);
	_viewSettingsMap[view].setView(view, viewType);
}

/*!
\lang_Russian
	Возвращает первое возможное представление.
\endlang

\lang_English
	Returns first taken view.
\endlang
*/
QAbstractItemView *ModelViewDescriptor::defaultView() const
{
	if (_viewSettingsMap.isEmpty())
		return NULL;
	return _viewSettingsMap.begin().key();
}


void ModelViewDescriptor::setViewAppearance(QAbstractItemView *view,
											const ViewAppearance app)
{
	Q_ASSERT(view != NULL);
	_viewSettingsMap[view].setViewAppearance(app);
}

void ModelViewDescriptor::setViewSorting(QAbstractItemView *view,
										 const bool &sortingEnabled)
{
	Q_ASSERT(view != NULL);
	_viewSettingsMap[view].setViewSorting(sortingEnabled);
}

/*!
\lang_Russian
	Устанавливает и применяет настройки представления.
\endlang

\lang_English
	Sets and applies view's settings.
\endlang
*/
void ModelViewDescriptor::setViewSetting(QAbstractItemView *view,
										 const int &columnIndex,
										 const SettingType &settingType,
										 const QVariant &settingValue)
{
//	Q_ASSERT(_viewSettingsMap.contains(view));
//	if (!_viewSettingsMap.contains(view))
//		return;

//	_viewSettingsMap[view].setColumnSetting(columnIndex, settingType, settingValue);

//	_setupView(view,
//			   columnIndex,
//			   _viewSettingsMap[view].viewType(),
//			   _viewSettingsMap[view].columnSetting(columnIndex),
//			   _viewSettingsMap[view].sortingEnabled());
}

/*!
\lang_Russian
	Добавляет QComboBox в массив объектов QComboBox и возвращает его итератор.
\endlang

\lang_English
	Adds comboBox and it's settings to the comboBox map. Returns it's iterator.
\endlang
*/
void ModelViewDescriptor::insertComboBox(QComboBox *comboBox)
{
	Q_ASSERT(comboBox != NULL);
	_comboBoxSettingsMap.insert(comboBox, ComboBoxSettings());
}

/*!
\lang_Russian
	Возвращает первый возможный comboBox.
\endlang

\lang_English
	Returns first taken comboBox.
\endlang
*/
QComboBox *ModelViewDescriptor::defaultComboBox() const
{
	if (_comboBoxSettingsMap.isEmpty())
		return NULL;

return _comboBoxSettingsMap.begin().key();
}

/*!
\lang_Russian
	Возвращает true, если view есть в массиве представлений.
\endlang

\lang_English
	Returns true if view it is in the view map.
\endlang
*/
bool ModelViewDescriptor::contains(QAbstractItemView *view) const
{
	return _viewSettingsMap.contains(view);
}

/*!
\lang_Russian
	Возвращает true, если comboBox есть в массиве представлений.
\endlang

\lang_English
	Returns true if comboBox it is in the comboBox map.
\endlang
*/
bool ModelViewDescriptor::contains(QComboBox *comboBox) const
{
	return _comboBoxSettingsMap.contains(comboBox);
}

/*!
\lang_Russian
	Отсоединяет view, удаляет его из массива.
\endlang

\lang_English
	Detaches view, removes it from the view array.
\endlang
*/
bool ModelViewDescriptor::removeView(QAbstractItemView *view)
{
	if (view == NULL)
		return false;

	view->setModel(NULL);

	_disconnectSignal(_model, view);

	return _viewSettingsMap.remove(view) > 0;
}

/*!
\lang_Russian
	Отсоединяет comboBox, удаляет его из массива.
\endlang

\lang_English
	Detaches comboBox, removes it from the comboBox array.
\endlang
*/
bool ModelViewDescriptor::removeComboBox(QComboBox *comboBox)
{
	if (comboBox == NULL)
		return false;

//	if (_comboBoxSettingsMap.contains(comboBox))
//		comboBox->setModel(_comboBoxSettingsMap[comboBox].dummyModel());

	comboBox->clear();
	return _comboBoxSettingsMap.remove(comboBox) > 0;
}

/*!
\lang_Russian
	Возвращает тип модели.
\endlang

\lang_English
	Returns model type.
\endlang
*/
ModelType ModelViewDescriptor::modelType() const
{
	return _modelType;
}

ViewType ModelViewDescriptor::viewType(QAbstractItemView *view) const
{
	Q_ASSERT(_viewSettingsMap.contains(view));
	return _viewSettingsMap[view].viewType();
}

/*!
\lang_Russian
	Устанавливает тип модели.
\endlang

\lang_English
	Sets model type.
\endlang
*/
void ModelViewDescriptor::setModelType(const ModelType & modelType)
{
	_modelType = modelType;
}

bool ModelViewDescriptor::reactivateModel(const QString &query,
										  const FieldRoleIndexMap &fieldRoleIndexMap,
										  const QSqlDatabase &db)
{
	Q_ASSERT(_model != NULL);
	if (_model == NULL)
		return false;

	if (_modelType == PlainModel)
	{
		QstPlainQueryModel *model = static_cast<QstPlainQueryModel*>(_model);
		return _setQuery(model, query, db);
	}
	else if (_modelType == TreeModel)
	{
		QstTreeQueryModel *model = static_cast<QstTreeQueryModel*>(_model);
		model->setFieldRoleIndexMap(fieldRoleIndexMap);
		return _setQuery(model, query, db);
	}
	return false;
}

bool ModelViewDescriptor::remodelize(const ViewAppearance &defaultAppSettings)
{
	bool ok = true;
	if (_setupViewModels())
		ok = _setupViews(defaultAppSettings, _model);

	_setupComboBoxes(defaultAppSettings, _model);
	return ok;
}

/*!
\lang_Russian
	Возвращает true, если модель не NULL.
\endlang

\lang_English
	Returns true if model is not NULL.
\endlang
*/
bool ModelViewDescriptor::isValid() const
{
	return _model != NULL;
}

/*!
\lang_Russian
	Очищает списки представлений.
\endlang

\lang_English
	Clears all views and comboBoxes.
\endlang
*/
void ModelViewDescriptor::clear()
{
	_viewSettingsMap.clear();
	_comboBoxSettingsMap.clear();
}

bool ModelViewDescriptor::_setupViewModels()
{
	foreach(QAbstractItemView* key, _viewSettingsMap.keys())
		_viewSettingsMap[key].setupSortFilterProxyModel(_model);
	return true;
}

bool ModelViewDescriptor::_setupViews(const ViewAppearance &defaultViewApp,
									  QAbstractItemModel *model)
{
	Q_ASSERT(model != NULL);
	if (model == NULL)
		return false;
	ViewSettingsMap::iterator iter = _viewSettingsMap.begin();
	while (iter != _viewSettingsMap.end())
	{
		iter.value().mergeViewAppearance(defaultViewApp);
		_setupView(iter.key(),
				   iter.value(),
				   _viewSettingsMap[iter.key()].viewModel(model));
		iter++;
	}
	return true;
}

void ModelViewDescriptor::_setupView(QAbstractItemView *view,
									 const ViewSettings &viewSets,
									 QAbstractItemModel *model)
{
	ColumnSettingsMap colSets = viewSets.viewAppearance().columnSettingsMap();
	ColumnSettingsMap::const_iterator iter = colSets.begin();
	while (iter != colSets.end())
	{
		_setupView(view,
				   iter.key(),
				   viewSets.viewType(),
				   iter.value(),
				   viewSets.sortingEnabled(),
				   model);
		iter++;
	}
}

void ModelViewDescriptor::_setupView(QAbstractItemView *view,
									 const int &columnIndex,
									 const ViewType &viewType,
									 const ColumnSetting &columnSetting,
									 const bool &sortingEnabled,
									 QAbstractItemModel *model)
{
	Q_ASSERT(model != NULL);
	Q_ASSERT(view  != NULL);
	view->setModel(model);
	view->model()->setHeaderData(columnIndex,
								 columnSetting.orientation(),
								 columnSetting.title(),
								 Qt::DisplayRole);

	if (viewType == TableView)
	{
		QTableView *v = static_cast<QTableView*>(view);
		v->setColumnHidden(columnIndex, !columnSetting.isVisible());
		v->setColumnWidth (columnIndex,  columnSetting.width());
		v->setSortingEnabled(sortingEnabled);
	}
	else if (viewType == TreeView)
	{
		QTreeView *v = static_cast<QTreeView*>(view);
		v->setColumnHidden(columnIndex, !columnSetting.isVisible());
		v->setColumnWidth(columnIndex, columnSetting.width());
		v->setSortingEnabled(sortingEnabled);
	}
	else if (viewType == ListView)
	{
		QListView *v = static_cast<QListView*>(view);
		if (columnSetting.isModelColumn())
			v->setModelColumn(columnIndex);
	}
}


/*!
\lang_Russian
	Привязывает выпадающие списки к модели.
	Восстанавливает текущую позицию в представлении.
\endlang

\lang_English
	Binds comboBoxes to models. Restores last current index in comboBoxes.
\endlang
*/
void ModelViewDescriptor::_setupComboBoxes(const ViewAppearance &defaultViewApp,
										   QAbstractItemModel *model)
{
	Q_ASSERT(model != NULL);
	if (model == NULL)
		return;

	ComboBoxSettingsMap::iterator iter = _comboBoxSettingsMap.begin();
	while(iter != _comboBoxSettingsMap.end())
	{
		iter.key()->setModel(model);
		if (iter.value().modelColumn() != -1)
			iter.key()->setModelColumn(iter.value().modelColumn());
		else
			iter.key()->setModelColumn(defaultViewApp.modelColumn());
		iter.key()->setCurrentIndex(iter.value().currentIndex());
		iter++;
	}
}

void ModelViewDescriptor::_connectSignal(QAbstractItemModel *model,
										 QAbstractItemView  *view)
{
	QObject::connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
					 view,  SLOT  (dataChanged(QModelIndex,QModelIndex)));
}

void ModelViewDescriptor::_disconnectSignal(QAbstractItemModel *model,
											QAbstractItemView  *view)
{
	QObject::disconnect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
						view,  SLOT  (dataChanged(QModelIndex,QModelIndex)));
}

bool ModelViewDescriptor::_setQuery(QstPlainQueryModel *model,
									const QString &query,
									const QSqlDatabase &db)
{
	qDebug() << "\n Query will be:\n" << query;
	model->setQuery(query, db);
	if (model->lastError().isValid())
	{
		qDebug() << model->lastError().text();
		return false;
	}
	return true;
}

bool ModelViewDescriptor::_setQuery(QstTreeQueryModel *model,
									const QString &query,
									const QSqlDatabase &db)
{
	qDebug() << "\n Query will be:\n" << query;
	model->setQuery(query, db);
	if (model->lastError().isValid())
	{
		qDebug() << model->lastError().text();
		return false;
	}
	return true;
}

}	// End of namespace Special
}	// End of namespace Qst
