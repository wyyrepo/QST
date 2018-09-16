/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
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

#include "qstabstractmodelhandler.h"

#include <QObject>
#include <QDebug>

#include <QSqlRecord>

#include "querygeneration/querybuilddirector.h"

namespace Qst
{

using namespace Qst::Special;


AbstractQueryBuilder *QstAbstractModelHandler::_queryBuilder;

void QstAbstractModelHandler::setQueryBuilder(AbstractQueryBuilder *queryBuilder)
{
	_queryBuilder = queryBuilder;
}

AbstractQueryBuilder *QstAbstractModelHandler::queryBuilder()
{
	return _queryBuilder;
}

QstAbstractModelHandler::QstAbstractModelHandler()
	:
	_loaded(false)
{
}

QstAbstractModelHandler::~QstAbstractModelHandler()
{
}

////////////////////////////////////////////////////////////////////////////////

bool QstAbstractModelHandler::reload(const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());
	if (_modelViewDescriptor.reactivateModel(generateQuery(_batch),
											 _batch.fieldRoleIndexMap(), db))
		_loaded = _modelViewDescriptor.remodelize(_batch.viewAppearance());
	return _loaded;
}

void QstAbstractModelHandler::unload()
{
	_unload();
}

bool QstAbstractModelHandler::isLoaded() const
{
	return _loaded;
}

void QstAbstractModelHandler::setQuery(const QstBatch &batch)
{
	_batch = batch;
}

void QstAbstractModelHandler::setQuery(const QString &queryString)
{
	QueryBatch qB = QueryBatch(UserDefinedQuery, "");
	qB.setUserDefinedQuery(queryString);
	_batch = QstBatch(qB);
}

void QstAbstractModelHandler::setModel(QstPlainQueryModel *model)
{
	_modelViewDescriptor.setModel(model, PlainModel);
}

void QstAbstractModelHandler::setModel(QstTreeQueryModel *model)
{
	_modelViewDescriptor.setModel(model, TreeModel);
}

void QstAbstractModelHandler::setView(QAbstractItemView * view,
									  const ViewType &viewType)
{
	_modelViewDescriptor.insertView(view, viewType);
}

void QstAbstractModelHandler::setTableView(QTableView *tableView)
{
	_modelViewDescriptor.insertView(tableView, TableView);
}

void QstAbstractModelHandler::setTreeView(QTreeView *treeView)
{
	_modelViewDescriptor.insertView(treeView, TreeView);
}

void QstAbstractModelHandler::setListView(QListView *listView)
{
	_modelViewDescriptor.insertView(listView, ListView);
}

void QstAbstractModelHandler::setComboBox(QComboBox * comboBox)
{
	_modelViewDescriptor.insertComboBox(comboBox);
}

void QstAbstractModelHandler::setViewSorting(QAbstractItemView *view,
											 const bool &sortingEnabled)
{
	_modelViewDescriptor.setViewSorting(view, sortingEnabled);
}

void QstAbstractModelHandler::setViewAppearance(QAbstractItemView *view,
												const ViewAppearance app)
{
	_modelViewDescriptor.setViewAppearance(view, app);
}

void QstAbstractModelHandler::setViewSetting(QAbstractItemView *view,
											 const QString &fieldName,
											 const SettingType &settingType,
											 const QVariant &settingValue)
{
	Q_ASSERT(view != NULL);
	if (view == NULL)
		return;

	int columnIndex = fieldColumnIndex(fieldName);
	_modelViewDescriptor.setViewSetting(view, columnIndex, settingType, settingValue);
}

bool QstAbstractModelHandler::unsetView(QAbstractItemView *view)
{
	return _modelViewDescriptor.removeView(view);
}

bool QstAbstractModelHandler::unsetComboBox(QComboBox * comboBox)
{
	return _modelViewDescriptor.removeComboBox(comboBox);
}

void QstAbstractModelHandler::clear()
{
	_unload();
}

