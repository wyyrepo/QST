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

#include "querybuilddirector.h"

#include <QDebug>

using namespace Qst;

QueryBuildDirector::QueryBuildDirector()
{
}

// Создает глобальный фрейм, в котором будет лежать весь запрос в виде подфреймов.
// Обрабатывает batch как пакет, содержащий подзапрос.
FramedQuery QueryBuildDirector::build(const QueryBatch &batch,
									  AbstractQueryBuilder *builder)
{
	Q_ASSERT(builder != NULL);

	QueryFrame subqueryFrame = builder->makeQueryFrame(batch);
	buildSubquery(batch, &subqueryFrame, builder);
	return FramedQuery(subqueryFrame);
}

void QueryBuildDirector::buildSubquery(const QueryBatch &batch,
									   QueryFrame *subqueryFrame,
									   AbstractQueryBuilder *builder)
{
	QueryScheme scheme = builder->queryScheme(batch.queryType());
	QueryClause clause = scheme.nextClause();

	while (clause != NoClause)
	{
		buildClause(clause, batch, subqueryFrame, builder);
		clause = scheme.nextClause();
	}
}

void QueryBuildDirector::buildClause(const QueryClause &queryClause,
									 const QueryBatch &batch,
									 QueryFrame *subqueryFrame,
									 AbstractQueryBuilder *builder)
{
	QueryBatchList bl = builder->validBatchList(queryClause, batch);
	if (bl.isEmpty())
		return;

	// Фрейм, содержащий ключевое слово секции.
	QueryFrame *keywordFrame = builder->makeKeywordFrame(subqueryFrame, queryClause);

	for (int j = 0; j < bl.count(); ++j)
	{
		/* case batches[j].queryType() of
			   QueryUndefined -> пакет batches[j] - секция.
			   _ -> пакет batches[j] - подзапрос. */

		if (bl[j].queryType() == QueryUndefined)
			builder->makeClauseItemFrame(keywordFrame, bl[j], j < bl.count()-1);
		else
		{
			QueryFrame inclFr = builder->makeQueryFrame(bl[j], true);
			buildSubquery(bl[j], &inclFr, builder);
			keywordFrame->appendFrame(inclFr);
		}
	}
}


