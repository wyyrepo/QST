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

#include "queryin.h"


using namespace Qst;

QueryIn::QueryIn()
{
}

QueryIn::QueryIn(const QString &fieldName,
				 const QVariantList &varList,
				 const Qst::Functor &functor)
	:
	  _field(QueryField(fieldName, QueryValueList(), functor, Qst::ClauseWhere))
{
	Q_ASSERT(functor == Qst::InList
			 || functor == Qst::NotInList);

	foreach (QVariant var, varList)
		_field.addValue(QueryValue(var, NoPercentPlaceholders));
}

QueryIn::QueryIn(const QString &fieldName,
				 const QstPlaceholder &placeholder,
				 const Qst::Functor &functor)
	:
	  _field(QueryField(fieldName, QueryValue(),
						functor, placeholder.name(), ClauseCondition_Mask))
{
	Q_ASSERT(functor == Qst::InList
			 || functor == Qst::NotInList);
}

QueryField QueryIn::field() const
{
	return _field;
}
