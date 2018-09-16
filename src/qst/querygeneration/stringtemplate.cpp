/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the QueryGeneration module of the QsT SQL Tools.
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

#include "stringtemplate.h"


/*!
\class StringTemplate
\brief
\lang_Russian
	Служебный класс. Используется при генерации запросов для упрощения кода.
	Нужен для обертки над функцией QString::arg(), чтобы избавиться от
	учета, сколько placeholders есть в строке. Здесь шаблон строки -
	это строка с неизвестным количеством placeholders.
\endlang

\lang_English
	Service class. Used in query generation to simplify the code. It is a wrapper
	for QString::arg() to avoid to know how many placeholders there are in the string.
	String teplate is the string with unknown count of placeholders.
\endlang
*/

StringTemplate::StringTemplate(const int & argsCount, const QString stringTemplate)
	:
	_argsCount(argsCount),
	_stringTemplate(stringTemplate)
{
}

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
StringTemplate::StringTemplate()
	:
	_argsCount(0),
	_stringTemplate(QString())
{
}

/*!
\lang_Russian
	Возвращает количество placeholders ('%1', '%2'...) в строке.
\endlang

\lang_English
	Returns count of the placeholders ('%1', '%2'...) in the string.
\endlang
*/
int StringTemplate::argsCount() const
{
	return _argsCount;
}

/*!
\lang_Russian
	Возвращает шаблон строки.
\endlang

\lang_English
	Returns template string.
\endlang
*/
QString StringTemplate::stringTemplate() const
{
	return _stringTemplate;
}

/*!
\lang_Russian
	Возвращает строку, подставляя arg1 вместо первого placeholder и пустые
	строчки вместо других placeholders, если они есть.
\endlang

\lang_English
	Returns a string, substituting arg1 instead of the first placeholder
	and empty string instead of the other placeholders, if any.
\endlang
*/
QString StringTemplate::string(const QString & arg1) const
{
	if (_argsCount == 3)
		return _stringTemplate.arg(arg1, "", "");
	else
		if (_argsCount == 2)
			return _stringTemplate.arg(arg1, "");

	return _stringTemplate.arg(arg1);
}

/*!
\lang_Russian
	Возвращает строку, подставляя arg1 и arg2 вместо %1%, %2%, и пустую
	строчку вместо других placeholders, если они есть. Если количество
	placeholders меньше, чем 2, QString не выдает warning.
\endlang

\lang_English
	Returns a string, substituting arg1 and arg2 instead of the %1% and %2%,
	 and empty string instead of the other placeholders, if any.
	 If the number of the placeholders is less than 2, QString does not issue warning.
\endlang
*/
QString StringTemplate::string(const QString & arg1, const QString & arg2) const
{
	if (_argsCount == 3)
		return _stringTemplate.arg(arg1, arg2, "");
	else
		if (_argsCount == 2)
			return _stringTemplate.arg(arg1, arg2);

	return _stringTemplate.arg(arg1);
}

/*!
\lang_Russian
	Возвращает строку, подставляя arg1, arg2 и arg3 вместо %1%, %2% и %3%. Если количество
	placeholders меньше, чем 3, QString не выдает warning.
\endlang

\lang_English
	Returns a string, substituting arg1, arg2 and arg3 instead of the %1%, %2%, %3%.
	 If the number of the placeholders is less than 3, QString does not issue warning.
\endlang
*/
QString StringTemplate::string(const QString & arg1, const QString & arg2, const QString & arg3) const
{
	if (_argsCount == 3)
		return _stringTemplate.arg(arg1, arg2, arg3);
	else
		if (_argsCount == 2)
			return _stringTemplate.arg(arg1, arg2);

	return _stringTemplate.arg(arg1);
}

