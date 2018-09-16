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

#include "queryframe.h"

#include <QDebug>

using namespace Qst;

/*
  SELECT F1, F2 FROM T1 WHERE (F1 > F2)

  QF("", "")
  |-QF("SELECT")
  | |-QF("F1")
  | |-QF("F2")
  |-QF("FROM")
  | |-QF("T1")
  |-QF("WHERE")
	|-QF("(", ")")
	  |-QF("F1 > F2")
*/

QueryFrame::QueryFrame()
	:
	  _leftToken(""),
	  _rightToken(""),
	  _partClause(NoClause)
{
}

QueryFrame::QueryFrame(const QString &leftToken,
					   const QString &rightToken,
					   const QueryFrameList &frameList)
   :
	 _leftToken(leftToken),
	 _rightToken(rightToken),
	_frameList(frameList),
	 _partClause(NoClause)
{
}

QueryFrame *QueryFrame::appendFrame(const QueryFrame &frame)
{
	_frameList.append(frame);

	return &_frameList[_frameList.count()-1];
}

QString QueryFrame::toString() const
{
	QString res;
	res = _leftToken;
	for (int i = 0; i < _frameList.count(); ++i)
		res += _frameList[i].toString();
	return (res + _rightToken);
}

QueryFrame QueryFrame::part(const Qst::QueryClause &clause) const
{
	if (this->_partClause == clause)
		return (*this);

	foreach (QueryFrame frame, _frameList)
		return frame.part(clause);

	return QueryFrame();
}

ClauseFrameMap QueryFrame::parts(const Qst::QueryClauses &clauses) const
{
	ClauseFrameMap map;

	QList<QueryClause> clauseList;
	clauseList << ClauseSelect       << ClauseFrom
			   << ClauseLeftJoin     << ClauseRightJoin
			   << ClauseFullJoin     << ClauseInnerJoin
			   << ClauseWhere        << ClauseInsert
			   << ClauseUpdate       << ClauseOrderBy
			   << ClauseGroupBy      << ClauseParameters
			   << ClauseHaving       << ClauseValues
			   << ClauseSet          << ClauseDelete
			   << ClauseExecute      << ClauseOn
			   << UserDefinedClauses;

	foreach(QueryClause clause, clauseList)
	{
		if (clauses.testFlag(clause))
			map.insert(clause, part(clause));
	}

	return map;
}

void QueryFrame::clear()
{
	_frameList.clear();
	_leftToken.clear();
	_rightToken.clear();
}

void QueryFrame::setPartClause(const Qst::QueryClause &clause)
{
	_partClause = clause;
}

QString QueryFrame::leftToken() const
{
	return _leftToken;
}

QString QueryFrame::rightToken() const
{
	return _rightToken;
}

QueryFrameList QueryFrame::frameList() const
{
	return _frameList;
}

QueryFrame &QueryFrame::operator<<(const QueryFrame &frame)
{
	appendFrame(frame);
	return *this;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QueryFrame &f)
{
#ifndef Q_BROKEN_DEBUG_STREAM
	dbg.nospace() << "QueryFrame (leftToken = " << f.leftToken();
	dbg.nospace() << ", rightToken = " << f.rightToken();
	dbg.nospace() << ", frameList = " << f.frameList() << ")";

	return dbg.space();
#else
	qWarning("This compiler doesn't support streaming QueryFrame to QDebug");
	return dbg;
	Q_UNUSED(f);
#endif
}
#endif
