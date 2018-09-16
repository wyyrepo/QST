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

#include "sqlitebuilder.h"

#include "sqliteformat.h"

#include <QDebug>

using namespace Qst;

SqLiteBuilder::SqLiteBuilder()
	:
	AbstractQueryBuilder::AbstractQueryBuilder()
{
	_insertKeyword(ClauseSelect,     SQL_SELECT);
	_insertKeyword(ClauseFrom,       SQL_FROM);
	_insertKeyword(ClauseLeftJoin,   SQL_LEFT_JOIN);
	_insertKeyword(ClauseRightJoin,  SQL_RIGHT_JOIN);
	_insertKeyword(ClauseFullJoin,   SQL_FULL_JOIN);
	_insertKeyword(ClauseInnerJoin,  SQL_INNER_JOIN);
	_insertKeyword(ClauseOn,         SQL_ON);
	_insertKeyword(ClauseWhere,      SQL_WHERE);
	_insertKeyword(ClauseGroupBy,    SQL_GROUP_BY);
	_insertKeyword(ClauseHaving,     SQL_HAVING);
	_insertKeyword(ClauseOrderBy,    SQL_ORDER_BY);
	_insertKeyword(ClauseInsert,     SQL_INSERT);
	_insertKeyword(ClauseValues,     SQL_VALUES);
	_insertKeyword(ClauseUpdate,     SQL_UPDATE);
	_insertKeyword(ClauseSet,        SQL_SET);
	_insertKeyword(ClauseDelete,     SQL_DELETE);
	_insertKeyword(ClauseExecute,    SQL_EXECUTE);
	_insertKeyword(ClauseParameters, SQL_PARAMETERS);
	_insertKeyword(ServiceJoinClause_Mask, "");
	_insertKeyword(UserDefinedClauses,     "");
}

QueryScheme SqLiteBuilder::queryScheme(const QueryType &queryType) const
{
	return AbstractQueryBuilder::queryScheme(queryType);
}

QueryFrame SqLiteBuilder::makeQueryFrame(const QueryBatch &queryBatch,
											 const bool &isSubquery) const
{
	return AbstractQueryBuilder::makeQueryFrame(queryBatch, isSubquery);
}

QueryFrame *SqLiteBuilder::makeKeywordFrame(QueryFrame *subqueryFrame,
											const QueryClause &queryClause) const
{
	if (queryClause & ClauseCondition_Mask)
	{
		QueryFrame *keywordFrame = AbstractQueryBuilder::makeKeywordFrame(subqueryFrame, queryClause);
		return keywordFrame->appendFrame(QueryFrame("(", ")", QueryFrameList()));
	}
	else if (queryClause == ClauseParameters)
	{
		QueryFrame *keywordFrame = AbstractQueryBuilder::makeKeywordFrame(subqueryFrame, queryClause);
		return keywordFrame->appendFrame(QueryFrame("(", ") " + SQL_AS_RETURN_VALUE, QueryFrameList()));
	}
	else
		return AbstractQueryBuilder::makeKeywordFrame(subqueryFrame, queryClause);
}

