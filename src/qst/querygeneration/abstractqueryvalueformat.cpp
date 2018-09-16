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

#include "abstractqueryvalueformat.h"

using namespace Qst;

AbstractQueryValueFormat::AbstractQueryValueFormat()
{
}

AbstractQueryValueFormat::~AbstractQueryValueFormat()
{
}

QString AbstractQueryValueFormat::toString(const QueryValue &queryValue,
										   const ApostropheBorderingFlags &borderingFlags,
										   const PercentPlaceholders &placeholders,
										   const bool &substituteByNULL) const
{
	QVariant varVal = queryValue.value();

	switch (varVal.type())
	{
	case QVariant::Date:      return toDateString(varVal.toDate(), borderingFlags.testFlag(DateBordering), placeholders, DEFAULT_DATE_FORMAT, substituteByNULL);
	case QVariant::Time:      return toTimeString(varVal.toTime(), borderingFlags.testFlag(TimeBordering), placeholders, DEFAULT_TIME_FORMAT, substituteByNULL);
	case QVariant::DateTime:  return toDateTimeString(varVal.toDateTime(), borderingFlags.testFlag(DateTimeBordering), placeholders, DEFAULT_DATE_TIME_FORMAT, substituteByNULL);
	case QVariant::String:
	case QVariant::ByteArray: return format(varVal.toString(), borderingFlags.testFlag(StringBordering), placeholders, substituteByNULL);
	//case QVariant::Bool:

	default: break;
	}

	return format(queryValue.value().toString(), borderingFlags.testFlag(OtherTypeBordering), placeholders, substituteByNULL);
}

QString AbstractQueryValueFormat::toDateString(const QDate &date,
											   const bool &apostropheBorder,
											   const PercentPlaceholders &percentPlaceholders,
											   const QString &dateFormat,
											   const bool &substituteByNULL) const
{
	QString templ = percentPlaceholdersTemplate(percentPlaceholders);

	if (substituteByNULL && date.isNull())
		return templ.arg("NULL");

	if (apostropheBorder)
	{
		QString strDate = templ.arg(date.toString(dateFormat));
		return "'" + strDate + "'";
	}
	else
		return templ.arg(date.toString(dateFormat));
}

QString AbstractQueryValueFormat::toTimeString(const QTime &time,
											   const bool &apostropheBorder,
											   const PercentPlaceholders &percentPlaceholders,
											   const QString &timeFormat,
											   const bool &substituteByNULL) const
{
	QString templ = percentPlaceholdersTemplate(percentPlaceholders);

	if (substituteByNULL && time.isNull())
		return templ.arg("NULL");

	if (apostropheBorder)
	{
		QString strTime = templ.arg(time.toString(timeFormat));
		return "'" + strTime + "'";
	}
	else
		return templ.arg(time.toString(timeFormat));
}

QString AbstractQueryValueFormat::toDateTimeString(const QDateTime &dateTime,
												   const bool &apostropheBorder,
												   const PercentPlaceholders &percentPlaceholders,
												   const QString &dateTimeFormat,
												   const bool &substituteByNULL) const
{
	QString templ = percentPlaceholdersTemplate(percentPlaceholders);

	if (substituteByNULL && dateTime.isNull())
		return templ.arg("NULL");

	if (apostropheBorder)
	{
		QString strDateTime = templ.arg(dateTime.toString(dateTimeFormat));
		return "'" + strDateTime + "'";
	}
	else
		return templ.arg(dateTime.toString(dateTimeFormat));
}

QString AbstractQueryValueFormat::format(const QString &string,
										 const bool &apostropheBorder,
										 const PercentPlaceholders &percentPlaceholders,
										 const bool &substituteByNULL) const
{
	if (substituteByNULL && string.isEmpty())
		return "NULL";

	QString res = percentPlaceholdersTemplate(percentPlaceholders).arg(string);
	if (apostropheBorder)
		res = "'" + res + "'";
	return res;
}

QString AbstractQueryValueFormat::percentPlaceholdersTemplate
		(const PercentPlaceholders &percentPlaceholders) const
{
	switch(percentPlaceholders)
	{
		case LeftPercent:		return QString("%%1");
		case RightPercent:		return QString("%1%");
		case LeftRightPercents:	return QString("%%1%");
		case NoPercentPlaceholders:	return QString("%1");
	};

return QString("%1");
}
