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

#include "ut_querybatch.h"

using namespace Qst;

Q_DECLARE_METATYPE (QueryBatch);

Q_DECLARE_METATYPE (QueryWhere);

Q_DECLARE_METATYPE (QueryBatchList);

Q_DECLARE_METATYPE (QueryClause);

#include <QDebug>

ut_QueryBatch::ut_QueryBatch()
{
	fieldValuesMap.insert("field1", QVariant(1));
	fieldValuesMap.insert("field2", QVariant("hoho"));

	valuesList << QVariant(1)
			   << QVariant("hoho");
}

void ut_QueryBatch::initTestCase()
{
	b1 = QueryBatch(QuerySelect);
	b1.select("field1");
	b1.from("table1");

	b2 = QueryBatch(QueryInsert);
	b2.insert("table1");
	b2.values(QVariantList() << QVariant(1) << QVariant("hoho"));

	b3 = QueryBatch(QueryInsert);
	b3.insert("table1",
			  QStringList() << "field1" << "field2");
	b3.values(QVariantList() << QVariant(1) << QVariant("hoho"));

	b4 = QueryBatch(QueryUpdate);
	b4.update("table1");
	b4.set(QueryFieldList()
		   << QueryField("field1", QueryValue(QVariant(1)), ClauseSet)
		   << QueryField("field2", QueryValue(QVariant("hoho")), ClauseSet));

	b5 = QueryBatch(QueryUpdate);
	b5.update("table1");
	b5.set(fieldValuesMap);

	b6 = QueryBatch(QueryDelete);
	b6.deleteFrom("table1");

	b7 = QueryBatch(QueryExecute);
	b7.execute("func1");
	b7.parameters(valuesList);

	b8 = QueryBatch(QueryExecute);
	b8.execute("func1");

	b9 = QueryBatch(UserDefinedQuery, "SELECT * FROM table1");

	sel1b = QueryBatch(QuerySelect, "sel1");
	sel1b.select("field1");
	sel1b.from("table1");

	sel2b = QueryBatch(QuerySelect, "sel2");
	sel2b.select("field1");
	sel2b.from("table1");
	sel2b.where("field1", QstPlaceholder());

	sel3b = QueryBatch(QuerySelect, "sel3");
	sel3b.select("field1");
	sel3b.from("table1");
	sel3b.where("field1", QstPlaceholder());
	sel3b.where("field2", QstPlaceholder());

	sel4b = QueryBatch(QuerySelect, "sel4");
	sel4b.select("field1");
	sel4b.select("field2");
	sel4b.select(QStringList() << "field3" << "field4");
	sel4b.select("field5, field6");
	sel4b.from("table1");
	sel4b.from("table2");

	sel5b = QueryBatch(QuerySelect, "sel5");
	sel5b.select("field1");
	sel5b.from("table1");
	sel5b.where("field1", QueryValue("string", Qst::LeftRightPercents), Qst::Like);
}

void ut_QueryBatch::fieldNames()
{
	QFETCH(QueryBatch, batch);
	QFETCH(QStringList, expected);

	QCOMPARE(batch.fieldNames(), expected);
}

void ut_QueryBatch::fieldNames_data()
{
	QTest::addColumn<QueryBatch>("batch");
	QTest::addColumn<QStringList>("expected");

	QStringList f1f2list;
	f1f2list << "field1" << "field2";

	QTest::newRow("fn 1-0")    << b1 << QStringList();
	QTest::newRow("fn 1-sel")  << b1.batchList(ClauseSelect).first() << (QStringList() << "field1");
	QTest::newRow("fn 1-from") << b1.batchList(ClauseFrom).first()   << QStringList();

	QTest::newRow("fn 2-0")    << b2 << QStringList();
	QTest::newRow("fn 2-ins")  << b2.batchList(ClauseInsert).first() << QStringList();
	QTest::newRow("fn 2-vals") << b2.batchList(ClauseValues).first() << (QStringList() << "" << "");

	QTest::newRow("fn 3-0")    << b3 << QStringList();
	QTest::newRow("fn 3-ins")  << b3.batchList(ClauseInsert).first() << f1f2list;
	QTest::newRow("fn 3-vals") << b3.batchList(ClauseValues).first() << (QStringList() << "" << "");

	QTest::newRow("fn 4-0")    << b4 << QStringList();
	QTest::newRow("fn 4-upd")  << b4.batchList(ClauseUpdate).first() << QStringList();
	QTest::newRow("fn 4-set")  << b4.batchList(ClauseSet).first()    << f1f2list;

	QTest::newRow("fn 5-0")    << b5 << QStringList();
	QTest::newRow("fn 5-upd")  << b5.batchList(ClauseUpdate).first() << QStringList();
	QTest::newRow("fn 5-set")  << b5.batchList(ClauseSet).first()    << f1f2list;

	QTest::newRow("fn 6-0")    << b6 << QStringList();
	QTest::newRow("fn 6-del")  << b6.batchList(ClauseDelete).first() << (QStringList());

	QTest::newRow("fn 7-0")    << b7 << QStringList();
	QTest::newRow("fn 7-exec") << b7.batchList(ClauseExecute).first() << (QStringList());

	QTest::newRow("fn 8-0")    << b8 << QStringList();
	QTest::newRow("fn 8-exec") << b8.batchList(ClauseExecute).first() << (QStringList());

	QTest::newRow("fn 9-0")    << b9 << QStringList();
}

