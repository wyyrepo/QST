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

#include "querybatch.h"

using namespace Qst;

#include <QDebug>

/*!

Структура QueryBatch - иерархическая.

-= QuerySelect =-

QueryBatch (QuerySelect, name, BatchSource, ClauseNone)
|
|__ _batchPacks:
	|__<ClauseSelect>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseSelect)
	|__<ClauseFrom>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseFrom)
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseFrom)
		|__ ~
	|__<ClauseFrom>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseFrom)
			|__ fields(ClauseFrom)


	|__<ServiceJoinClause_Mask>

		 |__ QueryBatch (QueryJoin, _, BatchSource, ServiceJoinClause_Mask)
			 |__ <ClauseLeftJoin>
				 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseLeftJoin)
			 |__ <ClauseOn>
				 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseOn)
						|__ QueryWhere (condition tree)

		 |__ QueryBatch (QueryJoin, _, BatchSource, ServiceJoinClause_Mask)
			 |__ <ClauseRightJoin>
				 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseRightJoin)
			 |__ <ClauseOn>
				 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseOn)
						|__ QueryWhere (condition tree)

		 |__ QueryBatch (QueryJoin, _, BatchSource, ServiceJoinClause_Mask)
			 |__ <ClauseInnerJoin>
				 |__ QueryBatch (QuerySelect, alias, BatchSource, ClauseInnerJoin)
					 |
					 |__ _batchPacks:
						 |__<ClauseSelect>
							 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseSelect)
						 |__<ClauseFrom>
							 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseFrom)
							 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseFrom)
							 |__ ~
			 |__ <ClauseOn>
				 |__ QueryBatch (QueryUndefined, name, StringSource, ClauseOn)
						|__ QueryWhere (condition tree)

		 |__ ~

	|__<ClauseWhere>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseWhere)
				|__ QueryWhere (condition tree)

	|__<ClauseGroupBy>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseGroupBy)

	|__<ClauseHaving>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseHaving)

	|__<ClauseOrderBy>
		|__ QueryBatch (QueryUndefined, name, StringSource, ClauseOrderBy)


-= QueryInsert =-

	QueryBatch (QueryInsert, name, BatchSource, ClauseNone)
	|
	|__ _batchPacks:
		|__<ClauseInsert>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseInsert)
		|__<ClauseValues>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseValues)
				|__ fields(ClauseValues)

-= QueryUpdate =-

	QueryBatch (QueryUpdate, name, BatchSource, ClauseNone)
	|
	|__ _batchPacks:
		|__<ClauseUpdate>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseUpdate)
		|__<ClauseSet>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseSet)
				|__ fields(ClauseSet)
		|__<ClauseWhere>
				|__ QueryBatch (QueryUndefined, name, StringSource, ClauseWhere)
						|__ QueryWhere (condition tree)

-= QueryDelete =-

	QueryBatch (QueryDelete, name, BatchSource, ClauseNone)
	|
	|__ _batchPacks:
		|__<ClauseDelete>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseDelete)
		|__<ClauseWhere>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseWhere)
					|__ QueryWhere (condition tree)

-= QueryExecute =-

	QueryBatch (QueryExecute, name, BatchSource, ClauseNone)
	|
	|__ _batchPacks:
		|__<ClauseExecute>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseExecute)
		|__<ClauseParameters>
			|__ QueryBatch (QueryUndefined, name, StringSource, ClauseParameters)
				|__ fields(ClauseParameters)

-= UserDefinedQuery =-

 QueryBatch (UserDefinedQuery, name, BatchSource, ClauseNone)
 |
 |__ _batchPacks:
	 |__<UserDefinedClauses>
		 |__ QueryBatch (QueryUndefined, name, StringSource, UserDefinedClauses)
*/


QueryClauseList QueryBatch::_placeholderedClauses()
{
	return QueryClauseList()
			<< ClauseParameters
			<< ClauseValues
			<< ClauseWhere
			<< ClauseSet;
}

