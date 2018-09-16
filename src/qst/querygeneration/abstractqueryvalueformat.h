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

#ifndef ABSTRACTQUERYVALUEFORMAT_H
#define ABSTRACTQUERYVALUEFORMAT_H

#include "queryvalue.h"

#include <QDate>
#include <QDateTime>
#include <QString>

class AbstractQueryValueFormat	// FIX ME: documentation // FULL REWRITE
{
public:
    AbstractQueryValueFormat();
	virtual ~AbstractQueryValueFormat() = 0;

	virtual QString toString(const QueryValue &queryValue,
							 const Qst::ApostropheBorderingFlags &borderingFlags = Qst::NotBordered,
							 const Qst::PercentPlaceholders &placeholders = Qst::NoPercentPlaceholders,
							 const bool &substituteByNULL = false) const = 0;

	virtual QString toDateString(const QDate &date,
								 const bool &apostropheBorder,
								 const Qst::PercentPlaceholders &percentPlaceholders,
								 const QString &dateFormat,
								 const bool &substituteByNULL) const = 0;

	virtual QString toTimeString(const QTime &time,
								 const bool &apostropheBorder,
								 const Qst::PercentPlaceholders &percentPlaceholders,
								 const QString &timeFormat,
								 const bool &substituteByNULL) const = 0;

	virtual QString toDateTimeString(const QDateTime &dateTime,
									 const bool &apostropheBorder,
									 const Qst::PercentPlaceholders &percentPlaceholders,
									 const QString &dateTimeFormat,
									 const bool &substituteByNULL) const = 0;

	virtual QString percentPlaceholdersTemplate
					(const Qst::PercentPlaceholders &percentPlaceholders) const = 0;

	virtual QString format(const QString &string,
						   const bool &apostropheBorder,
						   const Qst::PercentPlaceholders &percentPlaceholders,
						   const bool &substituteByNULL) const = 0;
};

#endif // ABSTRACTQUERYVALUEFORMAT_H
