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

#ifndef ABSTRACTQUERYBUILDER_H
#define ABSTRACTQUERYBUILDER_H

#include "qst/qstfield.h"
#include "stringtemplate.h"

#include "queryframe.h"

#include "querybatch.h"
#include "queryscheme.h"

#include "querywhere.h"

#include <QStringList>
#include <QMap>


typedef QMap<Qst::QueryClause, QString> QueryClauseKeywordMap;


class AbstractQueryBuilder
{
protected:

	QueryClauseKeywordMap _keywords;

public:
	AbstractQueryBuilder();

	virtual QueryScheme queryScheme(const Qst::QueryType &queryType) const = 0;

	virtual QueryFrame makeQueryFrame(const QueryBatch &queryBatch,
									  const bool &isSubquery = false) const = 0;
	virtual QueryFrame *makeKeywordFrame(QueryFrame *subqueryFrame,
										 const Qst::QueryClause &queryClause) const = 0;
	virtual void makeClauseItemFrame(QueryFrame *parentFrame,
									 const QueryBatch &batch,
									 const bool &delimiterEnded) const = 0;

	virtual QueryBatchList validBatchList(const Qst::QueryClause &queryClause,
										  const QueryBatch &batch) const;

protected:

	virtual void _insertKeyword(const Qst::QueryClause &clause,
								const QString &keyword);
	virtual QString _keyword(const Qst::QueryClause &clause) const;

	virtual StringTemplate _conditionTemplate(const Qst::Functor &functor) const;

	virtual QString _itemsDelimiter(const Qst::QueryClause &clause) const;
	virtual QString _boolOperator(const Qst::Relation &boolOperator) const;
};


#endif // ABSTRACTQUERYBUILDER_H

