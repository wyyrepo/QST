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

#ifndef STRINGTEMPLATE_H
#define STRINGTEMPLATE_H

#include <QString>

class StringTemplate
{
public:
	StringTemplate(const int & argsCount, const QString stringTemplate);
	StringTemplate();

	int argsCount() const;
	QString stringTemplate() const;

	QString string(const QString & arg1) const;
	QString string(const QString & arg1, const QString & arg2) const;
	QString string(const QString & arg1, const QString & arg2, const QString & arg3) const;

private:
	int			_argsCount;
	QString		_stringTemplate;
};

#endif // STRINGTEMPLATE_H
