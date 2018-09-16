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

#ifndef ABSTRACTITEMNAMEEXTRACTOR_H
#define ABSTRACTITEMNAMEEXTRACTOR_H

#include <QString>

namespace Qst
{
namespace Special
{

class AbstractItemNameExtractor
{
public:
	AbstractItemNameExtractor();
	AbstractItemNameExtractor(const QString &str);
	virtual ~AbstractItemNameExtractor() = 0;

	virtual QString extract() const;
	virtual QString extract(const QString &str) const;
};

} // End of namespace Special
} // End of namespace Qst;

#endif // ABSTRACTITEMNAMEEXTRACTOR_H
