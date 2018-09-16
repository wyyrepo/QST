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

#include "ut_querywhere.h"

Q_DECLARE_METATYPE(QueryWhere);

Q_DECLARE_METATYPE(QueryField);

Q_DECLARE_METATYPE(QueryWhere::ConditionValidity);

using namespace Qst;

#include <QDebug>

ut_QueryWhere::ut_QueryWhere()
{
}

void ut_QueryWhere::initTestCase()
{
	w0  = QueryWhere();
	w1  = QueryWhere("field1a > field1b");
	w2  = QueryWhere("field2", QueryValue("string",  Qst::LeftRightPercents), Qst::Like);
	w3  = QueryWhere("field3", QueryValue("string",  Qst::LeftRightPercents), Qst::Like);
	w4  = QueryWhere("field4", QueryValue("string"), Qst::Like);
	w5  = QueryWhere("field5", "10", "20");
	w6  = QueryWhere("field6", 10, 20);
	w7  = QueryWhere(QueryField());
	w8  = QueryWhere(QueryField("field8a > field8b"));
	w9  = QueryWhere(QueryField("field9", QueryValue(), Qst::NoFunctor));

	w10  = QueryWhere(QueryField("field10",  QstPlaceholder("field10")));
	w10a = QueryWhere(QueryField("field10a", QstPlaceholder("field10a")));
	w11  = QueryWhere(QueryField("field11",  QstPlaceholder("field11")));
	w11.updatePlaceholder("field11", QueryValue(10), Qst::Equal);
	w12  = QueryWhere(QueryField("field12", QstPlaceholder("field12")));
	w12.updatePlaceholder("field12", QVariantList() << QVariant(10), Qst::InList);

	w13  = QueryWhere(QueryField("field13", QstPlaceholder("field13")));
	w13a = w13;  w13a.updatePlaceholder("field13", QVariantList(), Qst::InList);
	w13b = w13a; w13b.updatePlaceholder("field13", QVariantList() << QVariant(10), Qst::InList);
	w13c = w13b; w13c.resetPlaceholders();

	w14  = QueryWhere(w1)  && w2;
	w15  = QueryWhere(w1)  || w2;
	w16  = QueryWhere(w14) && w15;

	// w1 = Valid, w10 = Invalid,Placeholder
	w17  = QueryWhere(w1)  && w10;
	w17a = w17;  w17a.updatePlaceholder("field10", QueryValue(QVariant()),   Qst::Equal);
	w17b = w17a; w17b.updatePlaceholder("field10", QueryValue(QVariant(10)), Qst::Equal);
	w17c = w17b; w17c.resetPlaceholders();

	// w10 = Invalid,Placeholder, w10a = InvalidPlaceholder
	w18  = QueryWhere(w10) && w10a;
	w18a = w18;  w18a.updatePlaceholder("field10a", QueryValue(QVariant(20)), Qst::Equal);
	w18b = w18a; w18b.updatePlaceholder("field10",  QueryValue(QVariant(10)), Qst::Equal);
	w18c = w18b; w18c.resetPlaceholders();
}

void ut_QueryWhere::validity()
{
	QFETCH(QueryWhere, cond);
	QFETCH(QueryWhere::ConditionValidity, expected);

	QCOMPARE(cond.validity(), expected);
}

void ut_QueryWhere::validity_data()
{
	QTest::addColumn<QueryWhere>("cond");
	QTest::addColumn<QueryWhere::ConditionValidity>("expected");

	QTest::newRow("v-w0-0")  << w0  << QueryWhere::Invalid;
	QTest::newRow("v-w1-0")  << w1  << QueryWhere::Valid;
	QTest::newRow("v-w2-0")  << w2  << QueryWhere::Valid;
	QTest::newRow("v-w3-0")  << w3  << QueryWhere::Valid;
	QTest::newRow("v-w4-0")  << w4  << QueryWhere::Valid;
	QTest::newRow("v-w5-0")  << w5  << QueryWhere::Valid;
	QTest::newRow("v-w6-0")  << w6  << QueryWhere::Valid;
	QTest::newRow("v-w7-0")  << w7  << QueryWhere::Invalid;
	QTest::newRow("v-w8-0")  << w8  << QueryWhere::Valid;
	QTest::newRow("v-w9-0")  << w9  << QueryWhere::Valid;
	QTest::newRow("v-w10-0") << w10 << QueryWhere::Invalid;
	QTest::newRow("v-w11-0") << w11 << QueryWhere::Valid;
	QTest::newRow("v-w12-0") << w12 << QueryWhere::Valid;

	QTest::newRow("v-w13-0") << w13  << QueryWhere::Invalid;
	QTest::newRow("v-w13a")  << w13a << QueryWhere::Invalid;
	QTest::newRow("v-w13b")  << w13b << QueryWhere::Valid;
	QTest::newRow("v-w13c")  << w13c << QueryWhere::Invalid;

	QTest::newRow("v-w14-0") << w14 << QueryWhere::Valid;
	QTest::newRow("v-w15-0") << w15 << QueryWhere::Valid;
	QTest::newRow("v-w16-0") << w16 << QueryWhere::Valid;

	QTest::newRow("v-w17-0") << w17  << QueryWhere::PartialValid;
	QTest::newRow("v-w17a")  << w17a << QueryWhere::PartialValid;
	QTest::newRow("v-w17b")  << w17b << QueryWhere::Valid;
	QTest::newRow("v-w17c")  << w17c << QueryWhere::PartialValid;

	QTest::newRow("v-w18-0") << w18  << QueryWhere::Invalid;
	QTest::newRow("v-w18a")  << w18a << QueryWhere::PartialValid;
	QTest::newRow("v-w18b")  << w18b << QueryWhere::Valid;
	QTest::newRow("v-w18c")  << w18c << QueryWhere::Invalid;
}

