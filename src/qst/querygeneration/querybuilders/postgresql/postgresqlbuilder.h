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

#ifndef POSTGRESQLBUILDER_H
#define POSTGRESQLBUILDER_H

#include "qst/querygeneration/abstractquerybuilder.h"
#include "postgresqlvalueformat.h"

class PostgreSqlBuilder : public AbstractQueryBuilder
{
private:

	PostgreSqlValueFormat _valueFormat;

public:
    PostgreSqlBuilder();

	QueryScheme queryScheme(const Qst::QueryType &queryType) const;

	QueryFrame makeQueryFrame(const QueryBatch &queryBatch,
							  const bool &isSubquery = false) const;
	QueryFrame *makeKeywordFrame(QueryFrame *subqueryFrame,
								 const Qst::QueryClause &queryClause) const;
	void makeClauseItemFrame(QueryFrame *parentFrame,
							 const QueryBatch &batch,
							 const bool &delimiterEnded) const;

	QueryBatchList validBatchList(const Qst::QueryClause &queryClause,
								  const QueryBatch &batch) const;

private:

	QString _composeCondition(const QueryField &field) const;

	QueryFrame _commonClauseFrame(QStringList itemsList,
								  QString delimiter,
								  const bool &delimiterEnded,
								  const QString &leftBracket = QString(),
								  const QString &rightBracket = QString()) const;

	QueryFrame _conditionClauseFrame(const QueryWhere &cond) const;
	QueryFrame _joinClauseFrame(const QueryBatch &batch) const;
	QueryFrame _statementClauseFrame(const QString &tableName,
									 const QStringList &fieldNames,
									 const QString &delimiter) const;
	QueryFrame _valuesClauseFrame(const QueryFieldList &fieldValues,
								  const QString &delimiter) const;
	QueryFrame _setClauseFrame(const QueryFieldList &fieldValues,
							   const QString &delimiter) const;
	QueryFrame _parametersClauseFrame(const QueryFieldList &fieldValues,
									  const QString &delimiter) const;

	QString _betweenCondition(const QueryField &field,
							  const StringTemplate &strTempl) const;

	QString _inListCondition(const QueryField &field,
							 const StringTemplate &strTempl) const;
};

#endif // POSTGRESQLBUILDER_H
