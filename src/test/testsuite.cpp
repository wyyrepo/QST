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

#include "testsuite.h"

#include <QTest>

#include "test/ut_querybatch.h"
#include "test/ut_qstbatch.h"
#include "test/ut_defaultitemnameextractor.h"
#include "test/ut_qstabstractmodelhandler.h"
#include "test/ut_qstconnectionsettings.h"

#include "test/ut_querywhere.h"
#include "test/ut_querygeneration.h"

using namespace QstTest;

TestSuite::TestSuite()
{
}

int TestSuite::doTests(int argc, char *argv[])
{
	QList<QObject*> testCases;

	testCases
				<< new ut_QueryWhere()
				<< new ut_QueryGeneration()
				<< new ut_QueryBatch()
				<< new ut_DefaultItemNameExtractor()
				<< new ut_QstAbstractModelHandler()
				<< new ut_QstConnectionSettings()
				;

	foreach(QObject *testCase, testCases)
	{
		int res = QTest::qExec(testCase, argc, argv);
		if (res != 0) break;
	}

	foreach(QObject* obj, testCases)
		delete obj;
	return 0;
}