/*! Компонует один фрейм для одного элемента секции. */
void SqLiteBuilder::makeClauseItemFrame(QueryFrame *parentFrame,
										const QueryBatch &batch,
										const bool &delimiterEnded) const
{
	QueryFrame frame;
	switch (batch.queryClause())
	{
	case ClauseSelect:       frame = _commonClauseFrame(batch.fieldNames(), _itemsDelimiter(ClauseSelect), delimiterEnded); break;
	case ClauseFrom:         frame = _commonClauseFrame(batch.sourceNames(), _itemsDelimiter(ClauseFrom), delimiterEnded);  break;
	case ClauseOn:
	case ClauseHaving:
	case ClauseWhere:        frame = _conditionClauseFrame(batch.condition());    break;
	case ClauseGroupBy:
	case ClauseOrderBy:      frame = _commonClauseFrame(batch.fieldNames(), _itemsDelimiter(ClauseOrderBy), delimiterEnded); break;
	case ClauseUpdate:       frame = _statementClauseFrame(batch.name(), QStringList(), ""); break;
	case ClauseDelete:       frame = _statementClauseFrame(batch.name(), QStringList(), ""); break;
	case ClauseExecute:      frame = _statementClauseFrame(batch.name(), QStringList(), ""); break;
	case ClauseInsert:       frame = _statementClauseFrame(batch.name(), batch.fieldNames(), _itemsDelimiter(ClauseInsert)); break;
	case ClauseValues:       frame = _valuesClauseFrame(batch.fields(ClauseValues), _itemsDelimiter(ClauseValues)); break;
	case ClauseSet:          frame = _setClauseFrame(batch.fields(ClauseSet), _itemsDelimiter(ClauseSet)); break;
	case ClauseParameters:   frame = _parametersClauseFrame(batch.fields(ClauseParameters), _itemsDelimiter(ClauseParameters)); break;
	case ClauseLeftJoin:     frame = _joinClauseFrame(batch.name()); break;
	case ClauseRightJoin:    frame = _joinClauseFrame(batch.name()); break;
	case ClauseFullJoin:     frame = _joinClauseFrame(batch.name()); break;
	case ClauseInnerJoin:    frame = _joinClauseFrame(batch.name()); break;
	case UserDefinedClauses: frame = QueryFrame(batch.name(), "", QueryFrameList()); break;

	case NoClause: case ClauseAll_Mask: case ClauseValued_Mask:
	case ClauseCondition_Mask: case ClauseHasFieldName_Mask:
	case ServiceJoinClause_Mask: default:
		Q_ASSERT(false);
	}

	parentFrame->appendFrame(frame);
}

QueryBatchList SqLiteBuilder::validBatchList(const QueryClause &queryClause,
											 const QueryBatch &batch) const
{
	if (queryClause & ClauseCondition_Mask)
	{
		QueryWhere w = batch.validOnlyConditions(queryClause);
		if (w.isValid())
			return QueryBatchList() << QueryBatch(w);
		else
			return QueryBatchList();
	}
	else if (queryClause == ClauseParameters)
	{
		QueryBatchList list = batch.batchList(queryClause);
		if (list.isEmpty())
			list.append(QueryBatch(ClauseParameters, ""));
		return list;
	}
	else return batch.batchList(queryClause);
}

QString SqLiteBuilder::_composeCondition(const QueryField &field) const
{
	if (field.isValuesValid())
	{
		Functor functor = field.functor();
		StringTemplate conditionTemplate = _conditionTemplate(functor);

			switch (functor)
			{
			case Between: return _betweenCondition(field, conditionTemplate);
			case InList:
			case NotInList: return _inListCondition(field, conditionTemplate);

			default: break;
			};

		QString strCondition = _valueFormat.toString(field.value(0),
													 ToStringTypes_Mask,
													 field.value(0).percentPlaceholders(),
													 true);
		return conditionTemplate.string(field.name(), strCondition);
	}
	else
		if (!field.placeholderName().isEmpty())
			return QString();
return field.name();
}

QueryFrame SqLiteBuilder::_commonClauseFrame(QStringList itemsList,
											 QString delimiter,
											 const bool &delimiterEnded,
											 const QString &leftBracket,
											 const QString &rightBracket) const
{
	QueryFrame frame(leftBracket, rightBracket, QueryFrameList());
	for (int i = 0; i < itemsList.count(); ++i)
	{
		QString res = itemsList[i] + ( (i < itemsList.count()-1) || delimiterEnded ? delimiter + " " : "");
		frame << QueryFrame(res, "", QueryFrameList());
	}
	return frame;
}

