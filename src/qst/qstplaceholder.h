/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
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

#ifndef QSTPLACEHOLDER_H
#define QSTPLACEHOLDER_H

#include <QString>
#include <QList>
#include <QStringList>

#include "qstglobal.h"

namespace Qst
{

class QstPlaceholder
{
private:

	QStringList  _names;

public:
	QstPlaceholder();
	explicit QstPlaceholder(const QString &placeholderName);

	QstPlaceholder(const QString &placeholderName,
				   const ValueCheckFunctor &valueCheckFunctor);

	void addName(const QString &placeholderName);

	int position(const QString &placeholderName) const;
	QStringList names() const;

	QString name() const;
};

////////////////////////////////////////////////////////////////////////////////

typedef QList<QstPlaceholder> QstPlaceholderList;

////////////////////////////////////////////////////////////////////////////////

} // End of namespace Qst

#endif // QSTPLACEHOLDER_H
