/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Special module of the QsT SQL Tools.
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

#ifndef DEFAULTITEMNAMEEXTRACTOR_H
#define DEFAULTITEMNAMEEXTRACTOR_H

#include "abstractitemnameextractor.h"

namespace Qst
{
namespace Special
{


class DefaultItemNameExtractor : public AbstractItemNameExtractor
{
public:
    DefaultItemNameExtractor();
	DefaultItemNameExtractor(const QString &str);

	QString extract() const;
	QString extract(const QString &str) const;

private:

	QString _str;

	QString _getFromBrackets(const QString &str) const;
	QString _getAfterQualifier(const QString &str) const;
};

} // End of namespace Special
} // End of namespace Qst;

#endif // DEFAULTITEMNAMEEXTRACTOR_H
