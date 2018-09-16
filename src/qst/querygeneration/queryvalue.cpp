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

#include "queryvalue.h"

#include <QDebug>

using namespace Qst;

QueryValue::QueryValue()
	:
	  _value(QVariant()),
	  _percentPlacehodlers(Qst::NoPercentPlaceholders)
{
}

QueryValue::QueryValue(const QVariant &value,
					   const PercentPlaceholders &percentPlaceholders)
	:
	  _value(value),
	  _percentPlacehodlers(percentPlaceholders)
{
}

QVariant QueryValue::value() const
{
	return _value;
}

void QueryValue::setValue(const QVariant &value)
{
	_value = value;
}

PercentPlaceholders QueryValue::percentPlaceholders() const
{
	return _percentPlacehodlers;
}

void QueryValue::setPercentPlaceholders(const PercentPlaceholders &percentPlaceholders)
{
	_percentPlacehodlers = percentPlaceholders;
}

bool QueryValue::isNull() const
{
	return _value.isNull();
}

bool QueryValue::isValid() const
{
	return _value.isValid();
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QueryValue &v)
{
#ifndef Q_BROKEN_DEBUG_STREAM
	dbg.nospace() << "QueryValue(value = " << v.value();
	dbg.nospace() << ", percentPlaceholders = " << v.percentPlaceholders();

	return dbg.space();
#else
	qWarning("This compiler doesn't support streaming QueryValue to QDebug");
	return dbg;
	Q_UNUSED(v);
#endif
}
#endif
