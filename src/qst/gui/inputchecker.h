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

#ifndef INPUTCHECKER_H
#define INPUTCHECKER_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QPoint>

#include <QLineEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include <QVariantMap>

#include <QPair>
#include <QStringList>

#include "qst/qstglobal.h"

namespace Qst
{
namespace Gui
{


class InputChecker : public QObject
{
	Q_OBJECT

private:

	QPoint _absolutePos;
	int	   _autofocusTimeout;

	QTimer	_widgetFocusTimer;
	QWidget *_widgetForFocus;

public:
	InputChecker(QObject *parent = NULL);

	void setAutofocusTimeout(const int &autofocusTimeout);
	void setAbsolutePosition(const QPoint &absolutePos);


	bool check(QLineEdit * lineEdit,
			   const ValueCheckFunctor &checker,
			   const QString &errorMessage,
			   const bool selectAllText = true);

	bool check(QLineEdit * lineEdit,
			   const ValueCheckFunctor &checker,
			   const QString &stringToCheck,
			   const QString &errorMessage,
			   const bool selectAllText = true);

	bool check(QDateTimeEdit * dateEdit,
			   const ValueCheckFunctor &checker,
			   const QDate &dateToCheck,
			   const QString &errorMessage);

	bool check(QSpinBox * spinBox,
			   const ValueCheckFunctor &checker,
			   const int &valueToCheck,
			   const QString &errorMessage);

	bool check(QDoubleSpinBox * spinBox,
			   const ValueCheckFunctor &checker,
			   const double &valueToCheck,
			   const QString &errorMessage);

	bool check(const bool &asserter,
			   const QString &errorMessage,
			   QWidget *widgetForFocus = NULL);

	StringListPair diff(const QVariantMap &valMap1,
						const QVariantMap &valMap2,
						const ValueCheckFunctor &checker,
						const DiffPolicy checkPolicy = FullDiff) const;

private slots:

	void _setFocusToWidget();
	void _startAutofocusTimer(QWidget *widgetForFocus);

	bool _check(const QVariant val1,
				const QVariant val2,
				const ValueCheckFunctor &checker) const;
};

} // End of namespace Gui
} // End of namespace Qst

#endif // INPUTCHECKER_H
