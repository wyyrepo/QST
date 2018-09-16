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

#ifndef QUERYFIELD_H
#define QUERYFIELD_H

#include "qst/qstglobal.h"
#include "qst/qstplaceholder.h"
#include "queryvalue.h"


#include <QList>

class QueryField // FIX ME: documentation
{
private:

	QString            _name;
	QueryValueList     _valueList;
	Qst::Functor       _functor;
	Qst::QueryClause   _clause;

	QString _placeholderName;

public:

	QueryField();

	explicit QueryField(const QString &name,
			   const Qst::QstPlaceholder &pl = Qst::QstPlaceholder(),
			   const Qst::Functor &functor = Qst::NoFunctor);

	QueryField(const QString &name,
			   const QString &placeholderName,
			   const Qst::Functor &functor,
			   const Qst::QueryClause &clause);

	QueryField(const QString &name,
			   const Qst::QueryClause &clause);

	QueryField(const QString &name,
			   const QueryValue &value,
			   const Qst::Functor &functor,
			   const QString &placeholderName = QString(),
			   const Qst::QueryClause clause = Qst::ClauseCondition_Mask);

	QueryField(const QString &name,
			   const QueryValue &value,
			   const Qst::QueryClause clause = Qst::ClauseValued_Mask);

	QueryField(const QString &name,
			   const QueryValue &value1,
			   const QueryValue &value2,
			   const Qst::QueryClause &clause = Qst::ClauseCondition_Mask);

	explicit QueryField(const QueryValue &value,
						const Qst::QueryClause clause = Qst::ClauseParameters);

	QueryField(const QString &name,
			   const QueryValueList &valList,
			   const Qst::Functor &functor,
			   const Qst::QueryClause &clause = Qst::ClauseCondition_Mask,
			   const QString &placeholderName = QString());

	QString name() const;
	void setName(const QString &name);

	bool testClause(const Qst::QueryClause &clause) const;
	void setClause (const Qst::QueryClause &clause);
	Qst::QueryClause clause() const;

	QueryValue value(const Qst::BetweenFilterValueOrder &order,
					 const QueryValue &defaultValue = QueryValue(QVariant(), Qst::NoPercentPlaceholders)) const;
	QueryValue value(const int &index = 0,
					 const QueryValue &defaultValue = QueryValue(QVariant(), Qst::NoPercentPlaceholders)) const;
	void setValue(const int &index, const QueryValue &value);
	void addValue(const QueryValue &value);

	QueryValueList valueList() const;
	void setValueList(const QueryValueList &list);

	Qst::Functor functor() const;
	void setFunctor(const Qst::Functor &functor);

	bool hasValue(const Qst::BetweenFilterValueOrder &order = Qst::LeftValue) const;
	bool hasValue(const int &index = 0) const;

	bool isValuesValid() const;
	bool isValid() const;

	void updatePlaceholder(const QString &placeholderName,
						   const QueryValue &value,
						   const Qst::Functor &functor);

	void updatePlaceholder(const QString &placeholderName,
						   const QVariantList &varList,
						   const Qst::Functor &functor);

	void resetPlaceholder();

	QString placeholderName() const;

	friend bool operator == (const QueryField &val1, const QueryField &val2)
	{
		return	(val1._name == val2._name)
				&& val1._functor == val2._functor
				&& val1._valueList == val2._valueList
				&& val1._placeholderName == val2._placeholderName
				;
	}
	friend bool operator != (const QueryField &val1, const QueryField &val2)
	{
		return !(val1 == val2);
	}
};

////////////////////////////////////////////////////////////////////////////////

typedef QList<QueryField>	QueryFieldList;

////////////////////////////////////////////////////////////////////////////////

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QueryField &);
#endif


#endif // QUERYFIELD_H
