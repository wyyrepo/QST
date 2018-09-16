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

#include "abstractquerybuilder.h"

using namespace Qst;

AbstractQueryBuilder::AbstractQueryBuilder()
{
}

QueryScheme AbstractQueryBuilder::queryScheme(const QueryType &queryType) const
{
	switch (queryType)
	{
	case QuerySelect:
		return QueryScheme()
				<< ClauseSelect
				<< ClauseFrom
				<< ServiceJoinClause_Mask
				<< ClauseWhere
				<< ClauseGroupBy
				<< ClauseHaving
				<< ClauseOrderBy
				;

	// Все join-пакеты отделены друг от друга, в каждом пакете только один JOIN.
	// Только один из ClauseXYZJoin будет задействован при обработке каждого пакета.
	case ServiceQueryJoin:
		return QueryScheme()
				<< ClauseLeftJoin
				<< ClauseRightJoin
				<< ClauseInnerJoin
				<< ClauseFullJoin
				<< ClauseOn
				;

	case QueryInsert:
		return QueryScheme()
				<< ClauseInsert
				<< ClauseValues
				;

	case QueryUpdate:
		return QueryScheme()
				<< ClauseUpdate
				<< ClauseSet
				<< ClauseWhere
				;

	case QueryDelete:
		return QueryScheme()
				<< ClauseDelete
				<< ClauseWhere
				;

	case QueryExecute:
		return QueryScheme()
				<< ClauseExecute
				<< ClauseParameters
				;

	case UserDefinedQuery:
		return QueryScheme()
				<< UserDefinedClauses;

	default: Q_ASSERT(false);
	}
return QueryScheme();
}

QueryFrame AbstractQueryBuilder::makeQueryFrame(const QueryBatch &queryBatch,
												const bool &isSubquery) const
{
	if (!isSubquery) return QueryFrame();
	return QueryFrame(_keywords[queryBatch.queryClause()],
					  queryBatch.alias(),
					  QueryFrameList());
}

QueryFrame *AbstractQueryBuilder::makeKeywordFrame(QueryFrame *subqueryFrame,
												   const QueryClause &queryClause) const
{
	QueryFrame keywordFrame = QueryFrame(" " + this->_keyword(queryClause) + " ", "", QueryFrameList());
	return subqueryFrame->appendFrame(keywordFrame);
}

/*! Компонует один фрейм для одного элемента секции. */
void AbstractQueryBuilder::makeClauseItemFrame(QueryFrame *parentFrame,
											   const QueryBatch &batch,
											   const bool &delimiterEnded) const
{
	Q_UNUSED(batch)
	Q_UNUSED(parentFrame)
	Q_UNUSED(delimiterEnded)
}

QueryBatchList AbstractQueryBuilder::validBatchList(const Qst::QueryClause &queryClause,
													const QueryBatch &batch) const
{
	Q_UNUSED(queryClause)
	Q_UNUSED(batch)

	return QueryBatchList();
}

void AbstractQueryBuilder::_insertKeyword(const Qst::QueryClause &clause,
										  const QString &keyword)
{
	_keywords[clause] = keyword;
}

QString AbstractQueryBuilder::_keyword(const QueryClause &clause) const
{
	Q_ASSERT(clause != NoClause);
	Q_ASSERT(_keywords.contains(clause));

	return _keywords.value(clause, QString());
}

StringTemplate AbstractQueryBuilder::_conditionTemplate(const Functor &functor) const
{
	//Здесь %1 - поле, %2 - значение1, %3 - значение2.

		switch(functor)
		{
		case Like:			return StringTemplate(2, "%1 LIKE %2");
		case Equal:			return StringTemplate(2, "%1 = %2");
		case NotEqual:		return StringTemplate(2, "%1 <> %2");
		case Less:			return StringTemplate(2, "%1 < %2");
		case Greater:		return StringTemplate(2, "%1 > %2");
		case LessEqual:		return StringTemplate(2, "%1 <= %2");
		case GreaterEqual:	return StringTemplate(2, "%1 >= %2");
		case Between:		return StringTemplate(3, "%1 BETWEEN %2 AND %3");
		case IsNull:		return StringTemplate(1, "%1 IS NULL");
		case IsNotNull:		return StringTemplate(1, "%1 IS NOT NULL");
		case NotEqualOrNull:		return StringTemplate(2, "(%1 <> %2 OR %1 IS NULL)");
		case EqualOrNull:			return StringTemplate(2, "(%1 == %2 OR %1 IS NULL)");
		case LessOrNull:			return StringTemplate(2, "(%1 < %2 OR %1 IS NULL)");
		case GreaterOrNull:			return StringTemplate(2, "(%1 > %2 OR %1 IS NULL)");
		case LessEqualOrNull:		return StringTemplate(2, "(%1 <= %2 OR %1 IS NULL)");
		case GreaterEqualOrNull:	return StringTemplate(2, "(%1 >= %2 OR %1 IS NULL)");
		case LikeOrNull:			return StringTemplate(2, "(%1 LIKE %2 OR %1 IS NULL)");
		case BitwiseOr:				return StringTemplate(2, "%1 | %2");
		case BitwiseAnd:			return StringTemplate(2, "%1 & %2");
		case BitwiseXor:			return StringTemplate(2, "%1 # %2");
		case BitwiseNot:			return StringTemplate(1, "~%1");
		case BitwiseShiftLeft:		return StringTemplate(2, "%1 << %2");
		case BitwiseShiftRight:		return StringTemplate(2, "%1 >> %2");
		case InList:				return StringTemplate(2, "%1 IN (%2)");
		case NotInList:				return StringTemplate(2, "%1 NOT IN (%2)");

		case NoFunctor: return StringTemplate(2, "%1 = %2");
		};
	return StringTemplate();
}

QString AbstractQueryBuilder::_itemsDelimiter(const QueryClause &clause) const
{
	switch (clause)
	{
	case ClauseSelect:
	case ClauseFrom:
	case ClauseOrderBy: return ",";

	case ClauseInsert:
	case ClauseSet:
	case ClauseParameters:
	case ClauseValues: return ",";

	default: Q_ASSERT(false);
	}
return QString();
}

QString AbstractQueryBuilder::_boolOperator(const Relation &boolOperator) const
{
	switch (boolOperator)
	{
	case And: return "AND";
	case Or: return "OR";

	default: Q_ASSERT(false);
	}
return QString();
}