QueryBatch::QueryBatch()
	:
	  _name(QString()),
	  _sourceType(StringSource),
	  _queryType(QueryUndefined),
	  _queryClause(NoClause)
{
}

QueryBatch::QueryBatch(const QueryType &queryType,
					   const QString &name,
					   const Qst::QueryClause &queryClause)
	:
	  _name(name),
	  _sourceType(BatchSource),
	  _queryType(queryType),
	  _queryClause(queryClause)
{
	if (queryType == UserDefinedQuery)
		setUserDefinedQuery(name);
}

QueryBatch::QueryBatch(const Qst::QueryClause &queryClause,
					   const QString &name)
	:
	  _name(name),
	  _sourceType(StringSource),
	  _queryType(QueryUndefined),
	  _queryClause(queryClause)
{
	Q_ASSERT(queryClause != NoClause);
}

QueryBatch::QueryBatch(const QString &name)
	:
	  _name(name),
	  _sourceType(StringSource),
	  _queryType(QueryUndefined),
	  _queryClause(NoClause)
{
}

QueryBatch::QueryBatch(const QueryWhere &condition, const Qst::QueryClause &queryClause)
	:
	  _name(QString()),
	  _sourceType(StringSource),
	  _queryType(QueryUndefined),
	  _queryClause(queryClause),
	  _condition(condition)
{
}

QueryBatch::QueryBatch(const QueryClause &clause,
					   const QString &name,
					   const SourceType &sourceType,
					   const QueryType &queryType)
	:
	  _name(name),
	  _sourceType(sourceType),
	  _queryType(queryType),
	  _queryClause(clause)
{
}

void QueryBatch::setName(const QString &name)
{
	_name = name;
}

QString QueryBatch::name() const
{
	return _name;
}

QString QueryBatch::alias() const
{
	if (_queryClause == NoClause)
		return "";
	return _name;
}

QueryBatch::SourceType QueryBatch::sourceType() const
{
	return _sourceType;
}

void QueryBatch::setQueryClause(const Qst::QueryClause &queryClause)
{
	_queryClause = queryClause;
}

Qst::QueryClause QueryBatch::queryClause() const
{
	return _queryClause;
}

void QueryBatch::setQueryType(const Qst::QueryType &queryType)
{
	_queryType = queryType;
}

Qst::QueryType QueryBatch::queryType() const
{
	return _queryType;
}

QueryBatchPack QueryBatch::batchPack() const
{
	return _batchPack;
}

QueryBatchList QueryBatch::batchList(const Qst::QueryClause &clause) const
{
	QueryClause packClause = _clauseInBatchPack(clause);

	if (packClause != NoClause && _batchPack.contains(packClause))
		return _batchPack[packClause];

	return QueryBatchList();
}

void QueryBatch::setBatchList(const Qst::QueryClause &queryClause,
							  const QueryBatchList &batchList)
{
	_batchPack[queryClause] = batchList;
}

/*! Добавляет batch в конец слота с индексом packClause.*/
void QueryBatch::addBatch(const Qst::QueryClause packClause,
						  const QueryBatch &batch)
{
	_batchPack[packClause].append(batch);
}

QueryWhere QueryBatch::condition() const
{
	return _condition;
}

QueryWhere QueryBatch::validOnlyConditions(const Qst::QueryClause &condClause) const
{
	if (_batchPack.contains(condClause) && !_batchPack[condClause].isEmpty())
	{
		if (_batchPack[condClause].count() == 1)
		{
			QueryWhere w = _batchPack[condClause].first().validOnlyConditions(condClause);
			if (w.isValid()) return w;

			return QueryWhere();
		}

		QueryWhere::ConditionList list;
		foreach (QueryBatch b, _batchPack[condClause])
		{
			QueryWhere w = b.condition().validOnly();
			if (w.isValid()) list.append(w);
		}

		if (list.isEmpty()) return QueryWhere();
		if (list.count() == 1) return list.first();

		QueryWhere w;
		w.setRelation(Qst::And);
		w.setConditionList(list);
		return w;
	}

	return _condition.validOnly();
}

