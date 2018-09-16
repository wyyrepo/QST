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

#ifndef UT_QUERYBATCH_H
#define UT_QUERYBATCH_H

#include "qst/querygeneration/querybatch.h"

#include <QObject>
#include <QTest>


class ut_QueryBatch : public QObject
{
	Q_OBJECT

public:
    ut_QueryBatch();

private slots:

	void initTestCase();

	void fieldNames();
	void fieldNames_data();

	void sourceNames();
	void sourceNames_data();

	void batchList();
	void batchList_data();

	void conditions();
	void conditions_data();

private:

	QVariantMap  fieldValuesMap;
	QVariantList valuesList;
	QueryBatch b1, b2, b3, b4, b5;
	QueryBatch b6, b7, b8, b9, b10;

	QueryBatch sel1b, sel2b, sel3b, sel4b, sel5b;
};

#endif // UT_QUERYBATCH_H
