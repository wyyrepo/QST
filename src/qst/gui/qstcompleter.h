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

#ifndef QSTCOMPLETER_H
#define QSTCOMPLETER_H

#include <QCompleter>
#include <QTimer>
#include <QObject>

#include <QLineEdit>
#include <QComboBox>

#include "qst/qstabstractmodelhandler.h"

namespace Qst
{
namespace Gui
{

class QstCompleter : public QObject
{
	Q_OBJECT

public:

	/*!
	\lang_Russian
		режим автоподстановки.
	\endlang

	\lang_English
		QstCompleter completion mode.
	\endlang
	*/
	typedef enum
	{
		PrefixPopupCompletion			= QCompleter::PopupCompletion,  /*!< Such as QCompleter::PopupCompletion. */
		PrefixUnfilteredPopupCompletion = QCompleter::UnfilteredPopupCompletion, /*!< Such as QCompleter::UnfilteredPopupCompletion. */
		PrefixInlineCompletion			= QCompleter::InlineCompletion, /*!< Such as QCompleter::InlineCompletion. */
		PrefixDBQueryingCompletion, /*!< In this mode, QstCompleter executes SQL query with WHERE clause to retrieve the data. Used prefix match for data ('val%').*/
		InfixDBQueryingCompletion, /*!< In this mode, QstCompleter executes SQL query with WHERE clause to retrieve the data. Used infix match for data (%val%).*/
		PostfixDBQueryingCompletion /*!< In this mode, QstCompleter executes SQL query with WHERE clause to retrieve the data. Used postfix match for data (%val).*/
	} QstCompletionMode;

	/*!
	\lang_Russian
		Тип сигнала у виджета, по которому должен срабатывать автокомплитер.
	\endlang

	\lang_English
		Widget signal at which to operate a completer.
	\endlang
	*/
	typedef enum
	{
		CustomCompleteSignal = 0,
		CompleteOnEdit,
		CompleteOnChange
	} CompleteSignal;

private:

	QCompleter *_completer;
	QCompleter _defaultCompleter;

	QstAbstractModelHandler *_handler;
	QstCompletionMode		_qstCompletionMode;

	QTimer	_completionTimer;

	int		_completionTimeout;
	int		_suffixLenght;
	QString _fieldName;

	bool	_enabled;

public:
	QstCompleter(QObject *parent = NULL);
	~QstCompleter();

	void setHandler(QstAbstractModelHandler *handler);
	QstAbstractModelHandler *handler();
	QstAbstractModelHandler *handler() const;

	void setQstCompletionMode(const QstCompletionMode &qstCompletionMode);
	QstCompletionMode qstCompletionMode() const;

	void setCompletionTimeout(const int &completionTimeout);
	int completionTimeout() const;

	void setCompletionSuffixLenght(const int &suffixLenght);
	int completionSuffixLenght() const;

	void setCompletionField(const QString &fieldName);
	QString completionField() const;

	void setCaseSensitivity(const Qt::CaseSensitivity &caseSensitivity);
	Qt::CaseSensitivity caseSensitivity() const;

	void setHelper(QLineEdit *lineEditHelper, const CompleteSignal &completeSignal = CustomCompleteSignal) const;
	void setHelper(QComboBox *comboBoxHelper, const CompleteSignal &completeSignal = CustomCompleteSignal) const;

	void setCompleter(QCompleter *completer);
	QCompleter *completer();

	bool prepare();
	bool isReady() const;

	void setFromOtherCompleter(const QstCompleter &otherCompleter);

	void setEnabled(const bool &enabled);
	bool isEnabled() const;

signals:

	void activated(const QVariantMap &selectedValuesMap);
	void highlighted(const QVariantMap &selectedValuesMap);

public slots:

	void complete(const QString &suffix);

private slots:

	void _complete();
	void _completerActivated(const QModelIndex &index);
	void _completerHighlighted(const QModelIndex &index);

private:

	void _setQstCompletionMode(QCompleter *completer,
							   const QstCompletionMode &qstCompletionMode);

	QueryField _completionField(const QString &suffix,
								const QstCompletionMode &qstCompletionMode);
};


} // End of namespace Gui
} // End of namespace Qst

#endif // QSTCOMPLETER_H