void QueryBatch::setCondition(const QueryWhere &cond)
{
	_condition = cond;
}

bool QueryBatch::contains(const QueryField &field) const
{
	return _fieldList.contains(field);
}

QStringList QueryBatch::fieldNames() const
{
QStringList resList;

	for (int i = 0; i < _fieldList.count(); ++i)
		resList << _fieldList[i].name();

	return resList;
}

/*!
	Returns source names for StringSource-type batch, if name not empty.
	Otherwise returns empty list.

	If you need batch name, use name() function instead.
*/
QStringList QueryBatch::sourceNames() const
{
	if (_sourceType == StringSource && !_name.isEmpty())
		return QStringList() << _name;

	return QStringList();
}

void QueryBatch::setFields(const QueryFieldList &fields)
{
	_fieldList = fields;
}

QueryFieldList QueryBatch::fields(const QueryClause &queryClause) const
{
	QueryFieldList fieldList;

	for (int i = 0; i < _fieldList.count(); ++i)
		if (_fieldList[i].testClause(queryClause))
			fieldList.append(_fieldList[i]);

return fieldList;
}

QueryFieldList QueryBatch::fieldsList() const
{
	return _fieldList;
}

void QueryBatch::addField(const QueryField &field)
{
	_fieldList.append(field);
}

QueryBatch & QueryBatch::updatePlaceholder(const QString &placeholderName,
										   const QueryValue &value,
										   const Qst::Functor &functor)
{
	foreach (Qst::QueryClause cl, _placeholderedClauses())
	{
		if (_batchPack.contains(cl))
			for (int i = 0; i < _batchPack[cl].count(); ++i)
				_batchPack[cl][i].updatePlaceholder(placeholderName, value, functor);
	}

	_condition.updatePlaceholder(placeholderName, value, functor);
	_updateFieldListPlaceholders(placeholderName, value, functor);
	return (*this);
}

QueryBatch & QueryBatch::updatePlaceholder(const QString &placeholderName,
										   const QVariantList &varList,
										   const Qst::Functor &functor)
{
	foreach (Qst::QueryClause cl, _placeholderedClauses())
	{
		if (_batchPack.contains(cl))
			for (int i = 0; i < _batchPack[cl].count(); ++i)
				_batchPack[cl][i].updatePlaceholder(placeholderName, varList, functor);
	}

	_condition.updatePlaceholder(placeholderName, varList, functor);
	_updateFieldListPlaceholders(placeholderName, varList, functor);
	return (*this);
}

QueryBatch & QueryBatch::updatePlaceholder(const QVariantMap &varMap)
{
	foreach (Qst::QueryClause cl, _placeholderedClauses())
	{
	if (_batchPack.contains(cl))
		for (int i = 0; i < _batchPack[cl].count(); ++i)
			_batchPack[cl][i].updatePlaceholder(varMap);
	}

	QVariantMap::const_iterator iter = varMap.begin();
	while (iter != varMap.end())
	{
		_updateFieldListPlaceholders(iter.key(),
									 QueryValue(iter.value(), NoPercentPlaceholders),
									 NoFunctor);
		iter++;
	}
	return (*this);
}

void QueryBatch::resetPlaceholders()
{
	foreach (Qst::QueryClause cl, _placeholderedClauses())
	{
		if (_batchPack.contains(cl))
			for (int i = 0; i < _batchPack[cl].count(); ++i)
				_batchPack[cl][i].resetPlaceholders();
	}

	_condition.resetPlaceholders();

	for (int i = 0; i < _fieldList.count(); ++i)
		_fieldList[i].resetPlaceholder();
}

/*! Добавляет batch в "свой" слот - слот с индексом batch.queryClause(). */
QueryBatch & QueryBatch::operator<<(const QueryBatch &batch)
{
	Q_ASSERT(batch.queryClause() != NoClause);
	if (batch.queryClause() == NoClause)
		return (*this);

	addBatch(batch.queryClause(), batch);
	return *this;
}