void ut_QueryBatch::sourceNames()
{
	QFETCH(QueryBatch, batch);
	QFETCH(QStringList, expected);

	QCOMPARE(batch.sourceNames(), expected);
}

void ut_QueryBatch::sourceNames_data()
{
	QTest::addColumn<QueryBatch>("batch");
	QTest::addColumn<QStringList>("expected");

	QStringList t1list;
	t1list << "table1";

	QTest::newRow("sn 1-0")    << b1 << QStringList();
	QTest::newRow("sn 1-sel")  << b1.batchList(ClauseSelect).first() << QStringList();
	QTest::newRow("sn 1-from") << b1.batchList(ClauseFrom).first()   << t1list;

	QTest::newRow("sn 2-0")    << b2 << QStringList();
	QTest::newRow("sn 2-ins")  << b2.batchList(ClauseInsert).first() << t1list;
	QTest::newRow("sn 2-vals") << b2.batchList(ClauseValues).first() << QStringList();

	QTest::newRow("sn 3-0")    << b3 << QStringList();
	QTest::newRow("sn 3-ins")  << b3.batchList(ClauseInsert).first() << t1list;
	QTest::newRow("sn 3-vals") << b3.batchList(ClauseValues).first() << QStringList();

	QTest::newRow("sn 4-0")    << b4 << QStringList();
	QTest::newRow("sn 4-upd")  << b4.batchList(ClauseUpdate).first() << t1list;
	QTest::newRow("sn 4-set")  << b4.batchList(ClauseSet).first()    << QStringList();

	QTest::newRow("sn 5-0")    << b5 << QStringList();
	QTest::newRow("sn 5-upd")  << b5.batchList(ClauseUpdate).first() << t1list;
	QTest::newRow("sn 5-set")  << b5.batchList(ClauseSet).first()    << QStringList();

	QTest::newRow("sn 6-0")    << b6 << QStringList();
	QTest::newRow("sn 6-del")  << b6.batchList(ClauseDelete).first() << t1list;

	QTest::newRow("sn 7-0")    << b7 << QStringList();
	QTest::newRow("sn 7-exec") << b7.batchList(ClauseExecute).first() << (QStringList() << "func1");

	QTest::newRow("sn 8-0")    << b8 << QStringList();
	QTest::newRow("sn 8-exec") << b8.batchList(ClauseExecute).first() << (QStringList() << "func1");

	QTest::newRow("sn 9-0")    << b9 << QStringList();
}

void ut_QueryBatch::batchList()
{
	QFETCH(QueryBatch, batch);
	QFETCH(QueryClause, clause);
	QFETCH(QueryBatchList, expected);

	if (batch.batchList(clause) != expected)
	{
		qDebug() << clause;
		qDebug() << "\n\nExpected:\n" << expected;
		qDebug() << "\n\nActual:\n" << batch.batchList(clause);
	}

	QCOMPARE(batch.batchList(clause), expected);
}

