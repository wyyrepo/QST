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

#include "ut_querygeneration.h"

#include <QDebug>

using namespace Qst;

Q_DECLARE_METATYPE(AbstractQueryBuilder*);

Q_DECLARE_METATYPE(QueryBatch);

Q_DECLARE_METATYPE(QueryFrame);

Q_DECLARE_METATYPE(QstBatch)

ut_QueryGeneration::ut_QueryGeneration()
{
}

void ut_QueryGeneration::initTestCase()
{
	pgBP  = &pgB;
	sqLBP = &sqLB;

	selb1 = QueryBatch(QuerySelect, "sel1");
	selb1.select("field1");
	selb1.from("table1");

	selb2 = QueryBatch(QuerySelect, "sel2");
	selb2.select("field1");
	selb2.from("table1");
	selb2.where("field1", QstPlaceholder());
	selb2a = selb2;
	selb2a.updatePlaceholder("field1", QueryValue(QVariant(10)), Equal);

	selb3 = QueryBatch(QuerySelect, "sel3");
	selb3.select("field1");
	selb3.from("table1");
	selb3.where("field1", QstPlaceholder());
	selb3.where("field2", QstPlaceholder());

	selb4 = QueryBatch(QuerySelect, "sel4");
	selb4.select("field1");
	selb4.select("field2");
	selb4.select(QStringList() << "field3" << "field4");
	selb4.select("field5, field6");
	selb4.from("table1");
	selb4.from("table2");

	selb5 = QueryBatch(QuerySelect, "sel5");
	selb5.select("field1");
	selb5.from("table1");
	selb5.where("field1", QueryValue("string", Qst::LeftRightPercents), Qst::Like);

	selb6 = QueryBatch(QuerySelect, "selb6");
	selb6.select("field1");
	selb6.from("table1");
	selb6.where(QueryWhere("field1 > field2") || QueryWhere("field3 < field4"));
	selb6.where("field5 = field6");

	selb7 = QueryBatch(QuerySelect, "selb7");
	selb7.select("field1");
	selb7.from("table1");
	selb7.where("field1 == field2");
	selb7.where("field3 < field4");
	selb7.where("field5 IN (1,2,3)");

	insb1 = QueryBatch(QueryInsert, "ins1");
	insb1.insert("table1");
	insb1.values(QVariantList() << QVariant(10) << QVariant("string"));

	updb1 = QueryBatch(QueryUpdate, "upd1");
	updb1.update("table1");
	updb1.set(QueryFieldList()
			  << QueryField("field1", QueryValue(10),                 ClauseSet)
			  << QueryField("field2", QueryValue("ABC"),              ClauseSet)
			  << QueryField("field3", QueryValue(QDate(2011, 12, 1)), ClauseSet));

	delb1 = QueryBatch(QueryDelete, "del1");
	delb1.deleteFrom("table1");
	delb1.where("field1", QstPlaceholder());
	delb1.updatePlaceholder("field1", QVariantList() << 10 << 20 << 30);


	qstb1.execute("function1");
	qstb1.parameters(QstPlaceholderList()
			<< QstPlaceholder("field1")
			<< QstPlaceholder("field2")
			<< QstPlaceholder("field3"));
	QVariantMap dataMap1;
	dataMap1.insert("field1", "Data1");
	dataMap1.insert("field2", "Data2");
	dataMap1.insert("field3", "Data3");
	qstb1.updatePlaceholders(dataMap1);

	qstb2.execute("function2");

	qstb3.execute("function3");
	qstb3.parameters(QstPlaceholderList()
					 << QstPlaceholder("field1")
					 << QstPlaceholder("field2")
					 << QstPlaceholder("field3"));
	QVariantMap dataMap2;
	dataMap2.insert("field1", "Data1");
	dataMap2.insert("field3", "Data3");
	qstb3.updatePlaceholders(dataMap2);

	qstb4	<< QstField("login")
			<< QstField("disabled");
	qstb4.from("user");
	qstb4.where("id",        QstPlaceholder());
	qstb4.where("login",     QstPlaceholder());
	qstb4.where("disabled",  QstPlaceholder());
	qstb4.where("person_id", QstPlaceholder());
	qstb4.updatePlaceholder("login",    QstValue("gas", Equal, Qst::NoPercentPlaceholders));
	qstb4.updatePlaceholder("disabled", QVariant(0));
}

void ut_QueryGeneration::simpleQuery()
{
	QFETCH(AbstractQueryBuilder*, builder);
	QFETCH(QueryBatch, batch);
	QFETCH(QString, expected);

	QString actual = bldDir.build(batch, builder).toString();

	if (actual != expected)
	{
		qDebug() << "\n\nExpected:\n" << expected;
		qDebug() << "\n\nActual:\n" << actual;
	}

	QCOMPARE(actual, expected);
}

