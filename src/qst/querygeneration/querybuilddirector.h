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

#ifndef QUERYBUILDDIRECTOR_H
#define QUERYBUILDDIRECTOR_H

#include "qst/qstglobal.h"
#include "abstractquerybuilder.h"
#include "framedquery.h"

class QueryBuildDirector
{
private:

	AbstractQueryBuilder *_builder;

public:
    QueryBuildDirector();

	FramedQuery build(const QueryBatch &batch, AbstractQueryBuilder *builder);

	void buildSubquery(const QueryBatch &batch,
					   QueryFrame *subqueryFrame,
					   AbstractQueryBuilder *builder);

	void buildClause(const Qst::QueryClause &queryClause,
					 const QueryBatch &batch,
					 QueryFrame *subqueryFrame,
					 AbstractQueryBuilder *builder);
};

#endif // QUERYBUILDDIRECTOR_H
