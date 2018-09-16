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

#ifndef QSTABSTRACTMODELHANDLER_H
#define QSTABSTRACTMODELHANDLER_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QTableView>
#include <QTreeView>
#include <QListView>
#include <QComboBox>

#include <QAbstractItemView>

#include <QMap>
#include <QVariantMap>

#include "qstglobal.h"

#include "qst/special/modelviewdescriptor.h"

#include "qst/mvc/qstplainquerymodel.h"
#include "qst/mvc/qsttreequerymodel.h"

#include "querygeneration/abstractquerybuilder.h"
#include "qstbatch.h"
#include "qstvalue.h"

namespace Qst
{

typedef QMap<QString, QVariantList> QstVariantListMap;

class QstAbstractModelHandler	//FIX ME: documentation // FULL REWRITE
{
protected:

	static AbstractQueryBuilder *_queryBuilder;

	Qst::Special::ModelViewDescriptor _modelViewDescriptor;

	QstBatch _batch;

	bool _loaded;

public:

	static void setQueryBuilder(AbstractQueryBuilder *queryBuilder);
	static AbstractQueryBuilder *queryBuilder();

	QstAbstractModelHandler();
	virtual ~QstAbstractModelHandler() = 0;

	virtual bool reload(const QSqlDatabase &db = QSqlDatabase::database());
	virtual void unload();
	virtual bool isLoaded() const;

	virtual void setQuery(const QstBatch &batch);
	virtual void setQuery(const QString  &queryString);
	virtual void setModel(QstPlainQueryModel *model);
	virtual void setModel(QstTreeQueryModel  *model);

	void setView     (QAbstractItemView *view, const ViewType &viewType);
	void setTableView(QTableView *tableView);
	void setTreeView (QTreeView  *treeView);
	void setListView (QListView  *listView);
	void setComboBox (QComboBox  *comboBox);

	void setViewSorting   (QAbstractItemView *view, const bool &sortingEnabled);
	void setViewAppearance(QAbstractItemView *view, const Qst::Special::ViewAppearance app);
	void setViewSetting   (QAbstractItemView *view,
						   const QString &fieldName,
						   const SettingType &settingType,
						   const QVariant &settingValue);

	bool unsetView(QAbstractItemView *view = NULL);
	bool unsetComboBox(QComboBox *comboBox = NULL);

	virtual void clear();

// ------------------------------------------------------------------------ //

	virtual QModelIndex modelIndex(const QVariant &keyValue,
								   const QAbstractItemView *view) const; // FIX ME: documentation. view for the sortFilterModel.

	virtual int fieldColumnIndex(const FieldRole &role) const;
	virtual int fieldColumnIndex(const QString &fieldName) const;

	virtual void setViewCurrentRow(const QVariant &keyValue,
								   QAbstractItemView *view = NULL);

	virtual void setComboBoxCurrentRow(const QVariant &keyValue,
									   QComboBox *comboBox = NULL);

// ------------------------------------------------------------------------ //
// Группа функций, извлекающих поля из модели для указанной
// или текущей строки во view (comboBox).

	virtual QVariantList viewSelectedKeys  (const FieldRole &role = RolePrimaryKey, QAbstractItemView *view = NULL) const;
	virtual QVariant     viewKeyValue      (const FieldRole &role = RolePrimaryKey, QAbstractItemView *view = NULL)  const;
	virtual QVariant     viewFieldValue    (const QString &fieldName, QAbstractItemView *view = NULL)  const;
	virtual QVariantMap  viewFieldsValueMap(const QStringList &fieldNames = QStringList(), QAbstractItemView *view = NULL)  const;

	virtual void viewExpandTo(const QVariant &keyValue,
							  QAbstractItemView *view = NULL,
							  const bool &setSelected = true); // FIX ME: documentation. // not work for sortfiltering.

	virtual QVariant comboBoxKeyValue  (const FieldRole &role = RolePrimaryKey, QComboBox *comboBox = NULL) const;
	virtual QVariant comboBoxFieldValue(const QString &fieldName, QComboBox *comboBox = NULL) const;
	virtual QVariantMap comboBoxFieldsValueMap(const QStringList &fieldNames = QStringList(),
											   QComboBox *comboBox = NULL) const;

	virtual QVariant    fieldValue(const QString &fieldName, const QModelIndex &index = QModelIndex()) const;
	virtual QVariantMap fieldsValueMap(const QStringList &fieldNames = QStringList(),
									   const QModelIndex &index = QModelIndex()) const;
	virtual QVariantList columnValuesList(const QString fieldName) const;
	static QVariant     fieldValue(const QstBatch batch,
								   const QString &fieldName,
								   const int &row = 0,
								   const QSqlDatabase &db = QSqlDatabase::database());
	static QVariantMap  fieldsValueMap(const QstBatch batch,
									   const QStringList &fieldNames = QStringList(),
									  const int &row = 0,
									  const QSqlDatabase &db = QSqlDatabase::database());

	// FIX ME: code removed
	virtual bool				isVirtuallyChechingEnabled() const;
	virtual QVariantList		virtuallyCheckedKeys(const FieldRole &role = RolePrimaryKey) const;		// FIX ME
	virtual QstVariantListMap	virtuallyCheckedFields(const QStringList &fieldNames,
													   const QstWhere &conditions) const;

	static  QSqlQuery evalQuery(const QString &query,
								const QSqlDatabase &db = QSqlDatabase::database());

	static QString generateQuery(const QstBatch &batch);
	static QVariant execute(const QstBatch &batch,
							const QString &returnValueName = QString("return_value"),
							const QSqlDatabase &db = QSqlDatabase::database());
	QVariant execute(const QString &returnValueName = QString("return_value"),
					 const QSqlDatabase &db = QSqlDatabase::database()) const;

// -------------------------------------------------------------------------- //

	virtual void updatePlaceholder(const QString &placeholderName,
								   const QstValue &qstValue);

	virtual void updatePlaceholder(const QString &placeholderName,
								   const QVariant &value,
								   const Functor &functor = NoFunctor,
								   const PercentPlaceholders &percentPlaceholders = NoPercentPlaceholders);

	virtual void updatePlaceholder(const QString &placeholderName,
								   const QVariantList &varList,
								   const Functor &functor = NoFunctor);

	virtual void updatePlaceholders(const QVariantMap &varMap);
	virtual void resetPlaceholders();

// -------------------------------------------------------------------------- //

	virtual Qst::Special::ModelViewDescriptor *modelViewDescriptor();
	virtual QAbstractItemModel *model() const;
	virtual QstBatch batch() const;

protected:
	virtual void _unload();

	static QVariant _fieldData(const int &row,
							   const int &column,
							   const QModelIndex &parent,
							   const QAbstractItemModel *model);

	virtual QModelIndexList		_virtuallyCheckedIndexes(QAbstractItemModel *model) const;	// FIX ME
	virtual QstVariantListMap	_virtuallyCheckedFields(const QStringList &fieldNames,		// FIX ME
														const QstWhere &conditions) const;

	virtual QModelIndex _getIndex(const QVariant &keyValue,
								  QAbstractItemModel *model) const;

	virtual void _viewExpandTo(const QModelIndex &index,
							   QTreeView *view); // FIX ME: documentation
};

}

#endif // QSTABSTRACTMODELHANDLER_H