void ut_QueryBatch::batchList_data()
{
	QTest::addColumn<QueryBatch>("batch");
	QTest::addColumn<QueryClause>("clause");
	QTest::addColumn<QueryBatchList>("expected");

	QueryBatch b1f1(ClauseSelect, "", QueryBatch::StringSource, Qst::QueryUndefined);
	b1f1.addField(QueryField("field1", ClauseSelect));
	QueryBatch bfrom(ClauseFrom, "table1", QueryBatch::StringSource, Qst::QueryUndefined);
	QTest::newRow("bl 1-sel")    << b1 << ClauseSelect << (QueryBatchList() << b1f1);
	QTest::newRow("bl 1-from")   << b1 << ClauseFrom   << (QueryBatchList() << bfrom);

	QueryBatch b2t1(ClauseInsert, "table1", QueryBatch::StringSource, Qst::QueryUndefined);
	QueryBatch bvals(ClauseValues, "", QueryBatch::StringSource, Qst::QueryUndefined);
	bvals.addField(QueryField(QueryValue(1), ClauseValues));
	bvals.addField(QueryField(QueryValue("hoho"), ClauseValues));
	QTest::newRow("bl 2-ins")    << b2 << ClauseInsert << (QueryBatchList() << b2t1);
	QTest::newRow("bl 2-vals")   << b2 << ClauseValues << (QueryBatchList() << bvals);

	QueryBatch b3t1(ClauseInsert, "table1", QueryBatch::StringSource, Qst::QueryUndefined);
	b3t1.addField(QueryField("field1", QueryValue(), Qst::NoFunctor, "", ClauseInsert));
	b3t1.addField(QueryField("field2", QueryValue(), Qst::NoFunctor, "", ClauseInsert));
	QTest::newRow("bl 3-ins")    << b3 << ClauseInsert << (QueryBatchList() << b3t1);
	QTest::newRow("bl 3-vals")   << b3 << ClauseValues << (QueryBatchList() << bvals);

	QueryBatch b4t1(ClauseUpdate, "table1", QueryBatch::StringSource, Qst::QueryUndefined);
	QueryBatch bset(ClauseSet, "", QueryBatch::StringSource, Qst::QueryUndefined);
	bset.addField(QueryField("field1", QueryValue(1, Qst::NoPercentPlaceholders), Qst::NoFunctor, "", ClauseSet));
	bset.addField(QueryField("field2", QueryValue("hoho", Qst::NoPercentPlaceholders), Qst::NoFunctor, "", ClauseSet));
	QTest::newRow("bl 4-upd")    << b4 << ClauseUpdate << (QueryBatchList() << b4t1);
	QTest::newRow("bl 4-set")    << b4 << ClauseSet    << (QueryBatchList() << bset);

	QueryBatch b5t1(ClauseUpdate, "table1", QueryBatch::StringSource, Qst::QueryUndefined);
	QTest::newRow("bl 5-upd")    << b5 << ClauseUpdate << (QueryBatchList() << b5t1);
	QTest::newRow("bl 5-set")    << b5 << ClauseSet    << (QueryBatchList() << bset);

	QueryBatch b6t1(ClauseDelete, "table1", QueryBatch::StringSource, Qst::QueryUndefined);
	QTest::newRow("bl 6-del")    << b6 << ClauseDelete << (QueryBatchList() << b6t1);

	QueryBatch b7f1(ClauseExecute, "func1", QueryBatch::StringSource, Qst::QueryUndefined);
	QueryBatch b7pars(ClauseParameters, "", QueryBatch::StringSource, Qst::QueryUndefined);
	b7pars.addField(QueryField(QueryValue(1), ClauseParameters));
	b7pars.addField(QueryField(QueryValue("hoho"), ClauseParameters));
	QTest::newRow("bl 7-exec")   << b7 << ClauseExecute    << (QueryBatchList() << b7f1);
	QTest::newRow("bl 7-pars")   << b7 << ClauseParameters << (QueryBatchList() << b7pars);

	QueryBatch b8f1(ClauseExecute, "func1", QueryBatch::StringSource, Qst::QueryUndefined);
	QTest::newRow("bl 8-exec")   << b8 << ClauseExecute    << (QueryBatchList() << b8f1);

	QueryBatch b9udq(UserDefinedClauses, "SELECT * FROM table1", QueryBatch::StringSource, Qst::QueryUndefined);
	QTest::newRow("bl 9-udq")    << b9 << UserDefinedClauses << (QueryBatchList() << b9udq);
}

void ut_QueryBatch::conditions()
{
	QFETCH(QueryBatch, batch);
	QFETCH(Qst::QueryClause, batchListClause);
	QFETCH(QueryWhere, validOnly);

	if (batch.validOnlyConditions(batchListClause) != validOnly)
	{
		qDebug() << "\nValid only:";
		qDebug() << "\n\nExpected:\n" << validOnly;
		qDebug() << "\n\nActual:\n" << batch.validOnlyConditions(batchListClause);
	}
QCOMPARE(batch.validOnlyConditions(batchListClause), validOnly);
}

void ut_QueryBatch::conditions_data()
{
	QTest::addColumn<QueryBatch>("batch");
	QTest::addColumn<Qst::QueryClause>("batchListClause");
	QTest::addColumn<QueryWhere>("validOnly");

	QTest::newRow("voc-sel1b")   << sel1b << ClauseWhere << QueryWhere();
	QTest::newRow("voc-0-sel2b") << sel2b << ClauseWhere << QueryWhere();
}
