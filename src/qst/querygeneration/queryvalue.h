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

#ifndef QUERYVALUE_H
#define QUERYVALUE_H

#include "qst/qstglobal.h"

#include <QVariant>
#include <QList>

class QueryValue // FIX ME: documentation // Full rewrite
{

private:

	QVariant _value;

	Qst::PercentPlaceholders _percentPlacehodlers;

public:

	QueryValue();

	// explicit for QueryWhere between constructor.
	explicit QueryValue(const QVariant &value,
						const Qst::PercentPlaceholders &percentPlaceholders = Qst::NoPercentPlaceholders);

	QVariant value() const;
	void     setValue(const QVariant &value);

	Qst::PercentPlaceholders percentPlaceholders() const;
	void setPercentPlaceholders(const Qst::PercentPlaceholders &percentPlaceholders);

	bool isNull() const;
	bool isValid() const;

	friend bool operator==(const QueryValue &val1, const QueryValue val2)
	{
		return val1._value == val2._value
				&& val1._percentPlacehodlers == val2._percentPlacehodlers;
	}
};

////////////////////////////////////////////////////////////////////////////////

typedef QList<QueryValue>         QueryValueList;
typedef QMap<QString, QueryValue> QueryValueMap;

////////////////////////////////////////////////////////////////////////////////

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QueryValue &);
#endif


#endif // QUERYVALUE_H
