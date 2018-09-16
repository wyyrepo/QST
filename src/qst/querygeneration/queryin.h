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

#ifndef QUERYIN_H
#define QUERYIN_H

#include <QString>
#include <QVariantList>

#include "queryfield.h"
#include "qst/qstplaceholder.h"

class QueryIn
{
private:

	QueryField _field;

public:
    QueryIn();

	QueryIn(const QString &fieldName,
			const QVariantList &varList,
			const Qst::Functor &functor = Qst::InList);

	QueryIn(const QString &fieldName,
			const Qst::QstPlaceholder &placeholder,
			const Qst::Functor &functor = Qst::InList);

	QueryField field() const;
};

#endif // QUERYIN_H
