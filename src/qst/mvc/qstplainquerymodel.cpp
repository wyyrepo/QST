/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst::Mvc module of the QsT SQL Tools.
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

#include "qstplainquerymodel.h"

namespace Qst
{

/*!
\class QstPlainQueryModel
\brief
\lang_Russian
	Плоская модель данных, основанная на QSqlQueryModel.
\endlang

\lang_English
	Plain data model based on QSqlQueryModel.
\endlang
*/

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
QstPlainQueryModel::QstPlainQueryModel(QObject *parent)
	: QSqlQueryModel(parent)
{
}

/*!
\lang_Russian
	Переопределенная функция match, находящая совпадения не только в столбцах,
	но и в строках.
\endlang

\lang_English
	Redefined function. Searches matches in rows and columns.
\endlang
*/
QModelIndexList QstPlainQueryModel::match (const QModelIndex & start,
										   int role,
										   const QVariant & value,
										   int hits,
										   Qt::MatchFlags flags) const
{
	if (hits == 0
		|| this->rowCount() == 0
		|| this->columnCount() == 0)
		return QModelIndexList();

	QModelIndexList list;
	int totalHits = 0;

	if (start.data(Qt::DisplayRole) == value)
	{
		list << start;
		totalHits = 1;
	}

	int columnCount = this->columnCount();
	int rowCount = this->rowCount();

	for (int i = start.row(); ((hits == -1) || hits != totalHits) && (i < rowCount); ++i)
	for (int j = start.column(); ((hits == -1) || hits != totalHits) && (j < columnCount); ++j)
	{
		if (this->data(this->index(i,j,QModelIndex())) == value)
		{
			list << this->index(i,j,QModelIndex());
			++totalHits;
		}
	}
return list;
}

} // End of namespace Qst
