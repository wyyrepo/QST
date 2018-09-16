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

#ifndef QUERYFRAME_H
#define QUERYFRAME_H

#include <QString>
#include <QList>
#include <QMap>

#include "qst/qstglobal.h"
#include "queryscheme.h"

class QueryFrame;

typedef QList<QueryFrame> QueryFrameList;
typedef QMap<Qst::QueryClause, QueryFrame> ClauseFrameMap;

class QueryFrame
{
private:

	QString			_leftToken;
	QString			_rightToken;
	QueryFrameList	_frameList;

	Qst::QueryClause _partClause;

public:
	QueryFrame();
	QueryFrame(const QString &leftToken,
			   const QString &rightToken,
			   const QueryFrameList &frameList);

	QueryFrame *appendFrame(const QueryFrame &frame);

	QString toString() const;

	QueryFrame     part(const Qst::QueryClause &clause) const;
	ClauseFrameMap parts(const Qst::QueryClauses &clauses) const;

	void clear();

	void setPartClause(const Qst::QueryClause &clause);

	QString leftToken() const;
	QString rightToken() const;
	QueryFrameList frameList() const;

	QueryFrame & operator<<(const QueryFrame &frame);

	friend bool operator==(const QueryFrame &fr1,
						   const QueryFrame &fr2)
	{
		return fr1._leftToken == fr2._leftToken
				&& fr1._rightToken == fr2._rightToken
				&& fr1._partClause == fr2._partClause
				&& fr1._frameList == fr2._frameList;
	}
	friend bool operator!=(const QueryFrame &fr1,
						   const QueryFrame &fr2)
	{ return ! (fr1 == fr2); }
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QueryFrame &);
#endif

#endif // QUERYFRAME_H
