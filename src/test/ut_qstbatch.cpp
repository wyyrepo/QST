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

#include "ut_qstbatch.h"

using namespace Qst;

typedef QstBatch T;

Q_DECLARE_METATYPE(QstBatch);

Q_DECLARE_METATYPE(QueryValue);

Q_DECLARE_METATYPE(QstField);

Q_DECLARE_METATYPE(QstFieldList);

namespace QstTest
{


/*!
\class ut_QstBatch
\brief
\lang_Russian
	Модульный тест для класса QstBatch.
\endlang

\lang_English
	Unit test for QstBatch.
\endlang
*/


ut_QstBatch::ut_QstBatch()
{
}

void ut_QstBatch::initTestCase()
{
//	m_b1 = QstBatch();

//	m_b2.addSource("Source1");
//	m_b2 << "Source2";
//	m_b2.addField(QstField());


//	m_b3.addSource("xSource");
//	m_b3	<< QstField(RolePrimaryKey, "Field4", FieldVisible, "EngTitle", 100);

//	m_b4.addSource("Tetha");
//	m_b4	<< QstField("Field1", FieldInvisible, "RusЗаголовок", 20)
//			<< QstField("Field4", FieldInvisible, "RusЗаголовок2", 20);


//	m_b5.addSource("vSomeSource");
//	m_b5	<< QstField(RolePrimaryKey, "ID", FieldVisible)
//			<< QstField(RoleParentKey, "Parent_ID", FieldVisible)
//			<< QstField("Field1", FieldVisible, "Поле1", 100)
//			<< QstField("Field2", FieldVisible, "Поле2", 50)
//			<< QstField("Field3", FieldVisible, "Поле3", 100)

//			<< QstField("Field1", QueryValue(), PurposeWhere)
//			<< QstField("Field2", QueryValue(Null), PurposeWhere)
//			<< QstField("Field3", QueryValue(10, Less), PurposeWhere)
//			<< QstField("Field4", QueryValue(QString("some string")), PurposeWhere)
//			<< QstField("Field5", QueryValue(QString("other string"), Equal, RightPercent), PurposeWhere)
//			;

//	m_b6.addSource("vView");
//	m_b6	<< QstField(RolePrimaryKey, "ID", FieldInvisible)
//			<< QstField(RoleParentKey, "Parent_ID", FieldInvisible)
//			<< QstField("Field1", FieldVisible, "ПолеX", 100)
//			<< QstField("Field2", FieldInvisible, "ПолеY", 50)
//			<< QstField("Field3", FieldVisible, "ПолеZ", 100)

//			<< QstField("Field4", QueryValue(QDate(2010, 03, 20)),
//						QueryValue(QDate(2010, 04, 1)))
//			<< QstField("Field5", QueryValue(3.14, GreaterEqualOrNull), PurposeInsert)
//			<< QstField("Field6", QueryValue(true), PurposeWhere)
//			;
}


} // End of namespace QstTest
