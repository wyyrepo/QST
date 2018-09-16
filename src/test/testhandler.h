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

#ifndef TESTHANDLER_H
#define TESTHANDLER_H

#include "qst/qstabstractmodelhandler.h"
#include <QString>
#include <QTableView>
#include <QTreeView>
#include <QComboBox>

namespace QstTest
{

const int TEST1 = 1;
const int TEST2 = 2;
const int TEST3 = 3;

class TestHandler : public Qst::QstAbstractModelHandler // FIX ME: FULL REWRITE
{
private:

	static int _connectionNumber;

	QString _connectionName;

public:
    TestHandler();

	QString connect();
	QString connectionName() const;
	void setData();

private:

//	QueryBatch _selector(const int &queryNumber) const;
};

} // End of namespace QstTest

#endif // TESTHANDLER_H
