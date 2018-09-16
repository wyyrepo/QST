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

#ifndef QSTIF_H
#define QSTIF_H

#include <QtGlobal>

namespace Qst
{

/*!
  \class QstIf
  \brief
\lang_Russian
	Шаблонный класс. Предназначен для выбора объектов по условию.
	Наиболее близкая аналогия - оператор "?:" .
	В классе, который принимает объект класса QstIf, определяется поведение, что делать
	с объектом, если условие true или false, а так же что делать, если не установлены
	истинное или ложное значения возвращаемого объекта.
\endlang

\lang_English
	Template class. Class is the closest analogue of operaotr "?:".
	It used for condition-depending object selecting.
	Class-receiver that takes object of QstIf knows what to do
	if condition is true or false and what to do if no true or false values seted.
\endlang

\code
class T;

	if (condition)
		receiver.addClass(T(OneCase))
	else
		receiver.addClass(T(OtherCase))
\endcode
\code
class T;
typedef QstIf<T> QstIf_T;

	receiver.addClass(QstIf_T(condition, T(OneCase), T(OtherCase)))
\endcode
*/

template <class Type> class QstIf
{
public:

	typedef Type T;

private:

	bool	_hasTrueValue;
	bool	_hasFalseValue;

	bool	_condition;
	Type	_trueValue;
	Type	_falseValue;

public:

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
QstIf()
	:
	_hasTrueValue(false),
	_hasFalseValue(false),
	_condition(false)
{
}

/*!
\lang_Russian
	Конструктор, принимающий только истинное значение объекта Type.
\endlang

\lang_English
	Constructor, that takes true value of Type only.
\endlang
*/
QstIf(const bool &cond,
	  const Type &trueValue)
		  :
		  _hasTrueValue(true),
		  _hasFalseValue(false),
		  _condition(cond),
		  _trueValue(trueValue)
{
}

/*!
\lang_Russian
	Конструктор, принимающий как истинное, так и ложное значение объекта Type.
\endlang

\lang_English
	Constructor taking true and false value of Type.
\endlang
*/
QstIf(const bool &cond,
	  const Type &trueValue,
	  const Type &falseValue)
		  :
		  _hasTrueValue(true),
		  _hasFalseValue(true),
		  _condition(cond),
		  _trueValue(trueValue),
		  _falseValue(falseValue)
{
}

/*!
\lang_Russian
	Возвраащет истинное значение, если cond == true и ложное, если cond == false.

	Если значение не установлено, возвращает значение по умолчанию.
\endlang

\lang_English
	Returns true value if condition is true, and false value otherwise.
\endlang
*/
Type value() const
{
	if (_condition && _hasTrueValue)
		return _trueValue;

	if (!_condition && _hasFalseValue)
		return _falseValue;

return Type();
}

/*!
\lang_Russian
	Возвращает истинное значение вне зависимости от состояния cond.

	\attention Истинное значение должно быть установлено.
\endlang

\lang_English
	Returns true value regardless of condition.

	\attention True value must be seted.
\endlang
*/
Type trueValue() const
{
	Q_ASSERT(_hasTrueValue);
	return _trueValue;
}

/*!
\lang_Russian
	Возвращает ложное значение вне зависимости от состояния cond.

	\attention Ложное значение должно быть установлено.
\endlang

\lang_English
	Returns false value regardless of condition.

	\attention False value must be seted.
\endlang
*/
Type falseValue() const
{
	Q_ASSERT(_hasFalseValue);
	return _falseValue;
}

/*!
\lang_Russian
	Возвращает true, если истинное значение установлено.
\endlang

\lang_English
	Returns true if true value seted.
\endlang
*/
bool hasTrueValue() const
{
	return _hasTrueValue;
}

/*!
\lang_Russian
	Возвращает true, если ложное значение установлено.
\endlang

\lang_English
	Returns true if false value seted.
\endlang
*/
bool hasFalseValue() const
{
	return _hasFalseValue;
}

/*!
\lang_Russian
	Возвращает условие.
\endlang

\lang_English
	Returns condition.
\endlang
*/
bool condition() const
{
	return _condition;
}

}; // End of class QstIf


} // End of namespace Qst

#endif // QSTIF_H
