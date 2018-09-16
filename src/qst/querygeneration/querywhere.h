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

#ifndef QUERYWHERE_H
#define QUERYWHERE_H

#include "queryfield.h"
#include "queryin.h"

#include "qst/qstplaceholder.h"

#include <QList>

class QueryWhere
{
public:

	typedef enum
	{
		Invalid = 0,
		Valid = 1,
		PartialValid = 2
	} ConditionValidity;

	typedef QList<QueryWhere> ConditionList;

private:

	QueryField		_field;
	ConditionList	_conditionList;
	Qst::Relation	_relation;

public:
    QueryWhere();

	QueryWhere(const QString &condition);

	QueryWhere(const QString &fieldName,
			   const QueryValue &value,
			   const Qst::Functor &functor);

	QueryWhere(const QString &fieldName,
			   const QVariant &value1,
			   const QVariant &value2);

	QueryWhere(const QueryField &field);

	QueryWhere(const QueryIn &inCondition);

	QueryField field() const;
	void setField(const QueryField &field);

	void appendCondition(const QueryWhere &cond);
	ConditionList conditionList() const;
	void setConditionList(const ConditionList &list);

	Qst::Relation relation() const;
	void setRelation(const Qst::Relation &conditionList);

	ConditionValidity validity() const;

	bool isValid() const;
	bool isEmpty() const;

	QueryWhere validOnly() const;

	void updatePlaceholder(const QString &placeholderName,
						   const QueryValue &value,
						   const Qst::Functor &functor);

	void updatePlaceholder(const QString &placeholderName,
						   const QVariantList &varList,
						   const Qst::Functor &functor);

	void resetPlaceholders();

	QueryWhere &operator ||(const QueryWhere &condition);
	QueryWhere &operator &&(const QueryWhere &condition);
	QueryWhere &operator ||(const QueryIn &inCondition);
	QueryWhere &operator &&(const QueryIn &inCondition);

	friend bool operator==(const QueryWhere &c1, const QueryWhere &c2)
	{
		return c1._field == c2._field
				&& c1._relation == c2._relation
				&& c1._conditionList == c2._conditionList;
	}

	friend bool operator!=(const QueryWhere &c1, const QueryWhere &c2)
	{
		return !(c1 == c2);
	}

private:
	QueryWhere & _appendCondition(const QueryWhere &cond,
								  const Qst::Relation &relation);

};

///////////////////////////////////////////////////////////////////////////////////////////

typedef QueryWhere QueryOn;
typedef QueryWhere QueryHaving;

///////////////////////////////////////////////////////////////////////////////////////////

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QueryWhere &);
#endif

#endif // QUERYWHERE_H