QueryBatch & QueryBatch::operator<<(const QueryWhere &condition)
{
	Q_ASSERT(_sourceType == BatchSource);
	_appendCondition(condition);
	return *this;
}

QueryBatch & QueryBatch::operator<<(const QueryField &field)
{
	QueryClause clause = field.clause();

	if (_sourceType == BatchSource)
	{
		if (clause == ClauseWhere)
		{
			addBatch(ClauseWhere, QueryBatch(QueryWhere(field)));
		}
		else
		{
			if (_batchPack[clause].isEmpty())
				addBatch(clause, QueryBatch(clause, QString()));

			_batchPack[clause].last()._addField(field);
		}
	}
	else
		_addField(field);

	return *this;
}

QueryBatch & QueryBatch::operator<<(const QString &sourceName)
{
	QueryClause clause;
	switch (_queryType)
	{
	case QuerySelect:
	case QueryDelete: clause = ClauseFrom; break;
	case QueryInsert: clause = ClauseInsert; break;
	case QueryUpdate: clause = ClauseUpdate; break;
	case QueryExecute: clause = ClauseExecute; break;

	default: Q_ASSERT(false);
	}

	addBatch(clause, QueryBatch(clause, sourceName));
	return *this;
}

QueryBatch & QueryBatch::setUserDefinedQuery(const QString &query)
{
	Q_ASSERT(queryType() == UserDefinedQuery);
	Q_ASSERT(!query.isEmpty());

	QueryBatch btch = QueryBatch(UserDefinedClauses, query);
	(*this) << btch;
	return (*this);
}

QueryBatch & QueryBatch::select(const QueryBatch &batch)
{
	Q_ASSERT(queryType() == QuerySelect);

	QueryBatch btch = batch;
	btch.setQueryClause(ClauseSelect);
	(*this) << btch;
	return (*this);
}

QueryBatch & QueryBatch::select(const QueryFieldList &fields)
{
	Q_ASSERT(queryType() == QuerySelect);
	Q_ASSERT(!fields.isEmpty());

	foreach (QueryField field, fields)
	{
		Q_ASSERT(!field.name().isEmpty());

		field.setClause(ClauseSelect);
		(*this) << field;
	}
	return (*this);
}

QueryBatch & QueryBatch::select(const QStringList &fieldNames)
{
	Q_ASSERT(queryType() == QuerySelect);
	Q_ASSERT(!fieldNames.isEmpty());

	foreach (QString name, fieldNames)
	{
		Q_ASSERT(!name.isEmpty());
		(*this) << QueryField(name, ClauseSelect);
	}

return (*this);
}

QueryBatch & QueryBatch::select(const QString &fieldName)
{
	Q_ASSERT(queryType() == QuerySelect);
	Q_ASSERT(!fieldName.isEmpty());

	(*this) << QueryField(fieldName,
						  QueryValue(),
						  Qst::NoFunctor,
						  "",
						  Qst::ClauseSelect);
	return (*this);
}

QueryBatch & QueryBatch::from(const QueryBatch &batch)
{
	QueryBatch btch(batch);

	btch.setQueryClause(ClauseFrom);

	(*this) << btch;
	return (*this);
}

QueryBatch & QueryBatch::from(const QString &source)
{
	Q_ASSERT(!source.isEmpty());

	(*this) << QueryBatch(ClauseFrom, source);
	return (*this);
}

QueryBatch & QueryBatch::join(const QueryBatch &batch,
							  const QueryOn &condition)
{
	Q_ASSERT(ServiceJoinClause_Mask &  batch.queryClause());
	Q_ASSERT(ServiceJoinClause_Mask != batch.queryClause());

	QueryBatch joinQueryBatch = QueryBatch(ServiceQueryJoin, "", ServiceJoinClause_Mask);

	joinQueryBatch.addBatch(batch.queryClause(), batch);
	joinQueryBatch.addBatch(ClauseOn, QueryBatch(condition, ClauseOn));

	addBatch(ServiceJoinClause_Mask, joinQueryBatch);
	return (*this);
}

