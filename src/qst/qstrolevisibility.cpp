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

#include "qstrolevisibility.h"

namespace Qst
{

namespace Special
{
/*!
\class QstRoleVisibility
\brief
\lang_Russian
	Служебный класс для сопоставления роли поля и видимости поля.
	Используется в QstBatch.
\endlang

\lang_English
	Service class for field role and field visibility binding.
	Used in QstBatch.
\endlang
*/

/*!
\lang_Russian
	Главный конструктор.
\endlang

\lang_English
	Main constructor.
\endlang
*/
QstRoleVisibility::QstRoleVisibility(const Qst::FieldRole &role,
									 const Qst::FieldVisibility &visibility)
	:
	_role(role),
	_visibility(visibility)
{
}

/*!
\lang_Russian
	Возвращает роль поля.
\endlang

\lang_English
	Returns field role.
\endlang
*/
Qst::FieldRole			QstRoleVisibility::role() const
{
	return _role;
}

/*!
\lang_Russian
	Возвращает видимость поля.
\endlang

\lang_English
	Returns field visibility.
\endlang
*/
Qst::FieldVisibility	QstRoleVisibility::visibility() const
{
	return _visibility;
}

}	// End of namespace Special

}	// End of namespace Qst