QueryFrame SqLiteBuilder::_conditionClauseFrame(const QueryWhere &cond) const
{
	QueryFrame resFrame;
	if (cond.relation() != UndefinedRelation)
	{
		Q_ASSERT(cond.conditionList().count() > 1);
		QString boolOp = " " + _boolOperator(cond.relation()) + " ";
		QueryWhere::ConditionList list = cond.conditionList();
		QueryFrame firstFrame = QueryFrame("(", ")",   QueryFrameList() << _conditionClauseFrame(list.first()));
		QueryFrame opFrame    = QueryFrame(boolOp, "", QueryFrameList());
		resFrame << firstFrame;
		for (int i = 1; i < list.count(); ++i)
		{
			QueryFrame f = QueryFrame("(", ")",   QueryFrameList() << _conditionClauseFrame(list[i]));
			resFrame << opFrame << f;
		}
	}
	else
		resFrame << QueryFrame(_composeCondition(cond.field()), "", QueryFrameList());
	return resFrame;
}

QueryFrame SqLiteBuilder::_joinClauseFrame(const QueryBatch &batch) const
{
	return QueryFrame(batch.name() + " " + batch.alias(), "", QueryFrameList());
}

QueryFrame SqLiteBuilder::_statementClauseFrame(const QString &tableName,
													const QStringList &fieldNames,
													const QString &delimiter) const
{
	if (fieldNames.isEmpty())
		return QueryFrame(tableName, "", QueryFrameList());
	QueryFrame frame(tableName + "(", ")", QueryFrameList());
	for (int i = 0; i < fieldNames.count(); ++i)
	{
		QString res = fieldNames[i] + ( (i < fieldNames.count()-1) ? delimiter + " " : "");
		frame << QueryFrame(res, "", QueryFrameList());
	}
	return frame;
}

QueryFrame SqLiteBuilder::_valuesClauseFrame(const QueryFieldList &fieldValues,
												 const QString &delimiter) const
{
	Q_ASSERT(!fieldValues.isEmpty());

	QStringList list;

	foreach (QueryField field, fieldValues)
		list << _valueFormat.toString(field.value(),
									  ToStringTypes_Mask,
									  NoPercentPlaceholders,
									  true);

return _commonClauseFrame(list, delimiter, false, "(", ")");
}

QueryFrame SqLiteBuilder::_setClauseFrame(const QueryFieldList &fieldValues,
											  const QString &delimiter) const
{
	QStringList list;

	foreach (QueryField field, fieldValues)
		list << field.name() + " = " + _valueFormat.toString(field.value(),
															 ToStringTypes_Mask,
															 NoPercentPlaceholders,
															 true);
return _commonClauseFrame(list, delimiter, false);
}

QueryFrame SqLiteBuilder::_parametersClauseFrame(const QueryFieldList &fieldValues,
													 const QString &delimiter) const
{
	QStringList list;

	foreach (QueryField field, fieldValues)
		list << _valueFormat.toString(field.value(),
									  ToStringTypes_Mask,
									  NoPercentPlaceholders,
									  true);

	return _commonClauseFrame(list, delimiter, false, "", "");
}

QString SqLiteBuilder::_betweenCondition(const QueryField &field,
											 const StringTemplate &strTempl) const
{
	QueryValue val1 = field.value(LeftValue, QueryValue());
	QueryValue val2 = field.value(RightValue, QueryValue());

	QString strVal1 = _valueFormat.toString(val1,
											ToStringTypes_Mask,
											val1.percentPlaceholders(),
											true);

	QString strVal2 = _valueFormat.toString(val2,
											ToStringTypes_Mask,
											val2.percentPlaceholders(),
											true);

	return strTempl.string(field.name(), strVal1, strVal2);
}

QString SqLiteBuilder::_inListCondition(const QueryField &field,
											const StringTemplate &strTempl) const
{
	Q_ASSERT(!field.valueList().isEmpty());

	QStringList list;
	foreach (QueryValue val, field.valueList())
		list << _valueFormat.toString(val,
									  ToStringTypes_Mask,
									  NoPercentPlaceholders,
									  true);

	return strTempl.string(field.name(), _commonClauseFrame(list, ",", false).toString());
}
