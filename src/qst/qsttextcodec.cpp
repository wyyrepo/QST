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

#include "qsttextcodec.h"

namespace Qst
{


/*!
\class QstTextCodec
\brief
\lang_Russian
	Класс позволяет настроить функцию tr() и установить кодек по имени.
	Для этого достаточно инстанцировать класс с нужным кодеком:
	QstTextCodec codec("UTF-8");
\endlang

\lang_English
	Class allows to setup the tr() function and the codec by it's name.
	Just declare object of class:
	QstTextCodec codec("UTF-8");
\endlang
*/

/*!
\lang_Russian
	Главный конструктор. Принимает название кодека.
\endlang

\lang_English
	Main constructor. Takes codec name.
\endlang
*/
QstTextCodec::QstTextCodec(const QString &codecName)
{
	QTextCodec *pTextCodec;
	pTextCodec = QTextCodec::codecForName(codecName.toAscii());
	Q_ASSERT_X(pTextCodec != NULL, "codecPointer", "Invalid Codec");
	QTextCodec::setCodecForTr(pTextCodec);
}

}
