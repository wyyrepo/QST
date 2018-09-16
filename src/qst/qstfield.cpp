/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
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

#include "qstfield.h"

#include <QObject>
#include <QDebug>

namespace Qst
{

/*!
	\class QstField

	\brief

	\lang_Russian
	Объекты класса QstField составляются в пакеты (QstBatch) и затем
	непосредственно участвуют в генерации SQL-запросов.
	Класс хранит имена полей для секций SELECT, ORDER BY, GROUP BY,
	хранит имя поля и значение для секций WHERE, JOIN, INSERT INTO / VALUES,
	UPDATE / SET, а так же представляет	параметр хранимой процедуры.

	Каждый класс QstField имеет свое назначение (QstField::purpose()).
	У поля может быть всего одно назначение. Чаще всего назначение
	задается по умолчанию и зависит от выбранного конструктора QstField.
	Есть конструкторы только для секции SELECT, только для секции WHERE,
	только для параметра процедуры. Есть общий конструктор для секций
	INSERT/UPDATE, WHERE и для параметра процедуры.
	Поле считается инвалидным, если его назначение установлено в PurposeNone.

	У поля есть имя - QstField::name(), котрое интерпретируется в зависимости
	от секции. Чаще всего оно представляет имя поля в БД.

	Те объекты QstField, которые предназначены для секции SELECT, могут
	быть снабжены дополнительной информацией, используемой при настройке
	представлений (QTableView, QTreeView, QListView, QComboBox):
		- видимость поля, невидимые поля отображаться не будут;
		- название колонки для этого поля (переводится функцией tr());
		- ширина колонки;
		- ориентация колонки (почти не используется).

	Подключаемые представления будут настраиваться в соответствии с этими
	параметрами.

	Чтобы назначить поле для QComboBox, следует установить
	параметр modelColumn в true. Если этот параметр не установлен, будет
	отображено первое видимое поле.

	\attention В QListView и QTreeView первым отображается поле
	с параметром isVisible() == true.

	\sa QstAbstractModelHandler

	Поле может иметь роль. роль используется для определения специальных полей,
	информация которых может пригодиться другим классам. Для ключевых полей
	есть специальный конструктор.

	Если указаны поля с ролями RoleParentKey и RoleTreeLevel, можно использовать
	древовидную модель данных QstTreeQueryModel совместно с QTreeView.
	Данные будут отображаться в виде дерева.
	\endlang

	\lang_English
	QstField represents such parts of SQL as field name in SELECT clause,
	one separate condition in WHERE clause, JOIN clause, field names in ORDER BY and GROUP BY
	clauses, field+value in INSERT and UPDATE queries and some other.

	Note that the table names, view names and names of stored procedures does not
	represented by QstField in query generation. In DFD ("declarative field descriptors")
	these entities called "query sources" and can be specified using simple string.
	See QstAbstractModelHandler and QstBatch for more information about
	query generation.

	Each QstField has it's own purpose, wich indicates how to use QstField.
	For example when someField.purpose() is PurposeWhere,
	query generator tried to compose WHERE clause with data someField.name(),
	someField.value().functor() and someField.value().value(). Here,
	someField.value() returns object of QueryValue().

	Only one purpose can be	seted. Often purpose is seted by default in the QstField constructors.
	There are constructors for SELECT clause only (so purpose is PurposeSelect),
	constructors for WHERE clause only (PurposeWhere), constructors
	for stored procedure parameter only and some other.

	Field name interpreting against query generation context but more often
	it's interpreting as table (view) field name.

	To get more GUI programming convenience for each SELECT-purposed
	field you can provide additional information:
	- column visibility;
	- column title (autotranslated by tr());
	- column width;
	- column orientation. (Usually not used.)

	Any view connected to query will be tunned respectively.

	To set a field is appeared QComboBox, you should set
	modelColumn parameter to true. If not, the first visible column is appear.

	\attention QListView shows first field with FieldVisible
	parameter.

	\sa QstAbstractModelHandler

	The field may have a role. Role is used to define special fields
	information which may be useful to other classes. For key fields
	there is a special constructor.

	If RoleParentKey-field available, you can use QstTreeQueryModel with
	QTreeView to get tree data view.
	\endlang
*/

/*! \lang_Russian Конструктор по умолчанию. Создает инвалидный QstField.
	\endlang

	\lang_English Default constructor. Creates invalid QstField.
	\endlang */

QstField::QstField()
	:
	_name(QString()),
	_visibility(VisibilityNone),
	_columnTitle(QString()),
	_columnWidth(0),
	_orientation(Qt::Horizontal),
	_role(RoleNone)
{
}

QstField::QstField(const FieldRole &role,
				  const QString &name,
				  const FieldVisibility &visibility,
				  const char *columnTitle,
				  const int &columnWidth,
				  const Qt::Orientation &titleOrientation)
	:
	_name(name),
	_visibility(visibility),
	_columnTitle(QObject::tr(columnTitle)),
	_columnWidth(columnWidth),
	_orientation(titleOrientation),
	_role(role)
{
}

/*! \lang_Russian Конструктор для поля в секции SELECT
	const * char версия для параметра columnTitle. Переводится функцией tr.
	\endlang

	\lang_English Constructor for SELECT field.
	const char * columnTitle parameter translated by tr().
	\endlang */
QstField::QstField(const QString &name,
				   const FieldVisibility &visibility,
				   const char *columnTitle,
				   const int &columnWidth,
				   const Qt::Orientation &titleOrientation)
	   :
	   _name(name),
	   _visibility(visibility),
	   _columnTitle(QObject::tr(columnTitle)),
	   _columnWidth(columnWidth),
	   _orientation(titleOrientation),
	   _role(RoleNone)
{
	if (!strlen(columnTitle))
		_columnTitle = name;
}

/*! \lang_Russian Возвращает полное имя поля (без извлечения реального имени).
	\sa QstAbstractItemNameExtractor
	\endlang

	\lang_English Returns field name (without field name extracting).
	\sa QstAbstractItemNameExtractor
	\endlang */
QString QstField::name() const
{
	return _name;
}

/*! \lang_Russian Устанавливает имя поля.
	\endlang

	\lang_English Sets field name.
	\endlang */
void QstField::setName(const QString &name)
{
	_name = name;
}

/*! \lang_Russian Устанавливает, видимое ли поле.
	\endlang

	\lang_English Sets if field is visible.
	\endlang

\verbatim
	bool == FieldVisible
	false == FieldInvisible
\endverbatim
*/
void QstField::setVisible(const bool &visible)
{
	if (visible)
		_visibility = FieldVisible;
	else
		_visibility = FieldInvisible;
}

/*! \lang_Russian Возвращает видимость поля.
	\endlang

	\lang_English Returns field visibility.
	\endlang */
FieldVisibility QstField::visibility() const
{
	return _visibility;
}

/*! \lang_Russian Устанавливает видимость поля.
	\endlang

	\lang_English Sets field visibility.
	\endlang */
void QstField::setVisibility(const FieldVisibility &visibility)
{
	_visibility = visibility;
}

/*! \lang_Russian Возвращает название колонки.
	\endlang

	\lang_English Returns column title.
	\endlang */
QString QstField::columnTitle() const
{
	return _columnTitle;
}

/*! \lang_Russian Устанавливает название колонки.
	\endlang

	\lang_English Sets column title.
	\endlang */
void QstField::setColumnTitle(const QString &title)
{
	_columnTitle = title;
}

/*! \lang_Russian Возвращает ширину колонки.
	\endlang

	\lang_English Returns column width.
	\endlang */
int QstField::columnWidth() const
{
	return _columnWidth;
}

/*! \lang_Russian Устанавливает ширину колонки.
	\endlang

	\lang_English Sets column width.
	\endlang */
void QstField::setColumnWidth(const int &width)
{
	_columnWidth = width;
}

/*! \lang_Russian Возвращает ориентацию колонки.
	\endlang

	\lang_English Returns column orientation.
	\endlang */
Qt::Orientation QstField::orientation() const
{
	return _orientation;
}

/*! \lang_Russian Устанавливает ориентацию колонки.
	\endlang

	\lang_English Sets column orientation.
	\endlang */
void QstField::setOrientation(const Qt::Orientation &orientation)
{
	_orientation = orientation;
}

/*! \lang_Russian Возвращает роль поля.
	\endlang

	\lang_English Returns field role.
	\endlang */
FieldRole	QstField::role() const
{
	return _role;
}

/*! \lang_Russian Возвращает true, если видимость поля - FieldVisible.
	\endlang

	\lang_English Returns true if field has FieldVisible visibility.
	\endlang */
bool QstField::isVisible() const
{
	return _visibility == FieldVisible;
}

/*! \lang_Russian Возвращает true, если видимость поля - FieldInvisible.
	\endlang

	\lang_English Returns true if field has FieldInvisible visibility.
	\endlang */
bool QstField::isInvisible() const
{
	return _visibility == FieldInvisible;
}

/*! \lang_Russian Возвращает true, если видимость поля - VisibilityNone.
	\endlang

	\lang_English Returns true if field has VisibilityNone visibility.
	\endlang */
bool QstField::isNoneVisibility() const
{
	return _visibility == VisibilityNone;
}

/*! \lang_Russian Возвращает true, если установлена роль поля, отличная от RoleNone.
	\endlang

	\lang_English Returns true if field has role different from RoleNone.
	\endlang */
bool QstField::hasRole() const
{
	return (_role != RoleNone);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QstField &f)
{
#ifndef Q_BROKEN_DEBUG_STREAM
	dbg.nospace() << "QstField(" << f.name() << "role: " << f.role() << ", visibility: " << f.visibility();
	dbg.nospace() << ", columnTitle: " << f.columnTitle() << ", columnWidth: " << f.columnWidth();
	dbg.nospace() << ", orientation: " << f.orientation();

	return dbg.space();
#else
	qWarning("This compiler doesn't support streaming QstField to QDebug");
	return dbg;
	Q_UNUSED(f);
#endif
}
#endif

}