void ut_QueryGeneration::simpleQuery_data()
{
	QTest::addColumn<AbstractQueryBuilder*>("builder");
	QTest::addColumn<QueryBatch>("batch");
	QTest::addColumn<QString>("expected");

	// PostgreSQL Builder
	QTest::newRow("sq-sel1-pg")    << pgBP  << selb1  << "SELECT field1 FROM table1";
	QTest::newRow("sq-sel2-pg")    << pgBP  << selb2  << "SELECT field1 FROM table1";
	QTest::newRow("sq-sel2a-pg")   << pgBP  << selb2a << "SELECT field1 FROM table1 WHERE (field1 = 10)";
	QTest::newRow("sq-sel3-pg")    << pgBP  << selb3  << "SELECT field1 FROM table1";
	QTest::newRow("sq-sel4-pg")    << pgBP  << selb4  << "SELECT field1, field2, field3, field4, field5, field6 FROM table1, table2";
	QTest::newRow("sq-sel5-pg")    << pgBP  << selb5  << "SELECT field1 FROM table1 WHERE (field1 LIKE '%string%')";
	QTest::newRow("sq-sel6-pg")    << pgBP  << selb6  << "SELECT field1 FROM table1 WHERE (((field1 > field2) OR (field3 < field4)) AND (field5 = field6))";
	QTest::newRow("sq-sel7-pg")    << pgBP  << selb7  << "SELECT field1 FROM table1 WHERE ((field1 == field2) AND (field3 < field4) AND (field5 IN (1,2,3)))";
	QTest::newRow("sq-ins1-pg")    << pgBP  << insb1  << "INSERT INTO table1 VALUES (10, 'string')";
	QTest::newRow("sq-upd1-pg")    << pgBP  << updb1  << "UPDATE table1 SET field1 = 10, field2 = 'ABC', field3 = '01.12.2011'";
	QTest::newRow("sq-del1-pg")    << pgBP  << delb1  << "DELETE FROM table1 WHERE (field1 IN (10, 20, 30))";
	QTest::newRow("sq-qstb1-pg")   << pgBP  << qstb1.queryBatch() << "SELECT function1  ('Data1', 'Data2', 'Data3') AS return_value";
	QTest::newRow("sq-qstb2-pg")   << pgBP  << qstb2.queryBatch() << "SELECT function2  () AS return_value";
	QTest::newRow("sq-qstb3-pg")   << pgBP  << qstb3.queryBatch() << "SELECT function3  ('Data1', NULL, 'Data3') AS return_value";
	QTest::newRow("sq-qstb4-pg")   << pgBP  << qstb4.queryBatch() << "SELECT login, disabled FROM user WHERE ((login = 'gas') AND (disabled = 0))";

	// SQLite Builder
	QTest::newRow("sq-sel1-sqL")   << sqLBP << selb1  << "SELECT field1 FROM table1";
	QTest::newRow("sq-sel2-sqL")   << sqLBP << selb2  << "SELECT field1 FROM table1";
	QTest::newRow("sq-sel2a-sqL")  << sqLBP << selb2a << "SELECT field1 FROM table1 WHERE (field1 = 10)";
	QTest::newRow("sq-sel3-sqL")   << sqLBP << selb3  << "SELECT field1 FROM table1";
	QTest::newRow("sq-sel4-sqL")   << sqLBP << selb4  << "SELECT field1, field2, field3, field4, field5, field6 FROM table1, table2";
	QTest::newRow("sq-sel5-sqL")   << sqLBP << selb5  << "SELECT field1 FROM table1 WHERE (field1 LIKE '%string%')";
	QTest::newRow("sq-sel6-sqL")   << sqLBP << selb6  << "SELECT field1 FROM table1 WHERE (((field1 > field2) OR (field3 < field4)) AND (field5 = field6))";
	QTest::newRow("sq-sel7-pg")    << sqLBP << selb7  << "SELECT field1 FROM table1 WHERE ((field1 == field2) AND (field3 < field4) AND (field5 IN (1,2,3)))";
	QTest::newRow("sq-ins1-sqL")   << sqLBP << insb1  << "INSERT INTO table1 VALUES (10, 'string')";
	QTest::newRow("sq-upd1-sqL")   << sqLBP << updb1  << "UPDATE table1 SET field1 = 10, field2 = 'ABC', field3 = '01.12.2011'";
	QTest::newRow("sq-del1-sqL")   << sqLBP << delb1  << "DELETE FROM table1 WHERE (field1 IN (10, 20, 30))";
	QTest::newRow("sq-qstb1-sqL")  << sqLBP << qstb1.queryBatch() << "SELECT function1  ('Data1', 'Data2', 'Data3')";
	QTest::newRow("sq-qstb2-sqL")  << sqLBP << qstb2.queryBatch() << "SELECT function2  ()";
	QTest::newRow("sq-qstb3-sqL")  << sqLBP << qstb3.queryBatch() << "SELECT function3  ('Data1', NULL, 'Data3')";
}