void ut_QueryWhere::field()
{
	QFETCH(QueryWhere, cond);
	QFETCH(QueryField, expected);

	if (cond.field() != expected)
	{
		qDebug() << "\n\nActual:\n\n" << cond.field();
		qDebug() << "\n\nExpected:\n\n" << expected;
		qDebug() << "\n\nfunctor :\n\n" << Qst::Like;
	}

	QCOMPARE(cond.field(), expected);
}

void ut_QueryWhere::field_data()
{
	QTest::addColumn<QueryWhere>("cond");
	QTest::addColumn<QueryField>("expected");

	QTest::newRow("f-w0")  << w0   << QueryField();
	QTest::newRow("f-w1")  << w1   << QueryField("field1a > field1b");
	QTest::newRow("f-w2")  << w2   << QueryField("field2", QueryValue("string",  Qst::LeftRightPercents), Qst::Like);
	QTest::newRow("f-w3")  << w3   << QueryField("field3", QueryValue("string",  Qst::LeftRightPercents), Qst::Like);
	QTest::newRow("f-w4")  << w4   << QueryField("field4", QueryValue("string"), Qst::Like);
}

void ut_QueryWhere::validOnly()
{
	QFETCH(QueryWhere, cond);
	QFETCH(QueryWhere, expected);

	if (cond.validOnly() != expected)
	{
		qDebug() << "\n\nExpected:\n" << expected;
		qDebug() << "\n\nActual:\n" << cond;
	}

	QCOMPARE(cond.validOnly(), expected);
}

void ut_QueryWhere::validOnly_data()
{
	QTest::addColumn<QueryWhere>("cond");
	QTest::addColumn<QueryWhere>("expected");

	QTest::newRow("vo-w0-0")  << w0   << QueryWhere();
	QTest::newRow("vo-w1-0")  << w1   << QueryWhere("field1a > field1b");
	QTest::newRow("vo-w14-0") << w14  << w14;
	QTest::newRow("vo-w15-0") << w15  << w15;
	QTest::newRow("vo-w16-0") << w16  << w16;

	QTest::newRow("vo-w17-0") << w17  << QueryWhere("field1a > field1b");
	QTest::newRow("vo-w17a")  << w17a << QueryWhere("field1a > field1b");
	QueryField f_w17b = QueryField("field10", QueryValue(10), Qst::Equal, "field10");
	QueryWhere vo_w17b;
	vo_w17b.appendCondition(QueryWhere("field1a > field1b"));
	vo_w17b.appendCondition(QueryWhere(f_w17b));
	vo_w17b.setRelation(Qst::And);
	QTest::newRow("vo-w17b")  << w17b << vo_w17b;

	QueryField f_w10  = QueryField("field10",  QueryValue(QVariant(10)), Qst::Equal, "field10");
	QueryField f_w10a = QueryField("field10a", QueryValue(QVariant(20)), Qst::Equal, "field10a");
	QueryWhere vo_w18b;
	vo_w18b.appendCondition(QueryWhere(f_w10));
	vo_w18b.appendCondition(QueryWhere(f_w10a));
	vo_w18b.setRelation(Qst::And);

	QTest::newRow("vo-w18-0") << w18  << QueryWhere();
	QTest::newRow("vo-w18a")  << w18a << QueryWhere(f_w10a);
	QTest::newRow("vo-w18b")  << w18b << vo_w18b;
}
