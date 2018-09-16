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

#include "querywhere.h"

/*! \class QueryWhere
	\brief QueryWhere


Figure1.
QueryWhere 1 && Queryhere 2:
 ______          ______
|QW1|f1|   &&   |QW2|f2|

Result:
 ___
|QW3|
&&|_ ______   ______
	|QW1|f1|-|QW2|f2|

Figure2.
QueryWhere 1 && QueryWhere x:
 ______            ___
|QW1|f1|   &&     |QWx|
				  Or|_ ______   ______
					  |QW2|f2|-|QW3|f3|-...

Result:
 _____
|_QWy_|
&&|_ ______   ___
	|QW1|f1|-|QWx|
			 Or|_ ______   ______
				 |QW2|f2|-|QW3|f3|-...
*/

#include <QDebug>

using namespace Qst;

QueryWhere::QueryWhere()
	:
	_relation(UndefinedRelation)
{
}

QueryWhere::QueryWhere(const QString &condition)
	:
	  _field(QueryField(condition)),
	  _relation(UndefinedRelation)
{
}

QueryWhere::QueryWhere(const QString &fieldName,
					   const QueryValue &value,
					   const Functor &functor)
	:
	  _field(QueryField(fieldName, value, functor)),
	  _relation(UndefinedRelation)
{
}

QueryWhere::QueryWhere(const QString &fieldName,
					   const QVariant &value1,
					   const QVariant &value2)
	:
	  _field(QueryField(fieldName,
						QueryValue(value1, Qst::NoPercentPlaceholders),
						QueryValue(value2, Qst::NoPercentPlaceholders),
						ClauseCondition_Mask)),
	  _relation(UndefinedRelation)
{
}

QueryWhere::QueryWhere(const QueryField &field)
	:
	_field(field),
	_relation(UndefinedRelation)
{
}

QueryWhere::QueryWhere(const QueryIn &inCondition)
	:
	  _relation(UndefinedRelation)
{
	_field = inCondition.field();
	_field.setClause(ClauseWhere);
}

QueryField QueryWhere::field() const
{
	return _field;
}

void QueryWhere::setField(const QueryField &field)
{
	_field = field;
}

void QueryWhere::appendCondition(const QueryWhere &cond)
{
	_conditionList.append(cond);
}

QueryWhere::ConditionList QueryWhere::conditionList() const
{
	return _conditionList;
}

void QueryWhere::setConditionList(const ConditionList &list)
{
	_conditionList = list;
}

Relation QueryWhere::relation() const
{
	return _relation;
}

void QueryWhere::setRelation(const Relation &relation)
{
	_relation = relation;
}

QueryWhere::ConditionValidity QueryWhere::validity() const
{
	bool v = false, i = false, p = false;
	foreach (QueryWhere cond, _conditionList)
		switch (cond.validity())
		{
		case Invalid: i      = true; break;
		case Valid:   v      = true; break;
		case PartialValid: p = true; break;
		}

	if ( v &&  i &&  p) return PartialValid;
	if ( v &&  i && !p) return PartialValid;
	if ( v && !i &&  p) return PartialValid;
	if ( v && !i && !p) return Valid;
	if (!v &&  i &&  p) return PartialValid;
	if (!v &&  i && !p) return Invalid;
	if (!v && !i &&  p) return PartialValid;
	if (!v && !i && !p)
	{
		if (_relation == UndefinedRelation && !_field.isValid())
			return Invalid;
		if (_relation & Operators_Mask)
			return Invalid;
	}
return Valid;
}

bool QueryWhere::isValid() const
{
	return validity() == Valid;
}

bool QueryWhere::isEmpty() const
{
	return (!_field.isValid()
			&& _conditionList.isEmpty());
}

QueryWhere QueryWhere::validOnly() const
{
	if (_relation & Operators_Mask)
	{
		Q_ASSERT(!_field.isValid());
		Q_ASSERT(!_conditionList.isEmpty());

		ConditionList list;
		foreach (QueryWhere w, _conditionList)
		{
			QueryWhere tmpw = w.validOnly();
			if (tmpw.isValid())
				list.append(w);
		}

		if (list.isEmpty()) return QueryWhere();
		if (list.count() == 1) return list.first();
		QueryWhere w;
		w.setConditionList(list);
		w.setRelation(_relation);
		return w;
	}
	else
	{
		Q_ASSERT(_relation == UndefinedRelation);
		Q_ASSERT(_conditionList.isEmpty());

		if (field().isValid()) return (*this);
	}
	return QueryWhere();
}

void QueryWhere::updatePlaceholder(const QString &placeholderName,
								   const QueryValue &value,
								   const Qst::Functor &functor)
{
	_field.updatePlaceholder(placeholderName, value, functor);

	for (int i = 0; i < _conditionList.count(); ++i)
		_conditionList[i].updatePlaceholder(placeholderName, value, functor);
}

void QueryWhere::updatePlaceholder(const QString &placeholderName,
								   const QVariantList &varList,
								   const Qst::Functor &functor)
{
	_field.updatePlaceholder(placeholderName, varList, functor);

	for (int i = 0; i < _conditionList.count(); ++i)
		_conditionList[i].updatePlaceholder(placeholderName, varList, functor);
}

void QueryWhere::resetPlaceholders()
{
	_field.resetPlaceholder();

	for (int i = 0; i < _conditionList.count(); ++i)
		_conditionList[i].resetPlaceholders();
}

QueryWhere & QueryWhere::operator ||(const QueryWhere &condition)
{
	return _appendCondition(condition, Or);
}

QueryWhere & QueryWhere::operator &&(const QueryWhere &condition)
{
	return _appendCondition(condition, And);
}

QueryWhere & QueryWhere::operator ||(const QueryIn &inCondition)
{
	return _appendCondition(QueryWhere(QueryField(inCondition.field().name(),
												  inCondition.field().valueList(),
												  inCondition.field().functor(),
												  ClauseWhere)), Or);
}

QueryWhere & QueryWhere::operator &&(const QueryIn &inCondition)
{
	return _appendCondition(QueryWhere(QueryField(inCondition.field().name(),
												  inCondition.field().valueList(),
												  inCondition.field().functor(),
												  ClauseWhere)), And);
}

QueryWhere & QueryWhere::_appendCondition(const QueryWhere &cond,
										  const Relation &relation)
{
	if (_relation == UndefinedRelation)
	{
		QueryWhere tmpCond = QueryWhere(_field);
		_field = QueryField();
		_relation = relation;

		_conditionList.append(tmpCond);
		_conditionList.append(cond);
	}
	else
	{
		if (_relation == relation)
			_conditionList.append(cond);
		else
		{
			QueryWhere tmpCond = (*this);
			_field = QueryField();
			_conditionList.clear();
			_relation = relation;

			_conditionList.append(tmpCond);
			_conditionList.append(cond);
		}
	}
return (*this);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QueryWhere &f)
{
#ifndef Q_BROKEN_DEBUG_STREAM
	dbg.nospace() << "QueryWhere(field = " << f.field();
	dbg.nospace() << ", Relation = " << f.relation();
	dbg.nospace() << ", conditionList = " << f.conditionList() << ").";

	return dbg.space();
#else
	qWarning("This compiler doesn't support streaming QueryWhere to QDebug");
	return dbg;
	Q_UNUSED(f);
#endif
}
#endif