QueryBatch & QueryBatch::join(const Qst::QueryClause &joinClause,
							  const QString &tableName,
							  const QueryOn &condition)
{
	Q_ASSERT(ServiceJoinClause_Mask & joinClause);
	Q_ASSERT(!tableName.isEmpty());

	join(QueryBatch(joinClause, tableName), condition);
	return (*this);
}

QueryBatch & QueryBatch::innerJoin(const QString &tableName,
								   const QueryOn &condition)
{
	Q_ASSERT(!tableName.isEmpty());

	join(QueryBatch(ClauseInnerJoin, tableName), condition);
	return (*this);
}

QueryBatch & QueryBatch::leftJoin(const QString &tableName,
								  const QueryOn &condition)
{
	Q_ASSERT(!tableName.isEmpty());

	join(QueryBatch(ClauseLeftJoin, tableName), condition);
	return (*this);
}

QueryBatch & QueryBatch::rightJoin(const QString &tableName,
								   const QueryOn &condition)
{
	Q_ASSERT(!tableName.isEmpty());

	join(QueryBatch(ClauseRightJoin, tableName), condition);
	return (*this);
}

QueryBatch & QueryBatch::fullJoin(const QString &tableName,
								  const QueryOn &condition)
{
	Q_ASSERT(!tableName.isEmpty());

	join(QueryBatch(ClauseFullJoin, tableName), condition);
	return (*this);
}

QueryBatch & QueryBatch::where(const QueryWhere &condition)
{
	_appendCondition(condition);
	return (*this);
}

QueryBatch & QueryBatch::where(const QString &strCond)
{
	Q_ASSERT(!strCond.isEmpty());
	_appendCondition(QueryWhere(strCond));
	return (*this);
}

QueryBatch & QueryBatch::where(const QString &fieldName,
							   const QueryValue &value,
							   const Qst::Functor &functor)
{
	Q_ASSERT(!fieldName.isEmpty());
	_appendCondition(QueryWhere(QueryField(fieldName, value, functor, "", ClauseWhere)));
	return (*this);
}

QueryBatch & QueryBatch::where(const QString &fieldName,
							   const QVariant &value1,
							   const QVariant &value2)
{
	Q_ASSERT(!fieldName.isEmpty());
	_appendCondition(QueryWhere(fieldName, value1, value2));
	return (*this);
}

QueryBatch & QueryBatch::where(const QString &fieldName,
							   const Qst::QstPlaceholder &placeholder)
{
	Q_ASSERT(!fieldName.isEmpty());

	QueryField f = QueryField(fieldName, placeholder.name(),
							  Qst::NoFunctor, ClauseWhere);
	_appendCondition(QueryWhere(f));
	return (*this);
}

QueryBatch & QueryBatch::groupBy(const QStringList &fieldNames)
{
	Q_ASSERT(!fieldNames.isEmpty());

	foreach (QString f, fieldNames)
	{
		Q_ASSERT(!f.isEmpty());
		(*this) << QueryField(f, ClauseGroupBy);
	}

	return (*this);
}

QueryBatch & QueryBatch::groupBy(const QString &fieldName)
{
	Q_ASSERT(!fieldName.isEmpty());

	(*this) << QueryField(fieldName, ClauseGroupBy);
	return (*this);
}

QueryBatch & QueryBatch::having(const QueryHaving &condition)
{
	addBatch(ClauseHaving, QueryBatch(condition));
	return (*this);
}

QueryBatch & QueryBatch::having(const QString &strCond)
{
	Q_ASSERT(!strCond.isEmpty());

	addBatch(ClauseHaving, QueryBatch(QueryHaving(strCond), ClauseHaving));
	return (*this);
}

QueryBatch & QueryBatch::orderBy(const QStringList &fieldNames)
{
	Q_ASSERT(!fieldNames.isEmpty());

	foreach (QString f, fieldNames)
	{
		Q_ASSERT(!f.isEmpty());
		(*this) << QueryField(f, ClauseOrderBy);
	}

	return (*this);
}

