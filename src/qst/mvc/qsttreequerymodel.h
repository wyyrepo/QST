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

#ifndef QSTTREEQUERYMODEL_H
#define QSTTREEQUERYMODEL_H


#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "qst/qstglobal.h"
#include "qst/mvc/abstracttreemodel.h"

namespace Qst
{

class QstTreeQueryModel : public Common::AbstractTreeModel
{
	Q_OBJECT

public:

	typedef QPair<QString, FieldRoleIndexMap> ExternalQuery;
	typedef QMap<int, ExternalQuery>  ExternalQueriesMap; // int - parent item's level an external query for.

private:

	FieldRoleIndexMap _fieldRoleIndexMap;

	QSqlError _lastError;
	QSqlQuery _query;

	bool _loadOnExpandMode;
	int  _defaultParentItemLevel;
	ExternalQueriesMap _leveledQueriesMap;

public:

	QstTreeQueryModel(QObject* parent = NULL);
	virtual ~QstTreeQueryModel();

	virtual void setFieldRoleIndex(const FieldRole &role, const int &index);
	virtual void setFieldRoleIndexMap(const FieldRoleIndexMap &fieldRoleIndexMap);

	virtual void setLoadOnExpandMode(const bool &loadOnExpand);
	virtual void setExternalQuery(const int &parentItemLevel,
								  const QString &query,
								  const FieldRoleIndexMap &fieldRoleIndexMap);
	virtual void setDefaultParentItemLevel(const bool &parentItemLevel);

	virtual void setQuery(const QSqlQuery &query);
	virtual QSqlQuery setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase::database());

// Reimplemented functions.

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
	virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
	virtual void fetchMore(const QModelIndex &parent = QModelIndex());

	virtual void clear();

	QSqlError lastError() const;

protected:

	virtual void _setupTree();

	virtual Common::TreeItem::ItemDataMap _columnTitles(const QSqlRecord &record) const; // FIX ME: documentation
	virtual Common::TreeItem::ItemDataMap _itemDataMap(const QSqlRecord &record) const; // FIX ME: documentation
	virtual Qt::ItemFlags _itemFlags(const QSqlRecord &record) const; // FIX ME: documentation
	virtual bool _isSelectable(const QSqlRecord &record) const; // FIX ME: documentation


	void _fetchMore(const QModelIndex &parent);
};

}

#endif // QSTTREEQUERYMODEL_H
