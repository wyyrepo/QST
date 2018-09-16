/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Test module of the QsT SQL Tools.
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

#ifndef UT_QUERYWHERE_H
#define UT_QUERYWHERE_H

#include "qst/querygeneration/querywhere.h"
#include <QObject>
#include <QTest>

class ut_QueryWhere : public QObject
{
	Q_OBJECT

public:
    ut_QueryWhere();

private slots:

	void initTestCase();

	void validity();
	void validity_data();

	void field();
	void field_data();

	void validOnly();
	void validOnly_data();

private:

	QueryWhere w0, w1, w2, w3, w4, w5, w6, w7, w8, w9;
	QueryWhere w10, w10a, w11, w12;
	QueryWhere w13, w13a, w13b, w13c, w14, w15, w16, w17, w17a, w17b, w17c, w18, w18a, w18b, w18c;
	QueryWhere w19, w20;
};

#endif // UT_QUERYWHERE_H
