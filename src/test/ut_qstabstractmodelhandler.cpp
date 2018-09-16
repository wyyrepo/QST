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

#include "ut_qstabstractmodelhandler.h"
#include <QDebug>

using namespace Qst;

Q_DECLARE_METATYPE(AbstractQueryBuilder*)

Q_DECLARE_METATYPE(QstAbstractModelHandler*)

Q_DECLARE_METATYPE(QstBatch)

namespace QstTest
{

/*!
\class ut_QstAbstractModelHandler
\brief
\lang_Russian
	Модульный тест для класса QstAbstractModelHandler.
\endlang

\lang_English
	Unit test for QstAbstractModelHandler.
\endlang
*/

ut_QstAbstractModelHandler::ut_QstAbstractModelHandler()
{
}

void ut_QstAbstractModelHandler::initTestCase()
{
	pgBP  = &pgB;
	sqLBP = &sqLB;

	b1	<< "sys.v_user"
		<< QstField(RolePrimaryKey, "id")
		<< QstField("person_id", FieldInvisible)
		<< QstField("disabled", FieldInvisible)
		<< QstField("is_superuser", FieldInvisible)
		<< QstField("login", FieldVisible, "Логин", 110)
		<< QstField("registration_date", FieldVisible, "регистрация", 100);

	b1.where("id",        QstPlaceholder());
	b1.where("login",     QstPlaceholder());
	b1.where("disabled",  QstPlaceholder());
	b1.where("person_id", QstPlaceholder());

	b1.updatePlaceholder("login",    QstValue("gas", Equal));
	b1.updatePlaceholder("disabled", QVariant(0),    Equal);
}

void ut_QstAbstractModelHandler::cleanupTestCase()
{
}

void ut_QstAbstractModelHandler::generateQuery()
{
	QFETCH(AbstractQueryBuilder*, builder);
	QFETCH(QstBatch, batch);
	QFETCH(QString, expected);

	QstAbstractModelHandler::setQueryBuilder(builder);

	QString q = QstAbstractModelHandler::generateQuery(batch);
	if (expected != q)
	{
		qDebug() << "\n\nExpected:\n" << expected;
		qDebug() << "\n\nActual:\n"   << q;
	}

	QCOMPARE(q, expected);
}

void ut_QstAbstractModelHandler::generateQuery_data()
{
	QTest::addColumn<AbstractQueryBuilder*>("builder");
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<QString>("expected");

	// PostgreSQL Builder
	QTest::newRow("sq-sel1-pg")  << pgBP  << b1 << "SELECT id, person_id, disabled, is_superuser, login, registration_date FROM sys.v_user WHERE ((login = 'gas') AND (disabled = 0))";

	// SQLite Builder
	QTest::newRow("sq-sel1-sqL") << sqLBP << b1 << "SELECT id, person_id, disabled, is_superuser, login, registration_date FROM sys.v_user WHERE ((login = 'gas') AND (disabled = 0))";
}

} // End of namespace QstTest
