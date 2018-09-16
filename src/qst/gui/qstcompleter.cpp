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

#include "qstcompleter.h"
#include "settings.h"

#include <QDebug>

namespace Qst
{
namespace Gui
{

/*!
  \class QstCompleter
  \brief
\lang_Russian
	QstCompleter реализует механизм автоподстановки для строк редактирования и выпадающих списков.
	QstCompleter расширяет функциональность стандартного QCompleter, используя в работе
	функции QST для доступа к данным из базы данных.

	Существует три режима работы QCompleter'a:
\verbatim
	QCompleter::PopupCompletion				== QstCompleter::PrefixPopupCompletion
	QCompleter::UnfilteredPopupCompletion	== QstCompleter::PrefixUnfilteredPopupCompletion
	QCompleter::InlineCompletion			== QstCompleter::PrefixInlineCompletion
\endverbatim
	QstCompleter поддерживает эти режимы, а так же имеет три своих:
\verbatim
	QstCompleter::PrefixDBQueryingCompletion
	QstCompleter::InfixDBQueryingCompletion
	QstCompleter::PostfixDBQueryingCompletion
\endverbatim

	Эти три режима используют класс-хэндлер, чтобы сгенерировать запрос и получить из базы данных
	список данных для автозавершения. Какой запрос выполнится и какие данные будут получены,
	определяется хэндлером, следовательно, есть все возможности QST, чтобы извлечь только
	то, что требуется.

	Комплитер поддерживает несколько полезных функций:
	- задержка автозавершения на время таймаута;
	- порог срабатывания (количество введенных символов);
	- поле автозавершения указывается мнемонически, как оно описано в классе-хэндлере;
	- может включаться и выключаться по требованию;
	- для доступа к подсвеченной строке и к выбранным данным имеет сигналы activated(QVariantMap) и highlighted(QVariantMap).

	Пример использования:
\endlang

\lang_English
	QstCompleter implements the completions mechanism for the edit and combo boxes.
	QstCompleter extends the functionality of a standard QCompleter, using
	the work function of QST to access the database data.

	There are three modes of operation QCompleter'a:
\verbatim
	QCompleter::PopupCompletion				== QstCompleter::PrefixPopupCompletion
	QCompleter::UnfilteredPopupCompletion	== QstCompleter::PrefixUnfilteredPopupCompletion
	QCompleter::InlineCompletion			== QstCompleter::PrefixInlineCompletion
\endverbatim
	QstCompleter support these modes, and also has three of its:
\verbatim
	QstCompleter::PrefixDBQueryingCompletion
	QstCompleter::InfixDBQueryingCompletion
	QstCompleter::PostfixDBQueryingCompletion
\endverbatim

	These three modes use the class-handler to generate a query
	and obtain from the database list of data for completion.
	Which query is executed and what data will be received,
	the handler is defined. Consequently, there is every possibility of QST
	to extract only what is required.

	QstCompleter supports several useful features:
	- delay in the completion timeout;
	- threshold (number of characters);
	- completion field is mnemonic, as described in the class-handler;
	- can turn on and off on demand;
	- to access the highlighted line and to the selected data signals is activated(QVariantMap) and highlighted(QVariantMap).

	Use case:
\endlang
\code
 SubscriberHandler		_subscriberHandler;
 Qst::QstPlainQueryModel _model;

 // ...

 Qst::Gui::QstCompleter *_completer;

 _subscriberHandler.setQuery(SUBSCRIBERS);
 _subscriberHandler.setModel(&_model);

 _completer = new QstCompleter(this);
 _completer->setQstCompletionMode(QstCompleter::PrefixDBQueryingCompletion);
 _completer->setCompletionTimeout(500);
 _completer->setCompletionSuffixLenght(3);
 _completer->setCompletionField("full_name");
 _completer->setCaseSensitivity(Qt::CaseInsensitive);
 _completer->setHelper(ui->lineEdit, QstCompleter::CompleteOnEdit);
 _completer->setHandler(&_subscriberHandler);
 bool prepared = _completer->prepare();

 QObject::connect(_completer, SIGNAL(activated(QVariantMap)), this, SLOT(processCompletionData(QVariantMap)));
 QObject::connect(_completer, SIGNAL(highlighted(QVariantMap)), this, SLOT(showHighlightedData(QVariantMap)));
\endcode
*/

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
QstCompleter::QstCompleter(QObject *parent)
	:
	QObject(parent),
	_completer(&_defaultCompleter),
	_handler(NULL),
	_qstCompletionMode(PrefixPopupCompletion),
	_completionTimeout(QST_GUI_DEFAULT_COMPLETION_TIMEOUT),
	_suffixLenght(QST_GUI_DEFAULT_COMPLETION_SUFFIX_LENGTH),
	_fieldName(QString()),
	_enabled(true)
{
	_completionTimer.setSingleShot(true);
	_setQstCompletionMode(_completer, _qstCompletionMode);

	QObject::connect(&_completionTimer, SIGNAL(timeout()),
					 this, SLOT(_complete()));

	QObject::connect(_completer, SIGNAL(activated(QModelIndex)),
					 this, SLOT(_completerActivated(QModelIndex)));

	QObject::connect(_completer, SIGNAL(highlighted(QModelIndex)),
					 this, SLOT(_completerActivated(QModelIndex)));
}

/*!
\lang_Russian
	Деструктор по умолчанию.
\endlang

\lang_English
	Default destructor.
\endlang
*/
QstCompleter::~QstCompleter()
{
}

/*!
\lang_Russian
	Устанавливает класс-хэндлер.
\endlang

\lang_English
	Sets class-handler.
\endlang
*/
void QstCompleter::setHandler(QstAbstractModelHandler *handler)
{
	Q_ASSERT(handler != NULL);

	if (handler == NULL)
	return;

	_handler = handler;
}

/*!
\lang_Russian
	Возвращает указатель на класс-хэндлер.
\endlang

\lang_English
	Returns pointer to the class-handler.
\endlang
*/
QstAbstractModelHandler *QstCompleter::handler()
{
	return _handler;
}

/*!
\lang_Russian
	Возвращает константный указатель на класс-хэндлер.
\endlang

\lang_English
	Returns const pointer to the class-handler.
\endlang
*/
QstAbstractModelHandler *QstCompleter::handler() const
{
	return _handler;
}

/*!
\lang_Russian
	Устанавливает режим автодополнения.
\endlang

\lang_English
	Sets completion mode.
\endlang
*/
void QstCompleter::setQstCompletionMode(const QstCompletionMode &qstCompletionMode)
{
	_qstCompletionMode = qstCompletionMode;

	_setQstCompletionMode(_completer, _qstCompletionMode);
}

/*!
\lang_Russian
	Возвращает режим автодополнения.
\endlang

\lang_English
	Returns completion mode.
\endlang
*/
QstCompleter::QstCompletionMode QstCompleter::qstCompletionMode() const
{
	return _qstCompletionMode;
}

/*!
\lang_Russian
	Устанавливает таймаут срабатывания.

	Если completionTimeout < 0, устанавливается таймаут по умолчанию (QST_GUI_DEFAULT_COMPLETION_TIMEOUT).
\endlang

\lang_English
	Sets completion timeout.

	If completionTimeout < 0, set the timeout to defaults (QST_GUI_DEFAULT_COMPLETION_TIMEOUT).
\endlang
*/
void QstCompleter::setCompletionTimeout(const int &completionTimeout)
{
	if (completionTimeout >= 0)
		_completionTimeout = completionTimeout;
	else
		_completionTimeout = QST_GUI_DEFAULT_COMPLETION_TIMEOUT;
}

/*!
\lang_Russian
	Возвращает таймаут срабатывания.
\endlang

\lang_English
	Returns completion timeout.
\endlang
*/
int QstCompleter::completionTimeout() const
{
	return _completionTimeout;
}

/*!
\lang_Russian
	Устанавливает порог срабатывания. suffixLenght - минимальное количество символов для автозавершения.

	Если suffixLenght < 0, устанавливает порог по умолчанию (QST_GUI_DEFAULT_COMPLETION_SUFFIX_LENGTH).
\endlang

\lang_English
	Sets the threshold. suffixLenght is the minimum number of characters for completion.

	If suffixLenght < 0, set the threshold to defaults (QST_GUI_DEFAULT_COMPLETION_SUFFIX_LENGTH).
\endlang
*/
void QstCompleter::setCompletionSuffixLenght(const int &suffixLenght)
{
	if (suffixLenght >= 0)
		_suffixLenght = suffixLenght;
	else
		_suffixLenght = QST_GUI_DEFAULT_COMPLETION_SUFFIX_LENGTH;
}

/*!
\lang_Russian
	Возвращает порог срабатывания.
\endlang

\lang_English
	Returns threshold.
\endlang
*/
int QstCompleter::completionSuffixLenght() const
{
	return _suffixLenght;
}

/*!
\lang_Russian
	Устанавливает поле для автозавершения.
\endlang

\lang_English
	Sets completion field.
\endlang
*/
void QstCompleter::setCompletionField(const QString &fieldName)
{
	_fieldName = fieldName;
}

/*!
\lang_Russian
	Возвращает поле для автозавершения.
\endlang

\lang_English
	Returns completion field.
\endlang
*/
QString QstCompleter::completionField() const
{
	return _fieldName;
}

/*!
\lang_Russian
	Устанавливает чувствительность к регистру.
	Для режимов DBQyerying чувствительность к регистру определяется СУБД.
\endlang

\lang_English
	Sets case sensitivity.
	For DBQyerying modes case sensitivity is determined by the DBMS.
\endlang
*/
void QstCompleter::setCaseSensitivity(const Qt::CaseSensitivity &caseSensitivity)
{
	_defaultCompleter.setCaseSensitivity(caseSensitivity);

	if (_completer != NULL)
		_completer->setCaseSensitivity(caseSensitivity);
}

/*!
\lang_Russian
	Возвращает чувствительность к регистру.
\endlang

\lang_English
	Returns case sensitivity.
\endlang
*/
Qt::CaseSensitivity QstCompleter::caseSensitivity() const
{
	return _defaultCompleter.caseSensitivity();
}

/*!
\lang_Russian
	Устанавливает input widget (QLineEdit) для автозавершения.

	completeSignal определяет, какой сигнал виджета автоматически подключить к слоту complete(QString).
\verbatim
	CompleteOnEdit			Подключить сигнал textEdited(QString).
	CompleteOnChange		Подключить сигнал textChanged(QString).
	CustomCompleteSignal	Не подключать сигнал виджета автоматически.
							Нужный сигнал будет подключен где-то в другом месте.
\endverbatim
\endlang

\lang_English
	Sets the input widget (QLineEdit) for completion.

	completeSignal determines what signal the widget will automatically connect to a slot complete(QString).
\verbatim
	CompleteOnEdit			Connect the signal textEdited(QString).
	CompleteOnChange		Connect the signal textChanged(QString).
	CustomCompleteSignal	Do not connect the signal of the widget automatically.
							Desired signal will be connected somewhere else.
\endverbatim
\endlang
*/
void QstCompleter::setHelper(QLineEdit *lineEditHelper, const CompleteSignal &completeSignal) const
{
	Q_ASSERT(lineEditHelper != NULL);
	if (lineEditHelper == NULL)
		return;

	lineEditHelper->setCompleter(_completer);
	_completer->setWidget(lineEditHelper);

	switch (completeSignal)
	{
		case CustomCompleteSignal: break;
		case CompleteOnEdit:
			QObject::connect(lineEditHelper, SIGNAL(textEdited(QString)), this, SLOT(complete(QString)));
			break;
		case CompleteOnChange:
			QObject::connect(lineEditHelper, SIGNAL(textChanged(QString)), this, SLOT(complete(QString)));
			break;
	}
}

/*!
\lang_Russian
	Устанавливает input widget (QComboBox) для автозавершения.

	completeSignal определяет, какой сигнал виджета автоматически подключить к слоту complete(QString).
\verbatim
	CompleteOnEdit,
	CompleteOnChange		Подключить сигнал editTextChanged(QString).
	CustomCompleteSignal	Не подключать сигнал виджета автоматически.
							Нужный сигнал будет подключен где-то в другом месте.
\endverbatim
\endlang

\lang_English
	Sets the input widget (QComboBox) for completion.

	completeSignal determines what signal the widget will automatically connect to a slot complete(QString).
\verbatim
	CompleteOnEdit,
	CompleteOnChange		Connect the signal editTextChanged(QString).
	CustomCompleteSignal	Do not connect the signal of the widget automatically.
							Desired signal will be connected somewhere else.
\endverbatim
\endlang
*/
void QstCompleter::setHelper(QComboBox *comboBoxHelper, const CompleteSignal &completeSignal) const
{
	Q_ASSERT(comboBoxHelper != NULL);
	if (comboBoxHelper == NULL)
		return;

	comboBoxHelper->setCompleter(_completer);
	_completer->setWidget(comboBoxHelper);

	switch (completeSignal)
	{
		case CustomCompleteSignal: break;
		case CompleteOnEdit:
		case CompleteOnChange:
			QObject::connect(comboBoxHelper, SIGNAL(editTextChanged(QString)), this, SLOT(complete(QString)));
			break;
	}
}

/*!
\lang_Russian
	Устанавливает внешний QCompleter, алгоритмы которого используются для автозавершения.
\endlang

\lang_English
	Sets the external QCompleter, algorithms which are used to completion.
\endlang
*/
void QstCompleter::setCompleter(QCompleter *completer)
{
	if (completer == NULL)
		_completer = &_defaultCompleter;
	else
		_completer = completer;

_setQstCompletionMode(_completer, _qstCompletionMode);
}

/*!
\lang_Russian
	Возвращает QCompleter, алгоритмы которого используются для автозавершения.

	Если QCompleter был установлен ранее функцией setCompleter(), возвращается он.
	В противном случае возвращается QCompleter, используемый по умолчанию.
\endlang

\lang_English
	Returns QCompleter, algorithms which are used to completion.

	If QCompleter was previously installed by function setCompleter(), returned it.
	Otherwise, it returns QCompleter, used by default.
\endlang
*/
QCompleter *QstCompleter::completer()
{
	return _completer;
}

/*!
\lang_Russian
	Подготавливает QstCompleter к использованию.
	Возвращает true, если комплитер подготовлен успешно.
\endlang

\lang_English
	Prepares QstCompleter to use.
	Returns true on success.
\endlang
*/
bool QstCompleter::prepare()
{
	Q_ASSERT(isReady());
	if (!isReady())
		return false;

	int column = _handler->fieldColumnIndex(_fieldName);
	Q_ASSERT(column != -1);
	if (column == -1)
		return false;

	if (_defaultCompleter.caseSensitivity() == Qt::CaseInsensitive
		&& (_qstCompletionMode == PrefixDBQueryingCompletion
			|| _qstCompletionMode == InfixDBQueryingCompletion
			|| _qstCompletionMode == PostfixDBQueryingCompletion))
	{
		qDebug() << "\nAttention! In <DB querying completion mode> case sensitivity depends on database management system.";
	}


	_setQstCompletionMode(_completer, _qstCompletionMode);
	_completer->setCompletionColumn(column);

	if (!_handler->isLoaded()
		&& (_qstCompletionMode == PrefixPopupCompletion
			|| _qstCompletionMode == PrefixUnfilteredPopupCompletion
			|| _qstCompletionMode == PrefixInlineCompletion))
		_handler->reload();

return true;
}

/*!
\lang_Russian
	Возвращает true, если загружены все данные для автозавершения.
\endlang

\lang_English
	Returns true, if you downloaded all the data for completion.
\endlang
*/
bool QstCompleter::isReady() const
{
	return _handler != NULL
			&& !_fieldName.isEmpty()
			&& _completer->widget() != NULL;
}

/*!
\lang_Russian
	Копирует настройки из другого QstCompleter'а.
\endlang

\lang_English
	Copies settings from another QstCompleter.
\endlang
*/
void QstCompleter::setFromOtherCompleter(const QstCompleter &otherCompleter)
{
	setHandler(otherCompleter.handler());
	setQstCompletionMode(otherCompleter.qstCompletionMode());
	setCompletionTimeout(otherCompleter.completionTimeout());
	setCompletionSuffixLenght(otherCompleter.completionSuffixLenght());
	setCompletionField(otherCompleter.completionField());
}

/*!
\lang_Russian
	Активирует QstCompleter.
\endlang

\lang_English
	Enables QstCompleter.
\endlang
*/
void QstCompleter::setEnabled(const bool &enabled)
{
	_enabled = enabled;
}

/*!
\lang_Russian
	Деактивирует QstCompleter.
\endlang

\lang_English
	Disables QstCompleter.
\endlang
*/
bool QstCompleter::isEnabled() const
{
	return _enabled;
}

/*!
\lang_Russian
	Инициирует автозавершение для строки suffix.
\endlang

\lang_English
	Initiates completion for a string suffix.
\endlang
*/
void QstCompleter::complete(const QString &suffix)
{
	if (!isReady()
		|| !isEnabled()
		|| suffix.length() < _suffixLenght)
	{
		_completer->setModel(NULL);
		_completionTimer.stop();
		return;
	}

	// Перезагрузить модель при соответствующем режиме автозавершения.
	if (_qstCompletionMode == InfixDBQueryingCompletion
		|| _qstCompletionMode == PrefixDBQueryingCompletion
		|| _qstCompletionMode == PostfixDBQueryingCompletion)
	{
		QueryField complField = _completionField(suffix, _qstCompletionMode);
//		_handler->updatePlaceholder(_fieldName, complField.value().value(), complField.functor());
		_handler->reload();
	}

	_completionTimer.start(_completionTimeout);
}

void QstCompleter::_complete()
{
	if (!isReady())
		return;

	_completer->setModel(_handler->modelViewDescriptor()->model());
	_completer->complete();
}

void QstCompleter::_completerActivated(const QModelIndex &index)
{
	emit activated(_handler->fieldsValueMap(QStringList(), index));
}

void QstCompleter::_completerHighlighted(const QModelIndex &index)
{
	emit highlighted(_handler->fieldsValueMap(QStringList(), index));
}

void QstCompleter::_setQstCompletionMode(QCompleter *completer,
										 const QstCompletionMode &qstCompletionMode)
{
	switch (qstCompletionMode)
	{
	case PrefixPopupCompletion:
		completer->setCompletionMode(QCompleter::PopupCompletion);
		break;

	case PrefixInlineCompletion:
		completer->setCompletionMode(QCompleter::InlineCompletion);
		break;

	case PrefixUnfilteredPopupCompletion:
	case PrefixDBQueryingCompletion:
	case InfixDBQueryingCompletion:
	case PostfixDBQueryingCompletion:
		completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
		break;
	};
}

QueryField QstCompleter::_completionField(const QString &suffix,
										  const QstCompletionMode &qstCompletionMode)
{
	switch (qstCompletionMode)
	{
	case PrefixPopupCompletion:
	case PrefixInlineCompletion:
	case PrefixUnfilteredPopupCompletion:
		return QueryField("", QueryValue(QVariant(suffix), NoPercentPlaceholders), Like);

	case PrefixDBQueryingCompletion:
		return QueryField("", QueryValue(QVariant(suffix), RightPercent), Like);

	case InfixDBQueryingCompletion:
		return QueryField("", QueryValue(QVariant(suffix), LeftRightPercents), Like);

	case PostfixDBQueryingCompletion:
		return QueryField("", QueryValue(QVariant(suffix), LeftPercent), Like);
	};

	return QueryField("", QueryValue(QVariant(suffix), NoPercentPlaceholders), Equal);
}


} // End of namespace Gui
} // End of namespace Qst
