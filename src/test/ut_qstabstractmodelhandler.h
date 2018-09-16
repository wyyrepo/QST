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

#ifndef UT_QSTABSTRACTMODELHANDLER_H
#define UT_QSTABSTRACTMODELHANDLER_H

#include <QObject>
#include <QTest>

#include "test/testhandler.h"
#include "qst/mvc/qstplainquerymodel.h"
#include "qst/mvc/qsttreequerymodel.h"

#include "qst/querygeneration/querybuilders/postgresql/postgresqlbuilder.h"
#include "qst/querygeneration/querybuilders/sqlite/sqlitebuilder.h"

namespace QstTest
{


class ut_QstAbstractModelHandler : public QObject	// FIX ME: FULL REWRITE
{

	Q_OBJECT

public:
	ut_QstAbstractModelHandler();

private slots:

	void initTestCase();
	void cleanupTestCase();

	void generateQuery();
	void generateQuery_data();

private:

	PostgreSqlBuilder pgB;
	SqLiteBuilder     sqLB;

	AbstractQueryBuilder* pgBP;
	AbstractQueryBuilder* sqLBP;

	Qst::QstBatch b1;

};

} // End of namespace QstTest

#endif // UT_QSTABSTRACTMODELHANDLER_H
