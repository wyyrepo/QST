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

#include "testhandler.h"
#include "qst/qstdbconnection.h"

using namespace Qst;

namespace QstTest
{

/*!
\class TestHandler
\brief
\lang_Russian
	Класс-хэндлер, использующийся в модульном тесте ut_QstAbstractModelHandler.
\endlang

\lang_English
	Handler class used in ut_QstAbstractModelHandler.
\endlang
*/

int TestHandler::_connectionNumber = 0;

TestHandler::TestHandler()
{
}

QString TestHandler::connect()
{
//	_connectionName = "Connection" + QString::number(_connectionNumber);
//	QstDBConnection con;
//	con.setDriverName("QSQLITE");
//	bool opened = con.connect("", ":memory:", "","", _connectionName);

//	if (!opened)
//	{
//		Q_ASSERT(false);
//		return QString();
//	}

//	_connectionNumber++;
	return _connectionName;
}

QString TestHandler::connectionName() const
{
	return _connectionName;
}

void TestHandler::setData()
{
//	QSqlQuery query = QSqlQuery(QSqlDatabase::database(_connectionName));

//		query.exec("create table smartphone (id int primary key, "
//				   "manufacturer_vendor varchar(200), model varchar(40), operating_system varchar(200))");
//		query.exec("insert into smartphone values(101, 'Acer', 'Liquid Mt Metal', 'Android 2.2')");
//		query.exec("insert into smartphone values(102, 'Samsung', 'Galaxy S', 'Android 2.2')");
//		query.exec("insert into smartphone values(103, 'Nokia', '5800', 'Symbian')");

//		query.exec("create table vendor (id int primary key, parent_id int, tree_level int, "
//				   "vendor_name varchar(200), it_priority_orientation varchar(40))");
//		query.exec("insert into vendors values(201, NULL, 0, 'Acer', 'Computers')");
//		query.exec("insert into vendors values(202, NULL, 0, 'Samsung', 'Computers')");
//		query.exec("insert into vendors values(203, NULL, 0, 'Nokia', 'Smartphones')");
//		query.exec("insert into vendors values(204, NULL, 0, 'Microsoft', 'Software')");
//		query.exec("insert into vendors values(205, 203,  1, 'Qt', 'Software')");
}

//QueryBatch TestHandler::_selector(const int &queryNumber) const
//{
//	QueryBatch batch;

//	if (queryNumber == TEST1)
//	{
//		batch = QueryBatch(QuerySelect, "Test1");

//		batch.select("id").select("name");
//		batch.from("table1");
//		batch.where("id", value("id"));
//		batch.where("order_date", value("date_begin"), value("date_end"));

//		batch	<< "table1"
//				<< QstField(RolePrimaryKey, "ID")
//				<< QstField("Name", FieldVisible, "Наименование", 120)

//				<< QstField()
//				<< QstField("OrderDate", value("DateBegin"), value("DateEnd"))
//				;
//	}
//	else
//		if (queryNumber == TEST2)
//		{
//			batch	<< "smartphone"
//					<< QstField(RolePrimaryKey, "id")
//					<< QstField("manufacturer_vendor", FieldVisible, "Вендор-производитель", 120)
//					<< QstField("model", FieldVisible, "Модель", 120, true)
//					<< QstField("operating_system", FieldVisible, "ОС", 120)

//					<< QstField("id", value("id"))
//					;
//		}
//	else
//		if (queryNumber == TEST3)
//		{
//			batch	<< "vendor"
//					<< QstField(RoleParentKey, "parent_id")
//					<< QstField(RoleTreeLevel, "tree_level")
//					<< QstField("vendor_name", FieldVisible, "Название", 120)
//					<< QstField(RolePrimaryKey, "id")
//					<< QstField("it_priority_orientation", FieldVisible, "Главная деятельность", 120, true)

//					<< QstField("id", value("id"))
//					;
//		}
//	else
//	{
//		Q_ASSERT(false);
//	}

//	return batch;
//}


} // End of namespace QstTest