QModelIndex QstAbstractModelHandler::modelIndex(const QVariant &keyValue,
												const QAbstractItemView *view) const
{
	Q_ASSERT(view != NULL);
	Q_ASSERT(view->model() != NULL);

	if (view == NULL
		|| view->model() == NULL)
	{
		qDebug()	<< "\nWARNING! Cant retreive modelIndex for value : " << keyValue << ".\n"
					<< "view != NULL:" << (view != NULL)
					<< ", view->model() != NULL: " << (view->model() != NULL) << ".";
		return QModelIndex();
	}

	return _getIndex(keyValue, view->model());
}

int QstAbstractModelHandler::fieldColumnIndex(const FieldRole &role) const
{
	Q_ASSERT(role != RoleNone);

	return _batch.columnIndex(role);
}

int QstAbstractModelHandler::fieldColumnIndex(const QString &fieldName) const
{
	Q_ASSERT(!fieldName.isEmpty());

	return _batch.columnIndex(fieldName);
}

void QstAbstractModelHandler::setViewCurrentRow(const QVariant &keyValue,
												QAbstractItemView *view)
{
	QAbstractItemView *v = view;
	if (v == NULL)
		v = _modelViewDescriptor.defaultView();

	Q_ASSERT(_modelViewDescriptor.contains(v));
	Q_ASSERT(isLoaded());

	if (!_modelViewDescriptor.contains(v)
		|| !isLoaded())
		return;

	if (!keyValue.isValid())
		return;

	v->setCurrentIndex(_getIndex(keyValue, _modelViewDescriptor.model()));
}

void QstAbstractModelHandler::setComboBoxCurrentRow(const QVariant &keyValue,
													QComboBox *comboBox)
{
	QComboBox *c = comboBox;
	if (c == NULL)
		c = _modelViewDescriptor.defaultComboBox();

	Q_ASSERT(_modelViewDescriptor.contains(c));
	Q_ASSERT(isLoaded());

	if (!_modelViewDescriptor.contains(c)
		|| !isLoaded())
		return;

	if (!keyValue.isValid())
		return;

	c->setCurrentIndex(_getIndex(keyValue, _modelViewDescriptor.model()).row());
}

QVariantList QstAbstractModelHandler::viewSelectedKeys(const FieldRole &role,
													   QAbstractItemView *view) const
{
	QAbstractItemView *v = view;
	if (v == NULL)
		v = _modelViewDescriptor.defaultView();

	Q_ASSERT(_modelViewDescriptor.contains(v));

	if (!_modelViewDescriptor.contains(v))
		return QVariantList();

	QVariantList keysList;

	int keysColumnIndex = fieldColumnIndex(role);
	QModelIndexList indexesList = v->selectionModel()->selectedIndexes();

	for (int i = 0; i < indexesList.size(); ++i)
		if (indexesList[i].column() == keysColumnIndex)
			keysList.append(indexesList[i].data());

return keysList;
}

QVariant QstAbstractModelHandler::viewKeyValue(const FieldRole &role,
											   QAbstractItemView *view)  const
{
	QAbstractItemView *v = view;
	if (view == NULL)
		v = _modelViewDescriptor.defaultView();

	Q_ASSERT(_modelViewDescriptor.contains(v));
	Q_ASSERT(isLoaded());
	if (!_modelViewDescriptor.contains(v) || !isLoaded())
		return QVariant();

	return _fieldData(v->currentIndex().row(),
					  _batch.columnIndex(role),
					  v->currentIndex().parent(),
					  v->model());
}

QVariant QstAbstractModelHandler::viewFieldValue(const QString &fieldName,
												 QAbstractItemView *view) const
{
	return viewFieldsValueMap(QStringList() << fieldName, view).value(fieldName);
}

