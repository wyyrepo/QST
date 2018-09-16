/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Special module of the QsT SQL Tools.
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

#include "abstractitemnameextractor.h"

namespace Qst
{
namespace Special
{

/*!
	\class QstAbstractItemNameExtractor
	\lang_Russian
	\brief Класс описывает интерфейс для извлечения имени поля из произвольного выражения.

	QstAbstractItemNameExtractor - абстрактный класс. С помощью его функций
	классы-наследники могут извлекать имя поля из произвольного строкового выражения,
	используя свои алгоритмы.
	\endlang

	\lang_English
	\brief The QstAbstractItemNameExtractor class is the abstract base
	class of interface for field name extracting from any SQL expression.

	This class implements an abstract interface. Subclasses of this class
	specify how the field name extracts from expressions. It can be complex
	regexp or simple string match depending of your needs.

	Field name extracting used when you call for field data by it's name.
	Look at the simple SELECT query:

	\code
	SELECT count(*) AS cnt
	FROM table
	\endcode

	Field represented as count(*) has name "cnt", so you can do that:

	\code
	TableModelHandler th;
	QstPlainQueryModel model;
	th.reload(SOME_QUERY_NAME, &model);
	QVariant data = th.fieldValue("cnt"); // Field name extracting == true by default.

	QVariant data = th.fieldValue("count(*) AS cnt", 0, false); // No field name extracting.
	\endcode

	\endlang
*/

/*! \lang_Russian Конструктор по умолчанию.
	\endlang

	\lang_English Default constructor.
	\endlang */
AbstractItemNameExtractor::AbstractItemNameExtractor()
{
}

/*! \lang_Russian Конструктор, принимающий строковую константу, которая содержит имя поля
для извлечения.
	\endlang

	\lang_English Constructor takes string constant for field name extracting.
	\endlang */
AbstractItemNameExtractor::AbstractItemNameExtractor(const QString &str)
{
}

/*! \lang_Russian Чисто виртуальный деструктор.
	\endlang

	\lang_English Pure virtual destructor.
	\endlang */
AbstractItemNameExtractor::~AbstractItemNameExtractor()
{
}

/*! \lang_Russian Извлекает имя поля из сохраненной ранее строковой константы.

	Виртуальная функция, возвращает по умолчанию QString(). Может быть переписана в классе-наследнике.
	\endlang

	\lang_English Extracts field name from early saved string constant.

	Virtual. In default implementation returns QString().
	\endlang */
QString AbstractItemNameExtractor::extract() const
{
	return QString();
}

/*! \lang_Russian Извлекает имя поля из переданной строковой константы.

	Виртуальная функция, возвращает QString(). Может быть переписана в классе-наследнике.
	\endlang

	\lang_English Extracts field name from str.

	Virtual. In default implementation returns str.
	\endlang */
QString AbstractItemNameExtractor::extract(const QString &str) const
{
	return str;
}


} // End of namespace Special
} // End of namespace Qst;
