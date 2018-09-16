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

#include "postgresqlvalueformat.h"

using namespace Qst;

PostgreSqlValueFormat::PostgreSqlValueFormat()
{
}

PostgreSqlValueFormat::~PostgreSqlValueFormat()
{
}

QString PostgreSqlValueFormat::toString(const QueryValue &queryValue,
						const Qst::ApostropheBorderingFlags &borderingFlags,
						const Qst::PercentPlaceholders &placeholders,
						const bool &substituteByNULL) const
{
	return AbstractQueryValueFormat::toString(queryValue,
											  borderingFlags,
											  placeholders,
											  substituteByNULL);
}

QString PostgreSqlValueFormat::toDateString(const QDate &date,
											const bool &apostropheBorder,
											const Qst::PercentPlaceholders &percentPlaceholders,
											const QString &dateFormat,
											const bool &substituteByNULL) const
{
	return AbstractQueryValueFormat::toDateString(date,
												  apostropheBorder,
												  percentPlaceholders,
												  dateFormat,
												  substituteByNULL);
}

QString PostgreSqlValueFormat::toTimeString(const QTime &time,
											const bool &apostropheBorder,
											const Qst::PercentPlaceholders &percentPlaceholders,
											const QString &timeFormat,
											const bool &substituteByNULL) const
{
	return AbstractQueryValueFormat::toTimeString(time,
												  apostropheBorder,
												  percentPlaceholders,
												  timeFormat,
												  substituteByNULL);
}

QString PostgreSqlValueFormat::toDateTimeString(const QDateTime &dateTime,
												const bool &apostropheBorder,
												const Qst::PercentPlaceholders &percentPlaceholders,
												const QString &dateTimeFormat,
												const bool &substituteByNULL) const
{
	return AbstractQueryValueFormat::toDateTimeString(dateTime,
													  apostropheBorder,
													  percentPlaceholders,
													  dateTimeFormat,
													  substituteByNULL);
}

QString PostgreSqlValueFormat::percentPlaceholdersTemplate
		(const Qst::PercentPlaceholders &percentPlaceholders) const
{
	return AbstractQueryValueFormat::percentPlaceholdersTemplate(percentPlaceholders);
}

QString PostgreSqlValueFormat::format(const QString &string,
									  const bool &apostropheBorder,
									  const Qst::PercentPlaceholders &percentPlaceholders,
									  const bool &substituteByNULL) const
{
	return AbstractQueryValueFormat::format(string,
											apostropheBorder,
											percentPlaceholders,
											substituteByNULL);
}