QVariantMap QstAbstractModelHandler::viewFieldsValueMap(const QStringList &fieldNames,
														QAbstractItemView *view)  const
{
	QAbstractItemView *v = view;
	if (v == NULL)
		v = _modelViewDescriptor.defaultView();

	Q_ASSERT(_modelViewDescriptor.contains(v));
	Q_ASSERT(isLoaded());
	if (!_modelViewDescriptor.contains(v) || !isLoaded())
		return QVariantMap();

	QStringList realFieldNames = (fieldNames.isEmpty())
			? _batch.fieldNames()
			: fieldNames;

	QVariantMap resMap;
	foreach (QString fieldName, realFieldNames)
		resMap[fieldName] = _fieldData(v->currentIndex().row(),
									   _batch.columnIndex(fieldName),
									   v->currentIndex().parent(),
									   v->model());
	return resMap;
}

void QstAbstractModelHandler::viewExpandTo(const QVariant &keyValue,
										   QAbstractItemView *view,
										   const bool &setSelected)
{
	QAbstractItemView *v = view;
	if (v == NULL)
		v = _modelViewDescriptor.defaultView();

	Q_ASSERT(_modelViewDescriptor.contains(v));
	Q_ASSERT(v->model() != NULL);
	Q_ASSERT(isLoaded());
	Q_ASSERT(_modelViewDescriptor.viewType(v) == TreeView);

	if (!_modelViewDescriptor.contains(v)
		|| v->model() == NULL
		|| !isLoaded()
		|| _modelViewDescriptor.viewType(v) != TreeView)
	{
		qDebug()	<< "\nWARNING! Cant expand to keyValue : " << keyValue << ".\n"
					<< "view exist:" << _modelViewDescriptor.contains(v)
					<< ", model == NULL: " << (v->model() == NULL)
					<< ", handler loaded: " << isLoaded()
					<< ", viewType: " << _modelViewDescriptor.viewType(v) << ".";
		return;
	}

	QModelIndex index = _getIndex(keyValue, v->model());

	_viewExpandTo(index, static_cast<QTreeView*>(v));

	if (setSelected)
		v->setCurrentIndex(index);
}

QVariant QstAbstractModelHandler::comboBoxKeyValue(const FieldRole &role,
												   QComboBox *comboBox) const
{
	QComboBox *c = comboBox;
	if (c == NULL)
		c = _modelViewDescriptor.defaultComboBox();

	Q_ASSERT(_modelViewDescriptor.contains(c));
	Q_ASSERT(isLoaded());
	if (!_modelViewDescriptor.contains(c)
		|| !isLoaded())
		return QVariant();

	return _fieldData(c->currentIndex(),
					  _batch.columnIndex(role),
					  QModelIndex(),
					  c->model());
}

QVariant QstAbstractModelHandler::comboBoxFieldValue(const QString &fieldName,
													 QComboBox *comboBox) const
{
	return comboBoxFieldsValueMap(QStringList() << fieldName, comboBox).value(fieldName);
}

QVariantMap QstAbstractModelHandler::comboBoxFieldsValueMap(const QStringList &fieldNames,
															QComboBox *comboBox) const
{
	QComboBox *c = comboBox;
	if (c == NULL)
		c = _modelViewDescriptor.defaultComboBox();

	Q_ASSERT(_modelViewDescriptor.contains(c));
	Q_ASSERT(isLoaded());
	if (!_modelViewDescriptor.contains(c)
		|| !isLoaded())
		return QVariantMap();

	QStringList realFieldNames = (fieldNames.isEmpty())
			? _batch.fieldNames()
			: fieldNames;

	QVariantMap resMap;
	foreach (QString fieldName, realFieldNames)
		resMap[fieldName] = _fieldData(c->currentIndex(),
									   _batch.columnIndex(fieldName),
									   QModelIndex(),
									   c->model());
return resMap;
}

QVariant QstAbstractModelHandler::fieldValue(const QString &fieldName,
											 const QModelIndex &index) const
{
	return fieldsValueMap(QStringList() << fieldName, index).value(fieldName);
}

