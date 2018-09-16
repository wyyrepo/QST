/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst::Gui module of the QsT SQL Tools.
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

#include "inputchecker.h"

#include "settings.h"

#include <QToolTip>
#include <QDebug>

namespace Qst
{
namespace Gui
{

/*! \class InputChecker
	\brief Класс проверяет значение в input-виджете на валидность.
	Может выдать QToolTip с ошибкой и передать фокус на input-виджет.

	Использование.

	InputChecker наследуется от QObject, чтобы работать с сигналами, из-за этого
	экземпляр класса может быть создан только через new. Данное органичение,
	возможно, будет снято переписыванием класса так, чтобы он не был наследником
	QObject, а вместо него с сигналами работал специальный класс-маппер.

	\code
	InputChecker *_inputChecker = new InputChecker(this);
	// this - родительский объект, который возьмет на себя обязанность удалить
	// inputChecker. В данном случае this - указатель на диалог, в котором
	// проводится проверка введенных значений.

	Q_ASSERT(_inputChecker != NULL);

	// Устанавливаем абсолютную позицию - позицию родительского окна (диалога).
	// Относительно нее будет размещаться toolTip с подсказкой.
	_inputChecker->setAbsolutePosition(this->pos());

	// Устанавливаем таймаут, по истечении которого inputWidget получит фокус.
	_inputChecker->setAutofocusTimeout(AUTOFOCUS_TIMEOUT);

	// Создаем две проверки:
	// - имя пользователя не должно быть пустым, не должно быть пробелами;
	// - пароль не должен быть пустым.

	if (!_inputChecker->check(ui->le_UserName,
							  CheckNotSimplifiedEmpty,
							  tr("Введите имя пользователя.")))
		return;

	if (!_inputChecker->check(ui->le_Password,
							  CheckNotEmpty,
							  tr("Введите пароль.")))
		return;

	\endcode
*/

/*! Конструктор по умолчанию. */
InputChecker::InputChecker(QObject *parent)
	: QObject(parent),
	_autofocusTimeout(AUTOFOCUS_TIMEOUT)
{
	QObject::connect(&_widgetFocusTimer, SIGNAL(timeout()),
					 this, SLOT(_setFocusToWidget()));

	_widgetFocusTimer.setSingleShot(true);
}

/*! Устанавливает таймаут для автофокуса.

	При autofocusTimeout == -1 автофокус отключен. */
void InputChecker::setAutofocusTimeout(const int &autofocusTimeout)
{
	_autofocusTimeout = autofocusTimeout;
}

/*! Устанавливает абсолютную позицию, относительно которой будет размещаться
	toolTip. Обычно это позиция левого верхнего угла родительского окна. */
void InputChecker::setAbsolutePosition(const QPoint &absolutePos)
{
	_absolutePos = absolutePos;
}

/*! Проверка текста в строке редактирования.

	Возвращает true, если текст соответствует правилу checker.
	Возвращает false и показывает toolTip с ошибкой errorMessage во всех остальных случаях.

	Положение toolTip вычисляется как absolutePos + lineEdit->pos.

	Если таймаут автофокуса != -1, виджет lineEdit получит фокус через это время.
	Если установлен параметр selectAllText, текст в виджете выделится.

	Не поддерживаются следующие элементы ValueChecker:
	CheckEqual, CheckNotEqual, CheckLess, CheckGreater, CheckLessEqual, CheckGreaterEqual.

	\sa setAbsolutePosition(), setAutofocusTimeout() */
bool InputChecker::check(QLineEdit * lineEdit,
						 const ValueCheckFunctor &checker,
						 const QString &errorMessage,
						 const bool selectAllText)
{
	Q_ASSERT(lineEdit != NULL);

bool pass = true;

	switch (checker)
	{
	case CheckIsEmpty:				pass = lineEdit->text().isEmpty();	break;
	case CheckIsSimplifiedEmpty:	pass = lineEdit->text().simplified().isEmpty();  break;
	case CheckNotEmpty:				pass = !lineEdit->text().isEmpty();  break;
	case CheckNotSimplifiedEmpty:	pass = !lineEdit->text().simplified().isEmpty();  break;

	case CheckEqual: case CheckNotEqual: case CheckLess: case CheckGreater:
	case CheckLessEqual: case CheckGreaterEqual:
		{
			qDebug() << "Warning! Check Equal, NotEqual, Less, Greater, LessEqual or GreaterEqual has no effect with this function.";
			Q_ASSERT(false);
			return false;
		}
	};

	if (!pass)
	{
		if (selectAllText)
			lineEdit->selectAll();

		QToolTip::showText(_absolutePos  + lineEdit->pos(),
						   errorMessage);

		_startAutofocusTimer(lineEdit);
	}

	return pass;
}

/*! Сравнение текста в lineEdit со строкой stringToCheck.

	Возвращает true, если выполняется отношение: lineEdit->text() <checker> stringToCheck.
	Возвращает false и показывает toolTip с ошибкой errorMessage во всех остальных случаях.

	Положение toolTip вычисляется как absolutePos + lineEdit->pos.

	Если таймаут автофокуса != -1, виджет lineEdit получит фокус через это время.
	Если установлен параметр selectAllText, текст в виджете выделится.

	Не поддерживаются следующие элементы ValueChecker:
	CheckIsEmpty, CheckIsSimplifiedEmpty, CheckNotEmpty, CheckNotSimplifiedEmpty.

	\sa setAbsolutePosition(), setAutofocusTimeout()*/
bool InputChecker::check(QLineEdit * lineEdit,
						 const ValueCheckFunctor &checker,
						 const QString &stringToCheck,
						 const QString &errorMessage,
						 const bool selectAllText)
{
	Q_ASSERT(lineEdit != NULL);

bool pass = true;

	switch (checker)
	{
	case CheckEqual:		pass = lineEdit->text() == stringToCheck;	break;
	case CheckNotEqual:		pass = lineEdit->text() != stringToCheck;  break;
	case CheckLess:			pass = lineEdit->text() < stringToCheck; break;
	case CheckGreater:		pass = lineEdit->text() > stringToCheck; break;
	case CheckLessEqual:	pass = lineEdit->text() <= stringToCheck; break;
	case CheckGreaterEqual:	pass = lineEdit->text() >= stringToCheck; break;

	case CheckIsEmpty: case CheckIsSimplifiedEmpty:
	case CheckNotEmpty: case CheckNotSimplifiedEmpty:
		{
			qDebug() << "Warning! Check IsEmpty, IsSimplifiedEmpty, NotEmpty or NotSimplifiedEmpty has no effect with this function.\nTry check(QLineEdit*, Checker, QString, bool) instead.";
			Q_ASSERT(false);
			return false;
		}
	};

	if (!pass)
	{
		if (selectAllText)
			lineEdit->selectAll();

		QToolTip::showText(_absolutePos  + lineEdit->pos(),
						   errorMessage);

		_startAutofocusTimer(lineEdit);
	}

	return pass;
}

/*! Сравнение даты в dateEdit с датой dateToCheck.

	Возвращает true, если выполняется отношение: dateEdit->date() <checker> dateToCheck.
	Возвращает false и показывает toolTip с ошибкой errorMessage во всех остальных случаях.

	Положение toolTip вычисляется как absolutePos + dateEdit->pos.

	Если таймаут автофокуса != -1, виджет dateEdit получит фокус через это время.

	Не поддерживаются следующие элементы ValueChecker:
	CheckIsEmpty, CheckIsSimplifiedEmpty, CheckNotEmpty, CheckNotSimplifiedEmpty.

	\sa setAbsolutePosition(), setAutofocusTimeout()*/
bool InputChecker::check(QDateTimeEdit * dateEdit,
						 const ValueCheckFunctor &checker,
						 const QDate &dateToCheck,
						 const QString &errorMessage)
{
	Q_ASSERT(dateEdit != NULL);

	bool pass = true;

		switch (checker)
		{
		case CheckEqual:		pass = dateEdit->date() == dateToCheck;	break;
		case CheckNotEqual:		pass = dateEdit->date() != dateToCheck;  break;
		case CheckLess:			pass = dateEdit->date() < dateToCheck; break;
		case CheckGreater:		pass = dateEdit->date() > dateToCheck; break;
		case CheckLessEqual:	pass = dateEdit->date() <= dateToCheck; break;
		case CheckGreaterEqual:	pass = dateEdit->date() >= dateToCheck; break;

		case CheckIsEmpty: case CheckIsSimplifiedEmpty:
		case CheckNotEmpty: case CheckNotSimplifiedEmpty:
			{
				qDebug() << "Warning! Check IsEmpty, IsSimplifiedEmpty, NotEmpty or NotSimplifiedEmpty has no effect with this function.";
				Q_ASSERT(false);
				return false;
			}
		};

		if (!pass)
		{
			QToolTip::showText(_absolutePos  + dateEdit->pos(),
							   errorMessage);

			_startAutofocusTimer(dateEdit);
		}

return pass;
}

/*! Сравнение целого числа в spinBox с числом valueToCheck.

	Возвращает true, если выполняется отношение: spinBox->value() <checker> valueToCheck.
	Возвращает false и показывает toolTip с ошибкой errorMessage во всех остальных случаях.

	Положение toolTip вычисляется как absolutePos + spinBox->pos.

	Если таймаут автофокуса != -1, виджет spinBox получит фокус через это время.

	Не поддерживаются следующие элементы ValueChecker:
	CheckIsEmpty, CheckIsSimplifiedEmpty, CheckNotEmpty, CheckNotSimplifiedEmpty.

	\sa setAbsolutePosition(), setAutofocusTimeout()
*/
bool InputChecker::check(QSpinBox * spinBox,
						 const ValueCheckFunctor &checker,
						 const int &valueToCheck,
						 const QString &errorMessage)
{
	Q_ASSERT(spinBox != NULL);

	bool pass = true;

		switch (checker)
		{
		case CheckEqual:		pass = spinBox->value() == valueToCheck; break;
		case CheckNotEqual:		pass = spinBox->value() != valueToCheck; break;
		case CheckLess:			pass = spinBox->value() <  valueToCheck; break;
		case CheckGreater:		pass = spinBox->value() >  valueToCheck; break;
		case CheckLessEqual:	pass = spinBox->value() <= valueToCheck; break;
		case CheckGreaterEqual:	pass = spinBox->value() >= valueToCheck; break;

		case CheckIsEmpty:  case CheckIsSimplifiedEmpty:
		case CheckNotEmpty: case CheckNotSimplifiedEmpty:
			{
				qDebug() << "Warning! Check IsEmpty, IsSimplifiedEmpty, NotEmpty or NotSimplifiedEmpty has no effect with this function.";
				Q_ASSERT(false);
				return false;
			}
		};

		if (!pass)
		{
			QToolTip::showText(_absolutePos  + spinBox->pos(), errorMessage);

			_startAutofocusTimer(spinBox);
		}

return pass;
}

bool InputChecker::check(QDoubleSpinBox * spinBox,
						 const ValueCheckFunctor &checker,
						 const double &valueToCheck,
						 const QString &errorMessage)
{
	Q_ASSERT(spinBox != NULL);

	bool pass = true;

		switch (checker)
		{
		case CheckEqual:		pass = spinBox->value() == valueToCheck; break;
		case CheckNotEqual:		pass = spinBox->value() != valueToCheck; break;
		case CheckLess:			pass = spinBox->value() <  valueToCheck; break;
		case CheckGreater:		pass = spinBox->value() >  valueToCheck; break;
		case CheckLessEqual:	pass = spinBox->value() <= valueToCheck; break;
		case CheckGreaterEqual:	pass = spinBox->value() >= valueToCheck; break;

		case CheckIsEmpty:  case CheckIsSimplifiedEmpty:
		case CheckNotEmpty: case CheckNotSimplifiedEmpty:
			{
				qDebug() << "Warning! Check IsEmpty, IsSimplifiedEmpty, NotEmpty or NotSimplifiedEmpty has no effect with this function.";
				Q_ASSERT(false);
				return false;
			}
		};

		if (!pass)
		{
			QToolTip::showText(_absolutePos  + spinBox->pos(), errorMessage);

			_startAutofocusTimer(spinBox);
		}

return pass;
}

/*! Проверка логического условия.

	Возвращает true, если asserter == true.
	Возвращает false и показывает toolTip с ошибкой errorMessage во всех остальных случаях.

	Положение toolTip вычисляется как absolutePos + widgetForFocus->pos.

	Если таймаут автофокуса != -1, и задан widgetForFocus, он получит фокус через это время.

	\sa setAbsolutePosition(), setAutofocusTimeout() */
bool InputChecker::check(const bool &asserter,
						 const QString &errorMessage,
						 QWidget *widgetForFocus)
{
	QPoint point;
	if (widgetForFocus != NULL)
		point = widgetForFocus->pos();

	if (!asserter)
	{
		QToolTip::showText(_absolutePos + point, errorMessage);
		_startAutofocusTimer(widgetForFocus);
	}

	return asserter;
}

/*! Сравнение двух массивов QVariant.

	Возвращает StringListPair, содержащий разницу в массивах, вычисленную
	в соответствии с политикой сравнения checkPolicy.

	Происходит сравнение QVariant-значений в массивах valMap1 и valMap2. Ключи,
	значения которых не проходят проверку по условию <checker>, заносятся
	в StringListPair.

	\code
	typedef QPair<QStringList, QStringList> StringListPair;
	\endcode

	Ключи valMap1 вносятся в StringListPair::first, ключи valMap2 вносятся
	в StringListPair::second.

	Политика JoinPolicy указывает, как обрабатывать ключ, который есть в одном
	массиве, но отсутствует в другом.

	\code
	FullJoin	Учитывается разница первого массива ко второму и второго к первому.
	LeftJoin	Учитывается только разница первого массива ко второму.
	RightJoin	Учитывается только разница второго массива к первому.
	InnerJoin	Несовпадающие ключи игнорируются.
	\endcode

	\sa check(), setAbsolutePosition(), setAutofocusTimeout() */
StringListPair InputChecker::diff(const QVariantMap &valMap1,
								  const QVariantMap &valMap2,
								  const ValueCheckFunctor &checker,
								  const DiffPolicy checkPolicy) const
{
	StringListPair differences;

	QVariantMap::const_iterator iter;

	for (iter = valMap1.begin(); iter != valMap1.end(); ++iter)
	{
		if (valMap2.contains(iter.key()))
		{
			if (!_check(iter.value(), valMap2[iter.key()], checker))
			{
				differences.first << iter.key();
				differences.second << iter.key();
			}
		}
		else
		{
			if (!(checkPolicy & RightDiff))
				differences.first << iter.key();
		}
	}

	if (!(checkPolicy & LeftDiff))
		for (iter = valMap2.begin(); iter != valMap2.end(); ++iter)
			if (!valMap1.contains(iter.key()))
			{
				differences.second << iter.key();
			}

	return differences;
}


void InputChecker::_setFocusToWidget()
{
	_widgetFocusTimer.stop();

	if (_widgetForFocus == NULL)
		return;

	_widgetForFocus->setFocus(Qt::OtherFocusReason);
	_widgetForFocus = NULL;
}

void InputChecker::_startAutofocusTimer(QWidget *widgetForFocus)
{
	if ((_autofocusTimeout != -1) && (widgetForFocus != NULL))
	{
		_widgetForFocus = widgetForFocus;
		_widgetFocusTimer.start(_autofocusTimeout);
	}
}

bool InputChecker::_check(const QVariant val1,
						  const QVariant val2,
						  const ValueCheckFunctor &checker) const
{
	switch (checker)
	{
	case CheckEqual:		return val1 == val2;
	case CheckNotEqual:		return val1 != val2;

	case CheckLess:			return val1.toString() < val2.toString();
	case CheckGreater:		return val2.toString() > val2.toString();
	case CheckLessEqual:	return val1.toString() <= val2.toString();
	case CheckGreaterEqual:	return val1.toString() >= val2.toString();

	case CheckIsEmpty:
	case CheckIsSimplifiedEmpty:
	case CheckNotEmpty:
	case CheckNotSimplifiedEmpty:
		{
			qDebug() << "Warning! Check IsEmpty, IsSimplifiedEmpty, NotEmpty or NotSimplifiedEmpty has no effect with this function.";
			Q_ASSERT(false);
			return false;
		}
	}
return false;
}

} // End of namespace Gui
} // End of namespace Qst
