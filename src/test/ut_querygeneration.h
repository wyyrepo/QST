/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Test module of the QsT SQL Tools.
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

#ifndef UT_QUERYGENERATION_H
#define UT_QUERYGENERATION_H

#include <QObject>
#include <QTest>

#include "qst/querygeneration/querybuilders/postgresql/postgresqlbuilder.h"
#include "qst/querygeneration/querybuilders/sqlite/sqlitebuilder.h"
#include "qst/querygeneration/querybatch.h"
#include "qst/querygeneration/querybuilddirector.h"
#include "qst/querygeneration/abstractquerybuilder.h"
#include "qst/qstbatch.h"

class ut_QueryGeneration : public QObject
{
	Q_OBJECT

public:
    ut_QueryGeneration();

private slots:

	void initTestCase();

	void simpleQuery();
	void simpleQuery_data();

private:

	PostgreSqlBuilder pgB;
	SqLiteBuilder     sqLB;

	AbstractQueryBuilder* pgBP;
	AbstractQueryBuilder* sqLBP;

	QueryBuildDirector bldDir;

	QueryBatch selb1, selb2, selb2a, selb3, selb4, selb5, selb6, selb7;
	QueryBatch insb1, updb1, delb1, b9, b10;

	Qst::QstBatch qstb1, qstb2, qstb3, qstb4;
};

#endif // UT_QUERYGENERATION_H