QVariantMap QstAbstractModelHandler::fieldsValueMap(const QStringList &fieldNames,
													const QModelIndex &index) const
{
	Q_ASSERT(isLoaded());
	if (!isLoaded()) return QVariantMap();

	QStringList realFieldNames = fieldNames.isEmpty()
			? _batch.fieldNames() : fieldNames;
	int row = index.row() < 0 ? 0 : index.row();

	QVariantMap resMap;
	foreach (QString fieldName, realFieldNames)
		resMap[fieldName] = _fieldData(row,
									   _batch.columnIndex(fieldName),
									   index.parent(),
									   _modelViewDescriptor.model());
	return resMap;
}

QVariantList QstAbstractModelHandler::columnValuesList(const QString fieldName) const
{
	Q_ASSERT(isLoaded());
	if (!isLoaded()) return QVariantList();

	QAbstractItemModel *model = _modelViewDescriptor.model();
	int column   = _batch.columnIndex(fieldName);
	int rowCount = model->rowCount();

	QVariantList list;
	for (int i = 0; i < rowCount; i++)
		list.append(model->data(model->index(i, column)));
	return list;
}

QVariant QstAbstractModelHandler::fieldValue(const QstBatch batch,
											 const QString &fieldName,
											 const int &row,
											 const QSqlDatabase &db)
{
	return fieldsValueMap(batch, QStringList() << fieldName, row, db).value(fieldName);
}

QVariantMap QstAbstractModelHandler::fieldsValueMap(const QstBatch batch,
													const QStringList &fieldNames,
													const int &row,
													const QSqlDatabase &db)
{
	QstPlainQueryModel m;
	m.setQuery(generateQuery(batch));

	qDebug() << "\nExecution of the query: ";
	qDebug() << generateQuery(batch);

	QStringList realFieldNames = fieldNames.isEmpty()
			? batch.fieldNames()
			: fieldNames;

	QVariantMap resMap;
	foreach (QString fieldName, realFieldNames)
		resMap[fieldName] =  _fieldData(row,
										batch.columnIndex(fieldName),
										QModelIndex(),
										&m);
	return resMap;
}

bool QstAbstractModelHandler::isVirtuallyChechingEnabled() const
{
	if (_modelViewDescriptor.modelType() != TreeModel)
		return false;

	QstTreeQueryModel *treeModel = static_cast<QstTreeQueryModel*>(_modelViewDescriptor.model());
	return treeModel->isVirtuallyCheckingEnabled();
}

QVariantList QstAbstractModelHandler::virtuallyCheckedKeys(const FieldRole &role) const
{
	return QVariantList();
}

QstVariantListMap QstAbstractModelHandler::virtuallyCheckedFields(const QStringList &fieldNames,
																  const QstWhere &conditions) const
{
	return QstVariantListMap();
}

QSqlQuery QstAbstractModelHandler::evalQuery(const QString &query,
											 const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());
	if (!db.isOpen())
		return false;

	qDebug() << "\n Execution of the user defined query: ";
	qDebug() << query;

	QSqlQuery q(db);
	if (!q.exec(query))
		qDebug() << q.lastError().text();
	return q;
}

QString QstAbstractModelHandler::generateQuery(const QstBatch &batch)
{
	QueryBuildDirector buildDir;
	FramedQuery framedQuery = buildDir.build(batch.queryBatch(), queryBuilder());
	return framedQuery.toString();
}

QVariant QstAbstractModelHandler::execute(const QstBatch &batch,
										  const QString &returnValueName,
										  const QSqlDatabase &db)
{
	QString    query = generateQuery(batch);
	QSqlQuery  q = evalQuery(query, db);
	if (q.isActive() && q.isSelect() && q.first())
	{
		QSqlRecord record = q.record();
		return record.value(returnValueName);
	}
	return QVariant();
}

QVariant QstAbstractModelHandler::execute(const QString &returnValueName,
										  const QSqlDatabase &db) const
{
	return execute(_batch, returnValueName, db);
}