QueryBatch & QueryBatch::orderBy(const QString &fieldName)
{
	Q_ASSERT(!fieldName.isEmpty());

	(*this) << QueryField(fieldName, ClauseOrderBy);
	return (*this);
}

QueryBatch & QueryBatch::insert(const QString &tableName,
								const QStringList &fieldNames)
{
	Q_ASSERT(queryType() == QueryInsert);
	Q_ASSERT(!tableName.isEmpty());

	QueryBatch batch = QueryBatch(ClauseInsert, tableName);

	foreach (QString f, fieldNames)
	{
		Q_ASSERT(!f.isEmpty());
		batch << QueryField(f, ClauseInsert);
	}

	addBatch(ClauseInsert, batch);
	return (*this);
}

QueryBatch & QueryBatch::values(const QueryFieldList &fieldValues)
{
	Q_ASSERT(!fieldValues.isEmpty());

	QueryBatch batch;
	batch.setFields(fieldValues);
	batch.setQueryClause(ClauseValues);

	addBatch(ClauseValues, batch);
	return (*this);
}

QueryBatch & QueryBatch::values(const Qst::QstPlaceholderList &placeholderList)
{
	Q_ASSERT(!placeholderList.isEmpty());

	QueryFieldList fieldList = _getFieldList(placeholderList,
											 ClauseValues,
											 Qst::NoFunctor);

	QueryBatch batch = QueryBatch(ClauseValues, QString());
	batch.setFields(fieldList);

	addBatch(ClauseValues, batch);
	return (*this);
}

QueryBatch & QueryBatch::values(const QueryValueList &values)
{
	Q_ASSERT(!values.isEmpty());

	QueryFieldList list;

	foreach(QueryValue val, values)
		list << QueryField(val, ClauseValues);

	QueryBatch batch = QueryBatch(ClauseValues, QString());
	batch.setFields(list);

	addBatch(ClauseValues, batch);
	return (*this);
}

QueryBatch & QueryBatch::values(const QVariantList &values)
{
	Q_ASSERT(!values.isEmpty());

	QueryFieldList list;

	foreach(QVariant val, values)
		list << QueryField(QueryValue(val, NoPercentPlaceholders), ClauseValues);

	QueryBatch batch = QueryBatch(ClauseValues, QString());
	batch.setFields(list);

	addBatch(ClauseValues, batch);
	return (*this);
}

QueryBatch & QueryBatch::update(const QString &tableName)
{
	Q_ASSERT(queryType() == QueryUpdate);
	Q_ASSERT(!tableName.isEmpty());

	addBatch(ClauseUpdate, QueryBatch(ClauseUpdate, tableName));
	return (*this);
}

QueryBatch & QueryBatch::set(const QueryFieldList &fields)
{
	Q_ASSERT(!fields.empty());

	if (_batchPack[ClauseSet].isEmpty())
		addBatch(ClauseSet, QueryBatch(ClauseSet, QString()));

	QueryFieldList list;
	foreach (QueryField field, fields)
	{
		Q_ASSERT(!field.name().isEmpty());
		field.setClause(ClauseSet);
//		field.setValue(0, QueryValue(field.value(0).value()));
		list << field;
	}
	_batchPack[ClauseSet].last().setFields(list);
	return (*this);
}

QueryBatch & QueryBatch::set(const QVariantMap &varMap)
{
	QueryFieldList list;
	foreach (QString key, varMap.keys())
		list << QueryField(key,
						   QueryValue(varMap.value(key),
									  NoPercentPlaceholders), NoFunctor);

	return set(list);
}

QueryBatch & QueryBatch::deleteFrom(const QString &tableName)
{
	Q_ASSERT(queryType() == QueryDelete);
	Q_ASSERT(!tableName.isEmpty());

	addBatch(ClauseDelete, QueryBatch(ClauseDelete, tableName));
	return (*this);
}

QueryBatch & QueryBatch::execute(const QString &funcName)
{
	Q_ASSERT(queryType() == QueryExecute);
	Q_ASSERT(!funcName.isEmpty());

	addBatch(ClauseExecute, QueryBatch(ClauseExecute, funcName));
	return (*this);
}

