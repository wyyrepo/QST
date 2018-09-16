/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the QueryGeneration module of the QsT SQL Tools.
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

#ifndef QUERYBATCH_H
#define QUERYBATCH_H

#include <QMap>
#include <QList>
#include <QString>
#include <QStringList>

#include "queryfield.h"
#include "querywhere.h"

class QueryBatch;

typedef QList<QueryBatch> QueryBatchList;

typedef QMap<Qst::QueryClause, QueryBatchList> QueryBatchPack;

class QueryBatch
{
public:

	typedef enum
	{
		StringSource = 1,
		BatchSource = 2
	} SourceType;

private:

	QString				_name;

	SourceType			_sourceType;
	Qst::QueryType		_queryType;
	Qst::QueryClause    _queryClause;

	QueryBatchPack		_batchPack;
	QueryFieldList		_fieldList;

	QueryWhere			_condition;

	static Qst::QueryClauseList _placeholderedClauses();

public:

	QueryBatch();

	QueryBatch(const Qst::QueryType &queryType,
			   const QString &name = QString(),
			   const Qst::QueryClause &queryClause = Qst::NoClause);

	QueryBatch(const Qst::QueryClause &queryClause,
			   const QString &name);

	QueryBatch(const QString &name);

	QueryBatch(const QueryWhere &condition,
			   const Qst::QueryClause &queryClause = Qst::ClauseWhere);

	QueryBatch(const Qst::QueryClause &clause,
			   const QString &name,
			   const SourceType &sourceType,
			   const Qst::QueryType &queryType);

	void    setName(const QString &name);
	QString name() const;
	QString alias() const;

	SourceType sourceType() const;

	void setQueryClause(const Qst::QueryClause &queryClause);
	Qst::QueryClause queryClause() const;

	void setQueryType(const Qst::QueryType &queryType);
	Qst::QueryType queryType() const;

	QueryBatchPack batchPack() const;
	QueryBatchList batchList(const Qst::QueryClause &queryClause = Qst::ClauseSelect) const;
	void setBatchList(const Qst::QueryClause &queryClause,
					  const QueryBatchList &batchList);

	void addBatch(const Qst::QueryClause packClause,
				  const QueryBatch &batch);

	QueryWhere condition() const;
	QueryWhere validOnlyConditions(const Qst::QueryClause &condClause) const;
	void setCondition(const QueryWhere &cond);

	bool contains(const QueryField &field) const;

	QStringList fieldNames() const;
	QStringList sourceNames() const;

	void setFields(const QueryFieldList &fields);
	QueryFieldList fields(const Qst::QueryClause &queryClause) const;
	QueryFieldList fieldsList() const;
	void addField(const QueryField &field);

	QueryBatch & updatePlaceholder(const QString &placeholderName,
								   const QueryValue &value,
								   const Qst::Functor &functor = Qst::NoFunctor);

	QueryBatch & updatePlaceholder(const QString &placeholderName,
								   const QVariantList &varList,
								   const Qst::Functor &functor = Qst::NoFunctor);

	QueryBatch & updatePlaceholder(const QVariantMap &varMap);

	void resetPlaceholders();

	QueryBatch & operator<<(const QueryBatch &batch);
	QueryBatch & operator<<(const QueryWhere &condition);
	QueryBatch & operator<<(const QueryField &field);
	QueryBatch & operator<<(const QString &sourceName);

//////////////////////////////////////////////////////////////////

	QueryBatch & setUserDefinedQuery(const QString &query);

	QueryBatch & select(const QueryBatch &batch);
	QueryBatch & select(const QueryFieldList &fields);
	QueryBatch & select(const QStringList &fieldNames);
	QueryBatch & select(const QString &fieldName);
	QueryBatch & from(const QueryBatch &batch);
	QueryBatch & from(const QString &source);
	QueryBatch & join(const QueryBatch &batch, const QueryOn &condition);
	QueryBatch & join(const Qst::QueryClause &joinClause,
					  const QString &tableName,
					  const QueryOn &condition);
	QueryBatch & innerJoin(const QString &tableName, const QueryOn &condition);
	QueryBatch & leftJoin(const QString &tableName, const QueryOn &condition);
	QueryBatch & rightJoin(const QString &tableName, const QueryOn &condition);
	QueryBatch & fullJoin(const QString &tableName, const QueryOn &condition);

	QueryBatch & where(const QueryWhere &condition);
	QueryBatch & where(const QString &strCond);
	QueryBatch & where(const QString &fieldName,
					   const QueryValue &value,
					   const Qst::Functor &functor);
	QueryBatch & where(const QString &fieldName,
					   const QVariant &value1,
					   const QVariant &value2);
	QueryBatch & where(const QString &fieldName,
					   const Qst::QstPlaceholder &placeholder);

	QueryBatch & groupBy(const QStringList &fieldNames);
	QueryBatch & groupBy(const QString &fieldName);
	QueryBatch & having(const QueryHaving &condition);
	QueryBatch & having(const QString &strCond);
	QueryBatch & orderBy(const QStringList &fieldNames);
	QueryBatch & orderBy(const QString &fieldName);

	QueryBatch & insert(const QString &tableName,
						const QStringList &fieldNames = QStringList());
	QueryBatch & values(const QueryFieldList &fieldValues);
	QueryBatch & values(const QueryValueList &values);
	QueryBatch & values(const QVariantList &values);
	QueryBatch & values(const Qst::QstPlaceholderList &placeholderList);

	QueryBatch & update(const QString &tableName);
	QueryBatch & set(const QueryFieldList &fields);
	QueryBatch & set(const QVariantMap &varMap);

	QueryBatch & deleteFrom(const QString &tableName);

	QueryBatch & execute(const QString &funcName);

	QueryBatch & parameters(const QVariantList &varList);
	QueryBatch & parameters(const Qst::QstPlaceholderList &placeholderList);


	friend bool operator==(const QueryBatch &btch1, const QueryBatch btch2)
	{
		return btch1._name == btch2._name
				&& btch1._sourceType == btch2._sourceType
				&& btch1._queryType == btch2._queryType
				&& btch1._queryClause == btch2._queryClause
				&& btch1._fieldList == btch2._fieldList
				&& btch1._condition == btch2._condition
				&& btch1._batchPack == btch2._batchPack;
	}

private:

	void _addField(const QueryField &field);
	void _andCondition(const QueryWhere &cond);
	void _appendCondition(const QueryWhere &cond);
	void _updateFieldListPlaceholders(const QString &placeholderName,
									  const QueryValue &value,
									  const Qst::Functor &functor);
	void _updateFieldListPlaceholders(const QString &placeholderName,
									  const QVariantList &varList,
									  const Qst::Functor &functor = Qst::NoFunctor);

	Qst::QueryClause _clauseInBatchPack(const Qst::QueryClause &clause) const;

	QueryFieldList _getFieldList(const Qst::QstPlaceholderList &plList,
								 const Qst::QueryClause &clause,
								 const Qst::Functor &functor) const;
	QueryFieldList _getFieldList(const QVariantList &list,
								 const Qst::QueryClause &clause) const;
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QueryBatch &b);
#endif

#endif // QUERYBATCH_H