void QstAbstractModelHandler::updatePlaceholder(const QString &placeholderName,
												const QstValue &qstValue)
{
	Q_ASSERT(_batch != QstBatch());
	_batch.updatePlaceholder(placeholderName, qstValue);
}

void QstAbstractModelHandler::updatePlaceholder(const QString &placeholderName,
												const QVariant &value,
												const Functor &functor,
												const PercentPlaceholders &percentPlaceholders)
{
	Q_ASSERT(_batch != QstBatch());
	_batch.updatePlaceholder(placeholderName, value, functor, percentPlaceholders);
}

void QstAbstractModelHandler::updatePlaceholder(const QString &placeholderName,
												const QVariantList &varList,
												const Functor &functor)
{
	Q_ASSERT(_batch != QstBatch());
	_batch.updatePlaceholder(placeholderName, varList, functor);
}

void QstAbstractModelHandler::updatePlaceholders(const QVariantMap &varMap)
{
	Q_ASSERT(_batch != QstBatch());
	_batch.updatePlaceholders(varMap);
}

void QstAbstractModelHandler::resetPlaceholders()
{
	_batch.resetPlaceholders();
}

ModelViewDescriptor *QstAbstractModelHandler::modelViewDescriptor()
{
	return &_modelViewDescriptor;
}

QAbstractItemModel *QstAbstractModelHandler::model() const
{
	return _modelViewDescriptor.model();
}

QstBatch QstAbstractModelHandler::batch() const
{
	return _batch;
}

void QstAbstractModelHandler::_unload()
{
	_modelViewDescriptor.clear();
	_loaded = false;
}

QVariant QstAbstractModelHandler::_fieldData(const int &row,
											 const int &column,
											 const QModelIndex &parent,
											 const QAbstractItemModel *model)
{
	Q_ASSERT(model != NULL);

	if (model->hasIndex(row, column, parent))
		return model->data(model->index(row, column, parent));
	return QVariant();
}

QModelIndexList QstAbstractModelHandler::_virtuallyCheckedIndexes(QAbstractItemModel *model) const
{
	QstTreeQueryModel *treeModel = static_cast<QstTreeQueryModel*>(model);
	return treeModel->virtuallyCheckedIndexes();
}

QstVariantListMap QstAbstractModelHandler::_virtuallyCheckedFields(const QStringList &fieldNames,
																   const QstWhere &conditions) const
{
//	QModelIndexList checkedIndexes = _virtuallyCheckedIndexes(_modelViewDescriptor.model());
//	QStringList		realFieldNames = (fieldNames.isEmpty()) ? _batch.fieldNames() : fieldNames;

	QstVariantListMap resMap;

//	foreach (QString fieldName, realFieldNames)
//	{
//		QVariantList list;
//		foreach (QModelIndex index, checkedIndexes)
//		{
//			bool pass = true;
//			foreach (QstField conditionField, conditions)
//			{
//				if (!conditionField.testValue(_fieldData(btch, conditionField.name(), index)))
//				{
//					pass = false;
//					break;
//				}
//			}

//			if (pass)
//				list.append(_fieldData(btch, fieldName, index));
//		}
//		resMap[fieldName] = list;
//	}
return resMap;
}

QModelIndex QstAbstractModelHandler::_getIndex(const QVariant &keyValue, QAbstractItemModel *model) const
{
	Q_ASSERT(model != NULL);

	QModelIndexList list;

	list = model->match(QModelIndex(),
						Qt::DisplayRole,
						keyValue,
						1,
						Qt::MatchExactly);

	if (list.isEmpty() || !list.first().isValid())
		return QModelIndex();

return list.first();
}

void QstAbstractModelHandler::_viewExpandTo(const QModelIndex &index,
											QTreeView *view)
{
	if (index.isValid())
		_viewExpandTo(index.parent(), view);

	view->expand(index);
}

}	// End of namespace Qst