QueryBatch & QueryBatch::parameters(const QVariantList &varList)
{
	Q_ASSERT(!varList.isEmpty());

	QueryFieldList fieldList = _getFieldList(varList,
											 ClauseParameters);

	QueryBatch batch = QueryBatch(ClauseParameters, QString());
	batch.setFields(fieldList);

	addBatch(ClauseParameters, batch);
	return (*this);
}

QueryBatch & QueryBatch::parameters(const Qst::QstPlaceholderList &placeholderList)
{
	Q_ASSERT(!placeholderList.isEmpty());

	QueryFieldList fieldList = _getFieldList(placeholderList,
											 Qst::ClauseParameters,
											 Qst::NoFunctor);

	QueryBatch batch = QueryBatch(ClauseParameters, QString());
	batch.setFields(fieldList);

	addBatch(ClauseParameters, batch);
	return (*this);
}

void QueryBatch::_addField(const QueryField &field)
{
	_fieldList.append(field);
}

void QueryBatch::_andCondition(const QueryWhere &cond)
{
	_condition && cond;
}

void QueryBatch::_appendCondition(const QueryWhere &cond)
{
	if (_batchPack[ClauseWhere].isEmpty())
		addBatch(ClauseWhere, QueryBatch(cond));
	else
		_batchPack[ClauseWhere].first()._andCondition(cond);
}

void QueryBatch::_updateFieldListPlaceholders(const QString &placeholderName,
											  const QueryValue &value,
											  const Qst::Functor &functor)
{
	for (int i = 0; i < _fieldList.count(); ++i)
		_fieldList[i].updatePlaceholder(placeholderName, value, functor);
}

void QueryBatch::_updateFieldListPlaceholders(const QString &placeholderName,
											  const QVariantList &varList,
											  const Qst::Functor &functor)
{
	for (int i = 0; i < _fieldList.count(); ++i)
		_fieldList[i].updatePlaceholder(placeholderName, varList, functor);
}

QueryClause QueryBatch::_clauseInBatchPack(const QueryClause &clause) const
{
	QList<QueryClause> keys = _batchPack.keys();

	foreach (QueryClause cl, keys)
		if (clause & cl)
			return cl;

return NoClause;
}

QueryFieldList QueryBatch::_getFieldList(const Qst::QstPlaceholderList &plList,
										 const Qst::QueryClause &clause,
										 const Qst::Functor &functor) const
{
	Q_ASSERT(!plList.isEmpty());
	QueryFieldList fieldList;
		foreach (QstPlaceholder pl, plList)
		{
			Q_ASSERT(!pl.names().isEmpty());
			QueryField f = QueryField(pl.name(), QueryValue(),
									  functor, pl.name(), clause);
			fieldList.append(f);
		}
	return fieldList;
}

QueryFieldList QueryBatch::_getFieldList(const QVariantList &list,
										 const Qst::QueryClause &clause) const
{
	QueryFieldList fieldList;
	foreach (QVariant var, list)
		fieldList.append(QueryField(QueryValue(var, NoPercentPlaceholders), clause));
	return fieldList;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QueryBatch &b)
{
#ifndef Q_BROKEN_DEBUG_STREAM
	dbg.nospace() << "QueryBatch\nname=" << b.name();
	dbg.nospace() << "\nsourceType = " << b.sourceType();
	dbg.nospace() << "\nqueryType = " << b.queryType();
	dbg.nospace() << "\nqueryClause = " << b.queryClause();
	dbg.nospace() << "\nbatchPack = " << b.batchPack();
	dbg.nospace() << "\nfieldList = "   << b.fieldsList();
	dbg.nospace() << "\ncondition = " << b.condition();
	dbg.nospace() << "\nqueryType = " << b.queryType();

	return dbg.space();
#else
	qWarning("This compiler doesn't support streaming QueryValue to QDebug");
	return dbg;
	Q_UNUSED(b);
#endif
}
#endif
